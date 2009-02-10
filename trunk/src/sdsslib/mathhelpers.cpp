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
//! \file  mathhelpers.cpp
//! \brief 

#include "sdsslib/mathhelpers.h"

#include "defines.h"
#include <assert.h>
#include <float.h>

unsigned int MathHelpers::factorial( unsigned int a )
{
	if ( a > 0 )
	{
		return MathHelpers::factorial(a-1)*a;
	}
	return 1;
}



unsigned int MathHelpers::binomial( unsigned int n, unsigned int k )
{
	assert( k <= n );

	if ( k > 0)
	{
		unsigned int a = factorial(n);
		unsigned int b = factorial(k)*factorial(n-k);
		return a/b;
	}

	return 1;
}


void MathHelpers::binomialFilter1D( float *factors, unsigned int n )
{
	assert( factors != NULL );
	float sum = 0.0f;
	for ( unsigned int i=0;i<=n;i++) 
	{
		factors[i] = static_cast<float>(binomial( n, i ));
		sum += factors[i];
	}
	for ( unsigned int i=0;i<=n;i++) 
	{
		factors[i] /= sum; 
	}
}


float MathHelpers::gauss1D( float _x, float _amplitude, float _phase, float _width )
{
	assert( _width > 0.0f );
	const float bx = (_x-_phase);
	const float c = _width*FWHM_GAUSS;
	const float val = _amplitude*powf(EULER,-bx*bx/(2.f*c*c));
	return val;
}



void MathHelpers::getMinMax( float *_values, const size_t _numValues, const size_t _strideInBytes, const size_t _offsetInBytes, float &_outMin, float &_outMax )
{
	assert( _values != NULL );
	assert( _strideInBytes != 0 );
	_outMin=FLT_MAX;
	_outMax=-FLT_MAX;

	char *p = reinterpret_cast<char*>(_values)+_offsetInBytes;
	for ( size_t i=0;i<_numValues;i++)
	{
		float *v = reinterpret_cast<float*>( p );
		if ( *v < _outMin )
		{
			_outMin = *v;
		}
		if ( *v > _outMax )
		{
			_outMax = *v;
		}
		p += _strideInBytes;
	}
}


void MathHelpers::getMeanDeviation( float *_values, 
								   size_t _numValues, 
								   const size_t _strideInBytes, 
								   const size_t _offsetInBytes, 
								   const MeanType _type, 
								   float &_outMean, 
								   float &_outDeviation )
{
	assert( _values != NULL );
	assert( _strideInBytes != 0 );
	_outMean=0.f;
	_outDeviation=0.f;

	if ( _numValues < 1 ) {
		return;
	}

	double m = 0.0;
	double d = 0.0;
	double nv = static_cast<double>(_numValues);

	// calc mean value
	char *p = reinterpret_cast<char*>(_values)+_offsetInBytes;
	for ( size_t i=0;i<_numValues;i++)
	{
		float *v = reinterpret_cast<float*>( p );
		double value = static_cast<double>(*v);
		switch(_type) {
			case MEAN_ABSOLUTE : m += abs(value); break;
			case MEAN_POSITIVE : m += (value>0.0) ? value : 0.0; break;
			case MEAN_NEGATIVE : m += (value<0.0) ? value : 0.0; break;
			default : m += value;
		}
		
		p += _strideInBytes;
	}
	m = m / nv;

	// calc deviation
	p = reinterpret_cast<char*>(_values)+_offsetInBytes;
	for ( size_t i=0;i<_numValues;i++)
	{
		float *v = reinterpret_cast<float*>( p );
		d += abs(static_cast<double>(*v)-m);
		p += _strideInBytes;
	}
	d = d / nv;

	_outMean = static_cast<float>(m);
	_outDeviation = static_cast<float>(d);
}


size_t MathHelpers::fold1D(float *_values, size_t _numValues )
{
	assert( _values != NULL );

	if ( _numValues < 2 ) {
		return 0;
	}

	size_t numValuesHalf = _numValues/2;
	size_t c=0;
	for ( size_t i=0;i<numValuesHalf;i++ ) {
		_values[i] = (_values[c]+_values[c+1])*0.5f;
		c+=2;
	}
	return numValuesHalf;
}



void MathHelpers::gradient1D( float *_values, const size_t _numValues, const size_t _strideInBytes, const size_t _offsetInBytes )
{
	assert( _values != NULL );
	assert( _strideInBytes != 0 );
	
	if ( _numValues < 2 ) {
		return;
	}

	char *p = reinterpret_cast<char*>(_values)+_offsetInBytes;
	for ( size_t i=0;i<_numValues-1;i++)
	{
		float *v0 = reinterpret_cast<float*>( p );
		p += _strideInBytes;
		float *v1 = reinterpret_cast<float*>( p );
		*v0 = *v1 - *v0;
	}
	float *v0 = reinterpret_cast<float*>( p );
	*v0 = 0.0f;
}


float MathHelpers::getError( const float *_values1, const float *_values2, const size_t _numValues ) 
{
	assert( _values1 != NULL );
	assert( _values2 != NULL );

	float error = 0.0f;
	for (size_t i=0;i<_numValues;i++)
	{
		const float d = _values1[i]-_values2[i];
		error += d*d;
	}

	return error;
}