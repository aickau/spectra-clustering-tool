//! \verbatim
//! ###########################################################################
//! # SDSS sorting prototype - Stage I
//! #
//! # Copyright (c) 2009 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 1/19/2009
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  spectraVFS.cpp
//! \brief virtual filesystem for spectra, uses main memory as cache

#include "spectraVFS.h"

#include "helpers.h"
#include "filehelpers.h"
#include "spectra.h"
#include "Timer.h"

#include <sstream>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <assert.h>

#include "memory.h"

//#define SPECTRAVFS_ASYNC_IO

static
__int64 HiLowtoUInt64( unsigned __int32 _nLowerPart, unsigned __int32 _nHigherPart )
{
	return (static_cast<unsigned __int64>(_nHigherPart)<<32)+static_cast<unsigned __int64>(_nLowerPart);
}

static
void UInt64toHiLow( unsigned __int64 _nInNumber, unsigned __int32 &_nOutLowerPart, unsigned __int32 &_nOutHigherPart )
{
	_nOutLowerPart = _nInNumber & 0x0ffffffff;
	_nOutHigherPart = _nInNumber >> 32;
}


SpectraVFS::SpectraVFS( const std::string &_sstrFilename, bool _readOnly )
:m_sstrDumpFilename(_sstrFilename)
,m_bReadOnly(_readOnly)
,m_nNumberOfSpectra(0)
,m_FileHandle(NULL)
,m_nTimeStamp(0)
,m_logFile(std::string(_sstrFilename+std::string(".log")).c_str())
{
	for ( size_t i=0;i<CACHELINES;i++)
	{
		m_TagTable[i].bCommitWrite = false;
		m_TagTable[i].nCacheLineIndex = 0;
		m_TagTable[i].nTimeStamp = 0;
		m_TagTable[i].status = CacheTag::STATUS_FREE;
		m_pCache[i] = NULL;
	}

	m_IOHandle.reset();

	DWORD accessMode = GENERIC_READ | ((_readOnly) ? 0 : GENERIC_WRITE);
#ifdef SPECTRAVFS_ASYNC_IO
	m_FileHandle = CreateFile( m_sstrDumpFilename.c_str(), accessMode, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL );
#else
	m_FileHandle = CreateFile( m_sstrDumpFilename.c_str(), accessMode, 0, NULL, OPEN_EXISTING, 0, NULL );
#endif

	if (m_FileHandle == INVALID_HANDLE_VALUE)
	{
		// file not found
		return;
	}

	// determine filesize
	LARGE_INTEGER nFileSize;
	nFileSize.LowPart = 0;
	nFileSize.HighPart = 0;

	if (!GetFileSizeEx( m_FileHandle, &nFileSize ))
	{
		// some fucking error 
		return;
	}

	unsigned __int64 fileSize = HiLowtoUInt64(nFileSize.LowPart, nFileSize.HighPart);
	unsigned __int64 spectraSize = static_cast<unsigned __int64>(SPECTRASIZE);
	unsigned __int64 fileCount = fileSize / spectraSize;
	unsigned __int64 remainder = fileSize % spectraSize;

	if ( remainder != 0 )
	{
		// wrong size
		return;
	}

	for ( size_t i=0;i<CACHELINES;i++)
	{
		m_pCache[i] = static_cast<Spectra*>( Memory::memAlignedAlloc( TOTALCACHELINEBYTES ) ); // new Spectra[CACHELINESIZE];

		if ( m_pCache == NULL )
		{
			// out of heap
			_cprintf("failed to allocate %i bytes. exit.\n", TOTALCACHELINEBYTES );
			CloseHandle(m_FileHandle);
			exit(-1);
			return;
		}

		m_TagTable[i].nCacheLineIndex = INDEX_INVALID;
		m_TagTable[i].nTimeStamp = 0;
		m_TagTable[i].bCommitWrite = false;
		m_TagTable[i].status = CacheTag::STATUS_FREE;
	}

	m_nNumberOfSpectra = static_cast<size_t>(fileCount);
}

