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
//! \file  specObjOperations.cpp
//! \brief all kinds of operational stuff that has to be quick hacked

#include "sdsslib/helpers.h"
#include "sdsslib/fileHelpers.h"
#include "sdsslib/spectraWrite.h"
#include "sdsslib/spectraVFS.h"
#include "sdsslib/spectraHelpers.h"
#include "sdsslib/spectra.h"
#include "sdsslib/defines.h"
#include "sdsslib/CSVExport.h"
#include "sdsslib/random.h"
#include "sdsslib/BigTiff.h"
#include "sdsslib/spectraDB.h"

#include "cfitsio/fitsio.h"
#include "cfitsio/longnam.h"

#include "devil/include/il/il.h"
#include "devil/include/il/ilu.h"


#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

#include <conio.h>
#include <math.h>
#include <float.h>
#include <assert.h>

#include <windows.h>


typedef char _TCHAR;

//#define DATADIR std::string("G:/SDSS_ANALYZE/fits/spectro/data/")
//#define DATADIR std::string("c:/prj/sdss_trunk/data/")
//#define DATADIR std::string("d:/dr7/1d_25/")
#define DATADIR std::string("e:/DR6SpectroFITS/")




#define TARGET_QSO_HIZ	0x00000001	 
#define TARGET_QSO_CAP	0x00000002	 
#define TARGET_QSO_SKIRT	0x00000004	 
#define TARGET_QSO_FIRST_CAP	0x00000008	 
#define TARGET_QSO_FIRST_SKIRT	0x00000010	 
#define TARGET_GALAXY_RED	0x00000020	 
#define TARGET_GALAXY	0x00000040	 
#define TARGET_GALAXY_BIG	0x00000080	 
#define TARGET_GALAXY_BRIGHT_CORE	0x00000100	 
#define TARGET_ROSAT_A	0x00000200	 
#define TARGET_ROSAT_B	0x00000400	 
#define TARGET_ROSAT_C	0x00000800	 
#define TARGET_ROSAT_D	0x00001000	 
#define TARGET_STAR_BHB	0x00002000	 
#define TARGET_STAR_CARBON	0x00004000	 
#define TARGET_STAR_BROWN_DWARF	0x00008000	 
#define TARGET_STAR_SUB_DWARF	0x00010000	 
#define TARGET_STAR_CATY_VAR	0x00020000	 
#define TARGET_STAR_RED_DWARF	0x00040000	 
#define TARGET_STAR_WHITE_DWARF	0x00080000	 
#define TARGET_SERENDIP_BLUE	0x00100000	 
#define TARGET_SERENDIP_FIRST	0x00200000	 
#define TARGET_SERENDIP_RED	0x00400000	 
#define TARGET_SERENDIP_DISTANT	0x00800000	 
#define TARGET_SERENDIP_MANUAL	0x01000000	 
#define TARGET_QSO_FAINT	0x02000000	 
#define TARGET_GALAXY_RED_II	0x04000000	 
#define TARGET_ROSAT_E	0x08000000	 
#define TARGET_STAR_PN	0x10000000	 
#define TARGET_QSO_REJECT	0x20000000	 



struct TspecObj 
{
int plate;
int mjd;
int fiberID;

};

void trackCatalogs()
{
	const bool dr8Catalog = false;
	const size_t gridSize(859);
	const size_t gridSizeSqr(gridSize*gridSize);

	SpectraVFS *pSourceVFS = new SpectraVFS( "allSpectra.bin", false );
	const size_t numSourceSpecra = pSourceVFS->getNumSpectra();
	size_t j=199;
//	for (size_t j=0;j<=200;j++)
	{
		std::string sstrCatalogueName("catalogueDownes2004");
		//std::string sstrCatalogueName("catalogueKoester2006");
	//	std::string sstrCatalogueName("catalogueHall2002");
		const std::string sstrCatalogueName_(sstrCatalogueName+"_"+Helpers::numberToString(j,3));
		sstrCatalogueName += ".csv";

		std::ofstream logFile(std::string(sstrCatalogueName_+"_log.txt").c_str());
		std::ifstream fin(sstrCatalogueName.c_str());


		int *pIndexlist= new int[gridSizeSqr];
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
			pRGBMap[i] = 1.0f;
		}

		int trackedObjectCount=0;
		int *pTargetPos = new int[gridSizeSqr]; 

		while (fin)
		{
			std::string sstrPlate, sstrMJD, sstrFiberID;
			fin >> sstrMJD;
			fin >> sstrPlate;
			fin >> sstrFiberID;

			const int plate = Helpers::stringToNumber<int>(sstrPlate);
			const int mjd = Helpers::stringToNumber<int>(sstrMJD);
			const int fiberID = Helpers::stringToNumber<int>(sstrFiberID);
			const unsigned int sid = Spectra::calcSpecObjID_DR7(plate,mjd,fiberID,0);
			std::string sstrFilename( Spectra::getSpecObjFileName(plate,mjd,fiberID, dr8Catalog));

			for (size_t i=0;i<gridSizeSqr;i++)
			{
				int index = pIndexlist[i];
				if ( index >= numSourceSpecra || index < 0) 
				{
					pRGBMap[i*3] = 0.9f;
					pRGBMap[i*3+1] = 0.9f;
					pRGBMap[i*3+2] = 0.9f;
				}
			}

			for (size_t i=0;i<gridSizeSqr;i++)
			{
				int index = pIndexlist[i];
				if ( index >= 0 && index < numSourceSpecra )
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

						pRGBMap[i*3] = 0.0f;
						pRGBMap[i*3+1] = 0.f;
						pRGBMap[i*3+2] = 0.f;

						pTargetPos[trackedObjectCount] = i; 

						trackedObjectCount++;
					}
					pSourceVFS->endRead(index);
				}
			}
		}

		logFile << sstrExport;


		// draw cross
		for (size_t i=0;i<trackedObjectCount;i++)
		{
			int ix = pTargetPos[i]%gridSize;
			int iy = pTargetPos[i]/gridSize;
			for (int k=1;k<=2;k++) {
				SETPIXEL(pRGBMap,ix+k,iy,gridSize,0.f);
				SETPIXEL(pRGBMap,ix-k,iy,gridSize,0.f);
				SETPIXEL(pRGBMap,ix,iy+k,gridSize,0.f);
				SETPIXEL(pRGBMap,ix,iy-k,gridSize,0.f);
			}
		}

		SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, sstrCatalogueName_);


		delete[] pIndexlist;
		delete[] pRGBMap;
	}
}


void writeSpectrTypes()
{
	SpectraVFS *pSourceVFS = new SpectraVFS( "allSpectra.bin", false );
	SpectraVFS *pNetworkVFS = new SpectraVFS( "sofmnet.bin", false );
	
	SpectraHelpers::writeSpectraClass( *pSourceVFS, *pNetworkVFS, "objectTypeMap" );
}

void writeZMap()
{
	SpectraVFS *pSourceVFS = new SpectraVFS( "allSpectra.bin", false );
	SpectraVFS *pNetworkVFS = new SpectraVFS( "sofmnet.bin", false );

	if ( pSourceVFS != NULL && pNetworkVFS != NULL ) 
	{
		SpectraHelpers::calcZMap( *pSourceVFS, *pNetworkVFS, "ZMap", true, false, 5 );
	}
}



void writeZMapFromIndexList()
{
	const size_t gridSize(859);
	const size_t gridSizeSqr(gridSize*gridSize);

	SpectraVFS *pSourceVFS = new SpectraVFS( "allSpectra.bin", false );
	const size_t numSourceSpecra = pSourceVFS->getNumSpectra();

	size_t j=199;
	//for (size_t j=0;j<=200;j++)
	{
		int *pIndexlist= new int[gridSize*gridSize];
		std::string sstrIndexList = "indexlist";
		std::string sstrFileName = "ZMap";
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

		// determine maximum Z
		float maxZ = 0.0f;
		for (size_t i=0;i<numSourceSpecra;i++) 
		{
			Spectra *sp = pSourceVFS->beginRead(i);
			maxZ = MAX(sp->m_Z,maxZ);
			pSourceVFS->endRead(i);
		}



		for (size_t i=0;i<gridSizeSqr;i++)
		{
			int index = pIndexlist[i];
			if ( index >= 0 && index < numSourceSpecra )
			{
				Spectra *sp = pSourceVFS->beginRead(index);

				float z = sp->m_Z;
				// logarithmic scale
				float scale = log10f( z+1.f ) / log10f( maxZ+1.f );
				SpectraHelpers::intensityToRGB( scale, &pRGBMap[i*3], true );			

				pSourceVFS->endRead(index);
			}
		}

		SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, sstrFileName);


		delete[] pIndexlist;
		delete[] pRGBMap;
	}
}


void writeOccupiedFromIndexList()
{
	const size_t gridSize(859);
	const size_t gridSizeSqr(gridSize*gridSize);

	SpectraVFS *pSourceVFS = new SpectraVFS( "allSpectra.bin", false );
	const size_t numSourceSpecra = pSourceVFS->getNumSpectra();

	size_t j=199;
	//for (size_t j=0;j<=200;j++)
	{
		int *pIndexlist= new int[gridSize*gridSize];
		std::string sstrIndexList = "indexlist";
		std::string sstrFileName = "occupied";
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
			pRGBMap[i] = 1.0f;
		}



		for (size_t i=0;i<gridSizeSqr;i++)
		{
			int index = pIndexlist[i];
			if ( index >= 0 && index < numSourceSpecra )
			{
				pRGBMap[i*3] = 0.0f;
				pRGBMap[i*3+1] = 0.0f;
				pRGBMap[i*3+2] = 0.0f;
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
	float minFlux = FLT_MAX;

	for ( size_t i=0;i<numSourceSpecra;i++)
	{
		Spectra *sp = pSourceVFS->beginRead(i);
		sp->calculateFlux();
		if ( sp->m_flux > maxFlux)
			maxFlux = sp->m_flux;
		if ( sp->m_flux < minFlux)
			minFlux = sp->m_flux;
		pSourceVFS->endRead(i);
	}

	//for (size_t j=0;j<=200;j++)
	size_t j=199;
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
			if ( index >= 0 && index < numSourceSpecra )
			{
				Spectra *sp = pSourceVFS->beginRead(index);
				float flux = sp->m_flux;
				if ( flux <= 0.0f ) {
					flux = 0.0001f;
				}
				float intensity = log10f(flux)/log10f(maxFlux);
				SpectraHelpers::intensityToRGB( intensity, &pRGBMap[i*3], true );			
				pSourceVFS->endRead(index);
			}
		}

		SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, sstrFileName);


		delete[] pIndexlist;
		delete[] pRGBMap;
	}
}

