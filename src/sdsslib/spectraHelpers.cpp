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
//! \file  spectraHelpers.cpp
//! \brief 

#include "spectraHelpers.h"


#include "defines.h"
#include "spectra.h"
#include "spectraVFS.h"
#include "helpers.h"
#include "mathhelpers.h"
#include "fileHelpers.h"
#include "spectraBaseHelpers.h"
#include "CSVExport.h"
#include "Timer.h"
#include "HTMLexport.h"

// TODO: port to linux
#ifndef __linux

#include "glhelper.h"

#include "devil/include/il/il.h"
#include "devil/include/il/ilu.h"

#endif

#include <assert.h>
#include <fstream>
#include <set>
#include <omp.h>



namespace SpectraHelpers
{



static bool s_IsInitialized = false;




struct TspecObj 
{
	int plate;
	int mjd;
	int fiberID;
};



bool readSelectionList( const std::string &_sstrSelectionListFilename, std::set<std::string> &_outFITSFilenameSet )
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

	std::string sstrLine;
	getline(fin,sstrLine);

	while( fin && !fin.fail() && !fin.eof()) 
	{	
		std::string sstrPlate, sstrMJD ,sstrFiberID;

		TspecObj specObj;
		fin >> sstrPlate;
		fin >> sstrMJD;
		fin >> sstrFiberID;

		specObj.fiberID = Helpers::stringToNumber<int>(sstrFiberID);
		specObj.plate = Helpers::stringToNumber<int>(sstrPlate);
		specObj.mjd = Helpers::stringToNumber<int>(sstrMJD);

		// generate both filename variants for dr7 (and below) and dr8 (and above).
		std::string sstrFileName = Spectra::getSpecObjFileName( specObj.plate, specObj.mjd, specObj.fiberID, false);
		_outFITSFilenameSet.insert(sstrFileName);
		sstrFileName = Spectra::getSpecObjFileName( specObj.plate, specObj.mjd, specObj.fiberID, true);
		_outFITSFilenameSet.insert(sstrFileName);
	}
/*
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
*/
	return true;
}

#ifndef __linux


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

	s_FontID = GLHelper::BuildFont(_hDC, "Arial", 20, false, false);
	s_largeFontID = GLHelper::BuildFont(_hDC, "Arial", 60, true, false);

	ilInit();
	s_IsInitialized = true;
}


int getDefaultFontID()
{
	return s_FontID;
}


