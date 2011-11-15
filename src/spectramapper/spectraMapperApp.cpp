//! \verbatim
//! ###########################################################################
//! # SDSS sorting prototype - Stage I
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
#include "sdsslib/filehelpers.h"

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
int g_maskSelection = 0;

SpectraMapper *g_spectraMapper=NULL;




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

	Helpers::print("Welcome to SDSS SpectraMapper "+sstrSDSSVersionString+" !\n\n\n", &logFile);
	Helpers::print("Plots multiple spectra into one graph.\n", &logFile);
	Helpers::print("Needs binary file with source spectra, an image mask for spectra selection and an index list from SDSS analyze that contains clustered positions.\n", &logFile);


	std::string sstrSourceSpectraFilename("allSpectra.bin");
	std::string sstrMaskFilename("mask.png");
	std::string sstrOutPlotFilename("spectraMapperPlot");
	bool bToRestFrame = false; // project spectrum to its restframe based on the z estimate from SDSS. 

	try {  

		std::string sstrExamples("examples:\n");
		sstrExamples += std::string("example: spectraMapper.exe -i allSpectra.bin\n");

		TCLAP::CmdLine cmd(sstrExamples, ' ', sstrSDSSVersionString);

		TCLAP::ValueArg<std::string> dumpFilenameArg("i", "inputdumpfile", "example: allSpectra.bin", false, sstrSourceSpectraFilename, "input dump file that contains all source spectra.");
		TCLAP::ValueArg<std::string> maskFilenameArg("m", "mask", "Image mask.", false, sstrMaskFilename, "mask.png");
		TCLAP::ValueArg<std::string> outFilenameArg("o", "outfile", "Output filename.", false, sstrOutPlotFilename, "spectraMapperPlot");		
		TCLAP::SwitchArg toRestframeArg("r","restframe","Project spectrum to its rest frame based on the z estimate from SDSS.", false);

		cmd.add( dumpFilenameArg );
		cmd.add( maskFilenameArg );
		cmd.add( outFilenameArg );
		cmd.add( toRestframeArg );

		cmd.parse( argc, argv );

		sstrSourceSpectraFilename = dumpFilenameArg.getValue();
		sstrMaskFilename = maskFilenameArg.getValue();
		sstrOutPlotFilename = outFilenameArg.getValue();
		bToRestFrame = toRestframeArg.getValue();
	}
	catch (TCLAP::ArgException &e)  
	{ 
		Helpers::print( "error: "+e.error()+" for argument "+e.argId()+"\n", &logFile );
	}

	delete[] argv;

	g_spectraMapper = new SpectraMapper( sstrSourceSpectraFilename, sstrMaskFilename, sstrOutPlotFilename );

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
	char captiontext[]={"SDSS Analyze                   "};

	SetWindowText( fr_hWnd, captiontext );

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	g_spectraMapper->draw( scr_width, scr_height, false, g_maskSelection, g_writeData );
	g_writeData = false;

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
	up = 1;
}

void ArrowDown()
{
	down = 1;
}

void Space()
{
	g_Mode++;
	g_writeData = true;
}