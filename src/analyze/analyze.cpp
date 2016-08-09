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
//! \file  analyze.cpp

#include "sdsslib/spectra.h"
#ifdef _WIN32
  #include "sdsslib/glhelper.h"
#endif
#include "sdsslib/defines.h"
#include "sdsslib/random.h"
#include "sdsslib/spectraVFS.h"
#include "sdsslib/spectraWrite.h"
#include "sdsslib/spectraHelpers.h"
#include "sdsslib/spectraBaseHelpers.h"
#include "sdsslib/helpers.h"
#include "sdsslib/sdssSoftwareVersion.h"

#include <stdlib.h>

#ifdef _WIN32

  #include "sdsslib/gltexture.h"
  #include "sdsslib/glshaderprogram.h"
#endif

#include "sdsslib/fileHelpers.h"

#include "framework.h"	
#include "SOFMNetwork.h"

#ifdef _WIN32
  #include "devil/include/il/il.h"
  #include "devil/include/il/ilu.h"
  #include "tclap/CmdLine.h"
#endif

#include <assert.h>

#ifdef _WIN32
  #include <Windows.h>
  #include <conio.h>
#endif
  
#include <string>

#ifdef _WIN32
  extern HWND	fr_hWnd;
  extern HDC fr_hDC;		
  int left,right,up,down;
  int scr_width, scr_height;
#endif
  
#include "mpi.h"

 
SOFMNetwork *g_pSOFM=NULL;

std::ofstream logFile("sofm_log.txt");

int g_CurrentSpectraIndex=0;

size_t g_Mode = 1;	// immediately start with clustering..
bool g_DisableOutput = false;

size_t g_numSpectra = 0;
SpectraVFS *g_pVFSSource = NULL; 


int InitGL()		
{

#ifdef _WIN32
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


	//SpectraVFS::write(30,75.0f, std::string("allspectra3.bin") );


	// convert command line
	int argc;
	std::string sstrCmdLine(GetCommandLine());
	char **argv = Helpers::getCommandLineFromString( sstrCmdLine, argc );
#endif
	
	Helpers::print("Welcome to ASPECT "+sstrSDSSVersionString+" !\n\n\n", &logFile);

	std::string sstrSourceSpectraFilename("allSpectra.bin");
	std::string sstrSelectionListFilename("");
	bool bContinue = false;
#ifdef _WIN32
	try {  

		std::string sstrExamples("examples:\n");
		sstrExamples += std::string("example: analyze.exe -i allSpectra.bin\n");
		sstrExamples += std::string("analyze.exe -i allSpectra.bin -s selection.txt\n");
		sstrExamples += std::string("analyze.exe -c\n");


		TCLAP::CmdLine cmd(sstrExamples, ' ', sstrSDSSVersionString);

		TCLAP::ValueArg<std::string> dumpFilenameArg("i", "inputdumpfile", "example: allSpectra.bin", false, sstrSourceSpectraFilename, "input dump file that contains all SDSS spectra to cluster.");
		TCLAP::ValueArg<std::string> selectionListFilenameArg("s", "selection", "Optional selection list of FITS files to dump a small subset of input spectra. File should contain plate-mjd-fiber pairs, e.g. 3586 55181 0001. First line in the file is the header and is ignored.", false, sstrSelectionListFilename, "selectionlist.txt");
		TCLAP::SwitchArg continueArg("c","continue","Continue computation.", false);
		TCLAP::SwitchArg visualizeOffArg("v","visualizeoff","Disable visualization of computation.", g_DisableOutput);

		cmd.add( dumpFilenameArg );
		cmd.add( selectionListFilenameArg );
		cmd.add( continueArg );
		cmd.add( visualizeOffArg );

		cmd.parse( argc, argv );

		sstrSourceSpectraFilename = dumpFilenameArg.getValue();
		sstrSelectionListFilename = selectionListFilenameArg.getValue();
		bContinue = continueArg.getValue();
		g_DisableOutput = visualizeOffArg.getValue();

	}
	catch (TCLAP::ArgException &e)  
	{ 
		Helpers::print( "Error: "+e.error()+" for argument "+e.argId()+"\n", &logFile );
	}
	delete[] argv;
#else

	std::string sstrExamples("examples:\n");
	sstrExamples += std::string("example: analyze.exe -i allSpectra.bin\n");
	sstrExamples += std::string("analyze.exe -i allSpectra.bin -s selection.txt\n");
	sstrExamples += std::string("analyze.exe -c\n");
	
// 	sstrSourceSpectraFilename = "";
// 	sstrSelectionListFilename = "";
//	bContinue = false;
	

#endif

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
		std::set<std::string> FITSFilenameSet;

		Helpers::print("Reading "+sstrSelectionListFilename+".\n", &logFile);

#ifdef _WIN32
		bool bSuccess = SpectraHelpers::readSelectionList(sstrSelectionListFilename, FITSFilenameSet);
#else		
		bool bSuccess = false;
#endif

		if ( bSuccess && !FITSFilenameSet.empty() )
		{
			size_t nCount = 0;
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
						nCount++;
					}

					g_pVFSSource->endRead(i);
				}
			}

			if (nCount==0)
			{
				Helpers::print("No match between selection and dump found, using unfiltered dump.\n", &logFile);
			}

			pVFSFiltered = new SpectraVFS( "filter.bin", false );
		}
		else
		{
			Helpers::print( "Selection list not found or empty, using unfiltered dump.\n", &logFile );
		}
	}

	// init network
	if ( pVFSFiltered != NULL && pVFSFiltered->getNumSpectra() > 0 )
	{
		Helpers::print( "Using "+Helpers::numberToString<size_t>(pVFSFiltered->getNumSpectra())+" out of "+Helpers::numberToString<size_t>(g_pVFSSource->getNumSpectra())+" spectra.\n", &logFile );
		g_pSOFM = new SOFMNetwork( pVFSFiltered, bContinue, &logFile );
	}
	else
	{
		g_pSOFM = new SOFMNetwork( g_pVFSSource, bContinue, &logFile );
	}

	Helpers::print( "testing performance..\n", &logFile );

	double mioSpectraComparePerSecond;
	double mioSpectraAdaptionPerSecond;
	SpectraBaseHelpers::testSpectraPerformance( mioSpectraComparePerSecond, mioSpectraAdaptionPerSecond );

	if ( mioSpectraComparePerSecond == -1.0 ) 
	{
		Helpers::print( "..skipped. Delete perftest.bin to test performance again.\n", &logFile );
	}
	else
	{
		Helpers::print( Helpers::numberToString<double>(mioSpectraComparePerSecond) +std::string(" million spectra compares per second.\n"), &logFile );
		Helpers::print( Helpers::numberToString<double>(mioSpectraAdaptionPerSecond) +std::string(" million spectra adaption per second.\n"), &logFile );
	}
