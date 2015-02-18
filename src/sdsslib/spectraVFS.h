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
//! \file  spectraVFS.h
//! \brief virtual filesystem for spectra, uses main memory as cache

#ifndef _SPECTRAVFS_H
#define _SPECTRAVFS_H


#include <string>
#include <fstream>
#include <set>

#include "debug.h"
#include "spectra.h"

class SpectraVFSCached;
class SpectraVFSMemOnly;


// virtual file system for a large set of homogeneous spectra.
class SpectraVFS
{
public:

	// number of spectra per cache line
	static const size_t CACHELINESIZE = 14000;

	// create VFS from a set of input fits files
	// sstrDir directory where fits files can be located
	// sstrFileName filename of VFS binary dump to write
	// _spectraFilter only write specific spectra, can be any combination of Spectra::SpectraType.
	// _logStream to write log output into.
	// returns the number of written spectra.
	static size_t write( const std::string &_sstrDir, const std::string &_sstrFileName, unsigned int _spectraFilter=0x0ffffffff, std::ofstream *_logStream=NULL, std::set<std::string> *pFITSFilenameSet=NULL );

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

	virtual ~SpectraVFS();

	// filename of vfs
	const std::string &getFileName() const;

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

	// dump existing VFS to another file
	void dumpToFile( const std::string _sstrFilename );

    void *exportData() const;
private:

	std::string m_sstrDumpFilename;										// file where we read from (and write to)
	std::ofstream m_logFile;											// error logfile 

	SpectraVFSMemOnly *m_pSpectraVFS;

// #if defined(X64) || defined(__linux)
// 	SpectraVFSMemOnly *m_pSpectraVFS;				
// #else
// 	SpectraVFSCached *m_pSpectraVFS;				
// #endif // SPECTRAVFS_MEMONLY
};

#endif
