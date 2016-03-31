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
//! \file  spectraVFSMemOnly.cpp
//! \brief virtual filesystem for spectra, in this version all spectra have to fit into main memory

#include "spectraVFSMemOnly.h"

#include "helpers.h"
#include "fileHelpers.h"
#include "helpers.h"
#include "spectra.h"
#include "Timer.h"
#include "memory.h"

#include <sstream>
#include <iostream>
#include <fstream>
#include <assert.h>



SpectraVFSMemOnly::SpectraVFSMemOnly( const std::string &_sstrFilename, std::ofstream &_logFile, bool _readOnly )
:m_sstrFilename(_sstrFilename)
,m_bReadOnly(_readOnly)
,m_nNumberOfSpectra(0)
,m_nTimeStamp(0)
,m_logFile(_logFile)
,m_pSpectaPool(NULL)
{
	const size_t fileSize = FileHelpers::getFileSize( _sstrFilename );
	const size_t fileCount = fileSize / SPECTRASIZE;
	const size_t remainder = fileSize % SPECTRASIZE;
		
	if (fileSize == 0 )
	{
		// file not found
		Helpers::print( std::string("Error: SpectraVFSMemOnly file not found ")+_sstrFilename+std::string("\n"), &m_logFile );
		return;
	}

	if  (fileSize < SPECTRASIZE || remainder != 0 )
	{
		// wrong size
		Helpers::print( std::string("Error: SpectraVFSMemOnly wrong file size for ")+_sstrFilename+". Maybe old file version because the file does not match with the size of each spectrum ("+Helpers::numberToString<size_t>(SPECTRASIZE)+std::string(" bytes).\n"), &m_logFile );
		return;
	}

	

	m_nNumberOfSpectra = static_cast<size_t>(fileCount);
	m_pSpectaPool = static_cast<Spectra*>( Memory::memAlignedAlloc( fileSize ) );

	Read( 0, m_nNumberOfSpectra, m_pSpectaPool );
}

SpectraVFSMemOnly::~SpectraVFSMemOnly()
{
	Memory::memAlignedFree( m_pSpectaPool );
}

void *SpectraVFSMemOnly::exportData() const
{
    return m_pSpectaPool;
}

size_t SpectraVFSMemOnly::getNumSpectra() const
{
	return m_nNumberOfSpectra;
}

Spectra *SpectraVFSMemOnly::beginRead( size_t _nSpectraIndex )
{
	assert( _nSpectraIndex < m_nNumberOfSpectra );
	return &m_pSpectaPool[_nSpectraIndex];
}



Spectra *SpectraVFSMemOnly::beginWrite( size_t _nSpectraIndex )
{
	assert( _nSpectraIndex < m_nNumberOfSpectra );
	if ( m_bReadOnly )
		return NULL;

	return &m_pSpectaPool[_nSpectraIndex];
}


void SpectraVFSMemOnly::endWrite( size_t _nSpectraIndex )
{
	if ( m_bReadOnly )
		return;
	// NOP
}


void SpectraVFSMemOnly::prefetch( size_t )
{
	// NOP
}


void SpectraVFSMemOnly::flush()
{
	// do not write read only spectra
	if ( m_nNumberOfSpectra > 0 && !m_bReadOnly )
	{
		Write( 0, m_nNumberOfSpectra, m_pSpectaPool );
	}
}



void SpectraVFSMemOnly::Read( size_t _nSpectraIndex, size_t _numSpectra, Spectra *_pDestination )
{
	Helpers::print("disk.read.\n");
	assert( _pDestination != NULL );

	FILE *f = fopen( m_sstrFilename.c_str(), "rb" );

	if ( f == NULL ) 
	{
		assert(0);
		Helpers::print( std::string("SpectraVFSMemOnly::Read() Error: file not found (invalid file? disk read error? device lost?).\n"), &m_logFile );
	}


	const size_t offsetInBytes = _nSpectraIndex*SPECTRASIZE;
	const size_t bytesToRead = _numSpectra*SPECTRASIZE;

	// check for out of array bounds
	if ( _nSpectraIndex+_numSpectra > m_nNumberOfSpectra )
	{
		// total missed the bounds -> skip
		assert(0);
		Helpers::print( std::string("SpectraVFSMemOnly::Read() Error: index out of range.\n"), &m_logFile );
		return;
	}


	_fseeki64( f, offsetInBytes, SEEK_SET );
	fread( &_pDestination[_nSpectraIndex], bytesToRead, 1, f );
	_fseeki64( f, 0, SEEK_SET );
	fclose(f);
}



void SpectraVFSMemOnly::Write(  size_t _nSpectraIndex, size_t _numSpectra, Spectra *_pSource )
{
	Helpers::print("disk.write.\n");
	assert( _pSource != NULL );

	FILE *f = fopen( m_sstrFilename.c_str(), "wb" );

	if ( f == NULL ) 
	{
		Helpers::print( std::string("SpectraVFSMemOnly::Write() Error: file not found (invalid file? disk read error? device lost?).\n"), &m_logFile );
	}


	const size_t offsetInBytes = _nSpectraIndex*SPECTRASIZE;
	const size_t bytesToWrite = _numSpectra*SPECTRASIZE;

	// check for out of array bounds
	if ( _nSpectraIndex+_numSpectra > m_nNumberOfSpectra )
	{
		// total missed the bounds -> skip
		assert(0);
		Helpers::print( std::string("SpectraVFSMemOnly::Write() Error: index out of range.\n"), &m_logFile );
		return;
	}


	_fseeki64( f, offsetInBytes, SEEK_SET );
	fwrite( &_pSource[_nSpectraIndex], bytesToWrite, 1, f );
	_fseeki64( f, 0, SEEK_SET );
	fclose(f);
}
