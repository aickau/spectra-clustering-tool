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
//! \file  spectraDB.cpp
//! \brief Read additional spectra info from FITs file.

#include "spectraDB.h"

#include "helpers.h"
#include "fileHelpers.h"

#include "cfitsio/fitsio.h"
#include "cfitsio/longnam.h"

#include <fstream>
#include <iostream>

bool SpectraDB::writeDB( DR dataRelease )
{
	std::string filename("specObj-dr9.fits");
	std::string outDBFilename("spectraParamDR9.bin");
	if ( dataRelease == DR10 )
	{
		filename = "specObj-dr10.fits";
		outDBFilename = "spectraParamDR10.bin";
	}

	fitsfile *f=NULL;

	// According to FITS standard, to perform a specific action, status must be always 0 before.
	int status = 0;
	int statusclose = 0; // we always want to close a given FITs file, even if a previous operation failed

	fits_open_file( &f, filename.c_str(), READONLY, &status );
	if ( status != 0 )
		return false;

	int numhdus = 0;
	int hdutype = 0; // should be BINARY_TBL
	fits_get_num_hdus( f, &numhdus, &status );
	fits_movabs_hdu( f, 2, &hdutype, &status );

	if ( numhdus<2 || hdutype != BINARY_TBL || status > 0 )
	{
		fits_close_file(f, &statusclose);
		return false;
	}


	long tblrows = 0;	// should be large, i.e. 2674200 for DR9, 3358200 for DR10
	int tblcols = 0;	// should be 127
	fits_get_num_rows( f, &tblrows, &status );
	fits_get_num_cols( f, &tblcols, &status );

	if ( tblrows<=0 && tblcols < 127 )
	{
		fits_close_file(f, &statusclose);
		return false;
	}

//	std::map<std::string,int>  hui;


	for ( int i=0;i<tblrows;i++ )
	{
		__int64 specObjID;
		Info info;
		char *arrVal[2];

		char val[4096];
		arrVal[0] = &val[0];
		arrVal[1] = NULL;

		fits_read_col( f, TSTRING, 22, i+1, 1, 1, NULL, arrVal, NULL, &status );
		specObjID = Helpers::stringToNumber<__int64>(val);

		arrVal[1] = NULL;
		fits_read_col( f, TSTRING, 37, i+1, 1, 1, NULL, arrVal, NULL, &status );
		info.type = Spectra::spectraTypeFromString(val);

// 		std::string ssstrSpectraType(val);
// 		std::map<std::string,int>::iterator it = hui.find(ssstrSpectraType);
// 		if (it == hui.end() )
// 			hui.insert(std::make_pair<std::string,int>(ssstrSpectraType,1) );


		fits_read_col( f, TDOUBLE, 65, i+1, 1, 1, NULL, &info.z, NULL, &status );
		m_spectraDB.insert( std::make_pair<__int64,Info>(specObjID, info) );
	}

// 	for ( std::map<std::string,int>::iterator it = hui.begin(); it != hui.end() ; it++ )
// 	{
// 		printf( "%s\n", it->first );
// 	}

	fits_close_file(f, &statusclose);


	std::ofstream file(outDBFilename, std::ios::out|std::ios::binary );
	if ( !file.is_open() )
	{
		return false;
	}
	
	// serialize map
	std::map<__int64,Info>::iterator it = m_spectraDB.begin();
	std::map<__int64,Info>::iterator itEnd = m_spectraDB.end();
	while ( it != itEnd )
	{
		file.write((const char*)&it->first, sizeof(__int64) );
		file.write((const char*)&it->second, sizeof(Info));
		it++;
	}
	file.close();


	return true;
}


bool SpectraDB::loadDB( DR dataRelease )
{
	// DB already loaded?
	if ( m_spectraDB.size() > 0 )
		return true;

	std::string dbFilename = "spectraParamDR9.bin";
	if ( dataRelease == DR10 )
	{
		dbFilename = "spectraParamDR10.bin";
	}

	const size_t fileSize = FileHelpers::getFileSize(dbFilename);
	const size_t elementSize = sizeof(Info)+4;

	if ( fileSize < elementSize || fileSize%elementSize != 0 )
		return false;
	
	const size_t numElements = fileSize / elementSize; 

	std::ifstream file(dbFilename, std::ios::in|std::ios::binary );
	if ( !file.is_open() )
	{
		return false;
	}
	for ( size_t i=0;i<numElements;i++ )
	{
		__int64 specObjID;
		Info info;
		file.read((char*)&specObjID, sizeof(__int64) );
		file.read((char*)&info, sizeof(Info) );
		m_spectraDB.insert( m_spectraDB.end(), std::make_pair<__int64,Info>(specObjID, info) );
	}

	return true;
}


bool SpectraDB::getInfo( __int64 _specObjID, Info &outInfo )
{
	std::map<__int64,Info>::iterator it = m_spectraDB.find(_specObjID);
	if ( it == m_spectraDB.end() )
		return false;

	outInfo = it->second;

	return true; 
}

