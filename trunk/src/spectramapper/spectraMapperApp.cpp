//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
//! #
//! # Copyright (c) 2009 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 11/15/2011
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  spectraMapperApp.cpp
//! \desc  Plot multiple spectra into one graph 

#include "sdsslib/spectra.h"
#include "sdsslib/glhelper.h"
#include "sdsslib/defines.h"
#include "sdsslib/random.h"
#include "sdsslib/spectraVFS.h"
#include "sdsslib/spectraWrite.h"
#include "sdsslib/spectraHelpers.h"
#include "sdsslib/helpers.h"
#include "sdsslib/sdssSoftwareVersion.h"
#include "sdsslib/fileHelpers.h"

#include "framework.h"	
#include "spectraMapper.h"

#include "devil/include/il/il.h"
#include "devil/include/il/ilu.h"

#include "tclap/CmdLine.h"

#include <assert.h>
#include <Windows.h>
#include <conio.h>

#include <string>



extern HWND	fr_hWnd;
extern HDC fr_hDC;		
int left,right,up,down;
int scr_width, scr_height;

std::ofstream logFile("spectramapper_log.txt");

int g_CurrentSpectraIndex=0;

size_t g_Mode = 1;	// immediately start with clustering..
bool g_DisableOutput = false;

size_t g_numSpectra = 0;
SpectraVFS *g_pVFSSource = NULL; 


bool g_writeData = false;
int g_maskSelection = -1;

SpectraMapper *g_spectraMapper=NULL;
bool g_bNormalizeByFlux = true;
bool g_bToRestFrame = false; // project spectrum to its restframe based on the z estimate from SDSS. 
float g_yScale = 1.0;
float g_brightness = 1.f;
bool g_generateMap = false;
bool g_writeFileOnly = false;


void displaySpectra( 
	const std::string &_sstrSpectraListFilename, 
	const std::string &_sstrIndexlistFilename,
	const std::string &_sstrSourceSpectraFilename,
	const std::string &_sstrOutPlotFilename );



