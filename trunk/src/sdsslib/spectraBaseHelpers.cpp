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
//! \file  spectraBaseHelpers.cpp
//! \brief Low-level spectra helper routines.

#include "spectraBaseHelpers.h"

#include "spectra.h"

#include <assert.h>
#include <omp.h>
#include <string.h>
#include <math.h>


namespace SpectraBaseHelpers
{


void foldSpectrum( float *_pSrcSpectrum, size_t _numSrcSamples, float *_pDstSpectrum, size_t _numDstSamples, size_t _numFoldIterations )
{
	assert( _pSrcSpectrum != NULL );
	assert( _pDstSpectrum != NULL );
	assert( _numSrcSamples > 1 );
	assert( _numDstSamples >= 1 );
	assert( _numDstSamples <= _numSrcSamples );
	assert( _numFoldIterations >= 1 );

	if ( _pSrcSpectrum == NULL || 
		_pDstSpectrum == NULL ||
		_numSrcSamples <= 1 ||
		_numDstSamples < 1 ||
		_numDstSamples > _numSrcSamples ||
		_numFoldIterations == 0 )
	{
		return;
	}

	// check if destination buffer is large enough.
	const size_t numDstSamplesRequired = _numSrcSamples/static_cast<size_t>(powf(2.f, static_cast<float>(_numFoldIterations) ));
	if ( numDstSamplesRequired > _numDstSamples )
	{
		assert(0); 
		return;
	}

	size_t numSamples = _numSrcSamples;
	for ( size_t j=0;j<_numFoldIterations;j++ )
	{
		size_t c=0;
		for (size_t i=0;i<numSamples-1;i+=2)
		{
			_pSrcSpectrum[c] = (_pSrcSpectrum[i]+_pSrcSpectrum[i+1]) * 0.5f;
			c++;
		}
		numSamples /= 2; 
	}

	memcpy( _pDstSpectrum, _pSrcSpectrum, sizeof(float)*numSamples );

	// fill unread samples with 0.0
	for ( size_t i=numSamples;i<_numDstSamples;i++)
	{
		_pDstSpectrum[i] = 0.0f;
	}
}



void compareSpectra(const Spectra &_a, Spectra *_pB, size_t _nCount, float *_pOutErrors )
{
	assert( _pB != NULL );
	assert( _pOutErrors != NULL );
	const int numElements = static_cast<int>(_nCount);

#pragma omp parallel for
	for (int i=0;i<numElements;i++)
	{
		if ( _pB[i].isEmpty() )
		{
			_pOutErrors[i] = _a.compare( _pB[i] );
		}
		else
		{
			_pOutErrors[i] = FLT_MAX;
		}
	}
}

void compareSpectra(const Spectra &_a, std::vector<Spectra*> &_pB, float *_pOutErrors )
{
	assert( _pOutErrors != NULL );
	const int numElements = static_cast<int>(_pB.size());

#pragma omp parallel for
	for (int i=0;i<numElements;i++)
	{
		if ( _pB[i]->isEmpty() )
		{
			_pOutErrors[i] = _a.compare( *_pB[i] );
		}
		else
		{
			_pOutErrors[i] = FLT_MAX;
		}
	}
}


void repairSpectra( float *_pixels, bool *_maskarray, int _numsamples )
{
	// repair isolated pixel errors
	for (int i=1;i<_numsamples-1;i++)
	{
		if (_maskarray[i] )
		{
			if ( !_maskarray[i-1] && !_maskarray[i+1] )
			{
				const float pixelRepaired = (_pixels[i-1] + _pixels[i+1]) / 2.0f;
				_pixels[i] = pixelRepaired;
			}
		}
	}

	// pixel repair at boundaries
	if ( _maskarray[0] && !_maskarray[1] )
	{
		_pixels[0] = _pixels[1];
	}
	{
		const size_t l1 = _numsamples-1;
		const size_t l2 = _numsamples-2;

		if ( _maskarray[l1] && !_maskarray[l2] )
		{
			_pixels[l1] = _pixels[l2];
		}
	}
}



}