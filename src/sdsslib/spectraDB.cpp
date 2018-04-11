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

// temporary disable FITS support under linux.
#ifndef _WIN32
#define _FITSDISABLED
#endif

#ifndef _FITSDISABLED
#include "cfitsio/fitsio.h"
#include "cfitsio/longnam.h"
#endif 

#include <fstream>
#include <iostream>

bool SpectraDB::writeDB( DR _dataRelease )
{
#ifdef _FITSDISABLED
	assert(0); // temporary disabled.
	return false;
#else

	int zColumnNumber = 65;
	int classColumnNumber = 63;
	std::string filename("specObj-dr9.fits");
	std::string outDBFilename("spectraParamDR9.bin");
	if ( _dataRelease == DR10 )
	{
		filename = "specObj-dr10.fits";
		outDBFilename = "spectraParamDR10.bin";
	} else if ( _dataRelease == DR12 ) 
	{
		filename = "specObj-dr12.fits";
		outDBFilename = "spectraParamDR12.bin";
		zColumnNumber = 66;
		classColumnNumber = 64;
	}
	else if ( _dataRelease == DR14 ) 
	{
		filename = "specObj-dr14.fits";
		outDBFilename = "spectraParamDR14.bin";
		zColumnNumber = 71;
		classColumnNumber = 69;
	}


	// specObj FITS file Documentation:
	// http://data.sdss3.org/datamodel/files/SPECTRO_REDUX/specObj.html

	fitsfile *f=NULL;

	// According to FITS standard, to perform a specific action, status must be always 0 before.
	int status = 0;
	int statusclose = 0; // we always want to close a given FITs file, even if a previous operation failed

	fits_open_file( &f, filename.c_str(), READONLY, &status );
	if ( status != 0 ) {
		printf( "SpectraDB::writeDB() Error: Cannot open file %s\n", filename );
		return false;
	}

	int numhdus = 0;
	int hdutype = 0; // should be BINARY_TBL
	fits_get_num_hdus( f, &numhdus, &status );
	fits_movabs_hdu( f, 2, &hdutype, &status ); // move to HDU1

	if ( numhdus<2 || hdutype != BINARY_TBL || status > 0 )
	{
		fits_close_file(f, &statusclose);
		return false;
	}


	long tblrows = 0;	// should be large, i.e. 2.674.200 for DR9, 3.358.200 for DR10, 4.355.200 for DR12, 4.851.200 for DR14
	int tblcols = 0;	// should be 127, 128 for DR12, 127 for DR14
	fits_get_num_rows( f, &tblrows, &status );
	fits_get_num_cols( f, &tblcols, &status );

	if ( tblrows<=0 && tblcols < 127 )
	{
		fits_close_file(f, &statusclose);
		return false;
	}

	// map spectra class, subclass to count
//	std::map<std::string,int>  spectraTypeDistribution;
	int lastPercentage =-1;
	for ( int rowNum=1;rowNum<=tblrows;rowNum++ )
	{
		uint64_t specObjID;
		Info info;
		char *arrVal[2];

		char val[4096];
		arrVal[0] = &val[0];
		arrVal[1] = NULL;

		const int percentage = 100*rowNum/tblrows;
		if ( percentage != lastPercentage )
		{
			printf("%i %%\n",percentage );
			lastPercentage = percentage;
		}


		// spec obj id
		fits_read_col( f, TSTRING, 22, rowNum, 1, 1, NULL, arrVal, NULL, &status );
		specObjID = Helpers::stringToNumber<uint64_t>(val);

		// source type
		arrVal[1] = NULL;
		fits_read_col( f, TSTRING, 37, rowNum, 1, 1, NULL, arrVal, NULL, &status );
		info.type = Spectra::spectraTypeFromString(val);

		// read class
		arrVal[1] = NULL;
		fits_read_col( f, TSTRING, classColumnNumber, rowNum, 1, 1, NULL, arrVal, NULL, &status );
		std::string sstrSpectraClass(val);
		const Spectra::SpectraClass spClass = Spectra::spectraClassFromString( sstrSpectraClass );

		// read subclass
		arrVal[1] = NULL;
		fits_read_col( f, TSTRING, classColumnNumber+1, rowNum, 1, 1, NULL, arrVal, NULL, &status );
		std::string sstrSpectraSubClass(val);
		const Spectra::SpectraSubClass spSubClass  = Spectra::spectraSubClassFromString( sstrSpectraSubClass );
		const bool spscHasBroadline = Spectra::spectraSubClassHasBroadlineFromString( sstrSpectraSubClass );
		info.spClass = Spectra::packSpectraClassAndSubclass( spClass, spSubClass, spscHasBroadline );

/*
		// to check spectra type distribution.
		std::string ssstrType = ssstrSpectraClass+" "+ssstrSpectraSubClass;
		std::map<std::string,int>::iterator it = spectraTypeDistribution.find(ssstrType);
		if (it == spectraTypeDistribution.end() )
			hui.insert(std::make_pair<std::string,int>(ssstrType,1) );
		else
			it->second++;
*/

		// apparently additional columns were inserted in DR12, therefore read z value from a different column number than in previous releases.

		// class 
		if ( info.type == Spectra::SPT_EXOTIC || info.type == Spectra::SPT_UNKNOWN ) {
			arrVal[1] = NULL;
			fits_read_col( f, TSTRING, classColumnNumber, rowNum, 1, 1, NULL, arrVal, NULL, &status );
			info.type = Spectra::spectraTypeFromString(val);
		}
		
		// apparently additional columns were inserted in DR12, therefore read z value from a different column number than in previous releases.
		fits_read_col( f, TDOUBLE, zColumnNumber, rowNum, 1, 1, NULL, &info.z, NULL, &status );
		m_spectraDB.insert(SpectraDBMap::value_type(specObjID, info) );

		if ( status != 0 ) {
			printf( "SpectraDB::writeDB() Error: Column read error at row number %i\n", rowNum );
			return false;
		}



		
	}

	/*
	// print spectra type distribution
 	for ( std::map<std::string,int>::iterator it = spectraTypeDistribution.begin(); it != spectraTypeDistribution.end() ; it++ )
 	{
 		printf( "%s\n", it->first );
 	}
	*/
	fits_close_file(f, &statusclose);


	std::ofstream file(outDBFilename, std::ios::out|std::ios::binary );
	if ( !file.is_open() )
	{
		printf( "SpectraDB::writeDB() Error: Cannot write db to file %s\n", outDBFilename );
		return false;
	}
	
	// serialize map
	SpectraDBMap::iterator it = m_spectraDB.begin();
	SpectraDBMap::iterator itEnd = m_spectraDB.end();
	while ( it != itEnd )
	{
		file.write((const char*)&it->first, sizeof(uint64_t) );
		file.write((const char*)&it->second, sizeof(Info));
		it++;
	}
	file.close();

	return true;
#endif // _WIN32
}



