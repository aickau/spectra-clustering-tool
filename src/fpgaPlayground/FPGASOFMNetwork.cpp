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
//! \file  SOFMNetwork.cpp

#include "FPGASOFMNetwork.h"

#include "sdsslib/random.h"
#include "sdsslib/defines.h"
#include "sdsslib/helpers.h"
#include "sdsslib/fileHelpers.h"
#include "sdsslib/Timer.h"
#include "sdsslib/spectraVFS.h"
#include "sdsslib/spectraBaseHelpers.h"
#include "sdsslib/spectraHelpers.h"
#include "sdsslib/mathhelpers.h"
#include "sdsslib/XMLExport.h"
#include "sdsslib/XMLParser.h"
#include "sdsslib/HTMLExport.h"
#include "sdsslib/sdssSoftwareVersion.h"

#include <conio.h>
#include <math.h>
#include <float.h>
#include <assert.h>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <map>
#include <omp.h>

//#define SDSS_SINETEST

// set BMU in the map and source spectrum
// _networkSpectrum artificial spectrum in the map
// _networkIndex network index [0..gridsizesqr-1]
// _bestMatchSpectrum source/input spectrum
// _bestMatchIndex index to input spectrum [0..numspectra-1]
static void setBestMatch( Spectra &_networkSpectrum, size_t _networkIndex, Spectra &_bestMatchSpectrum, size_t _bestMatchIndex )
{
	assert( _networkSpectrum.isEmpty() );
	// set best matching related info.
	_networkSpectrum.m_SpecObjID	= _bestMatchSpectrum.m_SpecObjID;
	_networkSpectrum.m_Index		= _bestMatchIndex;
	_networkSpectrum.m_version		= _bestMatchSpectrum.m_version;

	// remember best match position to NW for faster search
	_bestMatchSpectrum.m_Index = _networkIndex;

}


void FPGASOFMNetwork::BestMatch::reset()
{
	error = FLT_MAX;
	index = 0;
}



size_t FPGASOFMNetwork::getIndex( size_t _cellX, size_t _cellY )
{
	return _cellX+_cellY*m_gridSize;
}



FPGASOFMNetwork::FPGASOFMNetwork( SpectraVFS *_pSourceVFS, bool bContinueComputation )
:m_pSourceVFS(_pSourceVFS)
,m_numSpectra(_pSourceVFS->getNumSpectra())
,m_gridSize(0)
,m_gridSizeSqr(0)
,m_currentStep(0)
,m_params( SOFMParameters::defaultParameters )
,m_Min(FLT_MAX)
,m_Max(-FLT_MAX)
{
	std::string sstrSOFMFileName("sofmnet.bin");
	reset( m_params );

	Spectra::setOperationRange( true );

	calcFluxAndNormalizeInputDS();
	calcMinMaxInputDS();

	if ( !bContinueComputation )
	{
		//
		// start new computation 
		//
		m_gridSize = static_cast<size_t>(ceilf(sqrtf((float)m_numSpectra+1))*1.1f); // gives a factor of 1.1^2=1.21 in total
		m_gridSizeSqr = m_gridSize*m_gridSize;

		// generate random filled cluster and load it.
		SpectraVFS::write( m_gridSize, m_Min, m_Max*0.01f, sstrSOFMFileName );
		m_pNet = new SpectraVFS( sstrSOFMFileName, false );

		// fill network with random spectra, this improves the convergence times.
		// it does not matter if some spectra are inserted multiple times or other may missing since this is just for initialization purposes.
		Rnd r(m_params.randomSeed);
		for ( size_t i=0;i<m_gridSizeSqr;i++ )
		{
			size_t xp = i % m_gridSize;
			size_t yp = i / m_gridSize;
			const size_t inset = 0;
			if ( xp >= inset && xp <= (m_gridSize-inset) && yp >= inset && yp <= (m_gridSize-inset) )
			{
				Spectra *a = m_pNet->beginWrite( i );
				size_t spectraIndex = r.randomInt(m_numSpectra-1);
				Spectra *b = m_pSourceVFS->beginRead( spectraIndex );
				a->set( *b );
				m_pSourceVFS->endRead( spectraIndex );
				m_pNet->endWrite( i );
			}
		}

	}
	else
	{
		//
		// continue old computation 
		//

		m_pNet = new SpectraVFS( sstrSOFMFileName, false );

		m_gridSizeSqr = m_pNet->getNumSpectra();
		m_gridSize = sqrtf(m_gridSizeSqr);

		if ( m_gridSizeSqr == 0 )
		{
			// error
			assert(0);
			exit(0);
		}
	}
}