void renderDiagramToDisk( float *_values, size_t _valueCount, size_t _numDiagrams, size_t _strideInBytes, size_t _offsetInBytes, 
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

	float yo = static_cast<float>((_height*3.f)/4.f);
	float xs = static_cast<float>(_width)/static_cast<float>(_valueCount);
	float ys = yo/yMax;

	for ( size_t i=0;i<_numDiagrams;i++)
	{
		GLHelper::DrawDiagram( &_values[_valueCount*i], _valueCount, _strideInBytes, _offsetInBytes, 0.0f, yo, xs, ys, true );
	}
	GLHelper::DrawLine( 0.f, yo, static_cast<float>(_width), yo );

	float pos[]={10.f,getFBHeight()-_height-50,10};
	std::stringstream sstream;
	sstream << "min: " << yMin << "  max: " << yMax;
	std::string sstrOut( sstream.str() );
	GLHelper::Print( s_FontID, pos, sstrOut.c_str() );

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glPixelStorei(GL_PACK_ALIGNMENT,1);
	glReadPixels(0,getFBHeight()-_height-50,_width,_height,GL_RGB, GL_UNSIGNED_BYTE, ilGetData());
	glClearColor(0.0f,0.0f,0.f,1.f);
	glLineWidth(1.f);
	glColor3f(1,1,1);
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
								 float _yscale,
								 float _xScale,
								 int _smooth)
{
	if ( _spectra.m_SamplesRead <= 2 )
		return;
		
	const float xAxisCenter = 0.9f;	// 0.9=at the bottom, 0.5=center, 0.1 = on the top

	float xoffset=X2Win(_xp);
	float yoffset=Y2Win(_yp)-_height*(1.f-xAxisCenter); 
	float xscale=(static_cast<float>(_width-10)/static_cast<float>(Spectra::numSamples))*_xScale;
	float yscale=_yscale*static_cast<float>(_height*xAxisCenter);
 
	if ( _smooth>0 ) 
	{
		float values[Spectra::numSamples];
		MathHelpers::smooth(&_spectra.m_Amplitude[0], values, _spectra.m_SamplesRead-1, MIN(_smooth,10) );

		GLHelper::DrawDiagram( values, _spectra.m_SamplesRead-1, 4, 0, xoffset, yoffset, xscale, yscale, true  );
	}
	else
	{
		GLHelper::DrawDiagram( &_spectra.m_Amplitude[0], _spectra.m_SamplesRead-1, 4, 0, xoffset, yoffset, xscale, yscale, true  );
	}	
/*	
	GLHelper::SetLineStipple(0,0);
	glColor3f(0,0,0);
	glLineWidth(2.f);
	// draw x-axis
	GLHelper::DrawLine( xoffset, yoffset, xoffset+static_cast<float>(_width-10), yoffset );
*/
	// output specobj ID
// 	float pos[]={X2Win(_width/2),Y2Win(_height-_height/2),-10};
// 	std::stringstream sstream;
// 	sstream << (_spectra.m_SpecObjID>>22); 
// 	std::string sstrOut( sstream.str() );
// 	GLHelper::Print( s_largeFontID, pos, sstrOut.c_str() );


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
	const float yMargin = 1.03; // add 3% margin at the borders
	const float lineWidth = 2.f;
	float smoothing = 2;
	
 	if ( FileHelpers::fileExists(_sstrFilename) )
 		return;
	if ( _spectra.m_SamplesRead <= 0 )
		return;
 
	size_t saa = 4;

	size_t w4=_width*saa;
	size_t h4=_height*saa;

	const size_t fbWidth = getFBWidth();
	const size_t fbHeight = getFBHeight();

	if ( fbHeight < _width || fbHeight < _height ) {
		return;
	}

	if ( w4 > fbWidth ||  h4 > getFBHeight() )
	{
		w4 = _width;
		h4 = _height;
		saa = 1;
	} 

	ILuint image;
	glViewport(0,0,w4,h4);
	ilGenImages( 1, &image );
	ilBindImage(image);
	ilTexImage(w4,h4,1,3,IL_RGB, IL_UNSIGNED_BYTE, NULL );
	ilEnable(IL_FILE_OVERWRITE );
	iluImageParameter(ILU_FILTER,ILU_SCALE_BSPLINE);

	glColor3f(1,1,1);
	glLineWidth(saa*lineWidth);

	float g = MAX(_redness-1.f, 0.f);

	glClearColor(_redness,g,0,0);

	if ( (MIN(_redness,1)+g)*0.333f < 0.5f )
	{
		// use white for dark background
		glColor3f(1,1,1);
	}
	else
	{
		// use black for light background
		glColor3f(0,0,0);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// determine maximum, smooth spectrum to remove outliers a bit during min-max calculation
	float ymin, ymax;
	float values[Spectra::numSamples];
	if ( _spectra.m_version != Spectra::SP_LIGHTCURVE_SDSS &&
		 _spectra.m_version != Spectra::SP_LIGHTCURVE_RADEC &&
		 _spectra.m_version != Spectra::SP_LIGHTCURVE_PLAIN ) {
		MathHelpers::smooth(&_spectra.m_Amplitude[0], values, _spectra.m_SamplesRead-1, 10 );
		MathHelpers::getMinMax(values, _spectra.m_SamplesRead-1, 4, 0, ymin, ymax );
	}
	else {
		// disable smoothing for light curves
		ymin = _spectra.m_Min;
		ymax = _spectra.m_Max;
		smoothing = 0;
	}
	


	// if all amplitudes are equal, make sure we draw a straight line at the bottom of the icon.
	float ydelta = ymax-ymin;
	if ( ydelta == 0.0f ) {
		ymax = ymin + 1.f;
		ymin -= 0.1f; 
	} else {
		ymin *= yMargin;
		ymax *= yMargin;
	}


	float *pAmplitude = &_spectra.m_Amplitude[0];
	// smooth spectra a bit to reduce noise
	if ( smoothing>0 ) 
	{
		MathHelpers::smooth(&_spectra.m_Amplitude[0], values, _spectra.m_SamplesRead-1, MIN(smoothing,10) );
		pAmplitude = &values[0];
	}


	GLHelper::SetOrtho(0,_spectra.m_SamplesRead-1, ymin, ymax );
	// test:
	//GLHelper::DrawLine(0,0,_spectra.m_SamplesRead, -2.f);
	GLHelper::DrawDiagram( pAmplitude, _spectra.m_SamplesRead-1 );
	GLHelper::ResetProjection();


	GLHelper::SetOrtho(0,w4, h4, 0 );

	// draw markers
	float pos[]={ w4-40, h4-10,-2};
	glColor3f(0,0,1.f);
	
	// BOSS spectra -because of their extended wavelength range - get a little B on the lower right corner
	if ( _spectra.m_version == Spectra::SP_VERSION_BOSS )
	{
		GLHelper::Print( s_largeFontID, pos, "B" );
	}

	// Light curve spectra get a little L on the lower right corner
	if (_spectra.m_version == Spectra::SP_LIGHTCURVE_SDSS || 
		_spectra.m_version == Spectra::SP_LIGHTCURVE_RADEC || 
		_spectra.m_version == Spectra::SP_LIGHTCURVE_PLAIN )
	{
		GLHelper::Print( s_largeFontID, pos, "L" );
	}

	if ( _spectra.m_version == Spectra::SP_ARTIFICIAL )
	{
		GLHelper::Print( s_largeFontID, pos, "A" );
	}

	if ( _spectra.m_version == Spectra::SP_CSV )
	{
		GLHelper::Print( s_largeFontID, pos, "C" );
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glPixelStorei(GL_PACK_ALIGNMENT,1);
	glReadPixels(0,0,w4,h4,GL_RGB, GL_UNSIGNED_BYTE, ilGetData());

	iluScale(_width,_height,1);
	ilSave( IL_PNG, const_cast<char*>(_sstrFilename.c_str()) );
	ilDeleteImage(image);

	GLHelper::ResetProjection();
	glViewport(0,0,fbWidth,fbHeight);
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
		_sstrOutTable += HTMLExport::imageLink( sp->getImgURL(), sp->getURL() );
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



bool calcVectorField( SpectraVFS &_map1, SpectraVFS &_map2, std::vector<float>& _outVectorField )
{
	// check parameters
	assert( _map1.getNumSpectra() > 0 );
	assert( _map2.getNumSpectra() > 0 );
	assert( _map1.getNumSpectra() == _map2.getNumSpectra() );

	if ( _map1.getNumSpectra() <= 0 || 
		 _map2.getNumSpectra() <= 0 ||  
		 _map1.getNumSpectra() != _map2.getNumSpectra() )
	{
		return false;
	}

	const size_t nSize = _map1.getNumSpectra();
	const size_t nSize2 = nSize*2; 
	const size_t nGridSize = static_cast<int>(sqrtf(nSize));
	if ( nGridSize*nGridSize != nSize )
	{
		assert(0);
		return false;
	}

	
	int *posField1 = new int[nSize2];
	int *posField2 = new int[nSize2];

	// clear
	for (size_t i=0;i<nSize2;i++) 
	{
		posField1[i] = -1;
		posField2[i] = -1;
	}


	// fill position fields
	for (size_t i=0;i<nSize;i++) 
	{
		const int posX = i % nGridSize;
		const int posY = i / nGridSize;

		Spectra *sp1 = _map1.beginRead(i);
		Spectra *sp2 = _map2.beginRead(i);

		assert( sp1->m_Index < static_cast<int>(nSize) && sp1->m_Index >= -1 );
		assert( sp2->m_Index < static_cast<int>(nSize) && sp1->m_Index >= -1 );

		if ( sp1->m_Index != -1 )
		{
			posField1[sp1->m_Index*2+0] = posX;
			posField1[sp1->m_Index*2+1] = posY;
		}

		if ( sp2->m_Index != -1 )
		{
			posField2[sp2->m_Index*2+0] = posX;
			posField2[sp2->m_Index*2+1] = posY;
		}

		_map2.endRead(i);
		_map1.endRead(i);
	}

	_outVectorField.resize(nSize*5);

	size_t c=0;
	for (size_t i=0;i<nSize2;i+=2) 
	{
		if ( posField2[i+0] >= 0 )
		{
			// field in use

			if ( posField1[i+0] < 0 || posField1[i+1] < 0 || posField2[i+0] < 0 || posField2[i+1] < 0 )
			{
				// we found that maps do not match, abort.
				delete[] posField2;
				delete[] posField1;
				_outVectorField.clear();
				return false;
			}

			const float dx = static_cast<float>(posField2[i+0]-posField1[i+0]);
			const float dy = static_cast<float>(posField2[i+1]-posField1[i+1]);
			_outVectorField[c+0] = static_cast<float>(posField1[i+0]);
			_outVectorField[c+1] = static_cast<float>(posField1[i+1]);
			_outVectorField[c+2] = dx;
			_outVectorField[c+3] = dy;
			_outVectorField[c+4] = sqrtf(dx*dx+dy*dy);
		}
		else
		{
			// field is empty
			_outVectorField[c+0] = 0.f;
			_outVectorField[c+1] = 0.f;
			_outVectorField[c+2] = 0.f;
			_outVectorField[c+3] = 0.f;
			_outVectorField[c+4] = 0.f;
		}
		c+=5;
	}
 
	delete[] posField2;
	delete[] posField1;

	return true;
}



void writeSpectraInfoToTable( SpectraVFS &_sourceSpectra, SpectraVFS &_network, const std::string &_sstrTableFilename, std::ofstream *pLogFile )
{
	const int gridSizeSqr = _network.getNumSpectra();
	const int gridSize = (INT)sqrtf(gridSizeSqr);
	const int gridSizeSqr2 = gridSize*gridSize;
	const size_t numSourceSpecra = _sourceSpectra.getNumSpectra();


	
	if ( gridSizeSqr2 != gridSizeSqr || gridSizeSqr==0 )
	{
		Helpers::print("Error: could not load network sofmnet.bin.\n", pLogFile ); 
		return;
	}

	if ( numSourceSpecra == 0 )
	{
		Helpers::print("Error: could not load allSpectra.bin.\n", pLogFile ); 
		return;
	}
	if ( numSourceSpecra > gridSizeSqr )
	{
		Helpers::print("Error: Mismatch between allSpectra.bin and sofmnet.bin. Files do not belong together?\n", pLogFile ); 
		return;
	}

	CSVExport spectraTable(_sstrTableFilename);

	spectraTable.writeTableEntry("mapindex");
	spectraTable.writeTableEntry("xpos");
	spectraTable.writeTableEntry("ypos");
	spectraTable.writeTableEntry("specObjID");
	spectraTable.writeTableEntry("spectraIndex");
	spectraTable.writeTableEntry("plateID");
	spectraTable.writeTableEntry("MJD");
	spectraTable.writeTableEntry("fiberID");
	spectraTable.writeTableEntry("z");
	spectraTable.writeTableEntry("objtype");
	spectraTable.newRow();

	for (int i=0;i<gridSizeSqr;i++ )
	{
		Spectra *spSpec = _network.beginRead( i );

		const int xpos = i % gridSize;
		const int ypos = i / gridSize;

		spectraTable.writeTableEntry(i);
		spectraTable.writeTableEntry(xpos);
		spectraTable.writeTableEntry(ypos);


		const int index = spSpec->m_Index;
		if ( index >= 0 && index < numSourceSpecra )
		{
			Spectra *sp = _sourceSpectra.beginRead(index);

			if ( sp != NULL )
			{
				spectraTable.writeTableEntry(sp->m_SpecObjID);
				spectraTable.writeTableEntry(index);
				spectraTable.writeTableEntry(sp->getPlate());
				spectraTable.writeTableEntry(sp->getMJD());
				spectraTable.writeTableEntry(sp->getFiber());
				spectraTable.writeTableEntry((float)sp->m_Z);
				spectraTable.writeTableEntry(sp->m_Type);
				spectraTable.newRow();
			}
			else
			{
				printf("Error: could not read source spectra.\n");
				return;
			}

			_sourceSpectra.endRead(index);

		}
		else
		{
			spectraTable.writeTableEntry(0);
			spectraTable.writeTableEntry(index);
			spectraTable.writeTableEntry(0);
			spectraTable.writeTableEntry(0);
			spectraTable.writeTableEntry(0);
			spectraTable.writeTableEntry(0.0f);
			spectraTable.writeTableEntry(0);
			spectraTable.newRow();

		}

		_network.endRead( i );
	}
}


void writeFloatList( float *_pArray, size_t _size, const std::string &_sstrFilename )
{
	assert( _pArray != NULL );
	std::string sstrOutTable;
	for (size_t i=0;i<_size;i++)
	{
		sstrOutTable += Helpers::numberToString<float>(_pArray[i]);
		sstrOutTable += "\n";
	}

	std::ofstream fon(_sstrFilename.c_str());
	fon<<sstrOutTable;
}


void writeRect( float *_pRGBMap, size_t _size, size_t _planX, size_t _planY, size_t _planSize )
{
	const size_t maxPlanSize = _size/_planSize;
	const size_t pXBeg = (_planX>0) ? _planX-1 : _planX;
	const size_t pYBeg = (_planY>0) ? _planY-1 : _planY;
	const size_t pXEnd = _planX+2;
	const size_t pYEnd = _planY+2;

	const size_t xBeg = pXBeg*_planSize;
	const size_t yBeg = pYBeg*_planSize;
	const size_t xEnd = pXEnd*_planSize;
	const size_t yEnd = pYEnd*_planSize;

	for (size_t x=xBeg;x<xEnd;x++)
	{
		size_t a1 = CALC_ADRESS_SAFE( x, yBeg, _size, _size )*3;
		size_t a2 = CALC_ADRESS_SAFE( x, yEnd, _size, _size )*3;
		_pRGBMap[a1+0] = _pRGBMap[a2+0] = 1.0f;
		_pRGBMap[a1+1] = _pRGBMap[a2+1] = 0.0f;
		_pRGBMap[a1+2] = _pRGBMap[a2+2] = 0.0f;
	}

	for (size_t y=yBeg;y<yEnd;y++)
	{
		size_t a1 = CALC_ADRESS_SAFE( xBeg, y, _size, _size )*3;
		size_t a2 = CALC_ADRESS_SAFE( xEnd, y, _size, _size )*3;
		_pRGBMap[a1+0] = _pRGBMap[a2+0] = 1.0f;
		_pRGBMap[a1+1] = _pRGBMap[a2+1] = 0.0f;
		_pRGBMap[a1+2] = _pRGBMap[a2+2] = 0.0f;
	}
}

void writeMapWithSubpageMarkers( const std::string &_sstrFilenName, float *_pRGBMap, size_t _gridSize, size_t _planSize )
{
	const size_t gridSizeSqr = _gridSize*_gridSize;
	float *pRGBMap2 = new float[gridSizeSqr*3];
	const size_t planMax = _gridSize/_planSize+1;
	const size_t planMaxSqr = planMax*planMax;
	for ( size_t i=0;i<planMaxSqr;i++ )
	{
		const size_t planX = i%planMax;
		const size_t planY = i/planMax;
		memcpy( pRGBMap2, _pRGBMap, gridSizeSqr*3*sizeof(float) );
		writeRect( pRGBMap2, _gridSize, planX, planY, _planSize );
		std::string sstrPlan = "_"+Helpers::numberToString(planX,4)+"_"+Helpers::numberToString(planY,4);
		SpectraHelpers::saveIntensityMap( pRGBMap2, _gridSize, _gridSize, _sstrFilenName+sstrPlan );
	}
	delete[] pRGBMap2;
}



void intensityToRGB(float _intensity, float *_pRGB, bool _bRed )
{
	_intensity *= 3.f;
	if ( _intensity >= 0.0f )
	{
		_pRGB[2] = MIN(_intensity,1.f);
		_pRGB[1] = CLAMP(_intensity-1.f,0.f,1.f);
		_pRGB[0] = CLAMP(_intensity-2.f,0.f,1.f);
	}
	else
	{
		_intensity = -_intensity;
		_pRGB[0] = MIN(_intensity,1.f);
		_pRGB[1] = CLAMP(_intensity-1.f,0.f,1.f);
		_pRGB[2] = CLAMP(_intensity-2.f,0.f,1.f);
	}

	if ( _bRed )
	{
		float c = _pRGB[2];
		_pRGB[2] = _pRGB[0];
		_pRGB[1] = CLAMP(_intensity-1.f,0.f,1.f);
		_pRGB[0] = c;

	}
}


void intesityToRGBGradient(float _intensity, float *_pRGB, int gradientType )
{
	_intensity = CLAMP(_intensity,0.f,1.f);

	// 0..1: blue->lightblue->cyan->white
	if ( gradientType == 0 )
	{
		if ( _intensity <= 0.3333f )
		{
			// 0..0.333 -> 0..1
			_intensity = _intensity*3.f;
			_pRGB[0] = _intensity*0.5f;
			_pRGB[1] = _intensity*0.5f;
			_pRGB[2] = 1.f;

		} 
		else if ( _intensity <= 0.6666f )
		{
			// 0.333..0.666 -> 0..1
			_intensity = (_intensity-0.3333f)*3.f;
			_pRGB[0] = 0.5f-_intensity*0.5f;
			_pRGB[1] = 0.5f+_intensity*0.5f;
			_pRGB[2] = 1.f;

		}
		else
		{
			// 0.666..1.0 -> 0..1
			_intensity = (_intensity-0.6666f)*3.f;
			_pRGB[0] = _intensity;
			_pRGB[1] = 1.f;
			_pRGB[2] = 1.f;
		}
	}
	else 
	{
		// 0..1: violet->red->oragne->light yellow

		if ( _intensity <= 0.3333f )
		{
			// 0..0.333 -> 0..1
			_intensity = _intensity*3.f;
			_pRGB[0] = 0.5f+_intensity*0.5f;
			_pRGB[1] = 0.f;
			_pRGB[2] = 1.f-_intensity;

		} 
		else if ( _intensity <= 0.6666f )
		{
			// 0.333..0.666 -> 0..1
			_intensity = (_intensity-0.3333f)*3.f;
			_pRGB[0] = 1.f;
			_pRGB[1] = _intensity*0.5f;
			_pRGB[2] = 0.f;

		}
		else
		{
			// 0.666..1.0 -> 0..1
			_intensity = (_intensity-0.6666f)*3.f;
			_pRGB[0] = 1.f;
			_pRGB[1] = 0.5f+_intensity*0.5f;
			_pRGB[2] = _intensity*0.5f;
		}
	}
}




void calcUMatrix( SpectraVFS &_network, const std::string &_sstrFilenName, bool _bUseLogScale, bool _bShowEmpty, bool _bNormalize, bool _showRanges, size_t _planSize )
{
	if ( _network.getNumSpectra() == 0 )
	{
		return;
	}
	assert( !_sstrFilenName.empty() );

	const size_t gridSizeSqr = _network.getNumSpectra();
	const size_t gridSize = sqrtf(gridSizeSqr);

	float *pUMatrix = new float[gridSizeSqr];
	float *pRGBMap = new float[gridSizeSqr*3];

	float maxErr = 0.0f;

	// flash
	for (size_t i=0;i<gridSizeSqr;i++) 
	{
		pUMatrix[i] = 0.0f;
	}

	// calc errors
	for ( size_t y=1;y<gridSize-1;y++ )
	{
		for ( size_t x=1;x<gridSize-1;x++ )
		{
			const size_t i = CALC_ADRESS(x,y,gridSize);
			const size_t iLeft = CALC_ADRESS(x-1,y,gridSize);
			const size_t iRight = CALC_ADRESS(x+1,y,gridSize);
			const size_t iUp = CALC_ADRESS(x,y-1,gridSize);
			const size_t iBottom = CALC_ADRESS(x,y+1,gridSize);
			Spectra *spCenter = _network.beginRead( i );
			Spectra *spLeft = _network.beginRead( iLeft );
			Spectra *spRight = _network.beginRead( iRight );
			Spectra *spTop = _network.beginRead( iUp );
			Spectra *spBottom = _network.beginRead( iBottom );

			if ( _bShowEmpty || !spCenter->isEmpty() )
			{
				SSE_ALIGN Spectra backupCenter (*spCenter);
				SSE_ALIGN Spectra backupLeft(*spLeft);
				SSE_ALIGN Spectra backupRight(*spRight);
				SSE_ALIGN Spectra backupTop(*spTop);
				SSE_ALIGN Spectra backupBottom(*spBottom);

				if ( _bNormalize )
				{
					backupCenter.normalize();
					backupLeft.normalize();
					backupRight.normalize();
					backupTop.normalize();
					backupBottom.normalize();
				}

				pUMatrix[i] += backupCenter.compare( backupLeft );
				pUMatrix[i] += backupCenter.compare( backupRight );
				pUMatrix[i] += backupCenter.compare( backupTop );
				pUMatrix[i] += backupCenter.compare( backupBottom );

				maxErr = MAX( maxErr, pUMatrix[i] );
			}


			_network.endRead( i );
			_network.endRead( iLeft );
			_network.endRead( iRight );
			_network.endRead( iUp );
			_network.endRead( iBottom );
		}
	}

	// normalize
	if ( maxErr > 0.0f )
	{
		for (size_t i=0;i<gridSizeSqr;i++) 
		{
			float scale;
			if ( _bUseLogScale )
			{
				// logarithmic scale
				scale  = log10f( pUMatrix[i]+1.f ) / log10f( maxErr+1.f );
			}
			else
			{
				// linear scale
				scale  = pUMatrix[i] /= maxErr;
			}

			SpectraHelpers::intensityToRGB( scale,  &pRGBMap[i*3] );
		}
	}

	if (_showRanges)
	{
		writeMapWithSubpageMarkers( _sstrFilenName, pRGBMap, gridSize, _planSize );
	}

	saveIntensityMap( pRGBMap, gridSize, gridSize, _sstrFilenName );

	delete[] pUMatrix;
	delete[] pRGBMap;
}



void calcDifferenceMap( SpectraVFS &_sourceSpectra, SpectraVFS &_network, const std::string &_sstrFilenName, bool _bUseLogScale, bool _bNormalize, bool _bOutputAsTextFile, bool _showRanges, size_t _planSize )
{
	if ( _network.getNumSpectra() == 0 )
	{
		return;
	}
	if ( _sourceSpectra.getNumSpectra() == 0 )
	{
		return;
	}

	assert( !_sstrFilenName.empty() );

	const size_t gridSizeSqr = _network.getNumSpectra();
	const size_t gridSize = sqrtf(gridSizeSqr);

	float *pDiffMatrix = new float[gridSizeSqr];
	float *pRGBMap = new float[gridSizeSqr*3];

	float maxErr = 0.0f;

	std::string sstrDifferenceMap("");


	// calc errors
	for ( size_t i=0;i<gridSizeSqr;i++ )
	{
		Spectra *spNet = _network.beginRead( i );
		SSE_ALIGN Spectra backupNet(*spNet);

		if ( spNet->isEmpty() )
		{
			// mark empty cells
			pDiffMatrix[i] = -1.f;
		}
		else
		{
			Spectra *spSource = _sourceSpectra.beginRead( spNet->m_Index );
			SSE_ALIGN Spectra backupSource(*spSource);

			if ( _bNormalize )
			{
				backupSource.normalize();
				backupNet.normalize();
			}
			pDiffMatrix[i] = backupNet.compare( backupSource );
			maxErr = MAX( maxErr, pDiffMatrix[i] );
			_sourceSpectra.endRead( spNet->m_Index );
		}
		_network.endRead( i );
	}

	// normalize
	if ( maxErr > 0.0f )
	{
		for (size_t i=0;i<gridSizeSqr;i++) 
		{
			if ( pDiffMatrix[i] >= 0.0f )
			{
				float scale;
				if ( _bUseLogScale )
				{
					// logarithmic scale
					scale  = log10f( pDiffMatrix[i]+1.f ) / log10f( maxErr+1.f );
				}
				else
				{
					// linear scale
					scale  = pDiffMatrix[i] /= maxErr;
				}

				if ( _bOutputAsTextFile )
				{
					float differenceValue = pDiffMatrix[i] / maxErr;
					sstrDifferenceMap += Helpers::numberToString<float>(differenceValue) + "\n";
				}

				SpectraHelpers::intensityToRGB( scale, &pRGBMap[i*3] );
			}
			else
			{
				// mark empty cells
				pRGBMap[i*3]   = 0.5f;
				pRGBMap[i*3+1] = 0.5f;
				pRGBMap[i*3+2] = 0.5f;

				if ( _bOutputAsTextFile )
				{
					sstrDifferenceMap += "-1.0\n";
				}
			}
		}
	}

	if ( _bOutputAsTextFile )
	{
		std::string sstrTxtFile(_sstrFilenName);
		sstrTxtFile += ".txt";

		std::ofstream fon(sstrTxtFile.c_str());
		fon<<sstrDifferenceMap;
	}
	if (_showRanges)
	{
		SpectraHelpers::writeMapWithSubpageMarkers( _sstrFilenName, pRGBMap, gridSize, _planSize );
	}


	SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, _sstrFilenName );

	delete[] pDiffMatrix;
	delete[] pRGBMap;
}


void calcZMap( SpectraVFS &_sourceSpectra, SpectraVFS &_network, const std::string &_sstrFilename, bool _bUseLogScale, bool _showRanges, size_t _planSize )
{
	if ( _network.getNumSpectra() == 0 )
	{
		return;
	}
	if ( _sourceSpectra.getNumSpectra() == 0 )
	{
		return;
	}

	assert( !_sstrFilename.empty() );


	const size_t numSpectra = _sourceSpectra.getNumSpectra();
	const size_t gridSizeSqr = _network.getNumSpectra();
	const size_t gridSize = sqrtf(gridSizeSqr);

	float *pZMatrix = new float[gridSizeSqr];
	float *pRGBMap = new float[gridSizeSqr*3];

	float maxZ = 0.0f;

	// get maximum z
	for ( size_t i=0;i<numSpectra;i++ )
	{
		Spectra *a = _sourceSpectra.beginRead( i );
		if (maxZ < a->m_Z)
		{
			maxZ = a->m_Z;
		}
		_sourceSpectra.endRead( i );
	}

	// get z from net
	for (size_t i=0;i<gridSizeSqr;i++) 
	{
		Spectra *spNet = _network.beginRead( i );

		if ( spNet->isEmpty() )
		{
			// mark empty cells
			pZMatrix[i] = -1.f;
			pRGBMap[i*3]   = 0.5f;
			pRGBMap[i*3+1] = 0.5f;
			pRGBMap[i*3+2] = 0.5f;
		}
		else
		{
			Spectra *spSource = _sourceSpectra.beginRead( spNet->m_Index );
			pZMatrix[i] = spSource->m_Z;
			float scale;
			if ( _bUseLogScale )
			{
				// logarithmic scale
				scale  = log10f( pZMatrix[i]+1.f ) / log10f( maxZ+1.f );
			}
			else
			{
				// linear scale
				scale  = pZMatrix[i] /= maxZ;
			}
			_sourceSpectra.endRead( spNet->m_Index );
			SpectraHelpers::intensityToRGB( scale, &pRGBMap[i*3], true );			
		}
		_network.endRead( i );
	}
	if (_showRanges)
	{
		SpectraHelpers::writeMapWithSubpageMarkers( _sstrFilename, pRGBMap, gridSize, _planSize );
	}

	SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, _sstrFilename );

	delete[] pZMatrix;
	delete[] pRGBMap;
}


void writeSpectraClass( SpectraVFS &_sourceSpectra, SpectraVFS &_network, const std::string &_sstrFilename )
{
	if ( _network.getNumSpectra() == 0 )
	{
		return;
	}
	if ( _sourceSpectra.getNumSpectra() == 0 )
	{
		return;
	}

	assert( !_sstrFilename.empty() );


	const size_t numSpectra = _sourceSpectra.getNumSpectra();
	const size_t gridSizeSqr = _network.getNumSpectra();
	const size_t gridSize = sqrtf(gridSizeSqr);

	float *pRGBMap = new float[gridSizeSqr*3];

	// gray for empty neurons
	for ( size_t i=0;i<gridSizeSqr*3;i++)
	{
		pRGBMap[i] = 0.5f;
	}


	for (size_t i=0;i<gridSizeSqr;i++)
	{
		Spectra *spNet =  _network.beginRead(i);
		const int index = spNet->m_Index;
		_network.endRead(i);
		if ( index >= 0 && index < numSpectra )
		{
			Spectra *sp = _sourceSpectra.beginRead(index);

			float r=0.0f;		// dark cyan undefined spectra type
			float g=0.0f;
			float b=0.0f;

			const Spectra::SpectraClass spClass = sp->getClass();
			const Spectra::SpectraSubClass spSubClass = sp->getSubClass();


			switch ( spClass )
			{
			case Spectra::SPC_NOT_SET : r=1.f;g=1.0f; b= 1.0f;			// white
				break;
			case Spectra::SPC_UNKNOWN : r= 1.0; g=1.0; b= 1.0;			// white
				break;
			case Spectra::SPC_STAR : r= 0.0f; g=0.0f; b= 1.0f;			// blue
				break;
			case Spectra::SPC_GALAXY : r= 1.0f; g=0.0f; b= 0.0f;		// red
				break;
			case Spectra::SPC_QSO : r= 1.0f; g=0.5f; b= 0.0f;			// orange
				break;
			}
			pRGBMap[i*3+0] = r;
			pRGBMap[i*3+1] = g;
			pRGBMap[i*3+2] = b;
			_sourceSpectra.endRead(index);
		}
	}

	SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, _sstrFilename);

	delete[] pRGBMap;
}


