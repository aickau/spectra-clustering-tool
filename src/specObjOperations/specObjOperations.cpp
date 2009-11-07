//! \verbatim
//! ###########################################################################
//! # SDSS sorting prototype - Stage I
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
//! \file  specObjOperations.cpp
//! \brief program to generate binary dumps from a directory of FITS files.


#include <conio.h>

#include <sstream>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <math.h>
#include <float.h>
#include <assert.h>
#include <vector>
#include <set>
#include <map>

#include "sdsslib/helpers.h"
#include "sdsslib/filehelpers.h"
#include "sdsslib/spectraWrite.h"

typedef char _TCHAR;

//#define DATADIR std::string("G:/SDSS_ANALYZE/fits/spectro/data/*")
//#define DATADIR std::string("c:/prj/sdss_trunk/data/0266/*")
#define DATADIR std::string("d:/dr7/1d_25/*")




struct TspecObj 
{
int plate;
int mjd;
int fiberID;

};




std::string getSpecObjFileName(TspecObj &obj)
{
	// e.g. spSpec-52203-0716-002.fit
	std::string sstrFileName( "spSpec-" );

	char buf[64];
	sprintf_s( buf, "%05i", obj.mjd );
	sstrFileName += buf;
	sstrFileName += "-";

	sprintf_s( buf, "%04i", obj.plate );
	sstrFileName += buf;
	sstrFileName += "-";

	sprintf_s( buf, "%03i", obj.fiberID );
	sstrFileName += buf;
	sstrFileName += ".fit";

	return sstrFileName;
}

void main(int argc, char* argv[])
{
	std::ofstream logFile("specObjOperations_log.txt");


	std::ifstream fin("all_specObjDr4.csv");

	// skip first line
	std::string line,plateold("");
	fin >> line;
	fin >> line;
	fin >> line;


	typedef std::map<std::string,TspecObj> TSpecObjMap;
	TSpecObjMap filenameList;



	std::string plate,mjd,fiberID;


	size_t fcount=0;
	size_t c=0;
	while( fin  ) 
	{	
		TspecObj specObj;
		fin >> plate;
		fin >> mjd;
		fin >> fiberID;

		specObj.fiberID = Helpers::stringToNumber<int>(fiberID);
		specObj.plate = Helpers::stringToNumber<int>(plate);
		specObj.mjd = Helpers::stringToNumber<int>(mjd);

		std::string sstrFileName = getSpecObjFileName(specObj);
		std::pair<TSpecObjMap::iterator, bool> retVal =  filenameList.insert( std::make_pair<std::string,TspecObj>(sstrFileName, specObj));

		if ( !retVal.second )
		{
			Helpers::print( sstrFileName+std::string(" inserted twice!\n"), &logFile );
		}
		c++;
	}

	Helpers::print( Helpers::numberToString<int>(c) + std::string(" in file list.\n"), &logFile );

	std::vector<std::string> fileList;
	std::set<std::string> fileSet;


	Helpers::print( std::string("Scanning directories..\n"), &logFile );

	size_t numFiles = FileHelpers::getFileList( DATADIR, fileList );

	for ( size_t i=0;i<numFiles;i++ )
	{
		std::string &sstrFilename( fileList[i] );
		std::pair<std::set<std::string>::iterator, bool> retVal =  fileSet.insert( FileHelpers::getFileName(fileList[i]) );

		if ( !retVal.second )
		{
			Helpers::print( sstrFilename+std::string(" inserted twice!\n"), &logFile );
		}
	}
	Helpers::print( Helpers::numberToString<int>(numFiles) + std::string(" in directory ")+DATADIR+std::string(".\n"), &logFile );


	SpectroLisWriter lis(std::string("spectro.lis"));

	TSpecObjMap::iterator it = filenameList.begin();
	TSpecObjMap::iterator itEnd = filenameList.end();
	while (it != itEnd )
	{
		const std::string sstrCurrentFilename(it->first);

		std::set<std::string>::iterator itSearch = fileSet.find(sstrCurrentFilename);

		if ( itSearch == fileSet.end() )
		{
			Helpers::print( sstrCurrentFilename+std::string(" not found.\n"), &logFile );
			lis.writeEntry(it->second.mjd,it->second.plate,it->second.fiberID);
		}

		it++;
	}
		

	printf ("fin.\n" );

}

