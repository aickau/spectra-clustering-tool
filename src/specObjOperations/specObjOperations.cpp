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
#include <algorithm>

#include "devil/include/il/il.h"
#include "devil/include/il/ilu.h"

#include <windows.h>
#include "cfitsio/fitsio.h"
#include "cfitsio/longnam.h"

#include "sdsslib/helpers.h"
#include "sdsslib/filehelpers.h"
#include "sdsslib/spectraWrite.h"
#include "sdsslib/spectraVFS.h"
#include "sdsslib/spectraHelpers.h"
#include "sdsslib/spectra.h"
#include "sdsslib/defines.h"
#include "sdsslib/CSVExport.h"

typedef char _TCHAR;

//#define DATADIR std::string("G:/SDSS_ANALYZE/fits/spectro/data/")
//#define DATADIR std::string("c:/prj/sdss_trunk/data/")
//#define DATADIR std::string("d:/dr7/1d_25/")
#define DATADIR std::string("f:/DR6SpectroFITS/")




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
	const size_t gridSize(859);
	const size_t gridSizeSqr(gridSize*gridSize);

	SpectraVFS *pSourceVFS = new SpectraVFS( "allSpectra.bin", false );
	const size_t numSourceSpecra = pSourceVFS->getNumSpectra();
	size_t j=199;
