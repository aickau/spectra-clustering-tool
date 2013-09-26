//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
//! #
//! # Copyright (c) 2009 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 5/17/2009
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  spectraWrite.h
//! \brief 


#ifndef _SPECTRAWRITE_H
#define _SPECTRAWRITE_H


#include <string>
#include <stdio.h>

class Spectra;


// Helper class for scope based writing of spectra to disk.
// Note: the file is closed on destruction of the object.
class SpectraWrite
{
public:

	// creates a file with the given filename.
	SpectraWrite( const std::string &_sstrFileName );
	virtual ~SpectraWrite();

	// writes the given spectrum to file
	// returns true on success, otherwise false.
	bool write( const Spectra &_spectra );
private:
	FILE *m_fileHandle;
};


// Helper class for writing of rsync .lis files to download from SDSS directory.
// SDSS based .lis files for spectra download look like this:
//+ ./
//+ /spectro
//+ /spectro/1d_25
//+ /spectro/1d_25/0266
//+ /spectro/1d_25/0266/1d
//+ /spectro/1d_25/0266/1d/spSpec-51630-0266-002.fit
//+ /spectro/1d_25/0266/1d/spSpec-51630-0266-004.fit
//+ /spectro/1d_25/0266/1d/spSpec-51630-0266-039.fit
//- * 
//
class SpectroLisWriter
{
public:
	SpectroLisWriter(const std::string &_sstrFilename );
	~SpectroLisWriter();
	void writeEntry( int _MJD, int _plate, int _fiberID );
private:
	std::string m_sstrFilename;
	std::string m_sstrOutTable;
	int m_currentPlate;
};


#endif