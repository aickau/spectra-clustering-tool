#include "framework.h"	

#include <string>
#include <conio.h>
#include <assert.h>
#include <sstream>

#include "sdsslib/defines.h"
#include "sdsslib/helpers.h"
#include "sdsslib/filehelpers.h"
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
float g_YScale=.66f;

void setCaptionText( const std::string &_sstrFileName )
{
	std::string sstrCaptionText("SDSS Viewer - ");
	sstrCaptionText += _sstrFileName;
	SetWindowText(fr_hWnd, sstrCaptionText.c_str());
}


int InitGL( const std::string &sstrCmdLine )		
{
	std::string sstrCurrentDir( FileHelpers::getCurrentDirectory() );
	std::string sstrFileName("");

	sstrFileName = sstrCmdLine;
	/*
	std::istringstream sstrStream(sstrCmdLine);
	if (sstrStream) {
		sstrStream >> sstrFileName;	
	}*/

	bool bRetVal=true;
	if ( !sstrFileName.empty() )
	{
		if ( sstrFileName[0] == '"' )
		{
			sstrFileName.erase( sstrFileName.begin() );
			sstrFileName.erase( --sstrFileName.end() );
		}

		std::string sstrExtension(FileHelpers::getFileExtension(sstrFileName));
		if ( sstrExtension == ".fit" )
		{
			bRetVal = g_Spectrum.loadFromFITS( sstrFileName );
		}
		std::string sstrDir = FileHelpers::getFilePath( sstrFileName );
		if ( FileHelpers::exitsDirectory(sstrDir) )
		{
			sstrCurrentDir = sstrDir;
		}
	}
	else
	{
		g_Spectrum.setSine(1.f,1.f,1.f);
	}

	// get spectra files for current working dir
	sstrCurrentDir = FileHelpers::getFilePath( sstrCurrentDir );
	sstrCurrentDir += std::string("/*");
	std::vector<std::string> fileList;
	FileHelpers::getFileList( sstrCurrentDir, fileList );
	for (size_t i=0;i<fileList.size();i++) {
		if ( FileHelpers::getFileExtension(fileList[i]) == std::string(".fit") )
		{
			g_spectraFileList.push_back( fileList[i] );
		}
	}

	setCaptionText( sstrFileName );

	SpectraHelpers::init( fr_hDC );

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

	SpectraHelpers::drawSpectra( g_Spectrum, true, false, 0,0,scr_width,scr_height,g_YScale/g_Spectrum.m_Max );

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
		std::string sstrFN = g_spectraFileList[fileListPos];
		bool bSuccess = g_Spectrum.loadFromFITS( sstrFN );
		if ( !bSuccess )
		{
			sstrFN +=" - [ failed ]";
		}
		setCaptionText( sstrFN );
	}
	left = 1;
}


void ArrowRight()
{
	if ( fileListPos+1 < g_spectraFileList.size() ) {
		fileListPos++;
		std::string sstrFN = g_spectraFileList[fileListPos];
		bool bSuccess = g_Spectrum.loadFromFITS( sstrFN );
		if ( !bSuccess )
		{
			sstrFN +=" - [ failed ]";
		}
		setCaptionText( sstrFN );
	}

	right = 1;
}

void ArrowUp()
{
	g_YScale +=0.01f;
	up = 1;
}

void ArrowDown()
{
	if ( g_YScale > 0.0f )
	{
		g_YScale -=0.01f;
	}
	down = 1;
}

void Space()
{
}