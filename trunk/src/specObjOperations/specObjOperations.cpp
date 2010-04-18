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
//! \brief all kinds of operational stuff that has to be quick hacked


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

#include <windows.h>
#include "cfitsio/fitsio.h"
#include "cfitsio/longnam.h"

#include "sdsslib/helpers.h"
#include "sdsslib/filehelpers.h"
#include "sdsslib/spectraWrite.h"
#include "sdsslib/spectraVFS.h"
#include "sdsslib/spectraHelpers.h"
#include "sdsslib/spectra.h"

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

void trackCatalogs()
{
	const size_t gridSize(859);
	const size_t gridSizeSqr(gridSize*gridSize);

	SpectraVFS *pSourceVFS = new SpectraVFS( "allSpectra.bin", false );
	const size_t numSourceSpecra = pSourceVFS->getNumSpectra();

	for (size_t j=0;j<=200;j++)
	{
		std::string sstrCatalogueName("catalogueKoester2006");
		//std::string sstrCatalogueName("catalogueHall2002");
		const std::string sstrCatalogueName_(sstrCatalogueName+"_"+Helpers::numberToString(j,3));
		sstrCatalogueName += ".csv";

		std::ofstream logFile(std::string(sstrCatalogueName_+"_log.txt").c_str());
		std::ifstream fin(sstrCatalogueName.c_str());


		int *pIndexlist= new int[gridSize*gridSize];
		std::string sstrIndexList = "indexlist";
		sstrIndexList += Helpers::numberToString(j,4);
		sstrIndexList+= ".bin";
		FILE *f=fopen(sstrIndexList.c_str(),"rb");
		if ( f!= NULL)
		{
			fread(pIndexlist, 1, gridSizeSqr*sizeof(int), f);
			fclose(f);
		}

		std::string sstrExport("");
		sstrExport += "specFilename y x\n";
		float *pRGBMap = new float[gridSizeSqr*3];
		for ( size_t i=0;i<gridSizeSqr*3;i++)
		{
			pRGBMap[i] = 0.0f;
		}


		while (fin)
		{
			std::string sstrPlate, sstrMJD, sstrFiberID;
			fin >> sstrMJD;
			fin >> sstrPlate;
			fin >> sstrFiberID;

			const int plate = Helpers::stringToNumber<int>(sstrPlate);
			const int mjd = Helpers::stringToNumber<int>(sstrMJD);
			const int fiberID = Helpers::stringToNumber<int>(sstrFiberID);
			std::string sstrFilename( Spectra::getSpecObjFileName(plate,mjd,fiberID));

			for (size_t i=0;i<gridSizeSqr;i++)
			{
				int index = pIndexlist[i];
				if ( index > 0 && index < numSourceSpecra )
				{
					Spectra *sp = pSourceVFS->beginRead(index);
					int spFiber = sp->getFiber();
					int spPlate = sp->getPlate();
					int spMJD = sp->getMJD();
					if ( spFiber == fiberID && 
						spPlate == plate &&
						spMJD == mjd )
					{
						sstrExport += sstrFilename;
						sstrExport += " ";
						sstrExport += Helpers::numberToString(i/gridSize);
						sstrExport += " ";
						sstrExport += Helpers::numberToString(i%gridSize);
						sstrExport += "\n";

						pRGBMap[i*3] = 1.0f;
						pRGBMap[i*3+1] = (float)fiberID/640.f;
						pRGBMap[i*3+2] = (float)plate/1700.0;
						i=gridSizeSqr;
					}
					pSourceVFS->endRead(index);
				}
			}
		}
		logFile << sstrExport;

		SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, sstrCatalogueName_);


		delete[] pIndexlist;
		delete[] pRGBMap;
	}
}


