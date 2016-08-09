//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
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
//! \file  spectraVFSCached.cpp
//! \brief virtual filesystem for spectra, uses main memory as cache

#include "spectraVFSCached.h"

#include "helpers.h"
#include "fileHelpers.h"
#include "spectra.h"
#include "Timer.h"
#include "memory.h"


#include <sstream>
#include <iostream>
#include <fstream>
#ifdef WIN32
  #include <conio.h>
#endif
#include <assert.h>

#ifdef WIN32
#include <Windows.h>
#endif

//#define SPECTRAVFS_ASYNC_IO


SpectraVFSCached::SpectraVFSCached( const std::string &_sstrFilename, std::ofstream &_logFile, bool _readOnly )
:m_bReadOnly(_readOnly)
,m_nNumberOfSpectra(0)
,m_FileHandle(NULL)
,m_nTimeStamp(0)
,m_logFile(_logFile)
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
	m_FileHandle = CreateFile( _sstrFilename.c_str(), accessMode, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL );
#else
	m_FileHandle = CreateFile( _sstrFilename.c_str(), accessMode, 0, NULL, OPEN_EXISTING, 0, NULL );
#endif

	if (m_FileHandle == INVALID_HANDLE_VALUE)
	{
		// file not found
		Helpers::print( std::string("Error: SpectraVFSCached file not found ")+_sstrFilename+std::string("\n"), &m_logFile );
		return;
	}

	// determine filesize
	LARGE_INTEGER nFileSize;
	nFileSize.LowPart = 0;
	nFileSize.HighPart = 0;

	if (!GetFileSizeEx( m_FileHandle, &nFileSize ))
	{
		// some fucking error 
		Helpers::print( std::string("Error: SpectraVFSCached internal error (file size could not be evaluated for ")+_sstrFilename+std::string(").\n"), &m_logFile );
		return;
	}

	uint64_t fileSize = Helpers::HiLowtoUInt64(nFileSize.LowPart, nFileSize.HighPart);
	uint64_t spectraSize = static_cast<uint64_t>(SPECTRASIZE);
	uint64_t fileCount = fileSize / spectraSize;
	uint64_t remainder = fileSize % spectraSize;

	if ( remainder != 0 )
	{
		// wrong size
		Helpers::print( std::string("Error: SpectraVFSCached wrong file size for ")+_sstrFilename+". Maybe old file version because the file does not match with the size of each spectrum ("+Helpers::numberToString<size_t>(SPECTRASIZE)+std::string(" bytes).\n"), &m_logFile );
		return;
	}

	for ( size_t i=0;i<CACHELINES;i++)
	{
		m_pCache[i] = static_cast<Spectra*>( Memory::memAlignedAlloc( TOTALCACHELINEBYTES ) ); // new Spectra[CACHELINESIZE];

		if ( m_pCache[i] == NULL )
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

SpectraVFSCached::~SpectraVFSCached()
{
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


size_t SpectraVFSCached::getNumSpectra() const
{
	return m_nNumberOfSpectra;
}

Spectra *SpectraVFSCached::beginRead( size_t _nSpectraIndex )
{
	assert( _nSpectraIndex < m_nNumberOfSpectra );

	m_nTimeStamp++;

	size_t nCacheLineIndex = _nSpectraIndex / SpectraVFS::CACHELINESIZE;
	size_t nLineOffset = _nSpectraIndex - nCacheLineIndex*SpectraVFS::CACHELINESIZE;
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
		Write( currentTag.nCacheLineIndex*SpectraVFS::CACHELINESIZE, pCacheLineStart );
		currentTag.bCommitWrite = false;
	}

	currentTag.nCacheLineIndex = nCacheLineIndex;
	currentTag.nTimeStamp = m_nTimeStamp;
	currentTag.status = CacheTag::STATUS_INUSE;

	Read( nCacheLineIndex*SpectraVFS::CACHELINESIZE, pCacheLineStart, false );
	return &pCacheLineStart[nLineOffset];
}

void SpectraVFSCached::endRead( size_t _nSpectraIndex )
{
	size_t nCacheLineIndex = _nSpectraIndex / SpectraVFS::CACHELINESIZE;

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


Spectra *SpectraVFSCached::beginWrite( size_t _nSpectraIndex )
{
	if ( m_bReadOnly )
		return NULL;

	return beginRead( _nSpectraIndex );
}


void SpectraVFSCached::endWrite( size_t _nSpectraIndex )
{
	if ( m_bReadOnly )
		return;

	size_t nCacheLineIndex = _nSpectraIndex / SpectraVFS::CACHELINESIZE;

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


void SpectraVFSCached::prefetch( size_t _nSpectraIndex )
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

	size_t nCacheLineIndex = _nSpectraIndex / SpectraVFS::CACHELINESIZE;
	size_t nLineOffset = _nSpectraIndex - nCacheLineIndex*SpectraVFS::CACHELINESIZE;
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

		nIndexCount += SpectraVFS::CACHELINESIZE;
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
		Write( currentTag.nCacheLineIndex*SpectraVFS::CACHELINESIZE, pCacheLineStart );
		currentTag.bCommitWrite = false;
	}

	currentTag.nCacheLineIndex = nCacheLineIndex;
	currentTag.nTimeStamp = m_nTimeStamp;
	currentTag.status = CacheTag::STATUS_FETCHING;
	Read( nCacheLineIndex*SpectraVFS::CACHELINESIZE, pCacheLineStart, true );
}


void SpectraVFSCached::flush()
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
				Write( currentTag.nCacheLineIndex*SpectraVFS::CACHELINESIZE, pCacheLineStart );
				currentTag.bCommitWrite = false;
			}
		}
	}
}