void writeMagUGRIZ()
{
	const size_t gridSize(859);
	const size_t gridSizeSqr(gridSize*gridSize);

	SpectraVFS *pSourceVFS = new SpectraVFS( "allSpectra.bin", false );
	const size_t numSourceSpecra = pSourceVFS->getNumSpectra();

	float maxFlux = 0.0f;

	//for (size_t j=0;j<=200;j++)
	size_t j=199;
	{
		int *pIndexlist= new int[gridSize*gridSize];
		std::string sstrIndexList = "indexlist";
		std::string sstrFileName1 = "MagU";
		std::string sstrFileName2 = "MagG";
		std::string sstrFileName3 = "MagR";
		std::string sstrFileName4 = "MagI";
		std::string sstrFileName5 = "MagZ";
		std::string sstrFileName6 = "UGRIZ.dat";
		sstrIndexList += Helpers::numberToString(j,4);
		sstrFileName1 += Helpers::numberToString(j,4);
		sstrFileName2 += Helpers::numberToString(j,4);
		sstrFileName3 += Helpers::numberToString(j,4);
		sstrFileName4 += Helpers::numberToString(j,4);
		sstrFileName5 += Helpers::numberToString(j,4);

		sstrIndexList+= ".bin";
		FILE *f=fopen(sstrIndexList.c_str(),"rb");
		if ( f!= NULL)
		{
			fread(pIndexlist, 1, gridSizeSqr*sizeof(int), f);
			fclose(f);
		}

		FILE *f2=fopen(sstrFileName6.c_str(),"wb");

		float *pRGBMap[5];
		float ugrizMax[5];
		float ugrizMin[5];

		// for each band..
		for ( size_t b=0;b<5;b++ ) 
		{
			pRGBMap[b] = new float[gridSizeSqr*3];
			for ( size_t i=0;i<gridSizeSqr*3;i++)
			{
				pRGBMap[b][i] = 0.5f;
			}

			ugrizMax[b] = 31.f;
			ugrizMin[b] = 14.f;
		}

		for (size_t i=0;i<gridSizeSqr;i++)
		{
			int index = pIndexlist[i];
			if ( index >= 0 && index < numSourceSpecra )
			{
				Spectra *sp = pSourceVFS->beginRead(index);
				std::string sstrFileName = sp->getFileName(); 
				std::string sstrPlate = Helpers::numberToString(sp->getPlate(),4);
				std::string sstrPath = DATADIR+sstrPlate+"/1d/"+sstrFileName;


				fitsfile *f;
				int status = 0;
				float ugriz[5]={0.f};
				char mag[1024]={0};

				fits_open_file( &f, sstrPath.c_str(), READONLY, &status );
				if ( status == 0 )
				{
					fits_read_key( f, TSTRING, "MAG", mag, NULL, &status );
					sscanf(mag,"%f %f %f %f %f", &ugriz[0], &ugriz[1], &ugriz[2], &ugriz[3], &ugriz[4] );
					fits_close_file(f, &status);
					fwrite(&ugriz[0], 5*sizeof(float), 1, f2);
				}

				for ( size_t b=0;b<5;b++ ) 
				{
					pRGBMap[b][i*3] = ugriz[b];
				}
				pSourceVFS->endRead(index);
			}

			if ( i%5000==0)
			{
				printf("%f finished\n",((float)i/(float)gridSizeSqr)*100.0);
			}
		}

		fclose(f2);


		for (size_t i=0;i<gridSizeSqr;i++)
		{
			for ( size_t b=0;b<5;b++ ) 
			{
				pRGBMap[b][i*3] -= ugrizMin[b];
				pRGBMap[b][i*3] /= (ugrizMax[b]-ugrizMin[b]);
				pRGBMap[b][i*3] = 1.f - pRGBMap[b][i*3];
				pRGBMap[b][i*3+1] = pRGBMap[b][i*3];
				pRGBMap[b][i*3+2] = pRGBMap[b][i*3];

			}
		}

		SpectraHelpers::saveIntensityMap( &pRGBMap[0][0], gridSize, gridSize, sstrFileName1);
		SpectraHelpers::saveIntensityMap( &pRGBMap[1][0], gridSize, gridSize, sstrFileName2);
		SpectraHelpers::saveIntensityMap( &pRGBMap[2][0], gridSize, gridSize, sstrFileName3);
		SpectraHelpers::saveIntensityMap( &pRGBMap[3][0], gridSize, gridSize, sstrFileName4);
		SpectraHelpers::saveIntensityMap( &pRGBMap[4][0], gridSize, gridSize, sstrFileName5);


		delete[] pIndexlist;
		for ( size_t m=0;m<5;m++ ) 
		{
			delete[] pRGBMap[m];
		}
	}
}

void writePrimTargetFromBin()
{
	const size_t gridSize(859);
	const size_t gridSizeSqr(gridSize*gridSize);

	unsigned long *primTargets= new unsigned long[608792];
	int *pIndexlist= new int[gridSize*gridSize];
	SpectraVFS *pSourceVFS = new SpectraVFS( "allSpectra.bin", false );
	const size_t numSourceSpecra = pSourceVFS->getNumSpectra();


	{
		FILE *f=fopen("indexList0199.bin","rb");
		if ( f!= NULL)
		{
			fread(pIndexlist, 1, gridSizeSqr*sizeof(int), f);
			fclose(f);
		}
	}
	{
		FILE *f=fopen("primaryTarget0199.dat","rb");
		if ( f!= NULL)
		{
			fread(primTargets, 1, 608792*sizeof(unsigned long), f);
			fclose(f);
		}
	}

	float *pRGBMap = new float[gridSizeSqr*3];
	for ( size_t i=0;i<gridSizeSqr*3;i++)
	{
		pRGBMap[i] = 0.0f;
	}
	size_t c=0;

	for (size_t i=0;i<gridSizeSqr;i++)
	{
		int index = pIndexlist[i];
		if ( index >= 0 && index < numSourceSpecra )
		{

			float r=0;
			float g=0;
			float b=0;

			const float orangeR = 204.f/255.f;
			const float orangeG = 0.72f/255.f;

			const float cyanG = 1.0f;
			const float cyanB= 1.0f;

			const int primTarget = primTargets[c];


		switch ( primTarget )
		{
				case TARGET_GALAXY_RED : r=1.f;g=0; b= 0.0; // LRG: red 1
					break;
				case TARGET_GALAXY_RED_II : r= 1.0; g=0.0; b= 0.0;//  LRG: red 
					break;
				case TARGET_GALAXY : r= orangeR; g=orangeG; b=0.0;// GAL: orange 
					break; 
				case TARGET_GALAXY_BIG : r= orangeR; g=orangeG; b= 0.0;// GAL: orange 
					break;
				case TARGET_GALAXY_BRIGHT_CORE : r= orangeR; g=orangeG; b= 0.0; // GAL: orange 
					break;
				case TARGET_QSO_HIZ : r= 0.0; g=0.0; b= 1.0; // HIZ QSO: blue 3
					break;
				case TARGET_QSO_FAINT : r= 0.0; g=cyanG; b= cyanB;// QSO: cyan 
					break;
				case TARGET_QSO_CAP : r= 0.0;  g=cyanG; b= cyanB;// QSO: cyan 
					break;
				case TARGET_QSO_REJECT :  r= 0.0;  g=cyanG; b= cyanB;// QSO: cyan 
					break;

				case TARGET_QSO_SKIRT :  r= 0.0;  g=cyanG; b= cyanB;// QSO: cyan
					break;
				case TARGET_QSO_FIRST_CAP :  r= 0.0;  g=cyanG; b= cyanB;// QSO: cyan
					break;
				case TARGET_QSO_FIRST_SKIRT :  r= 0.0;  g=cyanG; b= cyanB;// QSO: cyan 4
					break;

				case TARGET_STAR_RED_DWARF : r= 1.0; g=1.0; b= 0.0; // late-type star: yellow 
					break;
				case TARGET_STAR_BROWN_DWARF : r= 1.0; g=1.0; b= 0.0; // late-type star: yellow 5
					break;
				case TARGET_STAR_CARBON : r= 0.0; g=1.0; b= 0.0; // green 6
					break;
				case TARGET_STAR_WHITE_DWARF : r= 1.0; g=0.0; b= 1.0;// WD etc. : pink
					break;
				case TARGET_STAR_PN : r= 1.0; g=0.0; b= 1.0;// WD etc. : pink
					break;
				case TARGET_STAR_BHB : r= 1.0; g=0.0; b= 1.0;// WD etc. : pink
					break;
				case TARGET_STAR_SUB_DWARF : r= 1.0; g=0.0; b= 1.0;// WD etc. : pink 7
					break;
				case TARGET_ROSAT_A : r= 1.0; g=1.0; b= 1.0;// ROSAT, SERENDIPITY: white 8 
					break;
				case TARGET_ROSAT_B : r= 1.0; g=1.0; b= 1.0;// ROSAT, SERENDIPITY: white 8 
					break;
				case TARGET_ROSAT_C : r= 1.0; g=1.0; b= 1.0;// ROSAT, SERENDIPITY: white 8 
					break;
				case TARGET_ROSAT_D : r= 1.0; g=1.0; b= 1.0;// ROSAT, SERENDIPITY: white 8 
					break;
				case TARGET_ROSAT_E : r= 1.0; g=1.0; b= 1.0;// ROSAT, SERENDIPITY: white 8 
					break;
				case TARGET_SERENDIP_BLUE : r= 1.0; g=1.0; b= 1.0;// ROSAT, SERENDIPITY: white 8 
					break;
				case TARGET_SERENDIP_FIRST : r= 1.0; g=1.0; b= 1.0;// ROSAT, SERENDIPITY: white 8 
					break;
				case TARGET_SERENDIP_RED : r= 1.0; g=1.0; b= 1.0;// ROSAT, SERENDIPITY: white 8 
					break;
				case TARGET_SERENDIP_DISTANT : r= 1.0; g=1.0; b= 1.0;// ROSAT, SERENDIPITY: white 8 
					break;
				case TARGET_SERENDIP_MANUAL : r= 1.0; g=1.0; b= 1.0;// ROSAT, SERENDIPITY: white 8 
					break;
				default:
					{
						// we have multiple flags set, try to find  if flags matches our classes:
						if ( ( primTarget & (TARGET_ROSAT_A|TARGET_ROSAT_B|TARGET_ROSAT_C|TARGET_ROSAT_D|TARGET_ROSAT_E|TARGET_SERENDIP_BLUE|TARGET_SERENDIP_FIRST|TARGET_SERENDIP_RED|TARGET_SERENDIP_DISTANT|TARGET_SERENDIP_MANUAL)) == primTarget ) {
							r= 1.0; g=1.0; b= 1.0;// ROSAT, SERENDIPITY: white 8 
						} else if ( (primTarget & (TARGET_STAR_WHITE_DWARF|TARGET_STAR_PN|TARGET_STAR_BHB|TARGET_STAR_SUB_DWARF)) == primTarget ) {
							r= 1.0; g=0.0; b= 1.0;// WD etc. : pink
						} else if ( (primTarget & (TARGET_STAR_RED_DWARF|TARGET_STAR_BROWN_DWARF)) == primTarget ) {
							r= 1.0; g=1.0; b= 0.0; // late-type star: yellow 5
						} else if ( (primTarget & (TARGET_QSO_FAINT|TARGET_QSO_CAP|TARGET_QSO_REJECT|TARGET_QSO_SKIRT|TARGET_QSO_FIRST_CAP|TARGET_QSO_FIRST_SKIRT)) == primTarget ) {
							r= 0.0;  g=cyanG; b= cyanB;// QSO: cyan 
						} else if ( (primTarget & (TARGET_GALAXY_RED|TARGET_GALAXY_RED_II)) == primTarget ) {
							r=1.f;g=0; b= 0.0; // LRG: red 1
						} else if ( (primTarget & (TARGET_GALAXY|TARGET_GALAXY_BIG|TARGET_GALAXY_BRIGHT_CORE)) == primTarget ) {
							r= orangeR; g=orangeG; b= 0.0;// GAL: orange 
						}


					}
				}

			pRGBMap[i*3] =r;
			pRGBMap[i*3+1] = g;
			pRGBMap[i*3+2] = b;

			pSourceVFS->endRead(index);

			c++;

		}
		else
		{
			pRGBMap[i*3] = 0.3;
			pRGBMap[i*3+1] = 0.3;
			pRGBMap[i*3+2] = 0.3;
		}
	}

	SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, "primaryTarget");

}


