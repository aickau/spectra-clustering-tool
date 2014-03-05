//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
//! #
//! # Copyright (c) 2009 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 11/1/2009
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  spectraVFSCached.h
//! \brief virtual filesystem for spectra, uses main memory as cache

#ifndef _SPECTRAVFSCACHED_H
#define _SPECTRAVFSCACHED_H

#include <Windows.h>
#include <string>
#include <fstream>
#include <map>

#include "debug.h"
#include "spectra.h"
#include "spectraVFS.h"


// virtual file system for a large set of homogeneous spectra.
// Only a small partition is hold in RAM (what in this context is called 'cached') using a LRU flavored caching scheme. 
// Use CACHESIZE to configure RAM usage.
class SpectraVFSCached
{
public:

	// number of cache lines
	static const size_t CACHELINES = 24;

	// construct a virtual file system by specifying a binary dump file which contains spectral data
	// _readOnly if true no data can be written
	SpectraVFSCached( const std::string &_sstrFilename, std::ofstream &_logFile, bool _bReadOnly=true );

	virtual ~SpectraVFSCached();

	// get number of spectra loaded. zero indicates some error, e.g. file not found or something.
	size_t getNumSpectra() const;

	// get spectrum for read access
	// all changes that are made to the spectrum might get lost in some circumstances.
	Spectra *beginRead( size_t _nSpectraIndex );

	// end read access. pointer will become invalid (and may used for other data).
	void endRead( size_t _nSpectraIndex );

	// get spectrum for rw access.
	Spectra *beginWrite( size_t _nSpectraIndex );

	// end rw access.
	void endWrite( size_t _nSpectraIndex );

	// prefetch a given spectrum into the cache
	void prefetch( size_t _nSpectraIndex );

	// flush all open write commits
	void flush();

private:

	static const size_t INDEX_INVALID = 0x0ffffffff;
	static const size_t SPECTRASIZE = sizeof(Spectra);								// size of each spectrum in bytes
	static const size_t TOTALCACHESIZE = SpectraVFS::CACHELINESIZE*CACHELINES;		// number of spectra should be stored in heap 
	static const size_t TOTALCACHELINEBYTES = SpectraVFS::CACHELINESIZE*SPECTRASIZE;

	STATIC_ASSERT(TOTALCACHELINEBYTES < 0x80000000);					// should be smaller than 2^31

	struct CacheTag
	{
		enum Status
		{
			STATUS_FREE,												// not in use
			STATUS_INUSE,												// in use for read / read-write access
			STATUS_FETCHING,											// is currently fetched
		};
		size_t nCacheLineIndex;											// CacheLineIndex := SpectraIndex / CACHELINESIZE 
		size_t nTimeStamp;												// higher = newer
		bool bCommitWrite;												// if true, cache line contains data which must be written to disk first
		Status status;													// status of cache line
	};

	struct IOHandle
	{
	public:
		IOHandle();
		IOHandle( OVERLAPPED &_overlapped );
		void set( uint32_t _offsetLow, uint32_t _offsetHigh );
		void reset();
		bool isSet();
		OVERLAPPED m_overlapped;
	};

	// Read cache line into cache
	// _nSpectraIndex to spectra in file
	// _pDestination to cache 
	// bAsyncRead if true read op is not blocked
	void Read( size_t _nSpectraIndex, Spectra *_pDestination, bool bAsyncRead = false );

	// write cache line to disk
	// _nSpectraIndex to spectra in file
	// _pDestination to cache 
	void Write( size_t _nSpectraIndex, Spectra *_pSource );

	// barrier
	// _handle IO Handle to wait for
	void WaitForIO( IOHandle &_handle );

	// File IO completion routine
	static void CALLBACK ReadFinished(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped );

	const bool m_bReadOnly;												// global read only flag
	size_t m_nNumberOfSpectra;											// total number of spectra (not only those which are cached)
	HANDLE m_FileHandle;												// handle to dump file 
	Spectra *m_pCache[CACHELINES];										// where the cache begins
	CacheTag m_TagTable[CACHELINES];									// tag RAM to track spectra cache lines
	size_t m_nTimeStamp;												// global timestamp
	IOHandle m_IOHandle;												// used for async IO operations
	std::ofstream &m_logFile;											// error logfile 
};

#endif // _SPECTRAVFSCACHED_H