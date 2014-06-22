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
//! \file  mathhelpers.h
//! \brief 

#include <math.h>
#include <stddef.h> 

namespace MathHelpers
{
	// mean or average type
	enum MeanType 
	{
		MEAN_STANDARD,			// < standard mean over all samples
		MEAN_ABSOLUTE,			// < absolute mean over all samples
		MEAN_POSITIVE,			// < positive mean over all positive samples
		MEAN_NEGATIVE			// < negative mean over all negative samples
	};

	// compare to float values
	bool floatEq (float a, float b, float epsilon); 

	 // t between 0.0 and 1.0
	float lerp (float a, float b, float t); 


	// logarithm with a certain base
	float logf(float value, float base);

	unsigned int log2(unsigned int val);
	
	// returns a!
	// warning: no overfow detection
	unsigned int factorial( unsigned int a );

	// returns binomial coefficient n over k
	// k<=n
	unsigned int binomial( unsigned int n, unsigned int k );

	// generate normalized binomial factors 
	// factors = float array of size n+1
	// n = number of factors
	// e.g. for n=4 this would yield to 1/16, 4/16, 6/16, 4/16, 1/16
	void binomialFilter1D( float *factors, unsigned int n );

	// one dimensional Gaussian function
	// _width is normalized to FWHM (full width at half maximum)
	float gauss1D( float _x,  float _amplitude=1.f, float _phase=0.f, float _width=1.f );

	// get global bounds of a given float array.
	void getMinMax( float *_values, const size_t _numValues, const size_t _strideInBytes, const size_t _offsetInBytes, float &_outMin, float &_outMax );

	// get mean value and deviation of a given float array.
	// _type = mean and deviation type:
	void getMeanDeviation( float *_values, 
							const size_t _numValues, 
							const size_t _strideInBytes, 
							const size_t _offsetInBytes, 
							const MeanType _type, 
							float &_outMean, 
							float &_outDeviation );

	// fold float array to half size (in situ)
	// _numValues > 2
	// returns size of folded float array
	size_t fold1D( float *_values, size_t _numValues );

	// calculate gradient of a given array (in situ)
	// _numValues > 2
	// last gradient element is always 0.0
	void gradient1D( float *_values, const size_t _numValues, const size_t _strideInBytes=4,  const size_t _offsetInBytes=0 );

	// get overall difference between 2 float arrays
	float getError( const float *_values1, const float *_values2, const size_t _numValues );

	// smooth float array using direct neighbors, weights are 0.25, 0.5, 0.25
	void smooth( const float *_valuesSrc, float *_valuesDst, size_t _numValues, size_t _numIterations=1);

	// Catmull Rom interpolation between points p1, p2
	// x 0..1
	double interpolateCatmullRom( double p0, double p1, double p2, double p3, double x );

};

inline
bool MathHelpers::floatEq (float a, float b, float epsilon)
{
	float d = a-b;
	return (d<epsilon && d>-epsilon);
}

inline
float MathHelpers::lerp (float a, float b, float t)
{
	return (a+(b-a)*t);
}

inline
float MathHelpers::logf(float value, float base)
{
	return log(value)/log(base);
}

inline
unsigned int MathHelpers::log2 (unsigned int val) 
{
	unsigned int ret = -1;
	while (val != 0) {
		val >>= 1;
		ret++;
	}
	return ret;
}

inline
double MathHelpers::interpolateCatmullRom( double p0, double p1, double p2,double p3, double x )
{
	double a0,a1,a2,a3,x2;

	x2 = x*x;
	a0 = -0.5*p0 + 1.5*p1 - 1.5*p2 + 0.5*p3;
	a1 = p0 - 2.5*p1 + 2*p2 - 0.5*p3;
	a2 = -0.5*p0 + 0.5*p2;
	a3 = p1;

	return(a0*x*x2+a1*x2+a2*x+a3);
}