void writePrimaryTarget()
{
	const size_t gridSize(859);
	const size_t gridSizeSqr(gridSize*gridSize);

	SpectraVFS *pSourceVFS = new SpectraVFS( "allSpectra.bin", false );
	const size_t numSourceSpecra = pSourceVFS->getNumSpectra();

	//for (size_t j=0;j<=200;j++)
	size_t j=199;
	{
		int *pIndexlist= new int[gridSize*gridSize];
		std::string sstrIndexList = "indexlist";
		sstrIndexList += Helpers::numberToString(j,4);
		sstrIndexList+= ".bin";

		std::string sstrFileName1 = "primaryTarget";
		sstrFileName1 += Helpers::numberToString(j,4);
		sstrFileName1 += ".dat";
		std::string sstrFileName2 = "primaryTargetMap";
		sstrFileName2 += Helpers::numberToString(j,4);

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


		FILE *f2=fopen(sstrFileName1.c_str(),"wb");



		for (size_t i=0;i<gridSizeSqr;i++)
		{
			int index = pIndexlist[i];
			if ( index >= 0 && index < numSourceSpecra )
			{
				Spectra *sp = pSourceVFS->beginRead(index);
				std::string sstrFileName = sp->getFileName(); 
				std::string sstrPlate = Helpers::numberToString(sp->getPlate(),4);
				std::string sstrPath = DATADIR+sstrPlate+"/1d/"+sstrFileName;


				fitsfile *f;
				int status = 0;
				unsigned long primTarget = 0;

				fits_open_file( &f, sstrPath.c_str(), READONLY, &status );
				if ( status == 0 )
				{
					fits_read_key( f, TULONG, "PRIMTARG", &primTarget, NULL, &status );
					fits_close_file(f, &status);
					fwrite(&primTarget, sizeof(unsigned long), 1, f2);
				}


				float r=0;
				float g=0;
				float b=0;

				switch ( primTarget )
				{
				case TARGET_GALAXY_RED : r=1.f;g=0; b= 0.0; // LRG: red 1
					break;
				case TARGET_GALAXY_RED_II : r= 1.0; g=0.0; b= 0.0;//  LRG: red 
					break;
				case TARGET_GALAXY : r= 1.0; g=0.5; b=0.0;// GAL: orange 
					break; 
				case TARGET_GALAXY_BIG : r= 1.0; g=0.5; b= 0.0;// GAL: orange 
					break;
				case TARGET_GALAXY_BRIGHT_CORE : r= 1.0; g=0.5; b= 0.0; // GAL: orange 
					break;
				case TARGET_QSO_HIZ : r= 1.0; g=1.0; b= 0.0; // HIZ QSO: yellow 3
					break;
				case TARGET_QSO_FAINT : r= 0.0; g=0.5; b= 1.0;// QSO: cyan 
					break;
				case TARGET_QSO_CAP : r= 0.0; g=0.5; b= 1.0;// QSO: cyan 
					break;
				case TARGET_QSO_REJECT :  r= 0.0; g=0.5; b= 1.0;// QSO: cyan 
					break;

				case TARGET_QSO_SKIRT :  r= 0.0; g=0.5; b= 1.0;// QSO: cyan
					break;
				case TARGET_QSO_FIRST_CAP :  r= 0.0; g=0.5; b= 1.0;// QSO: cyan
					break;
				case TARGET_QSO_FIRST_SKIRT :  r= 0.0; g=0.5; b= 1.0;// QSO: cyan 4
					break;

				case TARGET_STAR_RED_DWARF : r= 0.5; g=0.25; b= 0.5; // late-type star: brown 
					break;
				case TARGET_STAR_BROWN_DWARF : r= 0.5; g=0.25; b= 0.5; // late-type star: brown 5
					break;
				case TARGET_STAR_CARBON : r= 0.8; g=0.8; b= 0.8; // light grey 6
					break;
				case TARGET_STAR_WHITE_DWARF : r= 1.0; g=0.0; b= 1.0;// WD etc. : violet
					break;
				case TARGET_STAR_PN : r= 1.0; g=0.0; b= 1.0;// WD etc. : violet
					break;
				case TARGET_STAR_BHB : r= 1.0; g=0.0; b= 1.0;// WD etc. : violet
					break;
				case TARGET_STAR_SUB_DWARF : r= 1.0; g=0.0; b= 1.0;// WD etc. : violet 7
					break;
				case TARGET_ROSAT_A : r= 1.0; g=1.0; b= 1.0;// ROSAT, SERENDIPITY: white 8 
					break;
				case TARGET_ROSAT_B : r= 1.0; g=1.0; b= 1.0;// ROSAT, SERENDIPITY: white 8 
					break;
				case TARGET_ROSAT_C : r= 1.0; g=1.0; b= 1.0;// ROSAT, SERENDIPITY: white 8 
					break;
				case TARGET_ROSAT_D : r= 1.0; g=1.0; b= 1.0;// ROSAT, SERENDIPITY: white 8 
					break;
				case TARGET_ROSAT_E : r= 1.0; g=1.0; b= 1.0;// ROSAT, SERENDIPITY: white 8 
					break;
				case TARGET_SERENDIP_BLUE : r= 1.0; g=1.0; b= 1.0;// ROSAT, SERENDIPITY: white 8 
					break;
				case TARGET_SERENDIP_FIRST : r= 1.0; g=1.0; b= 1.0;// ROSAT, SERENDIPITY: white 8 
					break;
				case TARGET_SERENDIP_RED : r= 1.0; g=1.0; b= 1.0;// ROSAT, SERENDIPITY: white 8 
					break;
				case TARGET_SERENDIP_DISTANT : r= 1.0; g=1.0; b= 1.0;// ROSAT, SERENDIPITY: white 8 
					break;
				case TARGET_SERENDIP_MANUAL : r= 1.0; g=1.0; b= 1.0;// ROSAT, SERENDIPITY: white 8 
					break;
				default:
					{
	/*					// we have multiple flags set, try to find  if flags matches our classes:
						if ( ( primTarget & (TARGET_ROSAT_A|TARGET_ROSAT_B|TARGET_ROSAT_C|TARGET_ROSAT_D|TARGET_ROSAT_E|TARGET_SERENDIP_BLUE|TARGET_SERENDIP_FIRST|TARGET_SERENDIP_RED|TARGET_SERENDIP_DISTANT|TARGET_SERENDIP_MANUAL)) == primTarget ) 
						{
							r= 1.0; g=1.0; b= 1.0;// ROSAT, SERENDIPITY: white 8 
						} else if ( )
*/
					}
				}

				pRGBMap[i*3] =r;
				pRGBMap[i*3+1] = g;
				pRGBMap[i*3+2] = b;
	
				pSourceVFS->endRead(index);
			}
			else
			{
				pRGBMap[i*3] = 0.5;
				pRGBMap[i*3+1] = 0.5;
				pRGBMap[i*3+2] = 0.5;
			}


			if ( i%5000==0)
			{
				printf("%f finished\n",((float)i/(float)gridSizeSqr)*100.0);
			}

		}

		SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, sstrFileName2);

		fclose(f2);

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
	size_t j=199;
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
			if ( index >= 0 && index < numSourceSpecra )
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




void writePlate336()
{
	const size_t gridSize(859);
	const size_t gridSizeSqr(gridSize*gridSize);

	SpectraVFS *pSourceVFS = new SpectraVFS( "allSpectra.bin", false );
	const size_t numSourceSpecra = pSourceVFS->getNumSpectra();

	int objCount=0;

	//for (size_t j=0;j<=200;j++)
	size_t j=199;
	{
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

		float *pRGBMap = new float[gridSizeSqr*3];
		for ( size_t i=0;i<gridSizeSqr*3;i++)
		{
			pRGBMap[i] = 0.5f;
		}


		for (size_t i=0;i<gridSizeSqr;i++)
		{
			int index = pIndexlist[i];
			if ( index >= 0 && index < numSourceSpecra )
			{
				Spectra *sp = pSourceVFS->beginRead(index);
				if ( sp->getPlate() == 350) {
					int mjd = sp->getMJD();

					pRGBMap[i*3] = 1.0;
					pRGBMap[i*3+1] = sp->getFiber()/640.f;
					pRGBMap[i*3+2] = 0.0;
					objCount++;
				}
				pSourceVFS->endRead(index);
			}
		}

		SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, std::string("plate350"));


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
	size_t j=199;
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
			if ( index >= 0 && index < numSourceSpecra )
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
	size_t j=199;
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
			if ( index >= 0 && index < numSourceSpecra )
			{
				Spectra *sp = pSourceVFS->beginRead(index);
				std::string sstrFileName = sp->getFileName(); 
				std::string sstrPlate = Helpers::numberToString(sp->getPlate(),4);
				std::string sstrPath = DATADIR+sstrPlate+"/1d/"+sstrFileName;


				fitsfile *f;
				int status = 0;
				float ra = 0.f;
				float dec = 0.0f;

				fits_open_file( &f, sstrPath.c_str(), READONLY, &status );
				if ( status == 0 )
				{
					fits_read_key( f, TFLOAT, "RAOBJ", &ra, NULL, &status );//RADEG
					fits_read_key( f, TFLOAT, "DECOBJ", &dec, NULL, &status );//DECDEG
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

			if ( i%5000==0)
			{
				printf("%f finished\n",((float)i/(float)gridSizeSqr)*100.0);
			}
		}

		SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, sstrFileName);


		delete[] pIndexlist;
		delete[] pRGBMap;
	}
}



