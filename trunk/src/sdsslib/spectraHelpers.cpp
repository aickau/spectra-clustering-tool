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
//! \file  spectraHelpers.cpp
//! \brief 

#include "sdsslib/spectraHelpers.h"

#include <assert.h>
#include <fstream>
#include <set>

#include "devil/include/il/il.h"
#include "devil/include/il/ilu.h"

#include "sdsslib/defines.h"
#include "sdsslib/spectra.h"
#include "sdsslib/spectraVFS.h"
#include "sdsslib/helpers.h"
#include "sdsslib/mathhelpers.h"
#include "sdsslib/filehelpers.h"
#include "sdsslib/glhelper.h"
#include "sdsslib/HTMLexport.h"


namespace SpectraHelpers
{



static bool s_IsInitialized = false;
static int s_FontID = -1;



size_t getFBWidth()
{
	GLint params[4];
	glGetIntegerv( GL_VIEWPORT, params );
	return params[2];
}

size_t getFBHeight()
{
	GLint params[4];
	glGetIntegerv( GL_VIEWPORT, params );
	return params[3];
}

float X2Win( float _xp )
{
	return ( _xp );
}


float Y2Win( float _yp )
{
	int scrWidth, scrHeight;
	GLHelper::GetViewportSize(scrWidth, scrHeight);
	return (scrHeight - static_cast<float>(_yp) );
}


void init( HDC _hDC )
{
	if (s_IsInitialized)
		return;

	s_FontID = GLHelper::BuildFont(_hDC, "Arial", 10, false, false);
	ilInit();
	s_IsInitialized = true;
}


int getDefaultFontID()
{
	return s_FontID;
}


void renderDiagramToDisk( float *_values, size_t _valueCount, size_t _strideInBytes, size_t _offsetInBytes, 
						 size_t _width, size_t _height, const std::string &_sstrFilename )
{
	assert(_values != NULL);
	assert(_width<=getFBWidth());
	assert(_height<=getFBHeight());

	ILuint image;
	ilGenImages( 1, &image );
	ilBindImage(image);
	ilTexImage(static_cast<ILuint>(_width),static_cast<ILuint>(_height),1,3,IL_RGB, IL_UNSIGNED_BYTE, NULL );
	ilEnable(IL_FILE_OVERWRITE );
	iluImageParameter(ILU_FILTER,ILU_SCALE_BSPLINE);

	glColor3f(0,0,0);
	glLineWidth(1.f);
	glClearColor(1.0f,1.0f,1.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	float yMin, yMax;

	MathHelpers::getMinMax( _values, _valueCount, _strideInBytes, _offsetInBytes , yMin, yMax );

	float xs = static_cast<float>(_width)/static_cast<float>(_valueCount);
	float ys = static_cast<float>(_height*3.f/4.f)/yMax;

	GLHelper::DrawDiagram( _values, _valueCount, _strideInBytes, _offsetInBytes, 0.0f, static_cast<float>((_height*3.f)/4.f), xs, ys );

	float pos[]={10.f,10.f,-10};
	std::stringstream sstream;
	sstream << "min: " << yMin << "  max: " << yMax;
	std::string sstrOut( sstream.str() );
	GLHelper::Print( s_FontID, pos, sstrOut.c_str() );


	glLineWidth(1.f);
	glColor3f(1,1,1);
	glClearColor(0.0f,0.0f,0.f,1.f);

	glReadPixels(0,getFBHeight()-_height,_width,_height,GL_RGB, GL_UNSIGNED_BYTE, ilGetData());
	ilSave( IL_PNG, const_cast<char*>(_sstrFilename.c_str()) );
	ilDeleteImage(image);
}


void saveIntensityMap( float *_pMap, size_t _sizeX, size_t _sizeY, const std::string &_sstrFileName )
{
	assert( _pMap != NULL );
	assert( _sizeX > 0 );
	assert( _sizeY > 0 );

	std::string sstrFileNameFull = _sstrFileName;
	sstrFileNameFull += std::string(".png");

	ILuint image;
	ILenum err;
	ilGenImages( 1, &image );
	ilBindImage(image);
	ilEnable(IL_FILE_OVERWRITE );
	ilTexImage(_sizeX,_sizeY,1,3,IL_RGB, IL_FLOAT, _pMap );
	err = ilGetError();
	assert( err == IL_NO_ERROR );
	ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE );
	err = ilGetError();
	assert( err == IL_NO_ERROR );
	iluFlipImage();
	ilSave( IL_PNG, const_cast<char*>(sstrFileNameFull.c_str()) );
	err = ilGetError();
	assert( err == IL_NO_ERROR );
	ilDeleteImage(image);
}


void drawSpectra(Spectra &_spectra, 
								 bool _showInfo, 
								 bool _showSpectraLines, 
								 size_t _xp, 
								 size_t _yp, 
								 size_t _width, 
								 size_t _height, 
								 float _yscale )
{
	if ( _spectra.m_SamplesRead <= 2 )
		return;
		
	const float xAxisCenter = 0.9f;	// 0.9=at the bottom, 0.5=center, 0.1 = on the top

	float xoffset=X2Win(_xp);
	float yoffset=Y2Win(_yp)-_height*(1.f-xAxisCenter); 
	float xscale=static_cast<float>(_width-10)/static_cast<float>(Spectra::numSamples);
	float yscale=_yscale*static_cast<float>(_height*xAxisCenter);

	float values[Spectra::numSamples];

	MathHelpers::smooth(&_spectra.m_Amplitude[0], values, _spectra.m_SamplesRead-1, 2 );

	GLHelper::DrawDiagram( values, _spectra.m_SamplesRead-1, 4, 0, xoffset, yoffset, xscale, yscale );

	// draw x-axis
	GLHelper::DrawLine( xoffset, yoffset, xoffset+static_cast<float>(_width-10), yoffset );

#ifdef _USE_SPECTRALINES
	if ( _showSpectraLines )
	{
		for ( int i=0;i<_spectra.numSpectraLines;i++)
		{
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1.f, 61680 );
			if ( _spectra.m_Lines[i].wave>0.0f )
			{
				float x = static_cast<float>(Spectra::waveLengthToIndex(_spectra.m_Lines[i].wave, Spectra::waveBeginSrc, Spectra::waveEndSrc, Spectra::numSamples))*xscale+xoffset;
				float l1[3]={x,100,-10.f};
				float l2[3]={x,800,-10.f};
				GLHelper::DrawLine( l1, l2 );
			}
			glDisable(GL_LINE_STIPPLE);
		}
	}
#endif // _USE_SPECTRALINES

