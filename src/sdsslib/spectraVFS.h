#ifndef _SPECTRAVFS_H
#define _SPECTRAVFS_H

#include <Windows.h>
#include <string>

#include "spectra.h"


// virtual file system for a large set of homogeneous spectra.
// Only a small partion is hold in RAM (what in this context is called 'cached') using a LRU flavoured caching scheme. 
// Use CACHESIZE to configure RAM usage.
class SpectraVFS
{
public:

	// number of spectra per cache line
	static const size_t CACHELINESIZE = 8000;

	// number of cachelines
	static const size_t CACHELINES = 20;

	// create VFS from a set of input fits files
	// sstrDir directory where fits files can be located
	// sstrFileName filename of VFS binary dump to write
	// _spectraFilter only write specific spectra, can be any combination of Spectra::SpectraType.
	static void write( const std::string &_sstrDir, const std::string &_sstrFileName, unsigned int _spectraFilter=0x0ffffffff );

	// create grid based VFS with random noize spectra
	// _minPeak, _maxPeak = global minimum and maximum peaks in the generated spectra 
	// sstrFileName filename of VFS binary dump to write
	static void write( size_t _gridSize, float _minPeak, float _maxPeak, const std::string &_sstrFileName );

	// create test VFS 
	// _count number of test spectra to write
	// _noize overlay factor
	// sstrFileName filename of VFS binary dump to write
	static void write( size_t _count, float _noize, const std::string &_sstrFileName );

	// construct a virtual file system by specifying a binary dump file which contains spectral data
	// _readOnly if true no data can be written
	SpectraVFS( const std::string &_sstrFilename, bool _bReadOnly=true );

	~SpectraVFS();

	// filename of vfs
	std::string getFileName();

	// get number of spectra loaded. zero indicates some error, e.g. file not found or something.
	size_t getNumSpectra();

	// get spectrum for read access
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
	static const size_t SPECTRASIZE = sizeof(Spectra);
	static const size_t TOTALCACHESIZE = CACHELINESIZE*CACHELINES;		// number of spectra should be stored in heap 
	static const size_t TOTALCACHELINEBYTES = CACHELINESIZE*SPECTRASIZE;


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
		void Set( unsigned __int32 _offsetLow, unsigned __int32 _offsetHigh );
		void Reset();
		bool IsSet();
		OVERLAPPED m_overlapped;
	};

	// Read cacheline into cache
	// _nSpectraIndex to spectra in file
	// _pDestination to cache 
	// bAsyncRead if true read op is not blocked
	void Read( size_t _nSpectraIndex, Spectra *_pDestination, bool bAsyncRead = false );

	// write cacheline to disk
	// _nSpectraIndex to spectra in file
	// _pDestination to cache 
	void Write( size_t _nSpectraIndex, Spectra *_pSource );

	// barrier
	// _handle IO Handle to wait for
	void WaitForIO( IOHandle &_handle );

	// File IO completion routine
	static void CALLBACK ReadFinished(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped );

	std::string m_sstrDumpFilename;										// file where we read from (and write to)
	bool m_bReadOnly;													// global read only flag
	size_t m_nNumberOfSpectra;											// total number of spectra (not only those which are cached)
	HANDLE m_FileHandle;												// handle to dump file 
	Spectra *m_pCache[CACHELINES];										// where the cache begins
	CacheTag m_TagTable[CACHELINES];									// tag RAM to track spectra cache lines
	size_t m_nTimeStamp;												// global timestamp
	IOHandle m_IOHandle;												// used for async IO operations
};

#endif