void writeOtherZValues()
{
	const size_t gridSize(859);
	const size_t gridSizeSqr(gridSize*gridSize);

	SpectraVFS *pSourceVFS = new SpectraVFS( "allSpectra.bin", false );
	const size_t numSourceSpecra = pSourceVFS->getNumSpectra();

	float maxFlux = 0.0f;

	//for (size_t j=0;j<=200;j++)
	size_t j=199;
	size_t i=0;
	{
		int *pIndexlist= new int[gridSize*gridSize];
		std::string sstrIndexList = "indexlist";
		std::string sstrFileName1 = "zErr";
		std::string sstrFileName2 = "zConf";
		std::string sstrFileName3 = "zStatus";
		std::string sstrFileName4 = "zWarning";
		sstrIndexList += Helpers::numberToString(j,4);
		sstrIndexList+= ".bin";
		FILE *f=fopen(sstrIndexList.c_str(),"rb");
		if ( f!= NULL)
		{
			fread(pIndexlist, 1, gridSizeSqr*sizeof(int), f);
			fclose(f);
		}

		float *pZErr = new float[gridSizeSqr];
		float *pZConf = new float[gridSizeSqr];
		unsigned int *pZStatus = new unsigned int[gridSizeSqr];
		unsigned int *pZWarning = new unsigned int[gridSizeSqr];


		float *pRGBMap1 = new float[gridSizeSqr*3];
		float *pRGBMap2 = new float[gridSizeSqr*3];

		float zMaxErr = -110.0;

		for (i=0;i<gridSizeSqr;i++)
		{
			int index = pIndexlist[i];
			if ( index >= 0 && index < numSourceSpecra )
			{
				Spectra *sp = pSourceVFS->beginRead(index);
				std::string sstrFileName = sp->getFileName(); 
				std::string sstrPlate = Helpers::numberToString(sp->getPlate(),4);
				std::string sstrPath = DATADIR+sstrPlate+"/1d/"+sstrFileName;


				fitsfile *f;
				int status = 0;
				float zErr = 0.f;
				float zConf = 0.0f;
				unsigned long zStatus=0;
				unsigned long zWarning=0;

				fits_open_file( &f, sstrPath.c_str(), READONLY, &status );
				if ( status == 0 )
				{
					fits_read_key( f, TFLOAT, "Z_ERR", &zErr, NULL, &status );
					fits_read_key( f, TFLOAT, "Z_CONF", &zConf, NULL, &status );
					fits_read_key( f, TULONG, "Z_STATUS", &zStatus, NULL, &status );
					fits_read_key( f, TULONG, "Z_WARNIN", &zWarning, NULL, &status );
					fits_close_file(f, &status);
				}


				pZErr[i] = zErr;
				pZConf[i] = zConf;
				pZStatus[i] = zStatus;
				pZWarning[i] = zWarning;

				if ( zErr > zMaxErr )
				{
					zMaxErr = zErr;
				}

				pSourceVFS->endRead(index);
			}

			if ( i%5000==0)
			{
				printf("%f finished\n",((float)i/(float)gridSizeSqr)*100.0);
			}
		}
		FileHelpers::writeFile("zErr",pZErr,gridSizeSqr*sizeof(float),true);
		FileHelpers::writeFile("zConf",pZConf,gridSizeSqr*sizeof(float),true);
		FileHelpers::writeFile("zStatus",pZStatus,gridSizeSqr*sizeof(unsigned int),true);
		FileHelpers::writeFile("zWarning",pZWarning,gridSizeSqr*sizeof(unsigned int),true);

		// z err
		for (i=0;i<gridSizeSqr;i++)
		{
			int index = pIndexlist[i];
			if (index >= 0 && index < numSourceSpecra )
			{
				float intensity_lin = pZErr[i]/zMaxErr;
				float intensity_log = 0.0f;
				if  (pZErr[i] > 0.0f )
				{
					intensity_log = fabsf(log10f(pZErr[i])/log10f(zMaxErr));
				}

				SpectraHelpers::intensityToRGB( intensity_lin, &pRGBMap1[i*3], true );			
				SpectraHelpers::intensityToRGB( intensity_log, &pRGBMap2[i*3], true );			
			}
			else
			{
				pRGBMap1[i*3] = 0.5;
				pRGBMap1[i*3+1] = 0.5;
				pRGBMap1[i*3+2] = 0.5;
				pRGBMap2[i*3] = 0.5;
				pRGBMap2[i*3+1] = 0.5;
				pRGBMap2[i*3+2] = 0.5;
			}
		}
		SpectraHelpers::saveIntensityMap( pRGBMap1, gridSize, gridSize, sstrFileName1+"_linear");
		SpectraHelpers::saveIntensityMap( pRGBMap2, gridSize, gridSize, sstrFileName1+"_log");

		// z conf
		for (i=0;i<gridSizeSqr;i++)
		{
			int index = pIndexlist[i];
			if ( index >= 0 && index < numSourceSpecra )
			{
				float intensity_lin = pZConf[i];
				float intensity_log = 0.0f;
				if  (pZConf[i] > 0.0f )
				{
					intensity_log = -log10f(pZConf[i]);
				}
				
				SpectraHelpers::intensityToRGB( intensity_lin, &pRGBMap1[i*3], true );			
				SpectraHelpers::intensityToRGB( intensity_log, &pRGBMap2[i*3], true );			
			}
			else
			{
				pRGBMap1[i*3] = 0.5;
				pRGBMap1[i*3+1] = 0.5;
				pRGBMap1[i*3+2] = 0.5;
				pRGBMap2[i*3] = 0.5;
				pRGBMap2[i*3+1] = 0.5;
				pRGBMap2[i*3+2] = 0.5;
			}
		}
		SpectraHelpers::saveIntensityMap( pRGBMap1, gridSize, gridSize, sstrFileName2+"_linear");
		SpectraHelpers::saveIntensityMap( pRGBMap2, gridSize, gridSize, sstrFileName2+"_log");


		// z status
		for (i=0;i<gridSizeSqr;i++)
		{
			int index = pIndexlist[i];
			if ( index >= 0 && index < numSourceSpecra )
			{
				float r=0;
				float g=0;
				float b=0;

				switch ( pZStatus[i] )
				{
				case Spectra::SP_ZSTATUS_NOT_MEASURED : r=0;g=0; b= 0.0; // black
					break;
				case Spectra::SP_ZSTATUS_FAILED : r= 1.0; g=0.0; b= 0.0; // red
					break;
				case Spectra::SP_ZSTATUS_INCONSISTENT : r= 0.5; g=1.0; b= 0.0; // green yellow
					break;
				case Spectra::SP_ZSTATUS_XCORR_EMLINE : r= 0.5; g=1.0; b= 0.5; // light green
					break;
				case Spectra::SP_ZSTATUS_XCORR_HIC : r= 0.0; g=1.0; b= 0.0; // green
					break;
				case Spectra::SP_ZSTATUS_XCORR_LOC : r= 0.0; g=0.5; b= 0.0; // dark green
					break;
				case Spectra::SP_ZSTATUS_EMLINE_XCORR : r= 1.0; g=1.0; b= 0.5;// light yellow
					break;
				case Spectra::SP_ZSTATUS_EMLINE_HIC : r= 1.0; g=1.0; b= 0.0;// yellow
					break;
				case Spectra::SP_ZSTATUS_EMLINE_LOC : r= 0.6; g=0.6; b= 0.0;// dark yellow
					break;
				case Spectra::SP_ZSTATUS_MANUAL_HIC : r= 0.4; g=0.4; b= 1.0;// light blue
					break;
				case Spectra::SP_ZSTATUS_MANUAL_LOC : r= 0.0; g=0.0; b= 1.0;// blue
					break;
				case Spectra::SP_ZSTATUS_4000BREAK : r= 0.0; g=1.0; b= 1.0; // cyan
					break;
				case Spectra::SP_ZSTATUS_ABLINE_CAII : r= 1.0; g=0.0; b= 1.0;// violet
					break;
				}

				pRGBMap1[i*3] =r;
				pRGBMap1[i*3+1] = g;
				pRGBMap1[i*3+2] = b;
			}
			else
			{
				pRGBMap1[i*3] = 0.5;
				pRGBMap1[i*3+1] = 0.5;
				pRGBMap1[i*3+2] = 0.5;
			}
		}
		SpectraHelpers::saveIntensityMap( pRGBMap1, gridSize, gridSize, sstrFileName3);

		// z warning
		for (i=0;i<gridSizeSqr;i++)
		{
			int index = pIndexlist[i];
			if ( index >= 0 && index < numSourceSpecra )
			{

				float r=0;
				float g=0;
				float b=0;

				if (pZWarning[i] & Spectra::SP_ZWARNING_NO_SPEC )
				{
					r=0.2; g=0.2; b= 0.2; // dark grey
				} else
				if (pZWarning[i] & Spectra::SP_ZWARNING_NO_BLUE )
				{
				r=0; g=0; b= 1.0; // blue
				} else
				if (pZWarning[i] & Spectra::SP_ZWARNING_NO_RED )
				{
				r=0; g=0; b= 0.0; // red
				} else
				if (pZWarning[i] & Spectra::SP_ZWARNING_NOT_GAL )
				{
				r=0; g=1; b= 0.0; // green
				} else
				if (pZWarning[i] & Spectra::SP_ZWARNING_NOT_QSO )
				{
				r=0; g=1; b= 1.0; // cyan
				} else
				if (pZWarning[i] & Spectra::SP_ZWARNING_NOT_STAR )
				{
				r=1; g=1; b= 0.0; // yellow
				} else
				if (pZWarning[i] & Spectra::SP_ZWARNING_GAL_COEF )
				{
				r=0.5; g=0; b= 0.5; // dark violet
				} else
				if (pZWarning[i] & Spectra::SP_ZWARNING_EMAB_INC )
				{
				r=1; g=0; b= 1.0; // violet
				} else
				if (pZWarning[i] & Spectra::SP_ZWARNING_AB_INC )
				{
				r=1; g=0.5; b= 0.0; // orange
				} else
				if (pZWarning[i] & Spectra::SP_ZWARNING_LOW_SNG )
				{
				r=0.5; g=1; b= 0.0; // light green
				} else
				if (pZWarning[i] & Spectra::SP_ZWARNING_LOW_SNR )
				{
				r=1; g=0.5; b= 0.5; // rosa
				} else
				if (pZWarning[i] & Spectra::SP_ZWARNING_LOW_SNI )
				{
				r=0.5; g=0.5; b= 1.0; // light blue
				} else
				if (pZWarning[i] & Spectra::SP_ZWARNING_4000BREAK )
				{
				r=1; g=1; b= 1.0; // white
				} else
				if (pZWarning[i] & Spectra::SP_ZWARNING_NOT_MAPPED )
				{
				r=0.2; g=0; b= 0.0; // dark red
				} else
				if (pZWarning[i] & Spectra::SP_ZWARNING_MANUAL_MAPPED )
				{
				r=0; g=0.2; b= 0.0; // dark green
				} else
				if (pZWarning[i] & Spectra::SP_ZWARNING_LOADER_MAPPED )
				{
				r=0; g=0; b= 0.2; // dark blue
				} else
				if (pZWarning[i] & Spectra::SP_ZWARNING_EM_INC )
				{
				r=0.8; g=0.8; b= 0.8; // light grey
				} else
				if (pZWarning[i] & Spectra::SP_ZWARNING_HIZ )
				{
				r=0.5; g=1; b= 0.2; // light green
				} else
				if (pZWarning[i] & Spectra::SP_ZWARNING_LOC )
				{
				r=0.2; g=0.6; b= 0.4; // dark cyan
				}

				pRGBMap1[i*3] =r;
				pRGBMap1[i*3+1] = g;
				pRGBMap1[i*3+2] = b;
			}
			else
			{
				pRGBMap1[i*3] = 0.5;
				pRGBMap1[i*3+1] = 0.5;
				pRGBMap1[i*3+2] = 0.5;
			}
		}
		SpectraHelpers::saveIntensityMap( pRGBMap1, gridSize, gridSize, sstrFileName4);


		delete[] pZErr;
		delete[] pZConf; 
		delete[] pZStatus;
		delete[] pZWarning;
		delete[] pRGBMap1;
		delete[] pRGBMap2;
	}
}


void writeSpectraVersion()
{
	const size_t gridSize(859);
	const size_t gridSizeSqr(gridSize*gridSize);

	SpectraVFS *pSourceVFS = new SpectraVFS( "allSpectra.bin", false );
	const size_t numSourceSpecra = pSourceVFS->getNumSpectra();

	float maxFlux = 0.0f;

	size_t j=200;
	{
		int *pIndexlist= new int[gridSize*gridSize];
		std::string sstrIndexList = "indexlist";
		std::string sstrFileName = "spectraVersions";
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

		std::string sstrV1;
		std::string sstrV2;
		std::string sstrV3;
		std::string sstrV4;


		for (size_t i=0;i<gridSizeSqr;i++)
		{
			int index = pIndexlist[i];
			if ( index >= 0 && index < numSourceSpecra )
			{
				Spectra *sp = pSourceVFS->beginRead(index);
				std::string sstrFileName = sp->getFileName(); 
				std::string sstrPlate = Helpers::numberToString(sp->getPlate(),4);
				std::string sstrPath = DATADIR+sstrPlate+"/1d/"+sstrFileName;


				fitsfile *f;
				int status = 0;
				float ra = 0.f;
				float dec = 0.0f;
				char vers[32]={0};
				char vers1[32]={0};
				char vers2[32]={0};

				fits_open_file( &f, sstrPath.c_str(), READONLY, &status );
				if ( status == 0 )
				{
					fits_read_key( f, TSTRING, "VERSUTIL", vers1, NULL, &status );
					fits_read_key( f, TSTRING, "VERSREAD", vers2, NULL, &status );
			//		fits_read_key( f, TSTRING, "datVERS2D", vers3, NULL, &status );
					fits_read_key( f, TSTRING, "VERSCOMB", vers, NULL, &status );
					fits_close_file(f, &status);
				}
				std::string sstrVers(vers);
				std::string sstrVers1(vers1);
				std::string sstrVers2(vers2);

				sstrV1 += sstrVers1 + "\n";
				sstrV2 += sstrVers2 + "\n";
				sstrV4 += sstrVers + "\n";

				int dr=0;
				if ( sstrVers == "v5_3_2  " ||  sstrVers == "v5_3_2")
				{
					dr=6;
				}
				if (sstrVers == "v5_3_12 " || sstrVers == "v5_3_12")
				{
					dr=7;
				}

				float r = (dr==0) ? 1: 0;
				float g = (dr==6) ? 1: 0;
				float b = (dr==7) ? 1: 0;

				pRGBMap[i*3] = r;
				pRGBMap[i*3+1] = g;
				pRGBMap[i*3+2] = b;

				pSourceVFS->endRead(index);
			}

			if ( i%5000==0)
			{
				printf("%f finished\n",((float)i/(float)gridSizeSqr)*100.0);
			}
		}

		{
			std::ofstream fon("VERSUTIL.txt");
			fon<<sstrV1;
		}
		{
			std::ofstream fon("VERSREAD.txt");
			fon<<sstrV2;
		}
		{
			std::ofstream fon("VERSCOMB.txt");
			fon<<sstrV4;
		}


		SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, sstrFileName);


		delete[] pIndexlist;
		delete[] pRGBMap;
	}
}




void writeUMatrix()
{
	SpectraVFS *pNetworkVFS = new SpectraVFS( "sofmnet.bin", false );
	if ( pNetworkVFS != NULL )
	{
		SpectraHelpers::calcUMatrix( *pNetworkVFS, "UMatrix", true, true, false, false, 1 );
	}
}

void writeIndexListFromSOFMBin()
{
	SpectraVFS *pNetworkVFS = new SpectraVFS( "sofmnet.bin", false );
	if ( pNetworkVFS != NULL )
	{
		const size_t numSpectra = pNetworkVFS->getNumSpectra();
		int *pIndexBuffer = new int[numSpectra];

		for ( size_t i=0;i<numSpectra;i++ )
		{
			Spectra *spSpec = pNetworkVFS->beginRead( i );
			pIndexBuffer[i] = spSpec->m_Index;

			pNetworkVFS->endRead( i );
		}

		FileHelpers::writeFile( std::string("indexList.bin"), static_cast<void*>(pIndexBuffer), numSpectra*sizeof(int), true );
		delete[] pIndexBuffer;
	}
}



