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

#include "sdsslib/Timer.h"


#include<stdio.h>

//#include<cuda.h>
//#include<cuda_runtime.h>
//#include<cuda_runtime_api.h>

#ifndef EULER
#define EULER 2.718281828f
#endif 

#define NUMTHREADS_CLEAR 512
#define NUMBLOCKS_SEARCH 256
#define NUMTHREADS_SEARCHGATHER 512
#define NUMTHREADS_ADAPT 64

__global__ void clearBMUs( int *_BMUs, int _numNetworkSpectra )
{
	int index = threadIdx.x + blockIdx.x * blockDim.x; 

	if ( index < _numNetworkSpectra )
		_BMUs[index] = -1;
}


__global__ void clearErrors( float *_err, int _numNetworkSpectra )
{
	int index = threadIdx.x + blockIdx.x * blockDim.x; 

	if ( index < _numNetworkSpectra )
		_err[index] = 0.0;
}



__global__ void search( const float *_srcSpectrum, const float *_networkSpectra, float *_tempErr, float *_outErr, int _numNetworkSpectra, int _numElements)
{
	int indexDelta = (ceilf((float)_numNetworkSpectra/(float)NUMBLOCKS_SEARCH));
	int indexLow = blockIdx.x*indexDelta; 
	int indexHigh = indexLow + indexDelta;
	int thrdIdx = threadIdx.x;

	__shared__ float err[16536];

	indexLow = min( indexLow, _numNetworkSpectra-1 );
	indexHigh = min( indexHigh, _numNetworkSpectra );

	int nTotalThreads = blockDim.x;

	while(nTotalThreads > 1)
	{
		int halfPoint = (nTotalThreads >> 1);	

		if (threadIdx.x < halfPoint)
		{
			for ( int index = indexLow;index<indexHigh;index++)
			{
				int thread2 = threadIdx.x + halfPoint;

				int il = index-indexLow;
				int o = index*_numElements;
				float d1 = _srcSpectrum[thrdIdx]-_networkSpectra[o+thrdIdx];
				float d2 = _srcSpectrum[thread2]-_networkSpectra[o+thread2];

				err[il*blockDim.x+thrdIdx] = d1*d1+d2*d2;
			}
		}

		__syncthreads();

	}


	for ( int index = indexLow;index<indexHigh;index++)
	{
		int il = index-indexLow;
		_outErr[index] = err[il*blockDim.x];
	}
}

__global__ void gatherSearch( float *_errorList, int _numSpectra, int srcSpectraIndex, int *_outBMU )
{
	__shared__ float t_err[NUMTHREADS_SEARCHGATHER];
	__shared__ int t_erridx[NUMTHREADS_SEARCHGATHER];

	int index = threadIdx.x;
	int nWidth = _numSpectra/NUMTHREADS_SEARCHGATHER;
	int nLow = nWidth*threadIdx.x;
	int nHigh = nLow+nWidth;
	if ( nHigh >= _numSpectra ) 
		nHigh = _numSpectra-1;

	float err = 3.402823466e+38F;
	int erridx;
	for ( int i=nLow;i<nHigh;i++ )
	{
		if ( _errorList[i] < err && _outBMU[i] == -1 ) 
		{
			erridx = i;
			err = _errorList[i];
		}
	}

	t_err[index] = err;
	t_erridx[index] = erridx;

	__syncthreads();

	// gather all per thread search results 
	if ( index == 0)
	{
		err = t_erridx[0];
		erridx = t_erridx[0];

		for ( int i=1;i<NUMTHREADS_SEARCHGATHER;i++ )
		{
			if ( t_err[i] < err ) 
			{
				erridx = t_erridx[i];
				err = t_err[i];
			}

		}
		_outBMU[erridx] = srcSpectraIndex;
		_outBMU[_numSpectra] = erridx;
	}
}
/*
// Reduction (min/max/avr/sum), valid only when blockDim.x is a power of two:
int  thread2;
double temp;
__shared__ double min[BLOCK_SIZE], max[BLOCK_SIZE], avg[BLOCK_SIZE], sum[BLOCK_SIZE];

int nTotalThreads = blockDim.x;	// Total number of active threads

while(nTotalThreads > 1)
{
	int halfPoint = (nTotalThreads >> 1);	// divide by two
	// only the first half of the threads will be active.

	if (threadIdx.x < halfPoint)
	{
		thread2 = threadIdx.x + halfPoint;

		// Get the shared value stored by another thread
		temp = min[thread2];
		if (temp < min[threadIdx.x]) 
			min[threadIdx.x] = temp; 

		temp = max[thread2];
		if (temp > max[threadIdx.x]) 
			max[threadIdx.x] = temp;			

		sum[threadIdx.x] += sum[thread2];

		// when calculating the average, sum and divide
		avg[threadIdx.x] += avg[thread2];
		avg[threadIdx.x] /= 2;
	}
	__syncthreads();

	// Reducing the binary tree size by two:
	nTotalThreads = halfPoint;
}
*/

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
    const int bestMatchIndex = *_outBMU;
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
  		//was: for ( int x=0;x<_gridSize;x++)

  		const float distX1 = x-xpBestMatch;
 		const float distY1 = y-ypBestMatch;
  		const float distXSqr = distX1*distX1;
 		const float distYSqr = distY1*distY1;
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



