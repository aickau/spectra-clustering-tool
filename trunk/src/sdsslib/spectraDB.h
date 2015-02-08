//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
//! #
//! # Copyright (c) 2012 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 8/12/2012
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  spectraDB.h
//! \brief Read additional spectra info from FITs file.

#ifndef _SPECTRADB_H
#define _SPECTRADB_H

#include "spectra.h"
#include "defines.h"

#include <map>

// load/write additional spectra parameters that are not located in the individual spectra FITS files.
// Extra parameters are imported from specObj data model that contains the list of all spectra for a given data release, with associated parameters from the 2D and 1D pipelines for each.
// For details refer to the specObj FITS file Documentation:
//   http://data.sdss3.org/datamodel/files/SPECTRO_REDUX/specObj.html
//
// A selection of some parameters are then written back to a binary file and can be retrieved over spec obj ID.
class SpectraDB
{
public:

	// extend with other params if necessary 
	struct Info
	{
		double						z;
		Spectra::OldSpectraType		type;
		unsigned int				spClass;	//< encodes spectra class and subclass flags
												//< bits 0..2: SpectraClass 
												//< bits 3..9 SpectraSubClass
												//< bit 10 BROADLINE flag
	};

	enum DR
	{
		DR9,
		DR10,
		DR12
	};

	// read FITs table and export map entries to write binary table 
	// because we do not want to read a 3 GB FITs file for every clustering process.
	// true if operation was successful.
	bool writeDB( DR _dataRelease );

	// load binary DB into memory.
	bool loadDB( DR _dataRelease );

	// load newest DB that can be found.
	// _logStream output loading result to log file, or only to console if NULL
	bool loadNewestDB( std::ofstream *_logStream );

	bool getInfo( int64_t _specObjID, Info &_outInfo );

private:

	std::map<int64_t,Info> m_spectraDB;
};


#endif