void writeUMatrixForSource()
{
	bool _bUseLogScale = false;
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
		std::string sstrFileName = "UMatrixForSource";
		sstrIndexList += Helpers::numberToString(j,4);
		sstrFileName += Helpers::numberToString(j,4);
		sstrIndexList+= ".bin";
		FILE *f=fopen(sstrIndexList.c_str(),"rb");
		if ( f!= NULL)
		{
			fread(pIndexlist, 1, gridSizeSqr*sizeof(int), f);
			fclose(f);
		}

		float *pUMatrix = new float[gridSizeSqr];
		float *pRGBMap = new float[gridSizeSqr*3];

		float maxErr = 0.0f;

		// flash
		for (size_t i=0;i<gridSizeSqr;i++) 
		{
			pUMatrix[i] = -1.0f;
		}
		for ( size_t i=0;i<gridSizeSqr*3;i++)
		{
			pRGBMap[i] = 0.5f;
		}


		// calc errors
		for ( size_t y=1;y<gridSize-1;y++ )
		{
			for ( size_t x=1;x<gridSize-1;x++ )
			{
				const size_t i = CALC_ADRESS(x,y,gridSize);
				const size_t iLeft = CALC_ADRESS(x-1,y,gridSize);
				const size_t iRight = CALC_ADRESS(x+1,y,gridSize);
				const size_t iUp = CALC_ADRESS(x,y-1,gridSize);
				const size_t iBottom = CALC_ADRESS(x,y+1,gridSize);

				int index_i = pIndexlist[i];
				int index_iLeft = pIndexlist[iLeft];
				int index_iRight = pIndexlist[iRight];
				int index_iUp = pIndexlist[iUp];
				int index_iBottom = pIndexlist[iBottom];

				if ( index_i > 0 && index_i < numSourceSpecra )
				{
					Spectra *spCenter = pSourceVFS->beginRead( index_i );

					pUMatrix[i] = 0;

					if ( index_iLeft > 0 && index_iLeft < numSourceSpecra )
					{
						Spectra *sp = pSourceVFS->beginRead( index_iLeft );
						pUMatrix[i] += spCenter->compare( *sp );
						pSourceVFS->endRead( index_iLeft );
					}
					if ( index_iRight > 0 && index_iRight < numSourceSpecra )
					{
						Spectra *sp = pSourceVFS->beginRead( index_iRight );
						pUMatrix[i] += spCenter->compare( *sp );
						pSourceVFS->endRead( index_iRight );
					}
					if ( index_iUp > 0 && index_iUp < numSourceSpecra )
					{
						Spectra *sp = pSourceVFS->beginRead( index_iUp );
						pUMatrix[i] += spCenter->compare( *sp );
						pSourceVFS->endRead( index_iUp );
					}
					if ( index_iBottom > 0 && index_iBottom < numSourceSpecra )
					{
						Spectra *sp = pSourceVFS->beginRead( index_iBottom );
						pUMatrix[i] += spCenter->compare( *sp );
						pSourceVFS->endRead( index_iBottom );
					}
					maxErr = MAX( maxErr, pUMatrix[i] );
					pSourceVFS->endRead( index_i );
				}
			}
		}

		// normalize
		if ( maxErr > 0.0f )
		{
			for (size_t i=0;i<gridSizeSqr;i++) 
			{
				if ( pUMatrix[i] > 0 )
				{

					float scale;
					if ( _bUseLogScale )
					{
						// logarithmic scale
						scale  = log10f( pUMatrix[i]+1.f ) / log10f( maxErr+1.f );
					}
					else
					{
						// linear scale
						scale  = pUMatrix[i] /= maxErr;
					}

					SpectraHelpers::intensityToRGB( scale,  &pRGBMap[i*3] );
				}
			}
		}

		SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, sstrFileName);


		delete[] pIndexlist;
		delete[] pRGBMap;
	}
}