void writeSpectraGalaxyAndQSOSubclass( SpectraVFS &_sourceSpectra, SpectraVFS &_network, const std::string &_sstrFilename )
{
	if ( _network.getNumSpectra() == 0 )
	{
		return;
	}
	if ( _sourceSpectra.getNumSpectra() == 0 )
	{
		return;
	}

	assert( !_sstrFilename.empty() );


	const size_t numSpectra = _sourceSpectra.getNumSpectra();
	const size_t gridSizeSqr = _network.getNumSpectra();
	const size_t gridSize = sqrtf(gridSizeSqr);

	float *pRGBMap = new float[gridSizeSqr*3];

	// gray for empty neurons
	for ( size_t i=0;i<gridSizeSqr*3;i++)
	{
		pRGBMap[i] = 0.5f;
	}


	for (size_t i=0;i<gridSizeSqr;i++)
	{
		Spectra *spNet =  _network.beginRead(i);
		const int index = spNet->m_Index;
		_network.endRead(i);
		if ( index >= 0 && index < numSpectra )
		{
			Spectra *sp = _sourceSpectra.beginRead(index);

			float r=0.0f;		//black for all non QSO/Galaxy spectra 
			float g=0.0f;
			float b=0.0f;

			const Spectra::SpectraClass spClass = sp->getClass();
			const Spectra::SpectraSubClass spSubClass = sp->getSubClass();
			const bool isBroadline = sp->isBroadline();

			if ( spClass == Spectra::SPC_GALAXY ) 
			{
				if ( isBroadline )
				{
					switch ( spSubClass )
					{
					case Spectra::SPSC_AGN : r=0.7f;g=0.7f; b= 0.0f;			// dark yellow
						break;
					case Spectra::SPSC_STARBURST : r= 0.7; g=0.35; b= 0.0f;		// dark orange / brown
						break;
					case Spectra::SPSC_STARFORMING : r=0.5f;g=0.0f; b= 0.0f;	// dark red
						break;
					}

				}
				else
				{
					switch ( spSubClass )
					{
					case Spectra::SPSC_AGN : r=1.0f;g=1.0f; b= 0.2f;			// light yellow
						break;
					case Spectra::SPSC_STARBURST : r= 1.0; g=0.5; b= 0.2;		// light orange
						break;
					case Spectra::SPSC_STARFORMING : r=1.0f;g=0.2f; b= 0.2f;	// light red
						break;
					}

				}
			}
			else if ( spClass == Spectra::SPC_QSO ) 
			{
				if ( isBroadline )
				{
					switch ( spSubClass )
					{
					case Spectra::SPSC_AGN : r=0;g=0; b= 0.5f;					// dark blue
						break;
					case Spectra::SPSC_STARBURST : r= 0.0; g=0.5f; b= 0.5f;		// dark cyan
						break;
					case Spectra::SPSC_STARFORMING : r= 0.0f; g=0.5f; b= 0.0f;	// dark green
						break;
					}

				}
				else
				{
					switch ( spSubClass )
					{
					case Spectra::SPSC_AGN : r=0.5f; g=0.5f; b= 1.0f;			// light blue
						break;
					case Spectra::SPSC_STARBURST : r= 0.5; g=1.0f; b= 1.0f;		// light cyan
						break;
					case Spectra::SPSC_STARFORMING : r= 0.5f; g=1.0f; b= 0.5f;	// light green
						break;
					}

				}
			}
			pRGBMap[i*3+0] = r;
			pRGBMap[i*3+1] = g;
			pRGBMap[i*3+2] = b;
			_sourceSpectra.endRead(index);
		}
	}

	SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, _sstrFilename);

	delete[] pRGBMap;
}