#ifdef _WIN32
	return TRUE;
#else
	return true;
#endif
}



void UpdateGLView(int width, int height)	
{
  
#ifdef _WIN32
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
#endif
}	




void DrawNetwork( SOFMNetwork &network )
{
  
#ifdef _WIN32
	if (g_DisableOutput)
	{
		ShowWindow( fr_hWnd, SW_HIDE );
		return;
	}
	glClearColor(1.f,1.f,1.f,1.f);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	


	const size_t nMaxGridSize = 40;

	size_t gridSize = MIN( network.m_gridSize, nMaxGridSize );
	size_t stepSize = network.m_gridSize / gridSize;

	float w = scr_width/(gridSize);
	float h = scr_height/(gridSize);
	glColor3f(0.0,0,0);

	std::vector<float> vectorField;
	SpectraVFS &mapNew(network.getNetwork());
//	SpectraVFS mapOld( mapNew.getFileName()+"old" );
	bool bDrawVelocityField = false;
//	if ( mapOld.getNumSpectra() == mapNew.getNumSpectra() )
//	{
//		bDrawVelocityField = SpectraHelpers::calcVectorField( mapNew, mapOld, vectorField );
//	}

	glLineWidth(2.f);

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

	float maxAmplitude=-FLT_MAX;

	size_t yp=gridSize;
	for ( size_t y=0;y<network.m_gridSize;y+=stepSize)
	{
		yp--;
		size_t xp=0;
		for ( size_t x=0;x<network.m_gridSize;x+=stepSize)
		{
			SSE_ALIGN Spectra spectra;
			network.getSOFMSpectra(x,y,spectra);
			spectra.calcMinMax();
			if (maxAmplitude<spectra.m_Max)
			{
				maxAmplitude = spectra.m_Max;
			}
		}
	}


	float yscale = 1.f/fabsf(maxAmplitude*1.05f);

	size_t vecCount = 0;

	yp=gridSize;
	for ( size_t y=0;y<network.m_gridSize;y+=stepSize)
	{
		yp--;
		size_t xp=0;
		for ( size_t x=0;x<network.m_gridSize;x+=stepSize)
		{

			if ( bDrawVelocityField )
			{
				size_t av = (y*network.m_gridSize+x)*5;
				if ( vectorField[av+2] != 0.0f && vectorField[av+3] != 0.0 )
				{
					float sx1 = vectorField[av+0]*w+w/2;
					float sy1 = vectorField[av+1]*h+h/2;
					float sx2 = sx1+vectorField[av+2]*w;
					float sy2 = sy1+vectorField[av+3]*h;

					glColor3f((float)vecCount/(float)mapNew.getNumSpectra(), 1.0f, 0.f);
					GLHelper::DrawLine( sx1, sy1, sx2, sy2 );
					GLHelper::DrawPoint( sx1, sy1, 5.f );
					vecCount++;
				}
			}
			glLineWidth(2.f);


			SSE_ALIGN Spectra spectra;
			network.getSOFMSpectra(x,y,spectra);
			if ( spectra.isEmpty() )
			{
				GLHelper::SetLineStipple(3);
				glColor3f(0.0,0.0,0.5);
			}
			else
			{
				glColor3f(0,0,0);
			}
			float lp[3] = {xp*w+w/10,gridSize*h-(yp*h+h-h/8),-10};

			SpectraHelpers::drawSpectra(spectra, false, false, xp*w, yp*h, w, h, yscale );
			GLHelper::SetLineStipple(0,0);
			glLineWidth(1.f);

// 			SSE_ALIGN Spectra sourceSpectrum;
// 			if ( network.getInputSpectrum(x,y,sourceSpectrum) )
// 			{
// 				glColor3f(0,0,0 );
// 				if ( sourceSpectrum.pad[0] == 1 )
// 					GLHelper::Print(SpectraHelpers::getDefaultFontID(), lp, "cos(x)" );
// 				if ( sourceSpectrum.pad[0] == 2 )
// 					GLHelper::Print(SpectraHelpers::getDefaultFontID(), lp, "x" );
// 				if ( sourceSpectrum.pad[0] == 3 )
// 					GLHelper::Print(SpectraHelpers::getDefaultFontID(), lp, "-x" );
// 				if ( sourceSpectrum.pad[0] == 4 )
// 					GLHelper::Print(SpectraHelpers::getDefaultFontID(), lp, "x*x" );
// 			}
			xp++;
		}
	}
/* qt cluster vis
	size_t nc = network.m_QTCluster->getNumberOfClusters();
	for (size_t i=0;i<nc;i++)
	{
		size_t sc = network.m_QTCluster->getNumberOfSpectraForCluster(i);
		for (size_t j=0;j<sc;j++)
		{
			size_t spi = network.m_QTCluster->getSpectra(i,j);
			size_t yp=spi/network.m_gridSize;
			size_t xp=spi%network.m_gridSize;

			float lp[3] = {xp*w,yp*h+h*0.5f,-10};

			glColor3f(1,1,1);
			if ( i==0 )
			{
				GLHelper::Print(SpectraHelpers::getDefaultFontID(), lp, "uc" );
			}
			else
			{
				GLHelper::Print(SpectraHelpers::getDefaultFontID(), lp, "c%i", i );
			}

		}
	}
*/

#endif
}