int InitGL()		
{
	Helpers::createConsole();
	GLExtensions::loadGLExtensions(false);

	SpectraHelpers::init( fr_hDC );

	// opengl init stuff
	glShadeModel(GL_SMOOTH);							
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				
	glClearDepth(1.0f);									
	glEnable(GL_DEPTH_TEST);							
	glDepthFunc(GL_LEQUAL);			
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	

	

	// convert command line
	int argc;
	std::string sstrCmdLine(GetCommandLine());
	char **argv = Helpers::getCommandLineFromString( sstrCmdLine, argc );

	Helpers::print("Welcome to ASPECT SpectraMapper "+sstrSDSSVersionString+" !\n\n\n", &logFile);
	Helpers::print("Two operational modes:\n", &logFile);
	Helpers::print("(1) Plots multiple spectra into one graph.\n", &logFile);
	Helpers::print("    Needs binary file with source spectra, an image mask for spectra selection and an index list from ASPECT that contains clustered positions.\n\n", &logFile);
	Helpers::print("(2) In map display mode, takes a list of spectra and plots their locations into a map if they are found in the clustered pool.\n", &logFile);
	Helpers::print("    Needs binary file with source spectra, a text file which contains a list of spectra filenames and an index list from ASPECT that contains clustered positions.\n\n", &logFile);
	


	std::string sstrSourceSpectraFilename("allSpectra.bin");
	std::string sstrIndexlistFilename("indexlist0199.bin");
	std::string sstrMaskFilename("mask.png");
	std::string sstrOutPlotFilename("spectraMapperPlot");
	std::string sstrSpectraListFilename("spectralist.txt");
	

	try {  

		std::string sstrExamples("examples:\n");
		sstrExamples += std::string("example: spectraMapper.exe -i allSpectra.bin\n");

		TCLAP::CmdLine cmd(sstrExamples, ' ', sstrSDSSVersionString);

		TCLAP::ValueArg<std::string> dumpFilenameArg("i", "inputdumpfile", "Input dump file that contains all source spectra. Example: allSpectra.bin", false, sstrSourceSpectraFilename, "inputdumpfile");
		TCLAP::ValueArg<std::string> indexlistFilenameArg("x", "index", "index list binary file that contains the mapping from input spectra to clustered map Example: indexlist0199.bin", false, sstrIndexlistFilename, "indexlistfile");
		TCLAP::ValueArg<std::string> maskFilenameArg("m", "mask", "Image mask filename. Example mask.png", false, sstrMaskFilename, "outputfilename.png");
		TCLAP::ValueArg<std::string> spectraListFilenameArg("s", "spectralist", "Text file that contains the spectra FITS filenames. Example: spectralist.txt", false, sstrSpectraListFilename, "spectralistfile");		
		TCLAP::ValueArg<std::string> outFilenameArg("o", "outfile", "Output filename.", false, sstrOutPlotFilename, "spectraMapperPlot");		
		TCLAP::SwitchArg toRestframeArg("r","restframe","Project spectrum to its rest frame based on the z estimate from SDSS.", g_bToRestFrame);
		TCLAP::SwitchArg normalizeArg("n","normalize","Normalize all spectra by flux before plotting.", g_bNormalizeByFlux);
		TCLAP::SwitchArg modeArg("d","mapdisplay","Map display Mode: Take a list of spectra (that are part of the cluster result) and plot them in a map.", g_generateMap);
		TCLAP::SwitchArg writeFileOnlyArg("c","continue","Write plot to file and exit immediately.", g_writeFileOnly);

		cmd.add( dumpFilenameArg );
		cmd.add( indexlistFilenameArg );
		cmd.add( spectraListFilenameArg );
		cmd.add( maskFilenameArg );
		cmd.add( outFilenameArg );
		cmd.add( toRestframeArg );
		cmd.add( normalizeArg );
		cmd.add( modeArg );
		cmd.add( writeFileOnlyArg );
		

		cmd.parse( argc, argv );

		sstrSourceSpectraFilename		= dumpFilenameArg.getValue();
		sstrIndexlistFilename			= indexlistFilenameArg.getValue();
		sstrMaskFilename				= maskFilenameArg.getValue();
		sstrOutPlotFilename				= outFilenameArg.getValue();
		sstrSpectraListFilename			= spectraListFilenameArg.getValue();
		g_bToRestFrame					= toRestframeArg.getValue();
		g_bNormalizeByFlux				= normalizeArg.getValue();
		g_generateMap					= modeArg.getValue();
		g_writeFileOnly					= writeFileOnlyArg.getValue();
	}
	catch (TCLAP::ArgException &e)  
	{ 
		Helpers::print( "error: "+e.error()+" for argument "+e.argId()+"\n", &logFile );
	}

	delete[] argv;

	if ( g_generateMap )
	{
		displaySpectra( sstrSpectraListFilename, sstrIndexlistFilename, sstrSourceSpectraFilename, sstrOutPlotFilename );
		exit(0);
	}
	else
	{
		g_spectraMapper = new SpectraMapper( sstrSourceSpectraFilename, sstrIndexlistFilename, sstrMaskFilename, sstrOutPlotFilename );
	}


	return TRUE;
}



void UpdateGLView(int width, int height)	
{
	if (height == 0)										
	{
		height = 1;										
	}

	scr_width	= width;
	scr_height	= height;

	glViewport(0, 0, width, height);						

	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();			
	glOrtho(0,width, height, 0, 1, 100);
	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();			
}	



void DrawGLScene()
{
	char captiontext[]={"ASPECT"};

	SetWindowText( fr_hWnd, captiontext );

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	g_spectraMapper->draw( scr_width, scr_height, g_bToRestFrame, g_bNormalizeByFlux, g_yScale, g_brightness, g_maskSelection, g_writeData||g_writeFileOnly );
	g_writeData = false;
	if ( g_writeFileOnly )
		exit(0);

	Sleep(100);
}


void KillGL()	
{
}


void Export()
{
}

void DisableOutput()
{
}

void ArrowLeft()
{
	left = 1;

	if ( g_maskSelection > 0 )
		g_maskSelection--;
}


void ArrowRight()
{
	right = 1;

	g_maskSelection++;
}

void ArrowUp()
{
	g_yScale +=0.05f;
	up = 1;
}

void ArrowDown()
{
	g_yScale -=0.05f;

	down = 1;
}

void Space()
{
	g_Mode++;
	g_writeData = true;
}

void KeyOne()
{
	g_brightness += 0.05f;
}

void KeyTwo()
{
	if ( g_brightness >= 0.1)
		g_brightness -= 0.05f;
}