void spectroLisWrite()
{
	const bool dr8Catalog = false;
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

		std::string sstrFileName = Spectra::getSpecObjFileName(specObj.plate, specObj.mjd, specObj.fiberID, dr8Catalog );
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

	size_t numFiles = FileHelpers::getFileList( DATADIR+"*", fileList );

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

void test()
{	

	SSE_ALIGN Spectra spDR6;
	SSE_ALIGN Spectra spDR7;
	SSE_ALIGN Spectra spDR8;
	SSE_ALIGN Spectra spDR9;
	SSE_ALIGN Spectra spDR10_SDSS;
	SSE_ALIGN Spectra spDR10_BOSS;
	SSE_ALIGN Spectra spDR10_APOGEE;
	SSE_ALIGN Spectra spLightCurve;

	
	spDR6.loadFromFITS_SDSS("c:/sdss/r/src/3rdparty/api/cfitsio/doc/dr6spec-52199-0659-338.fit");
	spDR7.loadFromFITS_SDSS("c:/sdss/r/src/3rdparty/api/cfitsio/doc/dr7spec-51630-0266-633.fit");
	spDR8.loadFromFITS_DR8("c:/sdss/r/src/3rdparty/api/cfitsio/doc/dr8spec-2917-54556-0382.fits");
	spDR9.loadFromFITS_BOSS("c:/sdss/r/src/3rdparty/api/cfitsio/doc/dr9spec-3588-55184-0511.fits");	
	spDR10_SDSS.loadFromFITS("c:/sdss/r/src/3rdparty/api/cfitsio/doc/dr10spec_sdss-0266-51602-0003.fits");	
	spDR10_BOSS.loadFromFITS("c:/sdss/r/src/3rdparty/api/cfitsio/doc/dr10spec_BOSS-3586-55181-0002.fits");	
	spDR10_APOGEE.loadFromFITS("c:/sdss/r/src/3rdparty/api/cfitsio/doc/dr10spec_apogee-r3-4937-55760-002.fits");	
	spLightCurve.loadFromXML("c:/sdss/r2/data/xml/lightcurve.xml");
}

void writeParamsFromSelection(const std::string &sstrDataDir )
{
	std::ofstream logFile("specObjOperations_log.txt");

	Helpers::print("Write SDSS params from selection.\n\n Input:\n selection mask: mask.png\n source spectra index positions: indexlist0199.bin\n source spectra file: allSpectra.bin\n [optional] spectra in " + sstrDataDir +"\n", &logFile);


	// load mask
	ilLoadImage( (ILstring)"mask.png" );
	ILenum err = ilGetError();
	if( err != NULL )
	{
		Helpers::print("Error: could not load image.\n", &logFile); 
		return;
	}
	ilConvertImage( IL_LUMINANCE, IL_UNSIGNED_BYTE );
	int width = ilGetInteger( IL_IMAGE_WIDTH );
	int height = ilGetInteger( IL_IMAGE_HEIGHT );
	if ( width !=  height  ) {
		// wrong dimensions
		Helpers::print("Error: image not quadratic.\n", &logFile); 
		return;
	}
	unsigned char *pt = ilGetData();
	if ( pt == NULL )  {
		// nah, fail..
		Helpers::print("Error: could not load image data.\n", &logFile); 
		return;
	}

	const size_t gridSize(width);
	const size_t gridSizeSqr(gridSize*gridSize);




	SpectraVFS *pSourceVFS = new SpectraVFS( "allSpectra.bin", false );
	const size_t numSourceSpecra = pSourceVFS->getNumSpectra();
	if ( numSourceSpecra <= 0 )
	{
		Helpers::print("Error: could not load allSpectra.bin.\n", &logFile); 
		return;
	}

	size_t j=199;
	int *pIndexlist= new int[gridSize*gridSize];
	std::string sstrIndexList = "indexlist";
	sstrIndexList += Helpers::numberToString(j,4);
	
	sstrIndexList+= ".bin";


	const size_t fileSize = FileHelpers::getFileSize( sstrIndexList.c_str() );

	FILE *f=fopen(sstrIndexList.c_str(),"rb");
	if ( f== NULL) {
		Helpers::print("Error: could not load indexlist0199.bin.\n", &logFile); 
		// no index list
		return;
	}
	if ( fileSize != gridSizeSqr*4 )
	{
		Helpers::print("Error: indexlist0199.bin has  wrong filesize.\n", &logFile); 
		fclose(f);
		return;
	}

	fread(pIndexlist, 1, gridSizeSqr*sizeof(int), f);
	fclose(f);


	std::string sstrOutTable = "url; specOBJID; mjd; plate id; fiber id; z; total flux;	RAOBJ; DECOBJ; Z_ERR; Z_CONF; Z_STATUS;	Z_WARNIN; spectra type; MAG (fiber Mags ugriz)\n";

	int numObjectsSelected = 0;
	int numObjectsFound = 0;
	for (size_t i=0;i<gridSizeSqr;i++)
	{
		// if mask is selected..
		if (pt[i] > 128 ) {
			numObjectsSelected++;
			int index = pIndexlist[i];
			if ( index >= 0 && index < numSourceSpecra )
			{
				numObjectsFound++;
				Spectra *sp = pSourceVFS->beginRead(index);
				sp->calculateFlux();
				std::string sstrFileName = sp->getFileName(); 
				std::string sstrPlate = Helpers::numberToString(sp->getPlate(),4);
				std::string sstrPath = sstrDataDir+sstrPlate+"/1d/"+sstrFileName;
				std::string sstrUrl = sp->getURL();


				fitsfile *f;
				int status = 0;
				float zErr = 0.f;
				float zConf = 0.0f;
				unsigned long zStatus=0;
				unsigned long zWarning=0;
				float ra = 0.f;
				float dec = 0.0f;
				float uu = 0.0f;
				float gg = 0.0f;
				float rr = 0.0f;
				float ii = 0.0f;
				float zz = 0.0f;
				char mag[1024]={0};


				fits_open_file( &f, sstrPath.c_str(), READONLY, &status );

				Helpers::print("Opening FITs file: " +sstrPath+".\n", &logFile);
				if ( status == 0 )
				{
					fits_read_key( f, TFLOAT, "Z_ERR", &zErr, NULL, &status );

					if ( status != 0 )
					{
						Helpers::print("Dr9 Spectra. Some spectra data will be missing( z_err, z_conf, z_status, z_warning, ra, dec, mag).\n", &logFile);
						status = 0;
					}
					else
					{
						fits_read_key( f, TFLOAT, "Z_CONF", &zConf, NULL, &status );
						fits_read_key( f, TULONG, "Z_STATUS", &zStatus, NULL, &status );
						fits_read_key( f, TULONG, "Z_WARNIN", &zWarning, NULL, &status );

						fits_read_key( f, TFLOAT, "RAOBJ", &ra, NULL, &status );//RADEG
						fits_read_key( f, TFLOAT, "DECOBJ", &dec, NULL, &status );//DECDEG
						fits_read_key( f, TSTRING, "MAG", mag, NULL, &status );//DECDEG

						sscanf(mag,"%f %f %f %f %f", &uu, &gg, &rr, &ii, &zz );
					}

					fits_close_file(f, &status );
				}
				else
				{
					Helpers::print("Failed. Some spectra data will be missing( z_err, z_conf, z_status, z_warning, ra, dec, mag).\n", &logFile);
				}

				pSourceVFS->endRead(index);

		
				sstrOutTable += sstrUrl+"; ";
				sstrOutTable += Helpers::numberToString<uint64_t>(sp->m_SpecObjID)+"; ";
				sstrOutTable += Helpers::numberToString<int>(sp->getMJD())+"; ";
				sstrOutTable += Helpers::numberToString<int>(sp->getPlate())+"; ";
				sstrOutTable += Helpers::numberToString<int>(sp->getFiber())+"; ";
				sstrOutTable += Helpers::numberToString<float>(sp->m_Z)+"; ";
				
				sstrOutTable += Helpers::numberToString<float>(sp->m_flux)+"; ";

				sstrOutTable += Helpers::numberToString<float>(ra)+"; ";
				sstrOutTable += Helpers::numberToString<float>(dec)+"; ";

				sstrOutTable += Helpers::numberToString<float>(zErr)+"; ";
				sstrOutTable += Helpers::numberToString<float>(zConf)+"; ";
				sstrOutTable += Helpers::numberToString<unsigned long>(zStatus)+"; ";
				sstrOutTable += Helpers::numberToString<unsigned long>(zWarning)+"; ";
				
				sstrOutTable += Helpers::numberToString<unsigned int>(sp->m_Type)+"; ";

				sstrOutTable += Helpers::numberToString<float>(uu)+"; ";
				sstrOutTable += Helpers::numberToString<float>(gg)+"; ";
				sstrOutTable += Helpers::numberToString<float>(rr)+"; ";
				sstrOutTable += Helpers::numberToString<float>(ii)+"; ";
				sstrOutTable += Helpers::numberToString<float>(zz)+"; ";


				sstrOutTable += "\n";

			}
		}
	}

	std::string sstrsummary = Helpers::numberToString<unsigned int>(numObjectsFound)+" found from "+Helpers::numberToString<unsigned int>(numObjectsSelected)+" selected.";
	sstrOutTable += sstrsummary;

	std::ofstream fon("maskTable.csv");
	fon<<sstrOutTable;

	Helpers::print("\n\nFinish. "+sstrsummary+"\n", &logFile);
}


void medianSpectrumFromSelection()
{
	const size_t gridSize(859);
	const size_t gridSizeSqr(gridSize*gridSize);


	// load mask
	ilLoadImage( (ILstring)"mask.png" );
	ILenum err = ilGetError();
	if( err != NULL )
		return;
	ilConvertImage( IL_LUMINANCE, IL_UNSIGNED_BYTE );
	int width = ilGetInteger( IL_IMAGE_WIDTH );
	int height = ilGetInteger( IL_IMAGE_HEIGHT );
	if ( width != gridSize || height != gridSize ) {
		// wrong dimensions
		return;
	}
	unsigned char *pt = ilGetData();
	if ( pt == NULL )  {
		// nah, fail..
		return;
	}




	SpectraVFS *pSourceVFS = new SpectraVFS( "allSpectra.bin", false );
	const size_t numSourceSpecra = pSourceVFS->getNumSpectra();

	size_t j=199;
	int *pIndexlist= new int[gridSize*gridSize];
	std::string sstrIndexList = "indexlist";
	sstrIndexList += Helpers::numberToString(j,4);
	sstrIndexList+= ".bin";
	FILE *f=fopen(sstrIndexList.c_str(),"rb");
	if ( f== NULL) {
		// no index list
		return;
	}
	fread(pIndexlist, 1, gridSizeSqr*sizeof(int), f);
	fclose(f);

	
	SSE_ALIGN Spectra spAvg;
	SSE_ALIGN Spectra spMin;
	SSE_ALIGN Spectra spMax;
	spMin.add(FLT_MAX);

	size_t count = 0;
	spMax.add(-FLT_MAX);


	for (size_t i=0;i<gridSizeSqr;i++)
	{
		// if mask is selected..
		if (pt[i] > 128 ) {
			int index = pIndexlist[i];

			if ( index >= 0 && index < numSourceSpecra )
			{
				Spectra *sp = pSourceVFS->beginRead(index);

				spAvg.add( *sp );

				// min / max
				for ( size_t j=0;j<Spectra::numSamples;j++ )
				{
					if ( spMin.m_Amplitude[j] > sp->m_Amplitude[j] ) 
					{
						spMin.m_Amplitude[j] = sp->m_Amplitude[j];
					}
					if ( spMax.m_Amplitude[j] < sp->m_Amplitude[j] ) 
					{
						spMax.m_Amplitude[j] = sp->m_Amplitude[j];
					}
				}
				
				pSourceVFS->endRead(index);
				count++;

			}
		}
	}

	spAvg.multiply(1.f/static_cast<float>(count));

	CSVExport cvsExporter(std::string("spMask.csv"), ", ");

	cvsExporter.writeTableEntry(std::string("avg"));
	cvsExporter.writeTableEntry(std::string("min"));
	cvsExporter.writeTableEntry(std::string("max"));
	cvsExporter.newRow();

	for (size_t j=0;j<Spectra::numSamples;j++)
	{
		cvsExporter.writeTableEntry(spAvg.m_Amplitude[j]);
		cvsExporter.writeTableEntry(spMin.m_Amplitude[j]);
		cvsExporter.writeTableEntry(spMax.m_Amplitude[j]);
		cvsExporter.newRow();
	}

//	spAvg.saveToCSV( std::string("spAvg.csv") );
//	spMin.saveToCSV( std::string("spMin.csv") );
//	spMax.saveToCSV( std::string("spMax.csv") );
}



void extractGalaxyZooData()
{


	// photo obj id -> spec obj id
	std::map<uint64_t,uint64_t> specObjIDLookup;

	// spec obj id -> index
	std::map<uint64_t,int> indexLookup;
	std::map<uint64_t,int> indexSrcLookup;

	// our map
	const size_t gridSize = 859;
	const size_t gridSizeSqr = gridSize*gridSize;
	float *pRGBMap = new float[gridSizeSqr*3];
	for ( size_t i=0;i<gridSizeSqr*3;i++)
	{
		pRGBMap[i] = 0.0f;
	}
	/*

	for ( int y=0;y<20;y++)
	{
		for (int x=0;x<gridSize;x++)
		{
			int a = x+y*gridSize;
			a*=3;

			SpectraHelpers::intesityToRGBGradient( (float)x/(float)(gridSize-1.f), &pRGBMap[a], 0 );	 
			SpectraHelpers::intesityToRGBGradient( (float)x/(float)(gridSize-1.f), &pRGBMap[a+20*gridSize*3], 1 );	 
		}
	}
	*/
	
	size_t j=199;
	int *pIndexlist= new int[gridSize*gridSize];
	std::string sstrIndexList = "indexlist";
	sstrIndexList += Helpers::numberToString(j,4);
	sstrIndexList+= ".bin";
	FILE *ff=fopen(sstrIndexList.c_str(),"rb");
	if ( ff== NULL) {
		// no index list
		return;
	}
	fread(pIndexlist, 1, gridSizeSqr*sizeof(int), ff);
	fclose(ff);

	SpectraVFS *pSourceVFS = new SpectraVFS( "allSpectra.bin", false );
	const size_t numSourceSpecra = pSourceVFS->getNumSpectra();


	// build dictionary spec obj id -> index
	if ( pIndexlist != NULL )
	{
		for ( size_t i=0;i<gridSizeSqr;i++ )
		{
			int index = pIndexlist[i];
			if (index >= 0 && index < numSourceSpecra ) 
			{
				Spectra *spSpec = pSourceVFS->beginRead( index );
				if ( spSpec != NULL && spSpec->m_SpecObjID > 0 ) {
					indexSrcLookup.insert(std::make_pair<uint64_t,int>(spSpec->m_SpecObjID,index));
					indexLookup.insert(std::make_pair<uint64_t,int>(spSpec->m_SpecObjID,i));
				}
				pSourceVFS->endRead( index );
			}
			else
			{
				// mark empty space dark gray
				pRGBMap[i*3+0] = 0.1f;
				pRGBMap[i*3+1] = 0.1f;
				pRGBMap[i*3+2] = 0.1f;
			}
		}
	}


	// build dictionary photo obj id -> spec obj id
	int status = 0;
	fitsfile *f;
	fits_open_file( &f, "PhotoObjSpecObj.fit", READONLY, &status );
	if ( status != 0 ) {
		return;
	}

	int numhdus = 0;
	int hdutype = 0; // should be BINARY_TBL
	long tblrows = 0; // should be numLines
	int tblcols = 0; // should be 23
	uint64_t nullVal = 0;
	uint64_t photoObj, specObj;
	fits_get_num_hdus( f, &numhdus, &status );
	fits_movabs_hdu( f, 2, &hdutype, &status );


	fits_get_num_rows( f, &tblrows, &status );
	fits_get_num_cols( f, &tblcols, &status );

	for ( size_t i=0;i<tblrows;i++ )
	{
		size_t i1 = i+1;
		fits_read_col( f, TLONGLONG, 1, i1, 1, 1, &nullVal, &photoObj, NULL, &status );
		fits_read_col( f, TLONGLONG, 2, i1, 1, 1, &nullVal, &specObj, NULL, &status );

		specObjIDLookup.insert(std::make_pair<uint64_t,uint64_t>(photoObj,specObj));
	}

	fits_close_file(f, &status);


	// load galaxy zoo table & build map(s)
	std::string sstrCSV;
	const bool bSuccess = FileHelpers::loadFileToString( std::string("GalaxyZoo1_DR_table2.csv"), sstrCSV );
	if ( !bSuccess )
	{
		return;
	}

	std::stringstream fin(sstrCSV.c_str());
	std::string sstrTemp;
	getline(fin, sstrTemp, '\n');
	int mappingCount = 0;

	std::set<float,std::greater<float>> zValuesMax;
	std::set<float> zValuesMin;
	
	// the table
	// 00     01  02   03     04    05    06     07      08    09    10    11             12             13      14          15
	// OBJID, RA, DEC, NVOTE, P_EL, P_CW, P_ACW, P_EDGE, P_DK, P_MG, P_CS, P_EL_DEBIASED, P_CS_DEBIASED, SPIRAL, ELLIPTICAL, UNCERTAIN

	while( fin >> sstrTemp ) 
	{	
		std::replace(sstrTemp.begin(), sstrTemp.end(), ',', ' ');
		// row 0:PhotoObjID
		uint64_t photoObjID; 
		std::stringstream st0(sstrTemp.c_str() );
		st0 >> photoObjID;
		uint64_t specObjID=0; 

		// row 1,2,3: ra,dec,nvotes
		std::string ra,dec;
		int nVotes;
		st0 >> ra;
		st0 >> dec;
		st0 >> nVotes;
		
		// row 4,5,6,7,8,9,10 votes
		float vElliptical, vClockWise, vAntiClockwise, vEdge, vDontKnow, vMerger, vCombinedSpiral;
		st0 >> vElliptical;
		st0 >> vClockWise;
		st0 >> vAntiClockwise;
		st0 >> vEdge;
		st0 >> vDontKnow;
		st0 >> vMerger;
		st0 >> vCombinedSpiral;

		// row 11,12 debiased votes
		float vdElliptical, vdCombinedSpiral;
		st0 >> vdElliptical;
		st0 >> vdCombinedSpiral;

		// row 13,14,15 flags
		int fSpiral, fElliptical, fUncertain;
		st0 >> fSpiral;
		st0 >> fElliptical;
		st0 >> fUncertain;

		double zMax = 0.35;

		std::map<uint64_t,uint64_t>::iterator it = specObjIDLookup.find(photoObjID);
		if ( it != specObjIDLookup.end() ) {
			specObjID = it->second;
			std::map<uint64_t,int>::iterator it2 = indexSrcLookup.find(specObjID);
			if (it2 != indexSrcLookup.end() ) {
				int srcindex = it2->second;
				std::map<uint64_t,int>::iterator it3 = indexLookup.find(specObjID);
				int mapIndex = it3->second;
				Spectra *spSpec = pSourceVFS->beginRead( srcindex );

				float intensity = MIN(spSpec->m_Z/zMax,1.0);
				
				zValuesMax.insert(spSpec->m_Z);
				zValuesMin.insert(spSpec->m_Z);

// 				if ( vdElliptical >= 0.8f ) {//|| vdCombinedSpiral >= 0.8 || vMerger >= 0.4 ) {
// 
// 					SpectraHelpers::intesityToRGBGradient( (float)intensity, &pRGBMap[mapIndex*3], 1 );	
// 				} 
// 				if ( vdCombinedSpiral >= 0.8f ) {
// 					SpectraHelpers::intesityToRGBGradient( (float)intensity, &pRGBMap[mapIndex*3], 0 );	
// 				}

				// select what you want to output:
				//if ( vEdge >= 0.8 )
				//if ( vdElliptical >= 0.8 )
				//if ( fElliptical == 1 )
				//if ( vElliptical >= 0.8 )
				//if ( vdCombinedSpiral >= 0.8 )
				//if ( fSpiral == 1 )
				//if ( vCombinedSpiral >= 0.8 )
				if ( vMerger >= 0.4 )
				
				{
 					pRGBMap[mapIndex*3] = 1.f;
 					pRGBMap[mapIndex*3+1] = 1.f;
 					pRGBMap[mapIndex*3+2] = 1.f;
/* uncomment to draw cross around pixels
					// draw cross
					int ix = mapIndex%gridSize;
					int iy = mapIndex/gridSize;
					for (int k=1;k<=1;k++) {
						SETPIXEL(pRGBMap,ix+k,iy,gridSize,1.f);
						SETPIXEL(pRGBMap,ix-k,iy,gridSize,1.f);
						SETPIXEL(pRGBMap,ix,iy+k,gridSize,1.f);
						SETPIXEL(pRGBMap,ix,iy-k,gridSize,1.f);
					}
*/
				}





				pSourceVFS->endRead( srcindex );

				mappingCount++;
			}
		}

	}
	
	SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, "galaxyZooMergerVotes");

	delete[] pRGBMap;
}