FPGASOFMNetwork::~FPGASOFMNetwork()
{
	delete m_pNet;
}

void FPGASOFMNetwork::calcMinMax( SpectraVFS &_vfs, float &_outMin, float &_outMax )
{
	_outMin = FLT_MAX;
	_outMax = -FLT_MAX;

	const size_t numSpectra = _vfs.getNumSpectra();

	// calc min/max
	for ( size_t i=0;i<numSpectra;i++ )
	{
		Spectra *a = _vfs.beginRead( i );

		if ( _outMin > a->m_Min ) 
		{
			_outMin = a->m_Min;
		}
		if ( _outMax < a->m_Max) 
		{
			_outMax = a->m_Max;
		}
		_vfs.endRead( i );
	}
	// for sdds: -560 .. ~20.000 Angström
}


void FPGASOFMNetwork::calcMinMaxInputDS()
{
	calcMinMax( *m_pSourceVFS, m_Min, m_Max );
}

void FPGASOFMNetwork::calcFluxAndNormalizeInputDS( )
{
	m_flux = 0.0f;
	for ( size_t i=0;i<m_numSpectra;i++ )
	{
		Spectra *a = m_pSourceVFS->beginWrite( i );
		a->	normalizeByFlux();
		m_flux = MAX(a->m_flux, m_flux);
		m_pSourceVFS->endWrite( i );
	}
}





void FPGASOFMNetwork::reset( const SOFMParameters &_params )
{
	m_params = _params;
	m_currentStep = 0;
	m_Random.initRandom( m_params.randomSeed );
}



void FPGASOFMNetwork::adaptNetwork( const Spectra &_srcSpectrum, size_t _bestMatchIndex, float _adaptionThreshold, float _sigmaSqr, float _lRate )
{
	const size_t xpBestMatch = _bestMatchIndex % m_gridSize;
	const size_t ypBestMatch = _bestMatchIndex / m_gridSize;
	const float sigmaSqr2 = _sigmaSqr*(1.f/EULER);
	const float fGridSizeSqr = static_cast<float>(m_gridSizeSqr);

	const int gridSize = static_cast<int>(m_gridSize);


	// adjust weights of the whole network
#pragma omp parallel for schedule (dynamic)
	for ( int y=0;y<gridSize;y++)
	{
		const float distY1 = static_cast<float>(y)-static_cast<float>(ypBestMatch);
		//const float distY2 = (static_cast<float>(gridSize)-static_cast<float>(y))-static_cast<float>(ypBestMatch);
		const float distY1Sqr = distY1*distY1;
		//const float distY2Sqr = distY2*distY2;
		const float distYSqr = distY1Sqr; //MIN(distY1Sqr, distY2Sqr);

		for ( int x=0;x<gridSize;x++)
		{
			const float distX1 = static_cast<float>(x)-static_cast<float>(xpBestMatch);
			//const float distX2 = (static_cast<float>(gridSize)-static_cast<float>(x))-static_cast<float>(xpBestMatch);
			const float distX1Sqr = distX1*distX1;
			//const float distX2Sqr = distX2*distX2;
			const float distXSqr = distX1Sqr;//MIN(distX1Sqr, distX2Sqr);
			const float distSqr = (distXSqr+distYSqr)/fGridSizeSqr;					// normalize squared distance with gridsize

			// calculate neighborhood function
			//const float mexican_hat_term = 1.f-distSqr/_sigmaSqr;
			//const float hxy = exp(-(distSqr)/sigmaSqr2);							// original
			//const float hxy = exp(-(distSqr)/sigmaSqr2)*mexican_hat_term;			// Mexican hat
			const float hxy = exp(-sqrtf(distSqr)/sigmaSqr2);						// spike
			const float lratehsx = _lRate*hxy;

			if ( lratehsx > _adaptionThreshold )
			{
				const size_t spectraAdress = y*m_gridSize+x;
				Spectra *a = m_pNet->beginWrite( spectraAdress );
				a->adapt( _srcSpectrum, lratehsx );
				m_pNet->endWrite( spectraAdress );
			}
		}
	}
}


