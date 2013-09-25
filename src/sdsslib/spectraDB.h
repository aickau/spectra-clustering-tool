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

#include <map>

class SpectraDB
{
public:
	struct Info
	{
		double						z;
		Spectra::SpectraType		type;
	};

	enum DR
	{
		DR9,
		DR10
	};

	// read FITs table and export map entries to write binary table 
	// because we do not want to read a 3 GB FITs file for every clustering process.
	// true if operation was successful.
	bool writeDB( DR dataRelease );

	// load binary DB into memory.
	bool loadDB( DR dataRelease );

	bool getInfo( int64_t _specObjID, Info &outInfo );

private:

	std::map<__int64,Info> m_spectraDB;
};


#endif