void analyseSineTestDistributions()
{

	SpectraVFS *pSourceVFS = new SpectraVFS( "allSpectra.bin", false );
	const size_t numSourceSpecra = pSourceVFS->getNumSpectra();

	SpectraVFS *pNetworkVFS = new SpectraVFS( "sofmnet.bin", false );
	if ( pNetworkVFS == NULL )
	{
		return;
	}

	const size_t numSpectra = pNetworkVFS->getNumSpectra();

	// our map
	const size_t gridSize = sqrtf(numSpectra);
	const size_t gridSizeSqr = numSpectra;
	float *pRGBMap = new float[gridSizeSqr*3];
	int *pDistribMatrix = new int[gridSizeSqr];

	for ( size_t i=0;i<gridSizeSqr*3;i++)
	{
		pRGBMap[i] = 0.0f;
		if (i<gridSizeSqr)
		{
			pDistribMatrix[i]=0;
		}
	}


	// show frequency
	for ( size_t i=0;i<numSpectra;i++ )
	{
		Spectra *spSpec = pNetworkVFS->beginRead( i );
		int index = spSpec->m_Index;

		if (index >= 0 && index < numSourceSpecra ) 
		{
			Spectra *spSpec = pSourceVFS->beginRead( index );
			float intense = float(int(spSpec->m_SpecObjID >> 22))/8000.f;
			pSourceVFS->endRead( index );

			SpectraHelpers::intensityToRGB( intense, &pRGBMap[i*3], false );	

		}

		pNetworkVFS->endRead( i );
	}
	SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, "sineTestDistribution");

	// clear map again
	for ( size_t i=0;i<gridSizeSqr*3;i++)
	{
		pRGBMap[i] = 0.0f;
	}

	int maxDev = 0;

	// calc errors
	for ( size_t y=1;y<gridSize-1;y++ )
	{
		for ( size_t x=1;x<gridSize-1;x++ )
		{
			const size_t i = CALC_ADRESS(x,y,gridSize);
			const size_t iLeft = CALC_ADRESS(x-1,y,gridSize);
			const size_t iRight = CALC_ADRESS(x+1,y,gridSize);
			const size_t iUp = CALC_ADRESS(x,y-1,gridSize);
			const size_t iBottom = CALC_ADRESS(x,y+1,gridSize);

			Spectra *spSpec = pNetworkVFS->beginRead( i );
			int index_i = spSpec->m_Index;
			pNetworkVFS->endRead( i );

			spSpec = pNetworkVFS->beginRead( iLeft );
			int index_iLeft = spSpec->m_Index;
			pNetworkVFS->endRead( iLeft );

			spSpec = pNetworkVFS->beginRead( iRight );
			int index_iRight = spSpec->m_Index;
			pNetworkVFS->endRead( iRight );

			spSpec = pNetworkVFS->beginRead( iUp );
			int index_iUp = spSpec->m_Index;
			pNetworkVFS->endRead( iUp );

			spSpec = pNetworkVFS->beginRead( iBottom );
			int index_iBottom = spSpec->m_Index;
			pNetworkVFS->endRead( iBottom );

			if ( index_i > 0 && index_i < numSourceSpecra )
			{
				Spectra *spCenter = pSourceVFS->beginRead( index_i );
				int freqIndexCenter = spCenter->m_SpecObjID>>22;

				pDistribMatrix[i] = 0;

				if ( index_iLeft > 0 && index_iLeft < numSourceSpecra )
				{
					Spectra *sp = pSourceVFS->beginRead( index_iLeft );
					int freqIndexNB = sp->m_SpecObjID>>22;
					pDistribMatrix[i] += abs(freqIndexNB-freqIndexCenter);
					pSourceVFS->endRead( index_iLeft );
				}
				if ( index_iRight > 0 && index_iRight < numSourceSpecra )
				{
					Spectra *sp = pSourceVFS->beginRead( index_iRight );
					int freqIndexNB = sp->m_SpecObjID>>22;
					pDistribMatrix[i] += abs(freqIndexNB-freqIndexCenter);
					pSourceVFS->endRead( index_iRight );
				}
				if ( index_iUp > 0 && index_iUp < numSourceSpecra )
				{
					Spectra *sp = pSourceVFS->beginRead( index_iUp );
					int freqIndexNB = sp->m_SpecObjID>>22;
					pDistribMatrix[i] += abs(freqIndexNB-freqIndexCenter);
					pSourceVFS->endRead( index_iUp );
				}
				if ( index_iBottom > 0 && index_iBottom < numSourceSpecra )
				{
					Spectra *sp = pSourceVFS->beginRead( index_iBottom );
					int freqIndexNB = sp->m_SpecObjID>>22;
					pDistribMatrix[i] += abs(freqIndexNB-freqIndexCenter);
					pSourceVFS->endRead( index_iBottom );
				}
				pSourceVFS->endRead( index_i );

				if ( maxDev<pDistribMatrix[i]) 
				{
					maxDev = pDistribMatrix[i];
				}
			}
		}
	}

	maxDev = 100;


	for ( size_t i=0;i<gridSizeSqr;i++)
	{
		SpectraHelpers::intensityToRGB( float(MAX(pDistribMatrix[i]-6,0))/(float)maxDev, &pRGBMap[i*3], false );	
	}

	SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, "sineTestMeanDev");

}



void clusterStatisticsSim()
{
	CSVExport e("stats.cvs");

	const int numExperiments = 1000;	
	const int maxSamples = 100;	
	float sample_x[maxSamples];
	float sample_y[maxSamples];
	float dist[maxSamples];


	for (int j=0;j<numExperiments;j++) 
	{
		Rnd r(j);

		int numSamples = 2+r.randomInt(10);	

		float mean_x=0.f, mean_y=0.f;

		for (int i=0;i<numSamples;i++)
		{
			float x = r.randomFloat();
			float y = r.randomFloat();

			sample_x[i] = x;
			sample_y[i] = y;

			mean_x += x;
			mean_y += y;
		}
		mean_x /= (float)numSamples;
		mean_y /= (float)numSamples;
 
		float meandist = 0.0f;
		for (int i=0;i<numSamples;i++)
		{
			dist[i] = sqrtf( powf(sample_x[i]-mean_x,2.f)+powf(sample_y[i]-mean_y,2.f) );
			meandist += dist[i];
		}
		meandist /= (float) numSamples;

		float maxdist = sqrtf( powf(1.f-mean_x,2.f)+powf(1.f-mean_y,2.f) );
		//float maxdist = sqrtf( 0.5f );

		float plotValue = 1.f-(meandist/maxdist);
		e.writeTableEntry(plotValue);

		e.newRow();
	}

	e.flush();
}


std::string& replaceAll(std::string& context, const std::string& from, const std::string& to)
{
	size_t lookHere = 0;
	size_t foundHere;
	while((foundHere = context.find(from, lookHere)) != std::string::npos)
	{
		context.replace(foundHere, from.size(), to);
		lookHere = foundHere + to.size();
	}
	return context;
}

void analyseMarksClusters()
{

	const size_t gridSize(859);
	const size_t gridSizeSqr(gridSize*gridSize);

	float *pRGBMap = new float[gridSizeSqr*3];
	for ( size_t i=0;i<gridSizeSqr*3;i++)
	{
		pRGBMap[i] = 0.0f;
	}

	const std::string sstrCatalogueName("outimage");


	// thats the stuff we are reading in:
	// y,x :::114,27,2
	// y,x :::115,27,2
	// y,x :::115,28,1

	std::string sstrClusterArea;
	const bool bSuccess = FileHelpers::loadFileToString( "clusterareaYX.txt", sstrClusterArea );

	std::stringstream fin(sstrClusterArea);

	std::string sstrTemp1,sstrTemp2;
	void *b = NULL;
	int count = 0;
	do {
		b = getline(fin, sstrTemp1, '\n');

		sstrTemp1.erase(0,7);
		replaceAll(sstrTemp1,","," ");



		std::istringstream sstr;
		sstr.str(sstrTemp1);

		int x,y,t;

		sstr >> x;
		sstr >> y;
		sstr >> t;

		float col = 0.5;
		if (t==2) 
		{
			col = 1.f;
		}
		SETPIXEL(pRGBMap,y,x,gridSize,col);


		count++;

	} while (b!=NULL);


	SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, sstrCatalogueName);
	delete[] pRGBMap;

}



void analyseMarksClusters2( int clusternum )
{
	const size_t gridSize(859);
	const size_t gridSizeSqr(gridSize*gridSize);

	float *pRGBMap = new float[gridSizeSqr*3];
	for ( size_t i=0;i<gridSizeSqr*3;i++)
	{
		pRGBMap[i] = 0.0f;
	}

	const std::string sstrCatalogueName("outimage");

	CSVExport e("stats.cvs");

	// thats the stuff we are reading in:
	// y,x :::114,27,2
	// y,x :::115,27,2
	// y,x :::115,28,1

	std::string sstrClusterArea;
	const bool bSuccess = FileHelpers::loadFileToString( "cluster.10.txt", sstrClusterArea );

	std::stringstream fin(sstrClusterArea);

	std::string sstrTemp1,sstrTemp2;
	void *b = NULL;
	int count = 0;
	std::string strHeader("cluster ");
	strHeader += Helpers::numberToString(clusternum);
	do {
		b = getline(fin, sstrTemp1, '\n');
		count++;
	} while (b!=NULL &&  sstrTemp1 != strHeader );
	// remove 
	b = getline(fin, sstrTemp1, '\n');
	b = getline(fin, sstrTemp1, '\n');
	sstrTemp1.erase(0,22);
	const int numObjects = Helpers::stringToNumber<int>(sstrTemp1);

	std::vector<double> cluster;
	std::vector<double> dist;
	cluster.reserve(numObjects);
	dist.reserve(numObjects);
	count =0;

	double midX = 0;
	double midY = 0;

	double minX = gridSize;
	double minY = gridSize;
	double maxX = 0;
	double maxY = 0;

	do {
		b = getline(fin, sstrTemp1, '\n');
		sstrTemp1.erase(0,26);

		std::istringstream sstr;
		sstr.str(sstrTemp1);

		int x,y;

		sstr >> y;
		sstr >> x;

		minX = MIN(minX,x);
		minY = MIN(minY,y);
		maxX = MAX(maxX,x);
		maxY = MAX(maxY,y);


		cluster.push_back(x);
		cluster.push_back(y);

		midX += x;
		midY += y;

		float col = 1.0;
		SETPIXEL(pRGBMap,x,y,gridSize,col);

		count++;
	} while (b!=NULL &&  count < numObjects);

	if ( cluster.size()/2 != numObjects ) {
		return;
	}


	midX /= (double)numObjects;
	midY /= (double)numObjects;

	SETPIXEL(pRGBMap,(int)midX,(int)midY,gridSize,0.5);


	midX = 0;
	midY = 0;


	for (int i=0;i<numObjects;i++)
	{
		cluster[i*2] = (cluster[i*2]-minX)/(maxX-minX);
		cluster[i*2+1] = (cluster[i*2+1]-minY)/(maxY-minY);

		midX += cluster[i*2];
		midY += cluster[i*2+1];

	}
	midX /= (double)numObjects;
	midY /= (double)numObjects;




	double meandist = 0.0;
	double maxdist = 0.0;
	for (int i=0;i<numObjects;i++)
	{
		dist.push_back( sqrtf( powf(cluster[i*2]-midX,2.f)+powf(cluster[i*2+1]-midY,2.f) ) );
		maxdist = MAX(maxdist,dist[i]);
		meandist += dist[i];
	}
	meandist /= (double) numObjects;
	
	double plotValue = 1.0-(meandist/maxdist);

	e.writeTableEntry("mean distance");
	e.writeTableEntry((float)meandist);
	e.newRow();

	e.writeTableEntry("max distance");
	e.writeTableEntry((float)maxdist);
	e.newRow();

	e.writeTableEntry("plot value");
	e.writeTableEntry((float)plotValue);
	e.newRow();

	SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, sstrCatalogueName);
	delete[] pRGBMap;

}



void pixelCounter()
{
	size_t gridSize = 859;
	int s = gridSize*gridSize;
	// load mask
	ilLoadImage( (ILstring)"mask.png" );
	ILenum err = ilGetError();
	if( err != NULL )
		return;
	ilConvertImage( IL_LUMINANCE, IL_UNSIGNED_BYTE );
	int width = ilGetInteger( IL_IMAGE_WIDTH );
	int height = ilGetInteger( IL_IMAGE_HEIGHT );
	if ( width != gridSize || height != gridSize ) {
		// wrong dimensions
		return;
	}
	unsigned char *pt = ilGetData();
	if ( pt == NULL )  {
		// nah, fail..
		return;
	}

	int c=0;
	for (int i=0;i<s;i++)
	{
		if (pt[i] > 128 ) 
			c++;
	}
	printf("pixel count = %i\n",c);
}