SpectraVFSCached::IOHandle::IOHandle()
{
	reset();
}


SpectraVFSCached::IOHandle::IOHandle( OVERLAPPED &_overlapped )
{
	m_overlapped = _overlapped;
}

void SpectraVFSCached::IOHandle::reset()
{
	m_overlapped.hEvent = 0;
	m_overlapped.Offset = 0;
	m_overlapped.OffsetHigh = 0;
	m_overlapped.Internal = 0;
	m_overlapped.InternalHigh = 0;
}

bool SpectraVFSCached::IOHandle::isSet()
{
	return ( (int)m_overlapped.hEvent == 1 );
}


void SpectraVFSCached::Read( size_t _nSpectraIndex, Spectra *_pDestination, bool bAsyncRead )
{
	_cprintf("disk.read\n");
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
	if ( _nSpectraIndex+SpectraVFS::CACHELINESIZE >= m_nNumberOfSpectra )
	{
		// pre condition: _nIndex < m_nNumberOfSpectra 
		nBytesToRead = (m_nNumberOfSpectra-_nSpectraIndex)*SPECTRASIZE;
	}

	DWORD ndwBytesToRead = static_cast<DWORD>(nBytesToRead);

	uint64_t nOffset = static_cast<uint64_t>(_nSpectraIndex)*SPECTRASIZE;
	uint32_t nOffsetLow, nOffsetHigh;
	Helpers::UInt64toHiLow( nOffset, nOffsetLow, nOffsetHigh );
	m_IOHandle.set( nOffsetLow, nOffsetHigh );

	BOOL bReadSuccess = FALSE;

#ifdef SPECTRAVFS_ASYNC_IO
	bReadSuccess = ReadFileEx( m_FileHandle, _pDestination, ndwBytesToRead, &m_IOHandle.m_overlapped, &SpectraVFSCached::ReadFinished );

	if ( !bAsyncRead )
	{
		WaitForIO( m_IOHandle );
	}
#else
	DWORD ndwBytesRead;

	bReadSuccess = ReadFile( m_FileHandle, _pDestination, ndwBytesToRead, &ndwBytesRead, &m_IOHandle.m_overlapped );

	if ( ndwBytesRead != ndwBytesToRead )
	{
		Helpers::print( std::string("SpectraVFSCached::Read Error: not all bytes read.\n"), &m_logFile );
		assert(0);
	}
#endif

	if ( bReadSuccess == FALSE )
	{	
		DWORD err = GetLastError();
		Helpers::print( std::string("SpectraVFSCached::Read Error:")+Helpers::numberToString<DWORD>(err)+std::string("\n"), &m_logFile );
	}
}


void CALLBACK SpectraVFSCached::ReadFinished(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped )
{
	lpOverlapped->hEvent = 0;
}


void SpectraVFSCached::WaitForIO( IOHandle &_handle )
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


void SpectraVFSCached::Write(  size_t _nSpectraIndex, Spectra *_pSource )
{
	_cprintf("disk.write\n");
	assert( _pSource != NULL );

	size_t nBytesToWrite = TOTALCACHELINEBYTES;

	// check out of array bounds
	if ( _nSpectraIndex >= m_nNumberOfSpectra )
	{
		// total missed the bounds -> skip
		return;
	}

	// check for cacheline cropping
	if ( _nSpectraIndex+SpectraVFS::CACHELINESIZE >= m_nNumberOfSpectra )
	{
		// pre condition: _nIndex < m_nNumberOfSpectra 
		nBytesToWrite = (m_nNumberOfSpectra-_nSpectraIndex)*SPECTRASIZE;
	}

	DWORD ndwBytesToWrite = static_cast<DWORD>(nBytesToWrite);

	uint64_t nOffset = static_cast<uint64_t>(_nSpectraIndex)*SPECTRASIZE;
	uint32_t nOffsetLow, nOffsetHigh;
	Helpers::UInt64toHiLow( nOffset, nOffsetLow, nOffsetHigh );

	IOHandle writeHandle;
	writeHandle.set( nOffsetLow, nOffsetHigh );

	BOOL bWriteSuccess = FALSE;

#ifdef SPECTRAVFS_ASYNC_IO
	bWriteSuccess = WriteFileEx( m_FileHandle, _pSource, ndwBytesToWrite, &writeHandle.m_overlapped, &SpectraVFSCached::ReadFinished );
	WaitForIO( writeHandle );
#else
	DWORD ndwBytesWritten;
	bWriteSuccess = WriteFile( m_FileHandle, _pSource, ndwBytesToWrite, &ndwBytesWritten, &writeHandle.m_overlapped );
	if ( ndwBytesWritten != ndwBytesToWrite )
	{
		Helpers::print( std::string("SpectraVFSCached::Write Error: not all bytes written.\n"), &m_logFile );
		assert(0);
	}
#endif

	if ( bWriteSuccess == FALSE )
	{
		DWORD err = GetLastError();
		Helpers::print( std::string("SpectraVFSCached::Write Error:")+Helpers::numberToString<DWORD>(err)+std::string("\n"), &m_logFile );
	}
}