void displaySpectra( const std::string &_sstrSpectraListFilename, const std::string &_sstrIndexlistFilename, const std::string &_sstrSourceSpectraFilename, const std::string &_sstrOutPlotFilename )
{

	const size_t fileSize = FileHelpers::getFileSize(_sstrIndexlistFilename);

	if ( fileSize == 0 )
	{
		Helpers::print( "error: index list not found.\n", &logFile );
		exit(3);
	}

	const size_t gridSizeSqr(fileSize/4);
	const size_t gridSize(sqrtf(gridSizeSqr));

	if ( gridSize*gridSize != gridSizeSqr )
	{
		Helpers::print( "error: wrong file size for index list binary.\n", &logFile );
		exit(3);
	}

	float *pRGBMap = new float[gridSizeSqr*3];
	for ( size_t i=0;i<gridSizeSqr*3;i++)
	{
		pRGBMap[i] = 0.0f;
	}

	// thats the stuff we are reading in:
	// c:\sdss\r\data\spSpec-51614-0281-427.fit 255 0 128
	// hui.fit 128 23 23
	// spSpec-51614-0281-427.fit

	std::string sstrSpectraList;
	const bool bSuccess = FileHelpers::loadFileToString( _sstrSpectraListFilename, sstrSpectraList );
	if ( !bSuccess ) 
	{
		Helpers::print( "spectralist missing.\n", &logFile );
		exit(3);
	}

	std::stringstream fin(sstrSpectraList);

	std::string sstrLine;
	void *a = NULL;
	int count = 0;

	int *pIndexlist= new int[gridSizeSqr];
	FILE *f=fopen(_sstrIndexlistFilename.c_str(),"rb");
	if ( f!= NULL)
	{
		fread(pIndexlist, 1, gridSizeSqr*sizeof(int), f);
		fclose(f);
	}
	else
	{
		Helpers::print( " could not load indexlist.\n", &logFile );
		exit(3);
	}

	SpectraVFS *pSourceVFS = new SpectraVFS( _sstrSourceSpectraFilename, false );
	const int numSourceSpecra = pSourceVFS->getNumSpectra();
	const int numSpectra = gridSizeSqr;
	if (numSourceSpecra <= 0 ) 
	{
		Helpers::print("allSpectra.bin missing.\n", &logFile );
		exit(3);
	}


	for ( size_t i=0;i<numSpectra;i++ )
	{
		int index = pIndexlist[i];

		// mark empty
		if ( index < 0 )
		{		
			const int px = i%gridSize;
			const int py = i/gridSize;

			SETPIXEL(pRGBMap,px,py,gridSize,0.1);
		}
	}


	do {
		a = getline(fin, sstrLine, '\n');
		std::stringstream sstr(sstrLine);
		std::string sstrFilename;
		sstr >> sstrFilename;
		float r=255.f,g=255.f,b=255.f;
		if (!sstr.eof()) sstr >> r;
		if (!sstr.eof()) sstr >> g;
		if (!sstr.eof()) sstr >> b;

		r /= 255.f;
		g /= 255.f;
		b /= 255.f;

		SSE_ALIGN Spectra sp;
		if ( sp.loadFromFITS(sstrFilename) ) {

			const int plate = sp.getPlate();
			const int mjd = sp.getMJD();
			const int fiber = sp.getFiber();

			bool spectraFound = false;
			// search spectrum
			for ( size_t i=0;i<numSpectra;i++ )
			{  
				const int index = pIndexlist[i];

				if ( index >= 0 && index < numSourceSpecra ) {
					Spectra *spSpecSrc = pSourceVFS->beginRead(index);
					if ( plate == spSpecSrc->getPlate() && 
						mjd == spSpecSrc->getMJD() &&
						fiber == spSpecSrc->getFiber() )
					{
						// match found
						const int px = i%gridSize;
						const int py = i/gridSize;

						SETPIXELRGB(pRGBMap,px,py,gridSize,r,g,b);
						spectraFound = true;
						break;

						count++;
					}

					pSourceVFS->endRead(index);
				}
			}
			if ( !spectraFound ) {
				Helpers::print("no matches found for "+sstrFilename+"\n", &logFile );
			}


		} else {
			if ( !sstrFilename.empty() ) {
				Helpers::print("could not load "+sstrFilename+"\n", &logFile );
			}
		}


	} while (a!=NULL);




	SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, _sstrOutPlotFilename);
	delete[] pRGBMap;

}