void writeSpectrTypes()
{
	const size_t gridSize(859);
	const size_t gridSizeSqr(gridSize*gridSize);

	SpectraVFS *pSourceVFS = new SpectraVFS( "allSpectra.bin", false );
	const size_t numSourceSpecra = pSourceVFS->getNumSpectra();

	for (size_t j=0;j<=200;j++)
	{
		int *pIndexlist= new int[gridSize*gridSize];
		std::string sstrIndexList = "indexlist";
		std::string sstrFileName = "objectTypeMap";
		sstrIndexList += Helpers::numberToString(j,4);
		sstrFileName += Helpers::numberToString(j,4);
		sstrIndexList+= ".bin";
		FILE *f=fopen(sstrIndexList.c_str(),"rb");
		if ( f!= NULL)
		{
			fread(pIndexlist, 1, gridSizeSqr*sizeof(int), f);
			fclose(f);
		}

		float *pRGBMap = new float[gridSizeSqr*3];
		for ( size_t i=0;i<gridSizeSqr*3;i++)
		{
			pRGBMap[i] = 0.0f;
		}


		for (size_t i=0;i<gridSizeSqr;i++)
		{
			int index = pIndexlist[i];
			if ( index > 0 && index < numSourceSpecra )
			{
				Spectra *sp = pSourceVFS->beginRead(index);

				float r=0;
				float g=0;
				float b=0;

				switch ( sp->m_Type )
				{
					case Spectra::SPT_NOT_SET : r=0;g=0; b= 0.5; // dark blue
						break;
					case Spectra::SPT_SPEC_UNKNOWN : r= 1.0; g=1.0; b= 1.0; // white
						break;
					case Spectra::SPT_SPEC_STAR : r= 0.0; g=0.0; b= 1.0; // blue
						break;
					case Spectra::SPT_SPEC_GALAXY : r= 1.0; g=0.0; b= 0.0; // dark blue
						break;
					case Spectra::SPT_SPEC_QSO : r= 1.0; g=0.5; b= 0.0; // orange
						break;
					case Spectra::SPT_SPEC_HIZ_QSO : r= 1.0; g=1.0; b= 0.0; // yellow
						break;
					case Spectra::SPT_SPEC_SKY : r= 0.0; g=1.0; b= 1.0;// cyan
						break;
					case Spectra::SPT_STAR_LATE : r= 0.0; g=1.0; b= 0.0;// green
						break;
					case Spectra::SPT_GAL_EM : r= 1.0; g=0.0; b= 1.0;// violet
						break;
				}

				pRGBMap[i*3] =r;
				pRGBMap[i*3+1] = g;
				pRGBMap[i*3+2] = b;
				pSourceVFS->endRead(index);
			}
		}

		SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, sstrFileName);


		delete[] pIndexlist;
		delete[] pRGBMap;
	}
}


void writeFlux()
{
	const size_t gridSize(859);
	const size_t gridSizeSqr(gridSize*gridSize);

	SpectraVFS *pSourceVFS = new SpectraVFS( "allSpectra.bin", false );
	const size_t numSourceSpecra = pSourceVFS->getNumSpectra();

	float maxFlux = 0.0f;

	for ( size_t i=0;i<numSourceSpecra;i++)
	{
		Spectra *sp = pSourceVFS->beginRead(i);
		sp->calculateFlux();
		if ( sp->m_flux > maxFlux)
			maxFlux = sp->m_flux;
		pSourceVFS->endRead(i);
	}

	//for (size_t j=0;j<=200;j++)
	size_t j=200;
	{
		int *pIndexlist= new int[gridSize*gridSize];
		std::string sstrIndexList = "indexlist";
		std::string sstrFileName = "fluxMap";
		sstrIndexList += Helpers::numberToString(j,4);
		sstrFileName += Helpers::numberToString(j,4);
		sstrIndexList+= ".bin";
		FILE *f=fopen(sstrIndexList.c_str(),"rb");
		if ( f!= NULL)
		{
			fread(pIndexlist, 1, gridSizeSqr*sizeof(int), f);
			fclose(f);
		}

		float *pRGBMap = new float[gridSizeSqr*3];
		for ( size_t i=0;i<gridSizeSqr*3;i++)
		{
			pRGBMap[i] = 0.0f;
		}


		for (size_t i=0;i<gridSizeSqr;i++)
		{
			int index = pIndexlist[i];
			if ( index > 0 && index < numSourceSpecra )
			{
				Spectra *sp = pSourceVFS->beginRead(index);
				float intensity = log10f(sp->m_flux)/log10f(maxFlux);
				SpectraHelpers::intensityToRGB( intensity, &pRGBMap[i*3], true );			
				pSourceVFS->endRead(index);
			}
		}

		SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, sstrFileName);


		delete[] pIndexlist;
		delete[] pRGBMap;
	}
}