FPGASOFMNetwork::BestMatch FPGASOFMNetwork::searchBestMatchComplete( const Spectra &_src )
{
	float err[SpectraVFS::CACHELINESIZE];

	BestMatch bestMatch;
	bestMatch.reset();

	size_t j=0;
	while (j<m_gridSizeSqr)
	{
		const int jInc = MIN( SpectraVFS::CACHELINESIZE, (MIN(m_gridSizeSqr, j+SpectraVFS::CACHELINESIZE)-j));

		Spectra *a = m_pNet->beginRead( j );
		SpectraBaseHelpers::compareSpectra( _src, a, jInc, err );
		m_pNet->endRead( j );

		for ( size_t k=0;k<jInc;k++ )
		{
			if (err[k] < bestMatch.error )
			{
				bestMatch.error = err[k];
				bestMatch.index = j+k;
			}
		}

		j += jInc;
	}

	return bestMatch;
}


 
FPGASOFMNetwork::BestMatch FPGASOFMNetwork::searchBestMatchLocal( const Spectra &_src, const int _searchRadius )
{
	assert(_searchRadius > 0);
	BestMatch bestMatch;
	bestMatch.reset();
	bool bOnFrame = false;
	bool bFound = false;

	if ( _src.m_Index < 0 )
	{
		// no old position, due to continue computation, we have to use complete search!
		bestMatch = searchBestMatchComplete( _src );
		return bestMatch;
	}

	const int xpBestMatchOld = _src.m_Index % m_gridSize;
	const int ypBestMatchOld = _src.m_Index / m_gridSize;

	// calc boundaries
	const int xMin = MAX( xpBestMatchOld-_searchRadius, 0 );
	const int yMin = MAX( ypBestMatchOld-_searchRadius, 0 );
	const int xMax = MIN( xpBestMatchOld+_searchRadius+1, m_gridSize );
	const int yMax = MIN( ypBestMatchOld+_searchRadius+1, m_gridSize );

	const size_t numSpectraToSearch( (xMax-xMin)*(yMax-yMin) );

	// setup arrays
	std::vector<Spectra*> searchSpectraVec;
	std::vector<size_t> indexVec;
	std::vector<float> errorVec;
	searchSpectraVec.resize( numSpectraToSearch );
	indexVec.resize( numSpectraToSearch );
	errorVec.resize( numSpectraToSearch );

	// read spectra from vfs
	size_t c=0;
	for ( int y=yMin;y<yMax;y++ )
	{
		for ( int x=xMin;x<xMax;x++ )
		{
			const size_t spectraIndex = getIndex( x, y );
			indexVec[c] = spectraIndex;
			searchSpectraVec[c] = m_pNet->beginRead( spectraIndex );
			c++;
		}
	}

	// calculate errors/distances
	SpectraBaseHelpers::compareSpectra( _src, searchSpectraVec, &errorVec[0] );

	//end read, find bmu from error list
	for ( size_t i=0;i<numSpectraToSearch;i++ )
	{
		const size_t spectraIndex = indexVec[i];
		const float err = errorVec[i];

		if (err < bestMatch.error )
		{
			bestMatch.error = err;
			bestMatch.index = spectraIndex;
			bFound = true;
		}

		m_pNet->endRead(spectraIndex);
	}


	if ( !bFound )
	{
		// all spectra where used in the given search radius, use global search.
		//Helpers::print(".",NULL,false);
		bestMatch = searchBestMatchComplete( _src );
	}

	return bestMatch;
}