void ShowTrainData()
{
  
#ifdef _WIN32
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
#endif
}

bool finished=false;
bool bFirst = true;
void SOFM()
{
	if ( !bFirst && !finished )
	{
		finished = g_pSOFM->process();
		if ( finished )
		{
			exit(0);
		}
	}
	DrawNetwork( *g_pSOFM );

	if ( bFirst )
	{
		bFirst = false;
#ifdef _WIN32
		SwapBuffers(fr_hDC);
		SwapBuffers(fr_hDC);
#endif
	}

}

void DrawGLScene()
{
#ifdef _WIN32
        std::string sstrTitle("ASPECT: A spectra clustering tool for exploration of large spectral surveys. Step ");

	sstrTitle += Helpers::numberToString( g_pSOFM->m_currentStep ) + " / "+  Helpers::numberToString( g_pSOFM->m_params.numSteps );


	SetWindowText( fr_hWnd, sstrTitle.c_str() );
	g_Mode = g_Mode % 2;
/*
	if (g_Mode==1)
	{
		if ( up==1)up=0;
		else return;
	}
*/


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	


//	if (g_Mode==0)
	{
//		ShowTrainData();
	}
//	if (g_Mode==1)
	{
		SOFM();
	}
	

	Sleep(100);
#endif
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

#ifdef _WIN32
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

void KeyOne()
{
}

void KeyTwo()
{
}
#endif

#ifdef __linux__
int main(int argc, char* argv[]) {
#ifdef OPEN_MPI
    MPI_Init(&argc, &argv);
    int cluster_myrank;
    int cluster_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &cluster_myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &cluster_size);
#endif
 InitGL(); 

  bool finished=false;
  bool bFirst = true;
  
  
  while (finished == false ) {
      finished = g_pSOFM->process();
      if ( finished )
      {
#ifdef OPEN_MPI	
	MPI_Finalize();
#endif
	  exit(0);
      }
  }
#ifdef OPEN_MPI
   MPI_Finalize();
#endif
  exit(0);
}
#endif //linux