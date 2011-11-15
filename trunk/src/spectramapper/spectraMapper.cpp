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
//! \file  spectraMapper.cpp
//! \brief draw multiple spectra into one diagram from a mask selection


#include "spectraMapper.h"

#include "sdsslib/glhelper.h"
#include "sdsslib/spectraHelpers.h"
#include "sdsslib/helpers.h"
#include "sdsslib/filehelpers.h"

#include "devil/include/il/il.h"
#include "devil/include/il/ilu.h"

#include <assert.h>
#include <Windows.h>
#include <conio.h>
#include <iostream>

extern std::ofstream logFile;

// generate a combined diagram with all spectra from selection
SpectraMapper::SpectraMapper( const std::string &sstrSourceSpectraFilename, const std::string &sstSelectionMaskFilename, const std::string &sstrPlotImageFilename )
:m_sstrPlotImageFilename(sstrPlotImageFilename)
,m_gridSize(859)
,m_gridSizeSqr(m_gridSize*m_gridSize)
,m_numSourceSpecra(0)
,m_pSourceVFS(NULL)
,m_currentIndex(-1)
,m_imageWriteCount(0)
,m_plotCount(0.f)
{
	// load mask
	ilLoadImage( (ILstring)sstSelectionMaskFilename.c_str() );
	ILenum err = ilGetError();
	if( err != NULL )
	{
		Helpers::print("Error: Mask image "+sstSelectionMaskFilename+" could not be loaded.\n", &logFile);
		return;
	}
	ilConvertImage( IL_LUMINANCE, IL_UNSIGNED_BYTE );
	int width = ilGetInteger( IL_IMAGE_WIDTH );
	int height = ilGetInteger( IL_IMAGE_HEIGHT );
	if ( width != height ) {
		// wrong dimensions
		Helpers::print("Error: image dimensions do not match. Must be rectangular.\n", &logFile);

		return;
	}
	unsigned char *pt = ilGetData();
	if ( pt == NULL )  {
		Helpers::print("Error: Mask image "+sstSelectionMaskFilename+" could not be loaded.\n", &logFile);

		// nah, fail..
		return;
	}

	m_gridSize = width;
	m_gridSizeSqr = m_gridSize*m_gridSize;

	Helpers::print( "Reading dump file "+sstrSourceSpectraFilename+ "\n", &logFile );


	m_pSourceVFS = new SpectraVFS( sstrSourceSpectraFilename, false );
	m_numSourceSpecra = m_pSourceVFS->getNumSpectra();

	if ( m_numSourceSpecra <= 0 )
	{
		Helpers::print("Error: No spectral data found.\n", &logFile);

		// nah, fail..
		return;
	}
	else
	{
		Helpers::print(Helpers::numberToString<size_t>(m_numSourceSpecra)+" spectra found.\n", &logFile);
	}


	size_t j=199;
	int *pIndexlist= new int[m_gridSizeSqr];
	std::string sstrIndexList = "indexlist";
	sstrIndexList += Helpers::numberToString(j,4);
	sstrIndexList+= ".bin";
	FILE *f=fopen(sstrIndexList.c_str(),"rb");
	if ( f== NULL) {
		Helpers::print("Error: Missing index list.\n", &logFile);

		// no index list
		return;
	}
	fread(pIndexlist, 1, m_gridSizeSqr*sizeof(int), f);
	fclose(f);

	m_spMin.add(FLT_MAX);

	size_t count = 0;
	m_spMax.add(-FLT_MAX);
	int maskCount = 0;

	for (size_t i=0;i<m_gridSizeSqr;i++)
	{
		// if mask is selected..
		if (pt[i] > 200 ) {
			int index = pIndexlist[i];

			if ( index >= 0 && index < m_numSourceSpecra )
			{
				m_mappedSpectra.push_back(index);
				Spectra *sp = m_pSourceVFS->beginRead(index);
				SSE_ALIGN Spectra tsp(*sp);	
				tsp.normalizeByFlux();

				m_spAvg.add( tsp );

				// min / max
				for ( size_t j=0;j<Spectra::numSamples;j++ )
				{
					if ( m_spMin.m_Amplitude[j] > tsp.m_Amplitude[j] ) 
					{
						m_spMin.m_Amplitude[j] = tsp.m_Amplitude[j];
					}
					if ( m_spMax.m_Amplitude[j] < tsp.m_Amplitude[j] ) 
					{
						m_spMax.m_Amplitude[j] = tsp.m_Amplitude[j];
					}
				}

				m_pSourceVFS->endRead(index);
				count++;
			}
			maskCount++;
		}
	}


	delete[] pIndexlist;

	m_numSpectraToDraw = count;
	m_spAvg.multiply(1.f/static_cast<float>(m_numSpectraToDraw));
	m_spAvg.calcMinMax();
	m_spMax.calcMinMax();
	m_spAvg.m_SamplesRead= Spectra::numSamples;
	m_spMax.m_SamplesRead= Spectra::numSamples;
	m_spMin.m_SamplesRead= Spectra::numSamples;
}