SpectraVFS::~SpectraVFS()
{
	flush();
	if ( m_FileHandle != INVALID_HANDLE_VALUE )
	{
		CloseHandle(m_FileHandle);
	}
	for ( size_t i=0;i<CACHELINES;i++)
	{
		Memory::memAlignedFree( m_pCache[i] );
		m_pCache[i] = NULL;
	}
}

std::string SpectraVFS::getFileName()
{
	return m_sstrDumpFilename;
}


size_t SpectraVFS::getNumSpectra()
{
	return m_nNumberOfSpectra;
}

Spectra *SpectraVFS::beginRead( size_t _nSpectraIndex )
{
	assert( _nSpectraIndex < m_nNumberOfSpectra );

	m_nTimeStamp++;

	size_t nCacheLineIndex = _nSpectraIndex / CACHELINESIZE;
	size_t nLineOffset = _nSpectraIndex - nCacheLineIndex*CACHELINESIZE;
	size_t nLRUCacheIndex = INDEX_INVALID;
	size_t nLRUTimeStamp = INDEX_INVALID;

	for ( size_t i=0;i<CACHELINES;i++) 
	{
		// check if spectrum is in cache
		CacheTag &currentTag = m_TagTable[i];
		if ( currentTag.nCacheLineIndex == nCacheLineIndex )
		{
			if ( currentTag.status == CacheTag::STATUS_FETCHING )
			{
				// wait until prefetching is finished
				WaitForIO( m_IOHandle );
			}
			currentTag.status = CacheTag::STATUS_INUSE;
			currentTag.nTimeStamp = m_nTimeStamp;
			return &m_pCache[i][nLineOffset];
		}

		// mark LRU position where we have a free spot
		if ( currentTag.status == CacheTag::STATUS_FREE && currentTag.nTimeStamp < nLRUTimeStamp )
		{
			nLRUCacheIndex = i;
			nLRUTimeStamp = currentTag.nTimeStamp;
		}
	}

	if ( nLRUCacheIndex == INDEX_INVALID )
	{
		// if this asserts every cache line is in use for reading and writing. 
		// this should never happen.
		assert(0);
		return NULL;
	}

	assert( nLRUCacheIndex < CACHELINES );

	// oh no, a cache miss.
	CacheTag &currentTag = m_TagTable[nLRUCacheIndex];
	Spectra *pCacheLineStart = m_pCache[nLRUCacheIndex];
	
	if ( currentTag.bCommitWrite )
	{
		Write( currentTag.nCacheLineIndex*CACHELINESIZE, pCacheLineStart );
		currentTag.bCommitWrite = false;
	}

	currentTag.nCacheLineIndex = nCacheLineIndex;
	currentTag.nTimeStamp = m_nTimeStamp;
	currentTag.status = CacheTag::STATUS_INUSE;

	Read( nCacheLineIndex*CACHELINESIZE, pCacheLineStart, false );
	return &pCacheLineStart[nLineOffset];
}

void SpectraVFS::endRead( size_t _nSpectraIndex )
{
	size_t nCacheLineIndex = _nSpectraIndex / CACHELINESIZE;

	for ( size_t i=0;i<CACHELINES;i++) 
	{
		CacheTag &currentTag = m_TagTable[i];
		if ( currentTag.nCacheLineIndex == nCacheLineIndex )
		{
			currentTag.status = CacheTag::STATUS_FREE; 
			return;
		}
	}
}


Spectra *SpectraVFS::beginWrite( size_t _nSpectraIndex )
{
	if ( m_bReadOnly )
		return NULL;

	return beginRead( _nSpectraIndex );
}