bool FPGASOFMNetwork::process()
{
	if ( m_currentStep > m_params.numSteps )
	{
		return true;
	}


	if ( m_currentStep > 1 )
	{
		m_pNet->dumpToFile(m_pNet->getFileName()+"old");
	}

	const float lPercent = static_cast<float>(m_currentStep)/static_cast<float>(m_params.numSteps);
	const float lRate = m_params.lRateBegin*pow(m_params.lRateEnd/m_params.lRateBegin,lPercent);
	const float adaptionThreshold = m_params.lRateEnd*0.01f;
	const float sigma = m_params.radiusBegin*pow(m_params.radiusEnd/m_params.radiusBegin,lPercent);
	const float sigmaSqr = sigma*sigma;
	bool bFullSearch = true;
	size_t searchRadius = 1;

	// determine search strategy for BMUs for the current learning step
	if ( m_params.sstrSearchMode == SOFMNET_SETTINGS_SEARCHMODE_localfast )
	{
		// always use a constant search radius, never do a global search
		bFullSearch = (m_currentStep<1);
		searchRadius = 2;
	}
	else if ( m_params.sstrSearchMode == SOFMNET_SETTINGS_SEARCHMODE_local )
	{
		// global search for the first 5 steps, decreasing search radius for increasing number of learing steps
		bFullSearch = (m_currentStep<5);
		searchRadius = static_cast<size_t>(((1.f-lPercent)*0.5f*static_cast<float>(m_gridSize)))+2;
	}
	else // SOFMNET_SETTINGS_SEARCHMODE_global
	{
		// always use global search, never go local.
		bFullSearch = true;
	}

	// select random spectra from spectra dataset
	std::vector<size_t> spectraIndexList;

	// store all indicies in a list
	for ( size_t i=0;i<m_numSpectra;i++) 
	{
		spectraIndexList.push_back(i);
	}

	// shake well
	for ( size_t i=0;i<m_numSpectra*2;i++)
	{
		size_t ind0 = m_Random.randomInt(m_numSpectra-1);
		size_t ind1 = m_Random.randomInt(m_numSpectra-1);

		size_t hui = spectraIndexList[ind0];
		spectraIndexList[ind0] = spectraIndexList[ind1];
		spectraIndexList[ind1] = hui;
	}

	// clear names
	for ( size_t i = 0;i < m_gridSizeSqr;i++)
	{
		Spectra *a = m_pNet->beginWrite( i );
		a->m_SpecObjID = 0;
		a->m_Index = -1;
		m_pNet->endWrite( i );
	}

	// for each training spectra..
	double avgDist = 0.0;
	for ( size_t j=0;j<m_numSpectra;j++ )
	{
		// initialize best match batch
		BestMatch bmu;

		const size_t spectraIndex = spectraIndexList.at(j);
		Spectra &currentSourceSpectra = *m_pSourceVFS->beginWrite(spectraIndex);

		//		Timer t;

		// retrieve best match neuron for a source spectra
		if (bFullSearch) 
		{
			bmu = searchBestMatchComplete( currentSourceSpectra );
		}
		else
		{
			bmu = searchBestMatchLocal( currentSourceSpectra, searchRadius );
		}

	
		// mark best match neuron
		Spectra *bmuSpectrum = m_pNet->beginWrite( bmu.index );
		setBestMatch( *bmuSpectrum, bmu.index, currentSourceSpectra, spectraIndex );
		avgDist += currentSourceSpectra.compare( *bmuSpectrum );
		m_pNet->endWrite( bmu.index );

		// adapt neighborhood
		adaptNetwork( currentSourceSpectra, bmu.index, adaptionThreshold, sigmaSqr, lRate );

		m_pSourceVFS->endWrite(spectraIndex);
	}

	m_pNet->flush();
	m_currentStep++;

	// clustering not yet finished, need another learning step
	return false;
}
