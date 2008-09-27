#include "framework.h"	

#include <string>
#include <conio.h>
#include <assert.h>
#include <sstream>

#include "devil/include/il/il.h"
#include "devil/include/il/ilu.h"

#include "sdsslib/spectra.h"
#include "sdsslib/glhelper.h"
#include "sdsslib/defines.h"
#include "sdsslib/random.h"
#include "sdsslib/spectraVFS.h"
#include "sdsslib/helpers.h"

#include "SOFMNetwork.h"
#include "QTClustering.h"


extern HWND	fr_hWnd;
extern HDC fr_hDC;		
int left,right,up,down;
int scr_width, scr_height;

QTClustering *g_QTCluster;
SOFMNetwork *g_pSOFM=NULL;


int g_CurrentSpectraIndex=0;

int g_FontID=0;
size_t g_Mode = 0;
bool g_DisableOutput=false;

size_t g_numSpectra = 0;
SpectraVFS *g_pVFSSource = NULL; 




int InitGL( const std::string &sstrCmdLine )		
{
	mt_initrandom(23);
	AllocConsole();

	ilInit();

	// opengl init stuff
	glShadeModel(GL_SMOOTH);							
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				
	glClearDepth(1.0f);									
	glEnable(GL_DEPTH_TEST);							
	glDepthFunc(GL_LEQUAL);			
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	

	g_FontID = GLHelper::BuildFont(fr_hDC, "Arial", 16, true, false);

	//SpectraVFS::write(30,75.0f, std::string("allspectra3.bin") );

	bool bContinue = false;
	std::string sstrSourceSpectraFileName("allspectra.bin");


	_cprintf("Welcome to SDSS Analyze!\n\n\n");

	if ( sstrCmdLine.empty() )
	{
		_cprintf( "No parameters specified. Using default settings.\n\n");
		_cprintf( "usage: sdss.exe <source spectra filename>\n");
		_cprintf( "usage: sdss.exe /c\n");
		_cprintf( "example: sdss.exe allSpectra.bin\n\n");
		_cprintf( "example: sdss.exe /c\n continues computation.\n\n");

	}
	else if(sstrCmdLine=="/c")
	{
		bContinue=true;
	}
	else
	{
		std::istringstream sstrStream(sstrCmdLine);

		if (sstrStream) {
			sstrStream >> sstrSourceSpectraFileName;	
		}
	}
	_cprintf( "Reading dump file " );
	_cprintf( sstrSourceSpectraFileName.c_str() );
	_cprintf( "\n" );
	g_pVFSSource = new SpectraVFS( sstrSourceSpectraFileName, true );
	g_numSpectra = g_pVFSSource->getNumSpectra();

	if ( g_numSpectra == 0 )
	{
		_cprintf("Error: No spectral data found.\n");
		delete g_pVFSSource;
		return false;
	}
	else
	{
		_cprintf("%i spectra found.\n",g_numSpectra);
	}



	// init network
	g_pSOFM = new SOFMNetwork( g_pVFSSource, bContinue );

/*
	g_QTCluster = new QTClustering( g_pVFSSource, QTClustering::Parameters(10.f, 0.0f, 2 ) );
	g_QTCluster->Process();
	g_QTCluster->Export(std::string("export/qtclustering"));
*/
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


void DrawSpectra(Spectra &spectra, bool showfilename, float xoffset = 0.f, float yoffset=300, float xscale=0.8f, float yscale=2.f)
{
	if ( spectra.m_SamplesRead <= 2 )
		return;

	GLHelper::DrawDiagram( &spectra.m_Amplitude[0], spectra.m_SamplesRead-1, 4, 0, xoffset, yoffset, xscale, yscale );

	if (showfilename)
	{
		float pos[]={10,20,-10};
		GLHelper::Print( g_FontID, pos, spectra.getFileName().c_str() );
	}
}



void DrawNetwork( SOFMNetwork &network )
{
	if (g_DisableOutput)
		return;

	const size_t nMaxGridSize = 20;

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

	float yscale = h*20.f/(network.m_Max-network.m_Min);

	size_t yp=0;
	for ( size_t y=0;y<network.m_gridSize;y+=stepSize)
	{
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

			DrawSpectra(spectra, false, xp*w, yp*h+(h*3)/4, static_cast<float>(w)/static_cast<float>(Spectra::numSamples), yscale );

//			if ( !spectra.m_strFileName.empty() )
//			{
//			glColor3f(1,1,1);
//			GLHelper::Print(g_FontID, lp, spectra.m_strFileName.c_str() );
//			}
			xp++;
		}
		yp++;
	}
}