//	for (size_t j=0;j<=200;j++)
	{
		std::string sstrCatalogueName("catalogueDownes2004");
		//std::string sstrCatalogueName("catalogueKoester2006");
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
	
	size_t j=199;
	//for (size_t j=0;j<=200;j++)
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
			if ( index >= 0 && index < numSourceSpecra )
			{
				Spectra *sp = pSourceVFS->beginRead(index);

				float r=0;		// dark cyan undefined spectra type
				float g=0.5;
				float b=0.5;

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
				std::string sstrFileName = Spectra::getSpecObjFileName(sp->getPlate(),sp->getMJD(),sp->getFiber());
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
		if ( index > 0 && index < numSourceSpecra )
		{

			float r=0;
			float g=0;
			float b=0;


			if (primTargets[c]== TARGET_GALAXY_RED ) { r=1.f;g=0; b= 0.0; // red 
				} else
			if (primTargets[c]== TARGET_GALAXY_RED_II ) { r= 0.8; g=0.0; b= 0.0;// darker red
				} else
			if (primTargets[c]== TARGET_GALAXY ) { r= 1.0; g=0.5; b=0.0;// orange
				} else
			if (primTargets[c]== TARGET_GALAXY_BIG ) { r= 1.0; g=0.75; b= 0.0;// light orange
				} else
			if (primTargets[c]== TARGET_GALAXY_BRIGHT_CORE ) { r= 1.0; g=0.0; b= 0.5; // red pink
				} else
			if (primTargets[c]==  TARGET_QSO_HIZ ) { r= 1.0; g=1.0; b= 0.0; // yellow
				} else
			if (primTargets[c]==  TARGET_QSO_FAINT ) { r= 0.0; g=0.5; b= 1.0;// light blue
				} else
			if (primTargets[c]==  TARGET_QSO_CAP ) { r= 0.0; g=0.0; b= 0.5;// dark blue
				} else
			if (primTargets[c]==  TARGET_QSO_REJECT ) { r= 0.8; g=0.8; b= 0.8; // light gray
				} else
			if (primTargets[c]==  TARGET_QSO_SKIRT ) {  r= 0.2; g=0.2; b= 0.2; // dark gray
				} else
			if (primTargets[c]==  TARGET_QSO_FIRST_CAP ) { r= 0.0; g=1.0; b= 0.6; // light green
				} else
			if (primTargets[c]==  TARGET_QSO_FIRST_SKIRT ) { r= 0.0; g=0.5; b= 0.0; // dark green
				} else

			if (primTargets[c]==  TARGET_STAR_RED_DWARF ) { r= 0.5; g=0.0; b= 0.0; // low red
				} else
			if (primTargets[c]==  TARGET_STAR_BROWN_DWARF ) { r= 0.5; g=0.25; b= 0.0; // brown
				} else
			if (primTargets[c]==  TARGET_STAR_CARBON ) { r= 1.0; g=0.0; b= 1.0; // pink
				} else
			if (primTargets[c]==  TARGET_STAR_WHITE_DWARF ) { r= 1.0; g=1.0; b= 1.0; // white
				} else
			if (primTargets[c]==  TARGET_STAR_PN ) { r= 0.5; g=0.5; b= 1.0;// light blue
				} else
			if (primTargets[c]==  TARGET_STAR_BHB ) { r= 0.5; g=0.0; b= 1.0;// violet
				} else
			if (primTargets[c]==  TARGET_STAR_SUB_DWARF ) { r= 0.0; g=1.0; b= 0.0;// green
				} else
	
			if (primTargets[c]==  TARGET_ROSAT_A ) { r= 0.0; g=1.0; b= 1.0;// cyan
				} else
			if (primTargets[c]==  TARGET_ROSAT_B ) { r= 0.0; g=0.95; b= 0.0;// cyan
				} else
			if (primTargets[c]==  TARGET_ROSAT_C ) { r= 0.0; g=0.9; b= 0.0;// cyan
				} else
			if (primTargets[c]==  TARGET_ROSAT_D ) { r= 0.0; g=0.85; b= 0.0;// cyan
				} else
			if (primTargets[c]==  TARGET_ROSAT_E ) { r= 0.0; g=0.08; b= 0.0;// cyan
				} else
			if (primTargets[c]==  TARGET_SERENDIP_FIRST ) { r= 0.0; g=0.0; b= 1.0;// blue
				} else
			if (primTargets[c]==  TARGET_SERENDIP_RED ) { r= 0.0; g=0.0; b= 0.95;// blue
				} else
			if (primTargets[c]==  TARGET_SERENDIP_DISTANT ) {r= 0.0; g=0.0; b= 0.9;// blue
				} else
			if (primTargets[c]==  TARGET_SERENDIP_MANUAL ) { r= 0.0; g=0.0; b= 0.85;// blue
				} else
			if (primTargets[c]== TARGET_SERENDIP_BLUE ) { r= 0.0; g=0.0; b= 0.8;// blue
			}

			pRGBMap[i*3] =r;
			pRGBMap[i*3+1] = g;
			pRGBMap[i*3+2] = b;

			pSourceVFS->endRead(index);

			c++;

		}
		else
		{
			pRGBMap[i*3] = 0.5;
			pRGBMap[i*3+1] = 0.5;
			pRGBMap[i*3+2] = 0.5;
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
				std::string sstrFileName = Spectra::getSpecObjFileName(sp->getPlate(),sp->getMJD(),sp->getFiber());
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




				case TARGET_GALAXY_RED : r=1.f;g=0; b= 0.0; // red
					break;
				case TARGET_GALAXY_RED_II : r= 0.8; g=0.0; b= 0.0;// darker red
					break;
				case TARGET_GALAXY : r= 1.0; g=0.5; b=0.0;// orange
					break;
				case TARGET_GALAXY_BIG : r= 1.0; g=0.75; b= 0.0;// light orange
					break;
				case TARGET_GALAXY_BRIGHT_CORE : r= 0.0; g=1.0; b= 0.0; // green
					break;
				case TARGET_QSO_HIZ : r= 1.0; g=1.0; b= 0.0; // yellow
					break;
				case TARGET_QSO_FAINT : r= 0.0; g=0.5; b= 1.0;// cyan
					break;
				case TARGET_QSO_CAP : r= 0.2; g=0.2; b= 0.2;// dark grey
					break;
				case TARGET_QSO_REJECT : r= 1.0; g=1.0; b= 0.5; // light yellow
					break;

				case TARGET_QSO_SKIRT : r= 1.0; g=0.95; b= 0.5; // light yellow
					break;
				case TARGET_QSO_FIRST_CAP : r= 1.0; g=0.9; b= 0.5; // light yellow
					break;
				case TARGET_QSO_FIRST_SKIRT : r= 1.0; g=0.85; b= 0.5; // light yellow
					break;

				case TARGET_STAR_RED_DWARF : r= 0.5; g=0.0; b= 0.0; // low red
					break;
				case TARGET_STAR_BROWN_DWARF : r= 0.5; g=0.25; b= 0.5; // brown
					break;
				case TARGET_STAR_CARBON : r= 0.8; g=0.8; b= 0.8; // light grey
					break;
				case TARGET_STAR_WHITE_DWARF : r= 1.0; g=1.0; b= 1.0; // white
					break;
				case TARGET_STAR_PN : r= 0.5; g=0.5; b= 1.0;// light blue
					break;
				case TARGET_STAR_BHB : r= 1.0; g=0.0; b= 1.0;// violet
					break;
				case TARGET_STAR_SUB_DWARF : r= 0.0; g=0.5; b= 0.0;// dark green
					break;
				case TARGET_SERENDIP_BLUE : r= 0.0; g=0.0; b= 1.0;// blue
					break;
				case TARGET_ROSAT_A : r= 1.0; g=0.0; b= 0.55;// pink
					break;
				case TARGET_ROSAT_B : r= 1.0; g=0.0; b= 0.5;// pink
					break;
				case TARGET_ROSAT_C : r= 1.0; g=0.0; b= 0.45;// pink
					break;
				case TARGET_ROSAT_D : r= 1.0; g=0.0; b= 0.4;// pink
					break;
				case TARGET_ROSAT_E : r= 1.0; g=0.0; b= 0.35;// pink
					break;
				case TARGET_SERENDIP_FIRST : r= 0.5; g=1.0; b= 0.0;// light green
					break;
				case TARGET_SERENDIP_RED : r= 0.5; g=0.95; b= 0.0;// light green
					break;
				case TARGET_SERENDIP_DISTANT : r= 0.5; g=0.9; b= 0.0;// light green
					break;
				case TARGET_SERENDIP_MANUAL : r= 0.5; g=0.85; b= 0.0;// light green
					break;
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
	size_t j=200;
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
				if ( sp->getPlate() == 336) {
					int mjd = sp->getMJD();

					pRGBMap[i*3] = 1.0;
					pRGBMap[i*3+1] = sp->getFiber()/640.f;
					pRGBMap[i*3+2] = 0.0;
					objCount++;
				}
				pSourceVFS->endRead(index);
			}
		}

		SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, std::string("plate336"));


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
				std::string sstrFileName = Spectra::getSpecObjFileName(sp->getPlate(),sp->getMJD(),sp->getFiber());
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
	size_t j=200;
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
				std::string sstrFileName = Spectra::getSpecObjFileName(sp->getPlate(),sp->getMJD(),sp->getFiber());
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
				std::string sstrFileName = Spectra::getSpecObjFileName(sp->getPlate(),sp->getMJD(),sp->getFiber());
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
	SpectraVFS *pSourceVFS = new SpectraVFS( "allSpectra.bin", false );
	const size_t numSourceSpecra = pSourceVFS->getNumSpectra();

	double zmin = 100.f;
	double zmax = -100.f;
	size_t iZmin, iZmax;
	for ( size_t i=0;i<numSourceSpecra;i++ )
	{
		Spectra *a = pSourceVFS->beginRead(i);
		if ( a->m_Z < zmin )
		{
			zmin = a->m_Z;
			iZmin = i;
		}
		if ( a->m_Z > zmax )
		{
			zmax = a->m_Z;
			iZmax = i;
		}

		pSourceVFS->endRead(i);
	}

	Spectra *spmin = pSourceVFS->beginRead(iZmin);
	Spectra *smax = pSourceVFS->beginRead(iZmax);

	int fmax = smax->getFiber();
	int pmax = smax->getPlate();
	int mjdmax = smax->getMJD();
