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
//! \file  analyze.cpp

#include "sdsslib/spectra.h"
#include "sdsslib/glhelper.h"
#include "sdsslib/defines.h"
#include "sdsslib/random.h"
#include "sdsslib/spectraVFS.h"
#include "sdsslib/spectraWrite.h"
#include "sdsslib/spectraHelpers.h"
#include "sdsslib/helpers.h"
#include "sdsslib/sdssSoftwareVersion.h"

#include "framework.h"	
#include "SOFMNetwork.h"
#include "QTClustering.h"

#include "devil/include/il/il.h"
#include "devil/include/il/ilu.h"

#include "tclap/CmdLine.h"

#include <assert.h>
#include <Windows.h>
#include <shellapi.h>

#include <string>



extern HWND	fr_hWnd;
extern HDC fr_hDC;		
int left,right,up,down;
int scr_width, scr_height;

QTClustering *g_QTCluster;
SOFMNetwork *g_pSOFM=NULL;

std::ofstream logFile("sofm_log.txt");

int g_CurrentSpectraIndex=0;

size_t g_Mode = 1;	// immediately start with clustering..
bool g_DisableOutput=false;

size_t g_numSpectra = 0;
SpectraVFS *g_pVFSSource = NULL; 


int InitGL()		
{
	Helpers::createConsole();

	SpectraHelpers::init( fr_hDC );

	// opengl init stuff
	glShadeModel(GL_SMOOTH);							
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				
	glClearDepth(1.0f);									
	glEnable(GL_DEPTH_TEST);							
	glDepthFunc(GL_LEQUAL);			
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	

	//SpectraVFS::write(30,75.0f, std::string("allspectra3.bin") );

	// convert commandline
	int argc;
	std::string sstrCmdLine(GetCommandLine());
	size_t commandlineSize = sstrCmdLine.size();
	WCHAR *commandlinewc = new WCHAR[commandlineSize];
	char *commandline = new char[commandlineSize];
	mbstowcs( commandlinewc, GetCommandLine(), 16384 );
	LPWSTR *argvwc = CommandLineToArgvW( commandlinewc, &argc );
	char **argv = new char*[argc+1];
	size_t c=0;
	for (int i=0;i<argc;i++) {
		argv[i] = commandline+c;
		size_t len =  wcslen(argvwc[i]);
		wcstombs(&commandline[c],argvwc[i], len);
		c+= len+1;
		commandline[c-1] = 0;
	}
	argv[argc] = NULL; // terminate with zero.

	Helpers::print("Welcome to SDSS Analyze!\n\n\n", &logFile);


	std::string sstrSourceSpectraFilename("allSpectra.bin");
	std::string sstrSelectionListFilename("");
	bool bContinue = false;

	try {  

		std::string sstrExamples("examples:\n");
		sstrExamples += std::string("example: analyze.exe -i allSpectra.bin\n");
		sstrExamples += std::string("analyze.exe -i allSpectra.bin -s selection.txt\n");
		sstrExamples += std::string("analyze.exe -c\n");

		TCLAP::CmdLine cmd(sstrExamples, ' ', SDSSVERSIONSTRING);

		TCLAP::ValueArg<std::string> dumpFilenameArg("i", "inputdumpfile", "example: allSpectra.bin", false, sstrSourceSpectraFilename, "input dump file that contains all spectra to compare with.");
		TCLAP::ValueArg<std::string> selectionListFilenameArg("s", "selection", "Optional selection list of FITS files to cluster a small subset of input spectra.", false, sstrSelectionListFilename, "selectionlist.txt");
		TCLAP::SwitchArg continueArg("c","continue","Continue computation.", false);

		cmd.add( dumpFilenameArg );
		cmd.add( selectionListFilenameArg );
		cmd.add( continueArg );

		cmd.parse( argc, argv );

		sstrSourceSpectraFilename = dumpFilenameArg.getValue();
		sstrSelectionListFilename = selectionListFilenameArg.getValue();
		bContinue = continueArg.getValue();
	}
	catch (TCLAP::ArgException &e)  
	{ 
		Helpers::print( "error: "+e.error()+" for argument "+e.argId()+"\n", &logFile );
	}


	Helpers::print( "Reading dump file "+sstrSourceSpectraFilename+ "\n", &logFile );

	g_pVFSSource = new SpectraVFS( sstrSourceSpectraFilename, false );
	g_numSpectra = g_pVFSSource->getNumSpectra();

	if ( g_numSpectra == 0 )
	{
		Helpers::print("Error: No spectral data found.\n", &logFile);
		delete g_pVFSSource;
		return false;
	}
	else
	{
		Helpers::print(Helpers::numberToString<size_t>(g_numSpectra)+" spectra found.\n", &logFile);
	}

	SpectraVFS *pVFSFiltered=NULL;

	if ( !sstrSelectionListFilename.empty() )
	{
		std::ifstream fin(sstrSelectionListFilename.c_str());
		Helpers::print( "Reading selection list "+sstrSelectionListFilename+"\n", &logFile );

		if( fin ) 
		{
			// read selection list
			std::set<std::string> FITSFilenameSet;
			std::string sstrFITSFilename;
			while( getline(fin,sstrFITSFilename) ) 
			{
				FITSFilenameSet.insert(sstrFITSFilename);
			}

			if ( !FITSFilenameSet.empty() )
			{
				// create filtered dump
				{
					SpectraWrite w(std::string("filter.bin"));

					for (size_t i=0;i<g_numSpectra;i++)
					{
						Spectra *a = g_pVFSSource->beginRead(i);

						std::set<std::string>::iterator it( FITSFilenameSet.find( a->getFileName() ) );
						if (it != FITSFilenameSet.end() )
						{
							w.write(*a);
						}

						g_pVFSSource->endRead(i);
					}
				}

				pVFSFiltered = new SpectraVFS( "filter.bin", false );
			}
		}
		else
		{
			Helpers::print( "Selection list not found, using unfiltered dump.\n", &logFile );
		}
	}

	// init network
	if ( pVFSFiltered != NULL && pVFSFiltered->getNumSpectra() > 0 )
	{
		Helpers::print( "Using "+Helpers::numberToString<size_t>(pVFSFiltered->getNumSpectra())+" out of "+Helpers::numberToString<size_t>(g_pVFSSource->getNumSpectra())+" spectra.\n", &logFile );
		g_pSOFM = new SOFMNetwork( pVFSFiltered, bContinue );
	}
	else
	{
		g_pSOFM = new SOFMNetwork( g_pVFSSource, bContinue );
	}


	//g_QTCluster = new QTClustering( g_pVFSSource, QTClustering::Parameters(8.f, 0.0f, 2 ) );
	//g_QTCluster->Process();
	//g_QTCluster->Export(std::string("export/qtclustering"));
	//exit(0);

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




void DrawNetwork( SOFMNetwork &network )
{
	if (g_DisableOutput)
		return;

	const size_t nMaxGridSize = 40;

	size_t gridSize = MIN( network.m_gridSize, nMaxGridSize );
	size_t stepSize = network.m_gridSize / gridSize;

	float w = scr_width/(gridSize);
	float h = scr_height/(gridSize);
	glColor3f(1,0,0);

	for ( size_t y=0;y<gridSize;y++)
	{
		float lb1[3] = {0,y*h,-10};
		float lb2[3] = {scr_width,y*h,-10};
		GLHelper::DrawLine( lb1, lb2 );
	}

	for ( size_t x=0;x<gridSize;x++)
	{
		float lb1[3] = {x*w,0,-10};
		float lb2[3] = {x*w,scr_height,-10};
		GLHelper::DrawLine( lb1, lb2 );
	}

	float yscale = 1.f/(network.m_Max*0.05-network.m_Min);

	size_t yp=gridSize;
	for ( size_t y=0;y<network.m_gridSize;y+=stepSize)
	{
		yp--;
		size_t xp=0;
		for ( size_t x=0;x<network.m_gridSize;x+=stepSize)
		{
			Spectra &spectra(network.getSOFMSpectra(x,y));
			if ( spectra.isEmpty() )
			{
				glColor3f(0.8,0.8,0);
			}
			else
			{
				glColor3f(1,1,1);
			}
			float lp[3] = {xp*w,yp*h+h,-10};

			SpectraHelpers::drawSpectra(spectra, false, false, xp*w, yp*h, w, h, yscale );

//			if ( !spectra.m_strFileName.empty() )
//			{
//			glColor3f(1,1,1);
//			GLHelper::Print(SpectraHelpers::s_FontID, lp, spectra.m_strFileName.c_str() );
//			}
			xp++;
		}
	}
}



void ShowTrainData()
{
	if (g_numSpectra==0)
		return;

	Spectra *a = g_pVFSSource->beginRead( g_CurrentSpectraIndex );
	SpectraHelpers::drawSpectra( *a, true, false, 0, 0, scr_width, scr_height, 0.99f/a->m_Max );
	g_pVFSSource->endRead( g_CurrentSpectraIndex );

	if ( up || down )
	{
		if ( up )
		{
			g_CurrentSpectraIndex--;
		}
		if ( down )
		{
			g_CurrentSpectraIndex++;
		}

		g_CurrentSpectraIndex = CLAMP( g_CurrentSpectraIndex, 0, static_cast<int>(g_numSpectra-1) );
	}


	left = 0;
	right = 0;
	up = 0;
	down = 0;
}

void SOFM()
{
	DrawNetwork( *g_pSOFM );
	g_pSOFM->process();
}

void DrawGLScene()
{
	char captiontext[]={"SDSS Analyze                   "};

	sprintf( &captiontext[13], "%i / %i", g_pSOFM->m_currentStep, g_pSOFM->m_params.numSteps);

	SetWindowText( fr_hWnd, captiontext );
	g_Mode = g_Mode % 2;
/*
	if (g_Mode==1)
	{
		if ( up==1)up=0;
		else return;
	}
*/

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	


	if (g_Mode==0)
	{
		ShowTrainData();
	}
	if (g_Mode==1)
	{
		SOFM();
	}
	

	Sleep(100);
}


void KillGL()	
{
}


void Export()
{
	g_pSOFM->exportToHTML("output.html");
}

void DisableOutput()
{
	g_DisableOutput = !g_DisableOutput;
	if ( g_DisableOutput )
	{
		Helpers::print("Graphic output disabled.\n");
	}
	else
	{
		Helpers::print("Graphic output enabled.\n");
	}
}

void ArrowLeft()
{
	left = 1;
}


void ArrowRight()
{
	right = 1;
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
}