void SpectraVFS::endWrite( size_t _nSpectraIndex )
{
	if ( m_bReadOnly )
		return;

	size_t nCacheLineIndex = _nSpectraIndex / CACHELINESIZE;

	for ( size_t i=0;i<CACHELINES;i++) 
	{
		CacheTag &currentTag = m_TagTable[i];
		if ( currentTag.nCacheLineIndex == nCacheLineIndex )
		{
			// mark as free, but the commit flag ensures data is written before new data flows into this cache line.
			currentTag.status = CacheTag::STATUS_FREE; 
			currentTag.bCommitWrite = true; 
			return;
		}
	}
}


void SpectraVFS::prefetch( size_t _nSpectraIndex )
{
	if( _nSpectraIndex >= m_nNumberOfSpectra )
	{
		return;
	}

	// prefetch in progress?
	if ( !m_IOHandle.isSet() )
	{
		// only one prefetch operation allowed.
		return;
	}

	m_nTimeStamp++;

	size_t nCacheLineIndex = _nSpectraIndex / CACHELINESIZE;
	size_t nLineOffset = _nSpectraIndex - nCacheLineIndex*CACHELINESIZE;
	size_t nIndexCount = 0;
	size_t nLRUCacheIndex = INDEX_INVALID;
	size_t nLRUTimeStamp = INDEX_INVALID;

	for ( size_t i=0;i<CACHELINES;i++) 
	{
		// check if spectrum is in cache, if so return.
		CacheTag &currentTag = m_TagTable[i];
		if ( currentTag.nCacheLineIndex == nCacheLineIndex )
		{
			currentTag.status = CacheTag::STATUS_INUSE; 
			currentTag.nTimeStamp = m_nTimeStamp;
			return;
		}

		// mark LRU position where we have the best free spot
		if ( currentTag.status == CacheTag::STATUS_FREE && currentTag.nTimeStamp < nLRUTimeStamp )
		{
			nLRUCacheIndex = i;
			nLRUTimeStamp = currentTag.nTimeStamp;
		}

		nIndexCount += CACHELINESIZE;
	}

	if ( nLRUCacheIndex == INDEX_INVALID )
	{
		// if this asserts every cache line is in use for reading and writing. 
		// this should never happen.
		return;
	}

	assert( nLRUCacheIndex < CACHELINES );

	// start prefetching..
	CacheTag &currentTag = m_TagTable[nLRUCacheIndex];
	Spectra *pCacheLineStart = m_pCache[nLRUCacheIndex];

	if ( currentTag.bCommitWrite )
	{
		Write( currentTag.nCacheLineIndex*CACHELINESIZE, pCacheLineStart );
		currentTag.bCommitWrite = false;
	}

	currentTag.nCacheLineIndex = nCacheLineIndex;
	currentTag.nTimeStamp = m_nTimeStamp;
	currentTag.status = CacheTag::STATUS_FETCHING;
	Read( nCacheLineIndex*CACHELINESIZE, pCacheLineStart, true );
}


void SpectraVFS::flush()
{
	for ( size_t i=0;i<CACHELINES;i++) 
	{
		if ( m_pCache[i] != NULL )
		{
			// check if spectrum is in cache, if so return.
			CacheTag &currentTag = m_TagTable[i];
			if ( currentTag.bCommitWrite  )
			{
				Spectra *pCacheLineStart = m_pCache[i];
				Write( currentTag.nCacheLineIndex*CACHELINESIZE, pCacheLineStart );
				currentTag.bCommitWrite = false;
			}
		}
	}
}

struct S82
{
	__int64 specObjID;
	double z;
	double M_i;
};

#include <map>


