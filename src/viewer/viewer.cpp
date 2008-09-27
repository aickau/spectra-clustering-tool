#include "framework.h"	

#include <string>
#include <conio.h>
#include <assert.h>
#include <sstream>

#include "sdsslib/defines.h"
#include "sdsslib/helpers.h"
#include "sdsslib/spectra.h"
#include "sdsslib/glhelper.h"
#include "sdsslib/spectraHelpers.h"

extern HWND		fr_hWnd;

int left,right,up,down;
int scr_width, scr_height;

extern HDC fr_hDC;		


std::vector<std::string> g_FileList;

int g_FontID=0;
Spectra g_Spectrum;
float g_YScale=1.0f;




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

	SpectraHelpers::Init( fr_hDC );

	// opengl init stuff
	glShadeModel(GL_SMOOTH);							
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				
	glClearDepth(1.0f);									
	glEnable(GL_DEPTH_TEST);							
	glDepthFunc(GL_LEQUAL);			
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	

	SpectraHelpers::CombineSpectra( std::string("allSpectra_qso.bin"), "allSpectra.png" );

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



void DrawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	SpectraHelpers::DrawSpectra( g_Spectrum, true, false, 0,0,scr_width,scr_height,g_YScale );

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
	g_YScale +=0.05f;
	up = 1;
}

void ArrowDown()
{
	if ( g_YScale > 0.0f )
	{
		g_YScale -=0.05f;
	}
	down = 1;
}

void Space()
{
}