/*
	SSE_ALIGN Spectra spDR4;
	SSE_ALIGN Spectra spDR6;

	spDR4.loadFromFITS("c:/incoming/dr4/spSpec-53120-1448-351.fit");
	spDR6.loadFromFITS("c:/incoming/dr6/spSpec-53120-1448-351.fit");
*/	
}

void writeParamsFromSelection()
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


	std::string sstrOutTable = "specOBJID; mjd; plate id; fiber id; z; total flux;	RAOBJ; DECOBJ; Z_ERR; Z_CONF; Z_STATUS;	Z_WARNIN; spectra type; MAG (fiber Mags ugriz)\n";


	for (size_t i=0;i<gridSizeSqr;i++)
	{
		// if mask is selected..
		if (pt[i] > 128 ) {
			int index = pIndexlist[i];
			if ( index >= 0 && index < numSourceSpecra )
			{
				Spectra *sp = pSourceVFS->beginRead(index);
				sp->calculateFlux();
				std::string sstrFileName = Spectra::getSpecObjFileName(sp->getPlate(),sp->getMJD(),sp->getFiber());
				std::string sstrPlate = Helpers::numberToString(sp->getPlate(),4);
				std::string sstrPath = DATADIR+sstrPlate+"/1d/"+sstrFileName;


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
				if ( status == 0 )
				{
					fits_read_key( f, TFLOAT, "Z_ERR", &zErr, NULL, &status );
					fits_read_key( f, TFLOAT, "Z_CONF", &zConf, NULL, &status );
					fits_read_key( f, TULONG, "Z_STATUS", &zStatus, NULL, &status );
					fits_read_key( f, TULONG, "Z_WARNIN", &zWarning, NULL, &status );

					fits_read_key( f, TFLOAT, "RAOBJ", &ra, NULL, &status );//RADEG
					fits_read_key( f, TFLOAT, "DECOBJ", &dec, NULL, &status );//DECDEG
					fits_read_key( f, TSTRING, "MAG", mag, NULL, &status );//DECDEG

					sscanf(mag,"%f %f %f %f %f", &uu, &gg, &rr, &ii, &zz );
	
					fits_close_file(f, &status);
				}

				pSourceVFS->endRead(index);

		
				sstrOutTable += Helpers::numberToString<unsigned __int64>(sp->m_SpecObjID)+"; ";
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

	std::ofstream fon("maskTable.csv");
	fon<<sstrOutTable;
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
	std::map<unsigned __int64,unsigned __int64> specObjIDLookup;

	// spec obj id -> index
	std::map<unsigned __int64,int> indexLookup;

	// our map
	const size_t gridSize = 859;
	const size_t gridSizeSqr = gridSize*gridSize;
	float *pRGBMap = new float[gridSizeSqr*3];
	for ( size_t i=0;i<gridSizeSqr*3;i++)
	{
		pRGBMap[i] = 0.5f;
	}


	// build dictionary spec obj id -> index
	SpectraVFS *pNetworkVFS = new SpectraVFS( "sofmnet.bin", false );
	if ( pNetworkVFS != NULL )
	{
		const size_t numSpectra = pNetworkVFS->getNumSpectra();

		for ( size_t i=0;i<numSpectra;i++ )
		{
			Spectra *spSpec = pNetworkVFS->beginRead( i );
			if ( spSpec->m_SpecObjID > 0 ) {
				indexLookup.insert(std::make_pair<unsigned __int64,int>(spSpec->m_SpecObjID,i));
			}
			pNetworkVFS->endRead( i );
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
	unsigned __int64 nullVal = 0;
	unsigned __int64 photoObj, specObj;
	fits_get_num_hdus( f, &numhdus, &status );
	fits_movabs_hdu( f, 2, &hdutype, &status );


	fits_get_num_rows( f, &tblrows, &status );
	fits_get_num_cols( f, &tblcols, &status );

	for ( size_t i=0;i<tblrows;i++ )
	{
		size_t i1 = i+1;
		fits_read_col( f, TLONGLONG, 1, i1, 1, 1, &nullVal, &photoObj, NULL, &status );
		fits_read_col( f, TLONGLONG, 2, i1, 1, 1, &nullVal, &specObj, NULL, &status );

		specObjIDLookup.insert(std::make_pair<unsigned __int64,unsigned __int64>(photoObj,specObj));
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

	while( fin >> sstrTemp ) 
	{	
		std::replace(sstrTemp.begin(), sstrTemp.end(), ',', ' ');
		// row 0:PhotoObjID
		unsigned __int64 photoObjID; 
		std::stringstream st0(sstrTemp.c_str() );
		st0 >> photoObjID;
		unsigned __int64 specObjID=0; 

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

		std::map<unsigned __int64,unsigned __int64>::iterator it = specObjIDLookup.find(photoObjID);
		if ( it != specObjIDLookup.end() ) {
			specObjID = it->second;
			std::map<unsigned __int64,int>::iterator it2 = indexLookup.find(specObjID);
			if (it2 != indexLookup.end() ) {
				int index = it2->second;
				if ( vDontKnow >= 0.8f ) {
					pRGBMap[index*3] = 1.0f;
					pRGBMap[index*3+1] = 1.0f;
					pRGBMap[index*3+2] = 1.0f;
				} else {
					pRGBMap[index*3] = 0.0f;
					pRGBMap[index*3+1] = 0.0f;
					pRGBMap[index*3+2] = 0.0f;
				}
				//SpectraHelpers::intensityToRGB( (float)vAntiClockwise, &pRGBMap[index*3], false );	
				mappingCount++;
			}
		}

	}

	SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, "galaxyZooDontKnow");


	delete[] pRGBMap;

}


void main(int argc, char* argv[])
{
	SpectraHelpers::init(0);

	//spectroLisWrite();
	//trackCatalogs();
	//writeSpectrTypes();
	//writeFlux();
	//writeMagUGRIZ();
	//writePrimaryTarget();
	//writePrimTargetFromBin();
	//writePlate();
	//writePlate336();
	writeRADEC();
	//writeZMap();
	//writeZMapFromIndexList();
	//writeOtherZValues();
	//writeSpectraVersion();
	//writeMJD();
	//writeUMatrix();
	//writeUMatrixForSource(); 
	//test();
	//writeParamsFromSelection();
	//writeIndexListFromSOFMBin();
	//medianSpectrumFromSelection();
	//extractGalaxyZooData();
	
	printf ("fin.\n" );

}