void RenderDiagramToDisk( float *_values, size_t _valueCount, size_t _strideInBytes, size_t _offsetInBytes, 
						  size_t _width, size_t _height, const std::string &sstrFilename )
{
	assert(_values != NULL);
	assert(_width<=scr_width);
	assert(_height<=scr_height);

	ILuint image;
	ilGenImages( 1, &image );
	ilBindImage(image);
	ilTexImage(_width,_height,1,3,IL_RGB, IL_UNSIGNED_BYTE, NULL );
	ilEnable(IL_FILE_OVERWRITE );
	iluImageParameter(ILU_FILTER,ILU_SCALE_BSPLINE);

	glColor3f(1,1,1);
	glLineWidth(1.f);
	glClearColor(0.0f,0.0f,0.f,0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	float yMin, yMax;

	Helpers::getMinMax( _values, _valueCount, _strideInBytes, _offsetInBytes , yMin, yMax );

	float xs = static_cast<float>(_width)/static_cast<float>(_valueCount);
	float ys = static_cast<float>(_height*3.f/4.f)/yMax;

	GLHelper::DrawDiagram( _values, _valueCount, _strideInBytes, _offsetInBytes, 0, (_height*3)/4, xs, ys );

	glReadPixels(0,scr_height-_height,_width,_height,GL_RGB, GL_UNSIGNED_BYTE, ilGetData());
	ilSave( IL_PNG, const_cast<char*>(sstrFilename.c_str()) );
	ilDeleteImage(image);
}




void RenderSpectraIconToDisk( Spectra &_spectra, const std::string &_sstrFilename, size_t _width, size_t _height, float _yMax, float _redness )
{
	if ( Helpers::fileExists(_sstrFilename) )
		return;

	size_t w4=_width*8;
	size_t h4=_height*8;

	assert(w4<=scr_width);
	assert(h4<=scr_height);

	ILuint image;
	ilGenImages( 1, &image );
	ilBindImage(image);
	ilTexImage(w4,h4,1,3,IL_RGB, IL_UNSIGNED_BYTE, NULL );
	ilEnable(IL_FILE_OVERWRITE );
	iluImageParameter(ILU_FILTER,ILU_SCALE_BSPLINE);

	glColor3f(1,1,1);
	glLineWidth(12.f);
	
	float g = MAX(_redness-1.f, 0.f);

	glClearColor(_redness,g,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	DrawSpectra( _spectra, false, 0, h4*3/4, static_cast<float>(w4)/Spectra::numSamples, static_cast<float>(h4*3.f/4.f)/_yMax );

	glReadPixels(0,scr_height-h4,w4,h4,GL_RGB, GL_UNSIGNED_BYTE, ilGetData());
	iluScale(_width,_height,1);
	ilSave( IL_PNG, const_cast<char*>(_sstrFilename.c_str()) );
	ilDeleteImage(image);

	glLineWidth(1.f);
	glClearColor(0.0,0,0,0);

}


void ShowTrainData()
{
	if (g_numSpectra==0)
		return;

	Spectra *a = g_pVFSSource->beginRead( g_CurrentSpectraIndex );
	DrawSpectra( *a, true, 0, (3*scr_height)/4, static_cast<float>(scr_width-10)/static_cast<float>(Spectra::numSamples), 1.5f );
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
	g_pSOFM->Process();
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
	g_pSOFM->Export("output.html");
}

void DisableOutput()
{
	g_DisableOutput = !g_DisableOutput;
	if ( g_DisableOutput )
	{
		_cprintf("Graphic output disabled.\n");
	}
	else
	{
		_cprintf("Graphic output enabled.\n");
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