extern "C" void
process(
	const float *_sourceSpectra, 
	int _numSourceSpectra, 
	float *_networkSpectra, 
	int _numNetworkSpectra, 
	size_t *_pSpectraList, 
	float *_tempErr, 
	float *_outErr, 
	int *_outBMU, 
	int _numElements, 
	int _gridSize,
	float _adaptionThreshold, 
	float _sigmaSqr, 
	float _lRate )
{

	Timer t;

	double tSearch = 0.0;
	double tSearchCollect = 0.0;
	double tAdapt = 0.0;

	clearBMUs<<<(_numNetworkSpectra+NUMTHREADS_CLEAR-1)/NUMTHREADS_CLEAR,NUMTHREADS_CLEAR>>>( _outBMU, _numNetworkSpectra );

	cudaDeviceSynchronize();

	// for each input spectrum
	for ( int i=0;i<_numSourceSpectra;i++ ) 
	{
		int srcSpectraIndex = _pSpectraList[i];

		if ( srcSpectraIndex >= 0 || srcSpectraIndex <  _numSourceSpectra)
		{
			int o = srcSpectraIndex*_numElements;
			Timer t2;

			// calculate euclidean distances to codebook vectors
			clearErrors<<<(_numNetworkSpectra+NUMTHREADS_CLEAR-1)/NUMTHREADS_CLEAR,NUMTHREADS_CLEAR>>>( _outErr, _numNetworkSpectra );
			search<<<NUMBLOCKS_SEARCH,_numElements>>>( &_sourceSpectra[o], _networkSpectra, _tempErr, _outErr, _numNetworkSpectra, _numElements );
			cudaDeviceSynchronize();
 		
			tSearch += t2.getElapsedSecs();
			t2.start();

			// calculate best matching unit (BMU) from error list (i.e. euclidean distances)
			
  			gatherSearch<<<1,NUMTHREADS_SEARCHGATHER>>>( _outErr, _numNetworkSpectra, srcSpectraIndex, _outBMU );
 			cudaDeviceSynchronize();

			tSearchCollect += t2.getElapsedSecs();
			t2.start();
  
			// adapt code book vectors in vincinity.
 
 		    dim3 threadsPerBlock(NUMTHREADS_ADAPT, NUMTHREADS_ADAPT);
 			dim3 numBlocks((_gridSize+threadsPerBlock.x-1) / threadsPerBlock.x, (_gridSize+threadsPerBlock.y-1) / threadsPerBlock.y);
  			adaptNetwork<<<numBlocks,threadsPerBlock>>>( 
  					&_sourceSpectra[o], 
  					_networkSpectra, 
  					_numElements, 
  					&_outBMU[_numNetworkSpectra], 
  					_gridSize, 
  					_adaptionThreshold, 
  					_sigmaSqr, 
  					_lRate );
			cudaDeviceSynchronize();

			tAdapt += t2.getElapsedSecs();
		}
	}
	double dt = t.getElapsedSecs();

	printf("\n\n%f secs search\n", tSearch);
	printf("%f secs search collect\n", tSearchCollect);
	printf("%f secs adapt\n", tAdapt);
	printf("%f secs total\n\n", dt);
}