void SpectraVFS::write( const std::string &_sstrDir )
{
	std::vector<std::string> fileList;

	std::ofstream flog("dump_log.txt");


	// s 82 datastructure
	std::map<__int64,S82> s82;

	std::ifstream fin("S82belt.csv");
	if( !fin ) 
	{
		Helpers::print( std::string("S82belt.csv not found.\n"), &flog );

		exit(0);
	}
	std::string sstrHui;
	getline(fin,sstrHui);
	int count =0;
	while( getline(fin,sstrHui) ) 
	{
		std::istringstream sstrStream(sstrHui);
		int mjd_spec,plate,fiberID;
		int FIRSTmag,scienceprimary;

		S82 s82t;

		if (sstrStream) {
			sstrStream >> s82t.specObjID;	
		}

		if (sstrStream) {
			sstrStream >> mjd_spec;	
		}
		if (sstrStream) {
			sstrStream >> plate;	
		}
		if (sstrStream) {
			sstrStream >> fiberID;	
		}
		if (sstrStream) {
			sstrStream >> s82t.z;	
		}
		if (sstrStream) {
			sstrStream >> s82t.M_i;	
		}
		if (sstrStream) {
			sstrStream >> FIRSTmag;	
		}

		if (sstrStream) {
			sstrStream >> scienceprimary;	
		}


		s82.insert(std::make_pair<__int64,S82>(s82t.specObjID,s82t));
		count++;
	}


	size_t numSpectra = FileHelpers::getFileList( _sstrDir, fileList );

	std::string sstrFileName0("zrange0.bin");
	std::string sstrFileName1("zrange1.bin");
	std::string sstrFileName2("zrange2.bin");
	std::string sstrFileName3("zrange3.bin");

	HANDLE f0 = CreateFile( sstrFileName0.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL );
	HANDLE f1 = CreateFile( sstrFileName1.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL );
	HANDLE f2 = CreateFile( sstrFileName2.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL );
	HANDLE f3 = CreateFile( sstrFileName3.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL );

	Spectra spec;

	size_t c = 0;
	for ( size_t i=0;i<numSpectra;i++ )
	{
		spec.clear();
		bool bResult = spec.loadFromFITS( fileList.at(i) );

		if ( bResult )
		{
			std::map<__int64,S82>::iterator it = s82.find(spec.m_SpecObjID);

			if ( it != s82.end() )
			{
				spec.m_RealZ = it->second.z;
				spec.m_Mi = it->second.M_i;	

				// ignore if in wrong z range..
				if ( spec.m_RealZ >= 0.5f && spec.m_RealZ <= 0.6f )
				{
					// hack: read again, since we need m_RealZ for correct loading..
					spec.loadFromFITS( fileList.at(i) );

					DWORD bytesWritten = 0;
					bResult = (WriteFile( f0, &spec, sizeof(Spectra), &bytesWritten, NULL ) > 0) ? true : false;
					if ( bResult)
					{
						c++;
					}
				}
				if ( spec.m_RealZ >= 1.2f && spec.m_RealZ <= 1.4f )
				{
					// hack: read again, since we need m_RealZ for correct loading..
					spec.loadFromFITS( fileList.at(i) );

					DWORD bytesWritten = 0;
					bResult = (WriteFile( f1, &spec, sizeof(Spectra), &bytesWritten, NULL ) > 0) ? true : false;
					if ( bResult)
					{
						c++;
					}
				}
				if ( spec.m_RealZ >= 1.5f && spec.m_RealZ <= 1.6f )
				{
					// hack: read again, since we need m_RealZ for correct loading..
					spec.loadFromFITS( fileList.at(i) );

					DWORD bytesWritten = 0;
					bResult = (WriteFile( f2, &spec, sizeof(Spectra), &bytesWritten, NULL ) > 0) ? true : false;
					if ( bResult)
					{
						c++;
					}
				}
				if ( spec.m_RealZ >= 3.6f && spec.m_RealZ <= 3.8f )
				{
					// hack: read again, since we need m_RealZ for correct loading..
					spec.loadFromFITS( fileList.at(i) );

					DWORD bytesWritten = 0;
					bResult = (WriteFile( f3, &spec, sizeof(Spectra), &bytesWritten, NULL ) > 0) ? true : false;
					if ( bResult)
					{
						c++;
					}
				}
			}
		}
		if ( !bResult )
		{
			flog<<"failed to load ";
			flog<<fileList.at(i);
			flog<<"\n";
		}
	}
	flog<<c;
	flog<<" spectra written, each ";
	flog<<sizeof(Spectra);
	flog<<" bytes, which gives a total size of ";
	flog<<static_cast<__int64>(sizeof(Spectra)*c);
	flog<<" bytes.\n";

	CloseHandle(f0);
	CloseHandle(f1);
	CloseHandle(f2);
	CloseHandle(f3);
}