	if (_showInfo)
	{
		float pos[]={X2Win(10.f),Y2Win(_height-20.f),-10};
		GLHelper::Print( s_FontID, pos, _spectra.getFileName().c_str() );

		pos[1] = Y2Win(_height-30.f);

		std::stringstream sstream;
		sstream << "min: " << _spectra.m_Min << "  max: " << _spectra.m_Max << "  z: " << _spectra.m_Z;
		std::string sstrOut( sstream.str() );
		GLHelper::Print( s_FontID, pos, sstrOut.c_str() );

		float xp1,yp1,xp2,yp2;

		xp1=X2Win(_xp);
		yp1=Y2Win(_yp);
		xp2=X2Win(_width-1);
		yp2=Y2Win(_height-1);

		float l1[3]={xp1,yp1,-10.f};
		float l2[3]={xp2,yp1,-10.f};
		GLHelper::DrawLine( l1, l2 );
		l1[0]=xp2;
		l1[1]=yp2;
		GLHelper::DrawLine( l1, l2 );
		l2[0]=xp1;
		l2[1]=yp2;
		GLHelper::DrawLine( l1, l2 );
		l1[0]=xp1;
		l1[1]=yp1;
		GLHelper::DrawLine( l1, l2 );

	}
}




void renderSpectraIconToDisk( Spectra &_spectra, const std::string &_sstrFilename, size_t _width, size_t _height, float _redness )
{
	// take the n-biggest sample for resize
	const size_t spectraCutOFF = 10;
	
	if ( FileHelpers::fileExists(_sstrFilename) )
		return;

	size_t saa = 8;

	size_t w4=_width*saa;
	size_t h4=_height*saa;

	if ( w4 > getFBWidth() ||  h4 > getFBHeight() )
	{
		w4 = _width;
		h4 = _height;
		saa = 1;
	}

	ILuint image;
	ilGenImages( 1, &image );
	ilBindImage(image);
	ilTexImage(w4,h4,1,3,IL_RGB, IL_UNSIGNED_BYTE, NULL );
	ilEnable(IL_FILE_OVERWRITE );
	iluImageParameter(ILU_FILTER,ILU_SCALE_BSPLINE);

	glColor3f(1,1,1);
	glLineWidth(saa);

	float g = MAX(_redness-1.f, 0.f);

	glClearColor(_redness,g,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// calc histogram
	std::set<float> histogram;
	for ( size_t i=0;i<_spectra.m_SamplesRead; i++ ) {
		histogram.insert( _spectra.m_Amplitude[i] );
	}

	// determine maximum, smooth spectrum to remove outliers a bit
	float ymin, ymax;
	float values[Spectra::numSamples];
	MathHelpers::smooth(&_spectra.m_Amplitude[0], values, _spectra.m_SamplesRead-1, 10 );
	MathHelpers::getMinMax(values, _spectra.m_SamplesRead-1, 4, 0, ymin, ymax );

	if (ymax==0.0f)
	{
		ymax = 1.f;
	}

	drawSpectra( _spectra, false, false, 0, 0, w4, h4, 1.f/ymax );

	glReadPixels(0,getFBHeight()-h4,w4,h4,GL_RGB, GL_UNSIGNED_BYTE, ilGetData());
	iluScale(_width,_height,1);
	ilSave( IL_PNG, const_cast<char*>(_sstrFilename.c_str()) );
	ilDeleteImage(image);

	glLineWidth(1.f);
	glClearColor(0.0,0,0,0);

}



void combineSpectra( std::string &_sstrDumpFilename, const std::string &_sstrFilename )
{
	SpectraVFS *pVFS = new SpectraVFS( _sstrDumpFilename, true );
	size_t numSpectra = pVFS->getNumSpectra();

	if ( numSpectra == 0 ) {
		delete pVFS;
		return;
	}

	double accum[Spectra::numSamples];

	for ( size_t i=0;i<Spectra::numSamples;i++) {
		accum[i] = 0.0;
	}

	for ( size_t j=0;j<numSpectra;j++) 
	{
		Spectra *sp = pVFS->beginRead(j);
		for ( size_t i=0;i<Spectra::numSamples;i++ ) 
		{
			accum[i] += sp->m_Amplitude[i];
		}
		pVFS->endRead(j);
	}

	
	for ( size_t i=0;i<Spectra::numSamples;i++) {
		accum[i] /= static_cast<double>(numSpectra);
	}

	Spectra accumSpectra;

	accumSpectra.clear();

	for ( size_t i=0;i<Spectra::numSamples;i++) {
		accumSpectra.m_Amplitude[i] = accum[i];
	}
	accumSpectra.m_SamplesRead = Spectra::numSamples;
	accumSpectra.calcMinMax();
	
	int scrWidth, scrHeight;
	GLHelper::GetViewportSize(scrWidth, scrHeight);

	renderSpectraIconToDisk(accumSpectra, _sstrFilename, scrWidth, scrHeight, 1.f );

}


void writeTableEntry( const Spectra &_spectrum, float _error, std::string &_sstrOutTable )
{
	const Spectra *sp = &_spectrum;

	_sstrOutTable += HTMLExport::beginTableRow();
	_sstrOutTable += HTMLExport::beginTableCell();
	// insert link
	if ( !sp->getFileName().empty() )
	{
		_sstrOutTable += HTMLExport::imageLink( std::string("http://cas.sdss.org/dr6/en/get/specById.asp?id=")+Helpers::numberToString<__int64>(sp->m_SpecObjID), sp->getURL() );
		_sstrOutTable += HTMLExport::lineBreak();
		_sstrOutTable += "err=";
		_sstrOutTable += Helpers::numberToString<float>(_error);
		_sstrOutTable += "  z=";
		_sstrOutTable += Helpers::numberToString<float>(sp->m_Z);
		_sstrOutTable += "  ";
		_sstrOutTable += sp->getFileName();
	}
	else
	{
		// insert image
		_sstrOutTable += HTMLExport::image("export/empty.png");
	}

	_sstrOutTable += HTMLExport::endTableCell();
	_sstrOutTable += HTMLExport::endTableRow();
}

bool readSelectionList( const std::string &_sstrSelectionListFilename, std::map<std::string,float> &_outFITSFilenameSet )
{
	if ( _sstrSelectionListFilename.empty() )
	{
		// no filename
		return false;
	}

	std::ifstream fin(_sstrSelectionListFilename.c_str());

	if( !fin ) 
	{
		// could not open file
		return false;
	}

	// read selection list
	std::string sstrLine;
	while( getline(fin,sstrLine) ) 
	{
		std::string sstrFITSFilename;
		std::string sstrSeperator;
		float multiplier=1.f;

		std::istringstream sstrStream(sstrLine);
		if (sstrStream) 
		{
			sstrStream >> sstrFITSFilename;	
		}
		if (sstrStream) 
		{
			sstrStream >> sstrSeperator;	
		}
		if (sstrStream) 
		{
			sstrStream >> multiplier;	
		}
		_outFITSFilenameSet.insert(std::make_pair<std::string,float>(sstrFITSFilename,multiplier));
	}

	return true;
}

}
