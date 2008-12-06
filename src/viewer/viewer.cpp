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
int fileListPos = 0;

extern HDC fr_hDC;		


std::vector<std::string> g_spectraFileList;

int g_FontID=0;
Spectra g_Spectrum;
float g_YScale=2.0f;




int InitGL( const std::string &sstrCmdLine )		
{
	std::string sstrFileName("");
	std::istringstream sstrStream(sstrCmdLine);
	if (sstrStream) {
		sstrStream >> sstrFileName;	
	}

	bool bRetVal=true;
	if ( !sstrFileName.empty() )
	{
		if ( sstrFileName[0] == '"' )
		{
			sstrFileName.erase( sstrFileName.begin() );
			sstrFileName.erase( --sstrFileName.end() );
		}

		std::string sstrExtension(Helpers::getFileExtension(sstrFileName));
		if ( sstrExtension == ".fit" )
		{
			bRetVal = g_Spectrum.loadFromFITS( sstrFileName );
		}
	}
	else
	{
		g_Spectrum.setSine(1.f,1.f,1.f);
	}

	// get spectra files for current dir
	std::string sstrCurrentDir( Helpers::getCurrentDir() );
	sstrCurrentDir += std::string("/*");
	std::vector<std::string> fileList;
	Helpers::getFileList( sstrCurrentDir, fileList );
	for (size_t i=0;i<fileList.size();i++) {
		if ( Helpers::getFileExtension(fileList[i]) == std::string(".fit") )
		{
			g_spectraFileList.push_back( fileList[i] );
		}
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
	if ( fileListPos > 0 ) {
		fileListPos--;
		g_Spectrum.loadFromFITS( g_spectraFileList[fileListPos] );
	}
	left = 1;
}


void ArrowRight()
{
	if ( fileListPos+1 < g_spectraFileList.size() ) {
		fileListPos++;
		g_Spectrum.loadFromFITS( g_spectraFileList[fileListPos] );
	}

	right = 1;
}

void ArrowUp()
{
	g_YScale +=0.25f;
	up = 1;
}

void ArrowDown()
{
	if ( g_YScale > 0.0f )
	{
		g_YScale -=0.25f;
	}
	down = 1;
}

void Space()
{
}