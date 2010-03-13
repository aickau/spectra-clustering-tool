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
bool g_DisableOutput = false;

size_t g_numSpectra = 0;
SpectraVFS *g_pVFSSource = NULL; 


#include "sdsslib/gltexture.h"
#include "sdsslib/glshaderprogram.h"
#include "sdsslib/filehelpers.h"
#include "sdsslib/glFBO.h"

bool testGPUcompare( const Spectra &_src, SpectraVFS &_vfs )
{
	if ( _vfs.getNumSpectra() < 1 )
	{
		return false;
	}
	Spectra *a = _vfs.beginRead(0);
	_vfs.endRead(0);


	GLTexture t0;
	GLTexture t1;

	const size_t numSamples = 4; // Spectra::numSamples

	float amp0[numSamples]={0.0f,1.0f,0.0,2.0f};
	float amp1[numSamples]={-0.5f,0.0f,1.0,-1.0f};

	size_t sx=1, sy=1;

	GLFBO fbo(sx,sy);
	GLHelper::PrintError();

	fbo.attachRenderBuffer(GLFBO::RB_COLOR0);
	GLHelper::PrintError();
	fbo.attachRenderBuffer(GLFBO::RB_DEPTH);
	GLHelper::PrintError();
	fbo.bind();
	GLHelper::PrintError();

	t0.setTexture(numSamples/4, 1, GL_RGBA, GL_FLOAT, false, (void*)&amp0[0] );
	GLHelper::PrintError();
	t1.setTexture(numSamples/4, 1, GL_RGBA, GL_FLOAT, false,  (void*)&amp1[0] );


	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();			
	glOrtho(0,sx, sy, 0, 1, 100);
	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();			
	GLHelper::PrintError();


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	GLHelper::PrintError();


	std::string shaderSourceV;
	std::string shaderSourceP;
	FileHelpers::loadFileToString( "src/sdsslib/comparev.glsl", shaderSourceV );
	FileHelpers::loadFileToString( "src/sdsslib/comparep.glsl", shaderSourceP );
	
	GLShaderProgram sh;
	sh.create( shaderSourceV, shaderSourceP );
	sh.bind();

	
	t0.bind(0);
	t1.bind(1);

	sh.setSampler( "t0", 0 );
	sh.setSampler( "t1", 1 );

	float x= 1;
	float y= 1;

	float p[] = {
		0.0f, 0.0f,-2.f,
		x,0.f,-2.f,
		x,y,-2.f,
		0.f,y,-2.f
	};

	float t[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.f, 1.0f
	};

	glColor3f(1,0,0);
	GLHelper::DrawQuad(p,t);
	GLHelper::PrintError();



	t1.unbind(1);
	t0.unbind(0);
	sh.unbind();

	GLHelper::PrintError();
	float res[numSamples*4];
	glReadPixels(0,sy-1,numSamples/4, 1, GL_RGBA, GL_FLOAT, &res[0] );
	GLHelper::PrintError();

	fbo.unbind();


	return true;
}


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

	//SpectraVFS::write(30,75.0f, std::string("allspectra3.bin") );


	// convert command line
	int argc;
	std::string sstrCmdLine(GetCommandLine());
	char **argv = Helpers::getCommandLineFromString( sstrCmdLine, argc );

	Helpers::print("Welcome to SDSS Analyze "+sstrSDSSVersionString+" !\n\n\n", &logFile);


	std::string sstrSourceSpectraFilename("allSpectra.bin");
	std::string sstrSelectionListFilename("");
	bool bContinue = false;

	try {  

		std::string sstrExamples("examples:\n");
		sstrExamples += std::string("example: analyze.exe -i allSpectra.bin\n");
		sstrExamples += std::string("analyze.exe -i allSpectra.bin -s selection.txt\n");
		sstrExamples += std::string("analyze.exe -c\n");

		TCLAP::CmdLine cmd(sstrExamples, ' ', sstrSDSSVersionString);

		TCLAP::ValueArg<std::string> dumpFilenameArg("i", "inputdumpfile", "example: allSpectra.bin", false, sstrSourceSpectraFilename, "input dump file that contains all spectra to compare with.");
		TCLAP::ValueArg<std::string> selectionListFilenameArg("s", "selection", "Optional selection list of FITS files to cluster a small subset of input spectra.", false, sstrSelectionListFilename, "selectionlist.txt");
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
		Helpers::print( "error: "+e.error()+" for argument "+e.argId()+"\n", &logFile );
	}

	delete[] argv;


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
		std::map<std::string,float> FITSFilenameSet;

		Helpers::print("Reading "+sstrSelectionListFilename+".\n", &logFile);

		bool bSuccess = SpectraHelpers::readSelectionList(sstrSelectionListFilename, FITSFilenameSet);

		if ( bSuccess && !FITSFilenameSet.empty() )
		{
			size_t nCount = 0;
			// create filtered dump
			{
				SpectraWrite w(std::string("filter.bin"));

				for (size_t i=0;i<g_numSpectra;i++)
				{
					Spectra *a = g_pVFSSource->beginRead(i);

					std::map<std::string,float>::iterator it( FITSFilenameSet.find( a->getFileName() ) );
					if (it != FITSFilenameSet.end() )
					{
						if ( it->second != 1.f )
						{
							Helpers::print( "multiplying spectrum "+a->getFileName()+" with "+ Helpers::numberToString<float>(it->second)+"\n", &logFile );
							a->multiply(it->second);
						}
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
	SpectraHelpers::testSpectraPerformance( mioSpectraComparePerSecond, mioSpectraAdaptionPerSecond );

	if ( mioSpectraComparePerSecond == -1.0 ) 
	{
		Helpers::print( "..skipped. Delete perftest.bin to test performance again.\n", &logFile );
	}
	else
	{
		Helpers::print( Helpers::numberToString<double>(mioSpectraComparePerSecond) +std::string(" million spectra compares per second.\n"), &logFile );
		Helpers::print( Helpers::numberToString<double>(mioSpectraAdaptionPerSecond) +std::string(" million spectra adaption per second.\n"), &logFile );
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
	{
		ShowWindow( fr_hWnd, SW_HIDE );
		return;
	}
	glClearColor(0.f,0.f,0.f,1.f);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	


	const size_t nMaxGridSize = 40;

	size_t gridSize = MIN( network.m_gridSize, nMaxGridSize );
	size_t stepSize = network.m_gridSize / gridSize;

	float w = scr_width/(gridSize);
	float h = scr_height/(gridSize);
	glColor3f(1,0,0);

	std::vector<float> vectorField;
	SpectraVFS &mapNew(network.getNetwork());
	SpectraVFS mapOld( mapNew.getFileName()+"old" );
	bool bDrawVelocityField = false;
	if ( mapOld.getNumSpectra() == mapNew.getNumSpectra() )
	{
		bDrawVelocityField = SpectraHelpers::calcVectorField( mapNew, mapOld, vectorField );
	}

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


	float yscale = 1.f/fabsf(maxAmplitude*0.75f);

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

			SSE_ALIGN Spectra spectra;
			network.getSOFMSpectra(x,y,spectra);
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
	g_pSOFM->process();
	DrawNetwork( *g_pSOFM );
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
/*
	Spectra *a = g_pVFSSource->beginRead(1);
	testGPUcompare( *a, *g_pVFSSource );
	g_pVFSSource->endRead(1);
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