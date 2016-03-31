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
//! \file  spectraVFSMemOnly.h
//! \brief virtual filesystem for spectra, in this version all spectra have to fit into main memory

#ifndef _SPECTRAVFSMEMONLY_H
#define _SPECTRAVFSMEMONLY_H

#include <string>
#include <fstream>
#include <map>

#include "debug.h"
#include "spectra.h"


// virtual file system for a large set of homogeneous spectra.
// Mem-only version, all spectra are loaded into main memory at once.
class SpectraVFSMemOnly
{
public:

	// construct a virtual file system by specifying a binary dump file which contains spectral data
	// _readOnly if true no data can be written
	SpectraVFSMemOnly(  const std::string &_sstrFilename, std::ofstream &_logFile, bool _bReadOnly=true );

	virtual ~SpectraVFSMemOnly();

	// filename of VFS
	const std::string &getFileName() const;

	// get number of spectra loaded. zero indicates some error, e.g. file not found or something.
	size_t getNumSpectra() const;

	// get spectrum for read access
	// all changes that are made to the spectrum might get lost in some circumstances.
	Spectra *beginRead( size_t _nSpectraIndex );

	// end read access. pointer will become invalid (and may used for other data).
	// here: nop
	void endRead( size_t _nSpectraIndex ) {};

	// get spectrum for rw access.
	Spectra *beginWrite( size_t _nSpectraIndex );

	// end rw access.
	void endWrite( size_t _nSpectraIndex );

	// prefetch a given spectrum into the cache
	void prefetch( size_t _nSpectraIndex );

	// flush all open write commits
	void flush();

    //export data pointer
    void *exportData() const;
private:


	// Read spectra from disk
	// _nSpectraIndex to spectra in file
	// _numSpectra to read beginning at _nSpectraIndex
	// _pDestination to  
	void Read( size_t _nSpectraIndex, size_t _numSpectra, Spectra *_pDestination );

	// write spectra to disk
	// _nSpectraIndex to spectra in file
	// _pDestination to cache 
	void Write( size_t _nSpectraIndex, size_t _numSpectra, Spectra *_pSource );


	static const size_t SPECTRASIZE = sizeof(Spectra);

	std::string m_sstrFilename;
	const bool m_bReadOnly;												// global read only flag
	size_t m_nNumberOfSpectra;											// total number of spectra (not only those which are cached)
	size_t m_nTimeStamp;												// global timestamp
	std::ofstream &m_logFile;											// error logfile 
	Spectra *m_pSpectaPool;												// memory pool for spectra
};

#endif