void printNeighboursFromMask()
{
	size_t gridSize = 859;
	int s = gridSize*gridSize;
	// load mask
	ilLoadImage( (ILstring)"mask.png" );
	ILenum err = ilGetError();
	if( err != NULL )
		return;
	ilConvertImage( IL_LUMINANCE, IL_UNSIGNED_BYTE );
	int width = ilGetInteger( IL_IMAGE_WIDTH );
	int height = ilGetInteger( IL_IMAGE_HEIGHT );
	if ( width != gridSize || height != gridSize ) {
		// wrong dimensions
		return;
	}
	unsigned char *pt = ilGetData();
	if ( pt == NULL )  {
		// nah, fail..
		return;
	}

	SpectraVFS *pSourceVFS = new SpectraVFS( "allSpectra.bin", false );
	const size_t numSourceSpecra = pSourceVFS->getNumSpectra();
	if (numSourceSpecra == 0 )
		return;

	int *pIndexlist= new int[gridSize*gridSize];
	std::string sstrIndexList = "indexlist";
	std::string sstrFileName = "fluxMap";
	int j=199;
	sstrIndexList += Helpers::numberToString(j,4);
	sstrFileName += Helpers::numberToString(j,4);
	sstrIndexList+= ".bin";
	FILE *f=fopen(sstrIndexList.c_str(),"rb");
	if ( f!= NULL)
	{
		fread(pIndexlist, 1, s*sizeof(int), f);
		fclose(f);
	}
	else
	{
		return;
	}

	std::ofstream logFile("selectionList.html");


	unsigned char *m2 = new unsigned char[s];
	float *m3 = new float[s*3];
	memset(m2,0,s);

	int c=0;
	for (int i=0;i<s;i++)
	{
		if (pt[i] > 128 ) 
		{
			const int x=i%gridSize;
			const int y=i/gridSize;

			const int we = CALC_MAPADRESS(x-1,y,gridSize);
			const int ea = CALC_MAPADRESS(x+1,y,gridSize);
			const int no = CALC_MAPADRESS(x,y-1,gridSize);
			const int so = CALC_MAPADRESS(x,y+1,gridSize) ;
			const int weno = CALC_MAPADRESS(x-1,y+1,gridSize);
			const int eano = CALC_MAPADRESS(x+1,y+1,gridSize);
			const int weso = CALC_MAPADRESS(x-1,y-1,gridSize);
			const int easo = CALC_MAPADRESS(x+1,y-1,gridSize);
/*
			if ( pt[we] == 0)
				m2[we] = 255;
			if ( pt[ea] == 0)
				m2[ea] = 255;
			if ( pt[no] == 0)
				m2[no] = 255;
			if ( pt[so] == 0)
				m2[so] = 255;
*/
			if ( pt[weno] == 0)
				m2[weno] = 255;
			if ( pt[eano] == 0)
				m2[eano] = 255;
			if ( pt[weso] == 0)
				m2[weso] = 255;
			if ( pt[easo] == 0)
				m2[easo] = 255;
			c++;
		}
	}
	Helpers::print("num objects in mask "+Helpers::numberToString<int>(c)+"\n", &logFile, false );

	int c2 = 0;
	for (int i=0;i<s;i++)
	{
		float val = (float)m2[i]/255.f;
		m3[i*3] = val;
		m3[i*3+1] = val;
		m3[i*3+2] = val;

		if (m2[i] == 255)
		{
			int index = pIndexlist[i];
			if ( index >= 0 && index < numSourceSpecra )
			{
				Spectra *sp = pSourceVFS->beginRead(index);

				Helpers::print( "<a href=\""+sp->getURL()+"\">"+sp->getFileName()+"</a><br>\n", &logFile, false );
				pSourceVFS->endRead(index);
				c2++;
			}
		}
	}
	Helpers::print(Helpers::numberToString<int>(c2)+" direct neighbors\n", &logFile, false );



	SpectraHelpers::saveIntensityMap( m3, gridSize, gridSize, "slectionMask" );

}


void writeDifferenceMap()
{

	SSE_ALIGN Spectra compare;
	compare.loadFromFITS_SDSS("spSpec-53700-2285-421.fit");

	const size_t gridSize(859);
	const size_t gridSizeSqr(gridSize*gridSize);

	SpectraVFS *pSourceVFS = new SpectraVFS( "allSpectra.bin", false );
	const size_t numSourceSpecra = pSourceVFS->getNumSpectra();


	//for (size_t j=0;j<=200;j++)
	size_t j=199;
	{
		int *pIndexlist= new int[gridSize*gridSize];
		std::string sstrIndexList = "indexlist";
		std::string sstrFileName = "differencemap";
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
			if ( index >= 0 && index < numSourceSpecra )
			{
				Spectra *sp = pSourceVFS->beginRead(index);

				const float err = compare.compare(*sp);
				float intensity = (err-226.f)/255.f;

				SpectraHelpers::intensityToRGB( intensity, &pRGBMap[i*3], true );			
				pSourceVFS->endRead(index);
			}
		}

		SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, sstrFileName);


		delete[] pIndexlist;
		delete[] pRGBMap;
	}
}


void analyzeSpectraJumps()
{
	std::ofstream logFile("specObjOperations_log.txt");

	Helpers::print("Analyze spectra travel distance.\n", &logFile, true );
	Helpers::print("Input: indexListXXXX.bin .. indexListYYYY.bin in continuous range.\n", &logFile, true );

	size_t j = 0;
	size_t fileSize = 0;
	std::string sstrIndexList1;

	do
	{
		sstrIndexList1 = "indexlist";
		sstrIndexList1 += Helpers::numberToString(j,4);
		sstrIndexList1 += ".bin";
		fileSize = FileHelpers::getFileSize(sstrIndexList1);
		if ( fileSize == 0 )
		{
			j++;
		}
	}
	while ( j<9999 && fileSize == 0 );



	const size_t gridSize(sqrtf(fileSize/4));
	const size_t gridSizeSqr(gridSize*gridSize);

	if ( gridSizeSqr !=  (fileSize/4) )
	{
		Helpers::print("Error: "+sstrIndexList1+" wrong filesize. Abort.\n", &logFile, true );
		return;
	}
	Helpers::print("Starting with: "+sstrIndexList1+"\n", &logFile, true );


	int *pIndexlist1= new int[gridSizeSqr];
	int *pIndexlist2= new int[gridSizeSqr];

	std::ofstream resultFile("spectrajumps.csv");
	std::string sstrResult;
	int numSourceSpecra = 0;
	


	do
	{
		std::string sstrIndexList1 = "indexlist";
		std::string sstrIndexList2 = "indexlist";
		sstrIndexList1 += Helpers::numberToString(j,4);
		sstrIndexList1 += ".bin";
		sstrIndexList2 += Helpers::numberToString(j+1,4);
		sstrIndexList2 += ".bin";
		FILE *f1=fopen(sstrIndexList1.c_str(),"rb");
		if ( f1!= NULL)
		{
			fread(pIndexlist1, 1, gridSizeSqr*sizeof(int), f1);
			fclose(f1);
		}
		else
		{
			break;
		}

		if ( numSourceSpecra == 0 )
		{
			// determine number of source spectra
			for (size_t i=0;i<gridSizeSqr;i++)		
			{
				numSourceSpecra = MAX(numSourceSpecra, pIndexlist1[i]);
			}
			Helpers::print(Helpers::numberToString<int>(numSourceSpecra)+" source spectra.\n", &logFile, true );
			if ( numSourceSpecra<=0)
			{
				Helpers::print("Error: No source spectra. Abort.\n", &logFile, true );
				return;
			}

		}

		FILE *f2=fopen(sstrIndexList2.c_str(),"rb");
		if ( f2!= NULL)
		{
			fread(pIndexlist2, 1, gridSizeSqr*sizeof(int), f2);
			fclose(f2);
		}
		else
		{
			break;
		}

		Helpers::print("Analyzing learning step ("+Helpers::numberToString<int>(j)+","+Helpers::numberToString<int>(j+1)+")\n", &logFile, true );


		double summedD = 0.0;

		for (size_t i=0;i<gridSizeSqr;i++)
		{
			int x1 = i%gridSize;
			int y1 = i/gridSize;
			int index1 = pIndexlist1[i];
			if ( index1 < numSourceSpecra && index1 >= 0) 
			{
				// if spectrum stays at the same position, skip
				if ( index1 == pIndexlist2[i] )
					continue;
				// search for match
				for (size_t j=0;j<gridSizeSqr;j++)
				{
					int index2 = pIndexlist2[j];
					if ( index2 == index1 )
					{
						int x2 = j%gridSize;
						int y2 = j/gridSize;
						int dx = x2-x1;
						int dy = y2-y1;
						dx *= dx;
						dy *= dy;
						summedD += sqrtf(dx+dy);
						break;
					}
				}
			}
		}
		sstrResult = Helpers::numberToString<double>(summedD)+ "\n";
		resultFile << sstrResult;
		resultFile.flush();
		j++;
	} while (true);

	Helpers::print("Results written to spectrajumps.csv. Fin.\n", &logFile, true );

	delete[] pIndexlist1;
	delete[] pIndexlist2;
}



void writeSpectraInfoToTable()
{
	std::ofstream logFile("specObjOperations_log.txt");

	Helpers::print("Welcome to ASPECT spectra table writer.\n\nWrites mapindex, xpos, ypos, specObjID, aspect internal spectraIndex, plate ID, MJD, fiber ID, z, objtype to cvs table.\n Input: sofmnet.bin, allSpectra.bin.\n\n", &logFile, true );
	
	SpectraVFS *pNetworkVFS = new SpectraVFS( "sofmnet.bin", false );
	if ( pNetworkVFS == NULL )
	{
		Helpers::print("Error opening sofmnet.bin\n", &logFile, true );
		return;
	}

	SpectraVFS *pSourceVFS = new SpectraVFS( "allSpectra.bin", false );
	if ( pSourceVFS == NULL )
	{
		Helpers::print("Error opening allSpectra.bin\n", &logFile, true );
		return;
	}

	SpectraHelpers::writeSpectraInfoToTable( *pSourceVFS, *pNetworkVFS, "spectraTbl.csv", &logFile);

}



void writeMapToTIFF()
{
	std::string sstrFilename("map.tif");
	const int iconSize = 128;


	SpectraVFS *pNetworkVFS = new SpectraVFS( "sofmnet.bin", false );
	if ( pNetworkVFS != NULL )
	{
		const int gridSizeSqr = pNetworkVFS->getNumSpectra();
		const int gridSize = sqrtf(gridSizeSqr);
		const int gridSizeSqr2 = gridSize*gridSize;
		if ( gridSizeSqr2 != gridSizeSqr || gridSizeSqr==0 )
		{
			// could not load network.
			printf("Error: could not load network.\n"); 

			delete pNetworkVFS;
			return;
		}

		BigTIFF bigTIFF(sstrFilename.c_str(),gridSize,gridSize,iconSize,false);


		unsigned char empty[iconSize*iconSize*3];
		memset( empty,255,iconSize*iconSize*3);


		
		int currentTileSize = iconSize;
		int lod = 0;
		do 
		{

			for (int i=0;i<gridSizeSqr;i++ )
			{
				Spectra *spSpec = pNetworkVFS->beginRead( i );

				const int index = spSpec->m_Index;
				if ( index >= 0 && index < gridSizeSqr )
				{
					std::string sstrIconFilename = spSpec->getFileName();
					sstrIconFilename += ".png";
					std::string sstrPath = "export/" + Helpers::numberToString(spSpec->getPlate(),4)+"/"+sstrIconFilename;

					// load mask
					ilLoadImage( (ILstring)sstrPath.c_str() );
					ILenum err = ilGetError();
					if( err != NULL )
					{
						printf("Error: could not load image.\n"); 
						return;
					}
					int width = ilGetInteger( IL_IMAGE_WIDTH );
					int height = ilGetInteger( IL_IMAGE_HEIGHT );
					int bpp = ilGetInteger( IL_IMAGE_BPP ); //bytes per pixel
					if ( width !=  height && width != iconSize ) {
						// wrong dimensions
						printf("Error: wrong icon dimensions.\n"); 
						return;
					}
					if ( bpp != 3)
					{
						ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
					}
					if ( currentTileSize != iconSize )
						iluScale( currentTileSize, currentTileSize, 1);
				
					unsigned char *pt = ilGetData();
 					bigTIFF.writeTile( i, currentTileSize, pt);
				}
				else
				{
					// empty 
					bigTIFF.writeTile( i, currentTileSize, empty);
				}

				pNetworkVFS->endRead( i );
			}

 			currentTileSize /= 2;
 			lod++;

			bigTIFF.writeHeader(currentTileSize, lod);
		} while ( currentTileSize > 16 );


	}
	
//	BigTIFF::test();

}

void writeSpectraParmDB()
{
	SpectraDB db;
	db.writeDB(SpectraDB::DR9);
}

void main(int argc, char* argv[])
{

	printf("%i",Spectra::numSamples);
// 	int clusternum = 1;
// 	if ( argc > 1 ) {
// 		std::string strCommandLine = argv[1];
// 		clusternum = Helpers::stringToNumber<int>(strCommandLine);
// 	}
//  	SpectraHelpers::init(0);
 	//test();

// 	std::string sstrDir(DATADIR);
// 	if ( argc>1 )
// 		sstrDir = argv[1];

	//writeParamsFromSelection( sstrDir );

	//spectroLisWrite();
	//trackCatalogs();
	//writeSpectrTypes();
	//writeFlux();
	//writeMagUGRIZ();
	//writePrimaryTarget();
	//writePrimTargetFromBin();
	//writePlate();
	//writePlate336();
	//writeRADEC();
	//writeZMap();
	//writeZMapFromIndexList();
	//writeOccupiedFromIndexList();
	//writeOtherZValues();
	//writeSpectraVersion();
	//writeMJD();
	//writeUMatrix();
	//writeUMatrixForSource(); 
	//test();
	
	//writeIndexListFromSOFMBin();
	//medianSpectrumFromSelection();
	//extractGalaxyZooData();
	//analyseSineTestDistributions();
	//clusterStatisticsSim();
	//analyseMarksClusters1();
	//analyseMarksClusters2( clusternum );
	//displaySpectra();
	//pixelCounter();
	//printNeighboursFromMask();
	//writeDifferenceMap();
	//analyzeSpectraJumps();
	//writeMapToTIFF();
	//writeSpectraInfoToTable();
	//writeSpectraParmDB();

	printf ("fin.\n" );

}