void SpectraVFS::write( size_t _gridSize, float _minPeak, float _maxPeak, const std::string &_sstrFileName )
{
	size_t gridSizeSqr = _gridSize*_gridSize;
	float strengthScale = static_cast<float>(gridSizeSqr)*2.f;

	HANDLE f = CreateFile( _sstrFileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL );

	Spectra spec;
	spec.clear();
	for ( size_t i=0;i<gridSizeSqr;i++)
	{
		size_t x = i%_gridSize;
		size_t y = i/_gridSize;
		float strength = (static_cast<float>(x*x+y*y)*0.25f)/strengthScale;
		spec.randomize( _minPeak*strength, _maxPeak*strength );

		DWORD bytesWritten = 0;
		WriteFile( f, &spec, sizeof(Spectra), &bytesWritten, NULL );
	}
	CloseHandle(f);
}


void SpectraVFS::write( size_t _count, float _noize, const std::string &_sstrFileName )
{
	HANDLE f = CreateFile( _sstrFileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL );

	Spectra spec;
	spec.clear();
	float freq = 0.00001f;

	for ( size_t i=0;i<_count;i++)
	{
		spec.setSine( freq, 0.f, 1.f, _noize );
		freq += 0.000000125f;

		DWORD bytesWritten = 0;
		WriteFile( f, &spec, sizeof(Spectra), &bytesWritten, NULL );
	}
	CloseHandle(f);
}


SpectraVFS::IOHandle::IOHandle()
{
	reset();
}

SpectraVFS::IOHandle::IOHandle( OVERLAPPED &_overlapped )
{
	m_overlapped = _overlapped;
}

void SpectraVFS::IOHandle::set( unsigned __int32 _offsetLow, unsigned __int32 _offsetHigh )
{
#ifdef SPECTRAVFS_ASYNC_IO
	m_overlapped.hEvent = (HANDLE)1;
#else
	m_overlapped.hEvent = (HANDLE)0;
#endif
	m_overlapped.Offset = _offsetLow;
	m_overlapped.OffsetHigh = _offsetHigh;
}

void SpectraVFS::IOHandle::reset()
{
	m_overlapped.hEvent = 0;
	m_overlapped.Offset = 0;
	m_overlapped.OffsetHigh = 0;
	m_overlapped.Internal = 0;
	m_overlapped.InternalHigh = 0;
}

bool SpectraVFS::IOHandle::isSet()
{
	return ( (int)m_overlapped.hEvent == 1 );
}