void writePlate()
{
	const size_t gridSize(859);
	const size_t gridSizeSqr(gridSize*gridSize);

	SpectraVFS *pSourceVFS = new SpectraVFS( "allSpectra.bin", false );
	const size_t numSourceSpecra = pSourceVFS->getNumSpectra();

	int maxPlateID=0;
	for ( size_t i=0;i<numSourceSpecra;i++)
	{
		Spectra *sp = pSourceVFS->beginRead(i);
		if ( sp->getPlate() > maxPlateID)
			maxPlateID = sp->getPlate();
		pSourceVFS->endRead(i);
	}


	//for (size_t j=0;j<=200;j++)
	size_t j=200;
	{
		int *pIndexlist= new int[gridSize*gridSize];
		std::string sstrIndexList = "indexlist";
		std::string sstrFileName = "plateIDMap";
		sstrIndexList += Helpers::numberToString(j,4);
		sstrFileName += Helpers::numberToString(j,4);
		sstrIndexList+= ".bin";
		FILE *f=fopen(sstrIndexList.c_str(),"rb");
		if ( f!= NULL)
		{
			fread(pIndexlist, 1, gridSizeSqr*sizeof(int), f);
			fclose(f);
		}

		float *pRGBMap = new float[gridSizeSqr*3];
		for ( size_t i=0;i<gridSizeSqr*3;i++)
		{
			pRGBMap[i] = 0.5f;
		}


		for (size_t i=0;i<gridSizeSqr;i++)
		{
			int index = pIndexlist[i];
			if ( index > 0 && index < numSourceSpecra )
			{
				Spectra *sp = pSourceVFS->beginRead(index);
				float intensity = (float)(sp->getPlate()-266)/(float)(maxPlateID-266);
				SpectraHelpers::intensityToRGB( intensity, &pRGBMap[i*3], false );			
				pSourceVFS->endRead(index);
			}
		}

		SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, sstrFileName);


		delete[] pIndexlist;
		delete[] pRGBMap;
	}
}




void writeMJD()
{
	const size_t gridSize(859);
	const size_t gridSizeSqr(gridSize*gridSize);

	SpectraVFS *pSourceVFS = new SpectraVFS( "allSpectra.bin", false );
	const size_t numSourceSpecra = pSourceVFS->getNumSpectra();

	int maxMJD=0;
	int minMJD=0x0ffffff;
	for ( size_t i=0;i<numSourceSpecra;i++)
	{
		Spectra *sp = pSourceVFS->beginRead(i);
		if ( sp->getMJD() > maxMJD)
			maxMJD = sp->getMJD();
		if ( sp->getMJD() < minMJD)
			minMJD = sp->getMJD();
		pSourceVFS->endRead(i);
	}


	//for (size_t j=0;j<=200;j++)
	size_t j=200;
	{
		int *pIndexlist= new int[gridSize*gridSize];
		std::string sstrIndexList = "indexlist";
		std::string sstrFileName = "MJDMap";
		sstrIndexList += Helpers::numberToString(j,4);
		sstrFileName += Helpers::numberToString(j,4);
		sstrIndexList+= ".bin";
		FILE *f=fopen(sstrIndexList.c_str(),"rb");
		if ( f!= NULL)
		{
			fread(pIndexlist, 1, gridSizeSqr*sizeof(int), f);
			fclose(f);
		}

		float *pRGBMap = new float[gridSizeSqr*3];
		for ( size_t i=0;i<gridSizeSqr*3;i++)
		{
			pRGBMap[i] = 0.5f;
		}


		for (size_t i=0;i<gridSizeSqr;i++)
		{
			int index = pIndexlist[i];
			if ( index > 0 && index < numSourceSpecra )
			{
				Spectra *sp = pSourceVFS->beginRead(index);
				float intensity = (float)(sp->getMJD()-minMJD)/(float)(maxMJD-minMJD);
				SpectraHelpers::intensityToRGB( intensity, &pRGBMap[i*3], true );			
				pSourceVFS->endRead(index);
			}
		}

		SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, sstrFileName);


		delete[] pIndexlist;
		delete[] pRGBMap;
	}
}