bool SpectraDB::loadDB( DR _dataRelease, std::ofstream *_logStream )
{
	// DB already loaded?
	if ( m_spectraDB.size() > 0 )
		return true;

	std::string dbFilename = "spectraParamDR9.bin";
	if ( _dataRelease == DR10 )
	{
		dbFilename = "spectraParamDR10.bin";
	} 
	else 	if ( _dataRelease == DR12 )
	{
		dbFilename = "spectraParamDR12.bin";
	} 
	else 	if ( _dataRelease == DR14 )
	{
		dbFilename = "spectraParamDR14.bin";
	}


	const size_t fileSize = FileHelpers::getFileSize(dbFilename);
	const size_t elementSize = sizeof(uint64_t)+sizeof(Info);


	std::ifstream file(dbFilename.c_str(), std::ios::in|std::ios::binary );
	if ( !file.is_open() )
	{
		Helpers::print("Error: Could not open "+dbFilename+ ". File missing.\n", _logStream);
		return false;
	}
	if ( fileSize < elementSize || fileSize%elementSize != 0 )
	{
		Helpers::print("Error: Wrong file size of "+dbFilename+ ". File is not matching element size.\n", _logStream);
		return false;
	}

	const size_t numElements = fileSize / elementSize; 

	for ( size_t i=0;i<numElements;i++ )
	{
		uint64_t specObjID;
		Info info;
		file.read((char*)&specObjID, sizeof(uint64_t) );
		file.read((char*)&info, sizeof(Info) );
		//m_spectraDB.insert( m_spectraDB.end(), std::make_pair<uint64_t,Info>(specObjID, info) );
		m_spectraDB.insert(SpectraDBMap::value_type(specObjID, info) );	}

	return true;
}


bool SpectraDB::getInfo(uint64_t _specObjID, Info &_outInfo )
{
	SpectraDBMap::iterator it = m_spectraDB.find(_specObjID);
	if ( it == m_spectraDB.end() )
		return false;

	_outInfo = it->second;

	return true; 
}