SpectraMapper::~SpectraMapper()
{
	delete m_pSourceVFS;
}

// generate a combined diagram with all spectra from selection
void SpectraMapper::draw( int _width, int _height, bool _toRestFrame, bool _normalizeByFlux, float _yscale, float _brightness, int _selection, bool _writeDataToPNG )
{
	if ( m_numSourceSpecra <= 0 || m_pSourceVFS == NULL || m_numSpectraToDraw <= 0 )
		return;

	const float imgScale = _yscale;		// 1.5, 3.5 6.5
	const int imgYOffset = 100;
	const float brightness = (10.f*_brightness/(m_plotCount+1.f));//1.f/255.f; // 0.1f;//1.f/255.f;// 

	glClearColor(0.f,0.f,0.f,1.f);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	


	const float xDAll = Spectra::waveEndDst-Spectra::waveBeginDst;

	GLHelper::SetBlendMode(GLHelper::kBlendMode_Add);

	SSE_ALIGN Spectra tsp;

	int count = 0;
	for (size_t i=0;i<m_mappedSpectra.size();i++)
	{
		int index = m_mappedSpectra[i];

		if ( index >= 0 && index < m_numSourceSpecra )
		{
			Spectra *sp = m_pSourceVFS->beginRead(index);

			if ( _normalizeByFlux )
			{
				tsp = *sp;	

				tsp.normalizeByFlux();
				sp = &tsp;
			}

			float xO = 0.0f;
			float xD = xDAll;
			if ( _toRestFrame ) {
				float xB = Spectra::waveLenghtToRestFrame( Spectra::waveBeginSrc,tsp.m_Z );
				float xE = Spectra::waveLenghtToRestFrame( Spectra::waveEndSrc,tsp.m_Z );
				xD = xE-xB;
				xO = (xB-Spectra::waveBeginDst)/xDAll;
			}

			if ( count ==  (_selection % m_numSpectraToDraw) && (_selection >= 0))
			{
				glColor3f(0.f,1.f,0.f);

				if ( m_currentIndex != _selection )
				{
					std::string sstrInfo("MJD:");
					sstrInfo += Helpers::numberToString<int>(sp->getMJD());
					sstrInfo += "  plateID:";
					sstrInfo += Helpers::numberToString<int>(sp->getPlate());
					sstrInfo += "  fiberID:";
					sstrInfo += Helpers::numberToString<int>(sp->getFiber());
					sstrInfo += "\n";
					Helpers::print(sstrInfo, &logFile );
					Helpers::print(sp->getURL()+"\n", &logFile );
					m_currentIndex = _selection;
				}
			}
			else
			{
				glColor3f(brightness,brightness,brightness);
			}


//				sp->m_SamplesRead= Spectra::numSamples;
			SpectraHelpers::drawSpectra(*sp, false, false, xO*_width, imgYOffset, _width, _height, imgScale/m_spMax.m_Max, xD/xDAll, 0 );

			m_pSourceVFS->endRead(index);
			count++;
		}
	}
	m_plotCount = count;
/*
	if ( !_toRestFrame ) {
		GLHelper::SetBlendMode(GLHelper::kBlendMode_Off);
		glColor3f(0,1,0);
		SpectraHelpers::drawSpectra(spAvg, false, false, 0, imgYOffset, _width, _height, imgScale/spMax.m_Max, 1.f, 0 );
		glColor3f(1,1,0);
		SpectraHelpers::drawSpectra(spMin, false, false, 0, imgYOffset, _width, _height, imgScale/spMax.m_Max, 1.f,  0 );
		glColor3f(1,0,0);
		SpectraHelpers::drawSpectra(spMax, false, false, 0, imgYOffset, _width, _height, imgScale/spMax.m_Max,  1.f, 0 );
	}
*/



	if ( _writeDataToPNG )
	{
		std::string sstrFilename(m_sstrPlotImageFilename);
		sstrFilename += Helpers::numberToString<int>(m_imageWriteCount,2);
		sstrFilename += ".png";

		Helpers::print("writing spectra plot to "+sstrFilename+".", &logFile);
		ILuint image;
		ilGenImages( 1, &image );
		ilBindImage(image);
		ilTexImage(static_cast<ILuint>(_width),static_cast<ILuint>(_height),1,3,IL_RGB, IL_UNSIGNED_BYTE, NULL );
		ilEnable(IL_FILE_OVERWRITE );
		iluImageParameter(ILU_FILTER,ILU_SCALE_BSPLINE);

		glReadPixels(0,0,_width,_height,GL_RGB, GL_UNSIGNED_BYTE, ilGetData());

		ilSave( IL_PNG, const_cast<char*>(sstrFilename.c_str()) );
		ilDeleteImage(image);

		m_imageWriteCount++;
	}
}