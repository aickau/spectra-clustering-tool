#include "framework.h"	

#include <string>
#include <conio.h>
#include <assert.h>
#include <sstream>

#include "sdsslib/defines.h"
#include "sdsslib/helpers.h"
#include "sdsslib/spectra.h"
#include "sdsslib/glhelper.h"

extern HWND		fr_hWnd;

int left,right,up,down;
int scr_width, scr_height;

extern HDC fr_hDC;		


std::vector<std::string> g_FileList;

int g_FontID=0;
Spectra g_Spectrum;
float g_YScale=2.0f;




int InitGL( const std::string &sstrCmdLine )		
{
	std::string sstrFileName;
	std::istringstream sstrStream(sstrCmdLine);
	if (sstrStream) {
		sstrStream >> sstrFileName;	
	}

	if ( sstrFileName.empty() )
	{
		return FALSE;
	}

	if ( sstrFileName[0] == '"' )
	{
		sstrFileName.erase( sstrFileName.begin() );
		sstrFileName.erase( --sstrFileName.end() );
	}

	std::string sstrCaptionText("SDSS Viewer - ");
	sstrCaptionText += sstrFileName;
	SetWindowText(fr_hWnd, sstrCaptionText.c_str());

	// opengl init stuff
	glShadeModel(GL_SMOOTH);							
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				
	glClearDepth(1.0f);									
	glEnable(GL_DEPTH_TEST);							
	glDepthFunc(GL_LEQUAL);			
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	

	g_FontID = GLHelper::BuildFont(fr_hDC, "Arial", 16, true, false);

	bool bRetVal=false;
	std::string sstrExtension(Helpers::getFileExtension(sstrFileName));
	if ( sstrExtension == ".fit" )
	{
		bRetVal = g_Spectrum.loadFromFITS( sstrFileName );
	}

	return bRetVal;
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
	int stepSize = MAX(floorf(1.0/xscale),1);

	glBegin(GL_LINE_STRIP);
	for ( int i=0;i<spectra.m_SamplesRead-1;i+=stepSize)
	{
		glVertex3f(static_cast<float>(i)*xscale+xoffset, -spectra.m_Amplitude[i]*yscale+yoffset, -10.f );
	}
	glEnd();
	if (showfilename)
	{
		float pos[]={10,20,-10};
		GLHelper::Print( g_FontID, pos, spectra.getFileName().c_str() );
	}

	float pos[]={10,40,-10};
	std::stringstream sstream;
	sstream << "min: " << spectra.m_Min << "  max: " << spectra.m_Max;
	GLHelper::Print( g_FontID, pos, sstream.str().c_str() );
}



void DrawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	DrawSpectra( g_Spectrum, true, 0,scr_height*3/4,static_cast<float>(scr_width-10)/static_cast<float>(Spectra::numSamples),g_YScale );


	Sleep(50);
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
}


void ArrowRight()
{
	right = 1;
}

void ArrowUp()
{
	g_YScale +=0.2f;
	up = 1;
}

void ArrowDown()
{
	if ( g_YScale > 0.0f )
	{
		g_YScale -=0.2f;
	}
	down = 1;
}

void Space()
{
}