void writeRADEC()
{
	const size_t gridSize(859);
	const size_t gridSizeSqr(gridSize*gridSize);

	SpectraVFS *pSourceVFS = new SpectraVFS( "allSpectra.bin", false );
	const size_t numSourceSpecra = pSourceVFS->getNumSpectra();

	float maxFlux = 0.0f;

	//for (size_t j=0;j<=200;j++)
	size_t j=200;
	{
		int *pIndexlist= new int[gridSize*gridSize];
		std::string sstrIndexList = "indexlist";
		std::string sstrFileName = "RaDecMap";
		sstrIndexList += Helpers::numberToString(j,4);
		sstrFileName += Helpers::numberToString(j,4);
		sstrIndexList+= ".bin";
		FILE *f=fopen(sstrIndexList.c_str(),"rb");
		if ( f!= NULL)
		{
			fread(pIndexlist, 1, gridSizeSqr*sizeof(int), f);
			fclose(f);
		}

		float *pRGBMap = new float[gridSizeSqr*3];
		for ( size_t i=0;i<gridSizeSqr*3;i++)
		{
			pRGBMap[i] = 0.0f;
		}


		for (size_t i=0;i<gridSizeSqr;i++)
		{
			int index = pIndexlist[i];
			if ( index > 0 && index < numSourceSpecra )
			{
				Spectra *sp = pSourceVFS->beginRead(index);
				std::string sstrFileName = Spectra::getSpecObjFileName(sp->getPlate(),sp->getMJD(),sp->getFiber());
				std::string sstrPlate = Helpers::numberToString(sp->getPlate(),4);
				std::string sstrPath = "D:/dr7/1d_25/"+sstrPlate+"/1d/"+sstrFileName;


				fitsfile *f;
				int status = 0;
				float ra = 0.f;
				float dec = 0.0f;

				fits_open_file( &f, sstrPath.c_str(), READONLY, &status );
				if ( status == 0 )
				{
					fits_read_key( f, TFLOAT, "RADEG", &ra, NULL, &status );
					fits_read_key( f, TFLOAT, "DECDEG", &dec, NULL, &status );
					fits_close_file(f, &status);
				}
				
				
				// dec -30..90
				float r = (dec + 30.f)/120.f;
				float g = ra/360.f;
				float b = 0.0f;
				
				pRGBMap[i*3] = r;
				pRGBMap[i*3+1] = g;
				pRGBMap[i*3+2] = b;
				
				pSourceVFS->endRead(index);
			}
		}

		SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, sstrFileName);


		delete[] pIndexlist;
		delete[] pRGBMap;
	}
}


void spectroLisWrite()
{
	std::ofstream logFile("specObjOperations_log.txt");
	typedef std::map<std::string,TspecObj> TSpecObjMap;
	TSpecObjMap filenameList;

	std::string plate,mjd,fiberID;

	std::ifstream fin("all_specObjDr4.csv");

	// skip first line
	std::string line,plateold("");
	fin >> line;
	fin >> line;
	fin >> line;



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

		std::string sstrFileName = Spectra::getSpecObjFileName(specObj.plate, specObj.mjd, specObj.fiberID );
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
	size_t numFilesMissing = 0;
	while (it != itEnd )
	{
		const std::string sstrCurrentFilename(it->first);

		std::set<std::string>::iterator itSearch = fileSet.find(sstrCurrentFilename);

		if ( itSearch == fileSet.end() )
		{
			Helpers::print( sstrCurrentFilename+std::string(" not found.\n"), &logFile );
			lis.writeEntry(it->second.mjd,it->second.plate,it->second.fiberID);
			numFilesMissing++;
		}

		it++;
	}

	Helpers::print( Helpers::numberToString<int>(numFilesMissing) + std::string(" missing.\n"), &logFile );
}

void main(int argc, char* argv[])
{
	SpectraHelpers::init(0);

	//spectroLisWrite();
	//trackCatalogs();
	//writeSpectrTypes();
	//writeFlux();
	//writePlate();
	//writeRADEC();
	writeMJD();

	printf ("fin.\n" );

}