void SpectraVFS::Read( size_t _nSpectraIndex, Spectra *_pDestination, bool bAsyncRead )
{
	assert( _pDestination != NULL );

	size_t nBytesToRead = TOTALCACHELINEBYTES;

	// check out of array bounds
	if ( _nSpectraIndex >= m_nNumberOfSpectra )
	{
		assert(0);
		// total missed the bounds -> skip
		return;
	}

	// check for cacheline cropping
	if ( _nSpectraIndex+CACHELINESIZE >= m_nNumberOfSpectra )
	{
		// pre condition: _nIndex < m_nNumberOfSpectra 
		nBytesToRead = (m_nNumberOfSpectra-_nSpectraIndex)*SPECTRASIZE;
	}

	DWORD ndwBytesToRead = static_cast<DWORD>(nBytesToRead);

	unsigned __int64 nOffset = static_cast<__int64>(_nSpectraIndex)*SPECTRASIZE;
	unsigned __int32 nOffsetLow, nOffsetHigh;
	UInt64toHiLow( nOffset, nOffsetLow, nOffsetHigh );
	m_IOHandle.set( nOffsetLow, nOffsetHigh );

	BOOL bReadSuccess = FALSE;

#ifdef SPECTRAVFS_ASYNC_IO
	bReadSuccess = ReadFileEx( m_FileHandle, _pDestination, ndwBytesToRead, &m_IOHandle.m_overlapped, &SpectraVFS::ReadFinished );
	
	if ( !bAsyncRead )
	{
		WaitForIO( m_IOHandle );
	}
#else
	DWORD ndwBytesRead;

	bReadSuccess = ReadFile( m_FileHandle, _pDestination, ndwBytesToRead, &ndwBytesRead, &m_IOHandle.m_overlapped );

	if ( ndwBytesRead != ndwBytesToRead )
	{
		Helpers::print( std::string("SpectraVFS::Read Error: not all bytes read.\n"), &m_logFile );
		assert(0);
	}
#endif

	if ( bReadSuccess == FALSE )
	{	
		DWORD err = GetLastError();
		Helpers::print( std::string("SpectraVFS::Read Error:")+Helpers::numberToString<DWORD>(err)+std::string("\n"), &m_logFile );
	}
}


void CALLBACK SpectraVFS::ReadFinished(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped )
{
	lpOverlapped->hEvent = 0;
}


void SpectraVFS::WaitForIO( IOHandle &_handle )
{
#ifdef SPECTRAVFS_ASYNC_IO
	//	Timer t;
	while ( _handle.isSet() )
	{
		SleepEx(1,true);
	}
	//	float time = t.GetElapsedSecs(); 
	//	printf("%f sec\n", time );
#endif
}


void SpectraVFS::Write(  size_t _nSpectraIndex, Spectra *_pSource )
{
	assert( _pSource != NULL );

	size_t nBytesToWrite = TOTALCACHELINEBYTES;

	// check out of array bounds
	if ( _nSpectraIndex >= m_nNumberOfSpectra )
	{
		// total missed the bounds -> skip
		return;
	}

	// check for cacheline cropping
	if ( _nSpectraIndex+CACHELINESIZE >= m_nNumberOfSpectra )
	{
		// pre condition: _nIndex < m_nNumberOfSpectra 
		nBytesToWrite = (m_nNumberOfSpectra-_nSpectraIndex)*SPECTRASIZE;
	}

	DWORD ndwBytesToWrite = static_cast<DWORD>(nBytesToWrite);

	unsigned __int64 nOffset = static_cast<__int64>(_nSpectraIndex)*SPECTRASIZE;
	unsigned __int32 nOffsetLow, nOffsetHigh;
	UInt64toHiLow( nOffset, nOffsetLow, nOffsetHigh );

	IOHandle writeHandle;
	writeHandle.set( nOffsetLow, nOffsetHigh );

	BOOL bWriteSuccess = FALSE;

#ifdef SPECTRAVFS_ASYNC_IO
	bWriteSuccess = WriteFileEx( m_FileHandle, _pSource, ndwBytesToWrite, &writeHandle.m_overlapped, &SpectraVFS::ReadFinished );
	WaitForIO( writeHandle );
#else
	DWORD ndwBytesWritten;
	bWriteSuccess = WriteFile( m_FileHandle, _pSource, ndwBytesToWrite, &ndwBytesWritten, &writeHandle.m_overlapped );
	if ( ndwBytesWritten != ndwBytesToWrite )
	{
		Helpers::print( std::string("SpectraVFS::Write Error: not all bytes written.\n"), &m_logFile );
		assert(0);
	}
#endif

	if ( bWriteSuccess == FALSE )
	{
		DWORD err = GetLastError();
		Helpers::print( std::string("SpectraVFS::Write Error:")+Helpers::numberToString<DWORD>(err)+std::string("\n"), &m_logFile );
	}
}