void writeSpectraStarSubclass( SpectraVFS &_sourceSpectra, SpectraVFS &_network, const std::string &_sstrFilename )
{
	if ( _network.getNumSpectra() == 0 )
	{
		return;
	}
	if ( _sourceSpectra.getNumSpectra() == 0 )
	{
		return;
	}

	assert( !_sstrFilename.empty() );


	const size_t numSpectra = _sourceSpectra.getNumSpectra();
	const size_t gridSizeSqr = _network.getNumSpectra();
	const size_t gridSize = sqrtf(gridSizeSqr);

	float *pRGBMap = new float[gridSizeSqr*3];

	// gray for empty neurons
	for ( size_t i=0;i<gridSizeSqr*3;i++)
	{
		pRGBMap[i] = 0.5f;
	}


	for (size_t i=0;i<gridSizeSqr;i++)
	{
		Spectra *spNet =  _network.beginRead(i);
		const int index = spNet->m_Index;
		_network.endRead(i);
		if ( index >= 0 && index < numSpectra )
		{
			Spectra *sp = _sourceSpectra.beginRead(index);

			float r=0.0f;		// black for all non star spectra type
			float g=0.0f;
			float b=0.0f;

			const Spectra::SpectraClass spClass = sp->getClass();
			const Spectra::SpectraSubClass spSubClass = sp->getSubClass();

			if ( spClass == Spectra::SPC_STAR ) 
			{
				switch ( spSubClass )
				{
				case Spectra::SPSC_NOT_SET : r=0.2f;g=0.2f; b= 0.2;		// type not set
					break;

				case Spectra::SPSC_O8 :
				case Spectra::SPSC_O9 :
				case Spectra::SPSC_O : r=0;g=0; b= 1.0f;				// blue
					break;

				case Spectra::SPSC_OB : r=0.1f;g=0.1f; b= 1.0f;			// blue, tiny bit lighter
					break;

				case Spectra::SPSC_B0 : 
				case Spectra::SPSC_B1 : 
				case Spectra::SPSC_B2 : 
				case Spectra::SPSC_B3 : 
				case Spectra::SPSC_B4 : 
				case Spectra::SPSC_B5 : 
				case Spectra::SPSC_B6 : 
				case Spectra::SPSC_B7 : 
				case Spectra::SPSC_B8 : 
				case Spectra::SPSC_B9 : r= 0.5f; g=0.5f; b= 1.0f;		// light blue
					break;

				case Spectra::SPSC_A0 : 
				case Spectra::SPSC_A0p : 
				case Spectra::SPSC_A1 : 
				case Spectra::SPSC_A2 : 
				case Spectra::SPSC_A3 : 
				case Spectra::SPSC_A4 : 
				case Spectra::SPSC_A5 : 
				case Spectra::SPSC_A6 : 
				case Spectra::SPSC_A7 : 
				case Spectra::SPSC_A8 : 
				case Spectra::SPSC_A9 : r= 1.0f; g=1.0f; b= 1.0f;		// white
					break;

				case Spectra::SPSC_F0 : 
				case Spectra::SPSC_F1 : 
				case Spectra::SPSC_F2 : 
				case Spectra::SPSC_F3 : 
				case Spectra::SPSC_F4 : 
				case Spectra::SPSC_F5 : 
				case Spectra::SPSC_F6 : 
				case Spectra::SPSC_F7 : 
				case Spectra::SPSC_F8 : 
				case Spectra::SPSC_F9 : r= 1.0f; g=1.0f; b= 0.7f;		// white yellow
					break;

				case Spectra::SPSC_G0 : 
				case Spectra::SPSC_G1 : 
				case Spectra::SPSC_G2 : 
				case Spectra::SPSC_G3 : 
				case Spectra::SPSC_G4 : 
				case Spectra::SPSC_G5 : 
				case Spectra::SPSC_G6 : 
				case Spectra::SPSC_G7 : 
				case Spectra::SPSC_G8 : 
				case Spectra::SPSC_G9 : r= 1.0f; g=1.0f; b= 0.0f;		// yellow
					break;

				case Spectra::SPSC_K1 : 
				case Spectra::SPSC_K2 : 
				case Spectra::SPSC_K3 : 
				case Spectra::SPSC_K4 : 
				case Spectra::SPSC_K5 : 
				case Spectra::SPSC_K6 : 
				case Spectra::SPSC_K7 : 
				case Spectra::SPSC_K8 : 
				case Spectra::SPSC_K9 : r= 1.0f; g=0.5f; b= 0.0f;		// orange
					break;

				case Spectra::SPSC_M0V : 
				case Spectra::SPSC_M2V : 
				case Spectra::SPSC_M1 : 
				case Spectra::SPSC_M2 : 
				case Spectra::SPSC_M3 : 
				case Spectra::SPSC_M4 : 
				case Spectra::SPSC_M5 : 
				case Spectra::SPSC_M6 : 
				case Spectra::SPSC_M7 : 
				case Spectra::SPSC_M8 : 
				case Spectra::SPSC_M9 : r= 1.0f; g=0.0f; b= 0.0f;		// red
					break;

				case Spectra::SPSC_LDWARF : 
				case Spectra::SPSC_L0 : 
				case Spectra::SPSC_L1 : 
				case Spectra::SPSC_L2 : 
				case Spectra::SPSC_L3 : 
				case Spectra::SPSC_L4 : 
				case Spectra::SPSC_L5 : 
				case Spectra::SPSC_L55 : 
				case Spectra::SPSC_L6 : 
				case Spectra::SPSC_L7 : 
				case Spectra::SPSC_L8 : 
				case Spectra::SPSC_L9 : r= 0.5f; g=0.0f; b= 0.0f;		// dark red
					break;

				case Spectra::SPSC_T2 : r= 0.5f; g=0.3f; b= 1.0f;		// brown
					break;

				case Spectra::SPSC_CARBON_LINES : 
				case Spectra::SPSC_CARBONWD : 
				case Spectra::SPSC_CARBON : r= 0.0f; g=0.6f; b= 0.0f;		// dark green
					break;

				case Spectra::SPSC_CALCIUMWD : r= 0.0f; g=0.9f; b= 0.0f;	// lighter green
					break;

				case Spectra::SPSC_WD :
				case Spectra::SPSC_WDCOOLER :
				case Spectra::SPSC_WDHOTTER :
				case Spectra::SPSC_WDMAGNETIC :	r= 0.0f; g=1.0f; b= 1.0f;	// cyan
					break;
				}
			}

	
			pRGBMap[i*3+0] = r;
			pRGBMap[i*3+1] = g;
			pRGBMap[i*3+2] = b;
			_sourceSpectra.endRead(index);
		}
	}

	SpectraHelpers::saveIntensityMap( pRGBMap, gridSize, gridSize, _sstrFilename);

	delete[] pRGBMap;
}

#endif // _linux

}

