//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
//! #
//! # Copyright (c) 2009 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 12/25/2012
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  KernelProgsCUDA.h
//! \brief CUDA compute functions that are executed on the GPU (or any other CUDA device).


#ifndef EULER
#define EULER 2.718281828f
#endif 

__global__ void clearBMUs( int *_BMUs, int _numNetworkSpectra )
{
	int index = threadIdx.x + blockIdx.x * blockDim.x; 

	if ( index < _numNetworkSpectra )
		_BMUs[index] = -1;
}

__global__ void search( const float *_srcSpectrum, const float *_networkSpectra, float *_outErr, int _numNetworkSpectra, int _numElements)
{
	int index = threadIdx.x + blockIdx.x * blockDim.x; 
	int o = index*_numElements;

	if (index < _numNetworkSpectra)
	{
		float err = 0.0f;
		for (int i=0;i<_numElements;i++)
		{
			float d = _srcSpectrum[i]-_networkSpectra[o+i];
			err += d*d;
		}
		_outErr[index] = err;
	}
}

__global__ void getBMU( float *_errorList, int _numSpectra, int srcSpectraIndex, int *_outBMU )
{
	__syncthreads();

	float err = 100000000000000000000000000000.0;
	int idx = 0;
	for ( int i=0;i<_numSpectra;i++ )
	{
		if ( _errorList[i] < err && _outBMU[i] == -1 ) 
		{
			idx = i;
			err = _errorList[i];
		}
	}

	_outBMU[idx] = srcSpectraIndex;
	_outBMU[_numSpectra] = idx;

	__syncthreads();
}

__global__ void adaptNetwork( 
		const float *spectrum, 
		float *_networkSpectra, 
		int _numElements, 
		int *_outBMU,
		int _gridSize, 
		float _adaptionThreshold, 
		float _sigmaSqr, 
		float _lRate )
{
    const int bestMatchIndex = _outBMU[_numElements];
 	const int xpBestMatch = bestMatchIndex % _gridSize;
 	const int ypBestMatch = bestMatchIndex / _gridSize;
 	const float sigmaSqr2 = _sigmaSqr*(1.f/EULER);
 	const float fGridSizeSqr = _gridSize*_gridSize;


 	// adjust weights of the whole network

 	int x = threadIdx.x + blockIdx.x * blockDim.x; 
	int y = threadIdx.y + blockIdx.y * blockDim.y; 

	if ( x < _gridSize && y < _gridSize )
	{	
		//was: for ( int y=0;y<_gridSize;y++)

 		const float distY1 = y-ypBestMatch;
 		const float distY1Sqr = distY1*distY1;
 		const float distYSqr = distY1Sqr;
 
  		// was: for ( int x=0;x<_gridSize;x++)
  		const float distX1 = x-xpBestMatch;
  		const float distX1Sqr = distX1*distX1;
  		const float distXSqr = distX1Sqr;
  		const float distSqr = (distXSqr+distYSqr)/fGridSizeSqr;					// normalize squared distance with gridsize
  
  		// calculate neighborhood function
  		const float hxy = expf(-sqrtf(distSqr)/sigmaSqr2);						// spike
  		const float lratehsx = _lRate*hxy;
 
 		if ( lratehsx > _adaptionThreshold )
 		{
 			const int spectraAdress = (y*_gridSize+x)*_numElements;
	 		for ( int i=0;i<_numElements;i++ )
 			{
 	 			_networkSpectra[spectraAdress+i] += lratehsx*(spectrum[i]-_networkSpectra[spectraAdress+i]);
   			}
  		}
	}
}

#define NUMTHREADS 512




extern "C" void
process(
	const float *_sourceSpectra, 
	int _numSourceSpectra, 
	float *_networkSpectra, 
	int _numNetworkSpectra, 
	size_t *_pSpectraList, 
	float *_outErr, 
	int *_outBMU, 
	int _numElements, 
	int _gridSize,
	float _adaptionThreshold, 
	float _sigmaSqr, 
	float _lRate )
{

	clearBMUs<<<(_numNetworkSpectra+NUMTHREADS-1)/NUMTHREADS,NUMTHREADS>>>( _outBMU, _numNetworkSpectra );

	// for each input spectrum
	for ( int i=0;i<_numSourceSpectra;i++ ) 
	{
		int srcSpectraIndex = _pSpectraList[i];

		if ( srcSpectraIndex >= 0 || srcSpectraIndex <  _numSourceSpectra)
		{
			int o = srcSpectraIndex*_numElements;

			// calculate euclidean distances to codebook vectors
			search<<<(_numNetworkSpectra+NUMTHREADS-1)/NUMTHREADS,NUMTHREADS>>>( &_sourceSpectra[o], _networkSpectra, _outErr, _numNetworkSpectra, _numElements );
 
 			// calculate best matching unit (BMU) from error list (i.e. euclidean distances)
 			getBMU<<<1,1>>>( _outErr, _numNetworkSpectra, srcSpectraIndex, _outBMU );
 
 			// adapt code book vectors in vicinity.

		    dim3 threadsPerBlock(16, 16);
			dim3 numBlocks((_gridSize+threadsPerBlock.x-1) / threadsPerBlock.x, (_gridSize+threadsPerBlock.y-1) / threadsPerBlock.y);
 			adaptNetwork<<<numBlocks,threadsPerBlock>>>( 
 					&_sourceSpectra[o], 
 					_networkSpectra, 
 					_numElements, 
 					_outBMU, 
 					_gridSize, 
 					_adaptionThreshold, 
 					_sigmaSqr, 
 					_lRate );
		}
	}
}
