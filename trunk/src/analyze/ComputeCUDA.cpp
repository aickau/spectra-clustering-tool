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
//! \file  ComputeCUDA.cpp


#include "ComputeCUDA.h"

#include "sdsslib/helpers.h"

#include <cuda_runtime.h>


extern "C" void process(
	const float *_sourceSpectra, 
	int _numSourceSpectra, 
	float *_networkSpectra, 
	int _numNetworkSpectra, 
	size_t *_pSpectraList, 
	float *_tempErr, 
	float *outErr, 
	int *_outBMU, 
	int _numElements, 
	int _gridSize,
	float _adaptionThreshold, 
	float _sigmaSqr, 
	float _lRate );

ComputeCUDA::ComputeCUDA( SpectraVFS &_sourceSpectra, SpectraVFS &_network, std::ofstream *_logStream )
	:m_sourceSpectraRef(_sourceSpectra)
	,m_networkRef(_network)
	,m_pLogStream(_logStream)
	,m_bInit(false)
	,m_pSourceSpectraCUDA(NULL)
	,m_pNetworkSpectraCUDA(NULL)
	,m_pErrorCUDA(NULL)
	,m_pErrorTempCUDA(NULL)
	,m_BMUCUDA(NULL)
	,m_BMUHost(NULL)
{	 
	m_bInit = initCUDADevice(); 
}

ComputeCUDA::~ComputeCUDA()
{
	if ( m_bInit )
	{
		cudaError_t err = cudaSuccess;
		err = cudaFree(m_pSourceSpectraCUDA);
		err = cudaFree(m_pNetworkSpectraCUDA);
		err = cudaFree(m_pErrorTempCUDA);	
		err = cudaFree(m_pErrorCUDA);	
		err = cudaFree(m_BMUCUDA);
		delete[] m_BMUHost;
	}
}

bool ComputeCUDA::hasCUDADevice() const
{
	return m_bInit;
}


bool ComputeCUDA::initCUDADevice()
{
	Helpers::print("Checking for CUDA devices..\n", m_pLogStream);

	int deviceCount = 0;
	cudaError_t errorID = cudaGetDeviceCount(&deviceCount);

	if (errorID != cudaSuccess )
	{
		Helpers::print("Error, "+std::string(cudaGetErrorString(errorID)), m_pLogStream );
	}
		
	if ( deviceCount > 0 )
	{
		Helpers::print(Helpers::numberToString<int>(deviceCount)+" CUDA devices found.\n", m_pLogStream);

		cudaDeviceProp deviceProp;
		int driverVersion = 0, runtimeVersion = 0;

		cudaSetDevice(0);
		cudaGetDeviceProperties( &deviceProp, 0 );
		cudaDriverGetVersion( &driverVersion );
		cudaRuntimeGetVersion( &runtimeVersion );

		Helpers::print("   CUDA Driver Version / Runtime Version     :   "+Helpers::numberToString<int>(driverVersion/1000)+"."+Helpers::numberToString<int>((driverVersion%100)/10)+ " / "+ Helpers::numberToString<int>(runtimeVersion/1000)+"."+Helpers::numberToString<int>((runtimeVersion%100)/10)+"\n", m_pLogStream);
		Helpers::print("   CUDA Capability Major/Minor version number:   "+Helpers::numberToString<int>(deviceProp.major)+"."+Helpers::numberToString<int>(deviceProp.minor)+"\n" );
		Helpers::print("   Available memory (MB)                     :   "+Helpers::numberToString<size_t>(deviceProp.totalGlobalMem/(1024*1024))+"\n");

		return true;
	}
	Helpers::print("No CUDA devices found.\n", m_pLogStream);

	return false;
}

bool ComputeCUDA::uploadSpectra()
{
	if (!m_bInit)
		return false;

	cudaError_t err = cudaSuccess;

	const size_t numSourceSpectra = m_sourceSpectraRef.getNumSpectra();
	const size_t numNetworkSpectra = m_networkRef.getNumSpectra();
	const size_t numElements = Spectra::pixelEnd-Spectra::pixelStart;
	const size_t numBytesPerSpectrum = numElements*sizeof(float);

	if ( numSourceSpectra == 0 || numNetworkSpectra == 0 || numElements == 0 )
		return false;

	if ( m_pNetworkSpectraCUDA == NULL )
	{
		err = cudaMalloc((void **)&m_pNetworkSpectraCUDA, numNetworkSpectra*numBytesPerSpectrum);
		if ( err != cudaSuccess)
			return false;
	}

	if ( m_pSourceSpectraCUDA == NULL )
	{
		err = cudaMalloc((void **)&m_pSourceSpectraCUDA, numSourceSpectra*numBytesPerSpectrum );
		if ( err != cudaSuccess)
			return false;
	}


	if ( m_pErrorCUDA == NULL )
	{
		err = cudaMalloc((void **)&m_pErrorCUDA, numNetworkSpectra*sizeof(float));
		if ( err != cudaSuccess)
			return false;
	}

	if ( m_pErrorTempCUDA == NULL )
	{
		err = cudaMalloc((void **)&m_pErrorTempCUDA, numElements*numNetworkSpectra*sizeof(float));
		if ( err != cudaSuccess)
			return false;
	}

	if ( m_BMUCUDA == NULL )
	{
		err = cudaMalloc((void **)&m_BMUCUDA, (numNetworkSpectra+1)*sizeof(int));
		if ( err != cudaSuccess)
			return false;
	}

	if ( m_BMUHost == NULL )
	{
		m_BMUHost = new int[numNetworkSpectra];
	}

	// copy SRC spectra
	size_t o=0;
	for ( size_t i=0;i<numSourceSpectra;i++ )
	{
		Spectra *sp = m_sourceSpectraRef.beginRead(i);
		err = cudaMemcpy( &m_pSourceSpectraCUDA[o], &sp->m_Amplitude[Spectra::pixelStart], numBytesPerSpectrum, cudaMemcpyHostToDevice);
		assert(err==cudaSuccess);
		o += numElements;
		m_sourceSpectraRef.endRead(i);
	}

	// copy network spectra
	o=0;
	for ( size_t i=0;i<numNetworkSpectra;i++ )
	{
		Spectra *sp = m_networkRef.beginRead(i);
		err = cudaMemcpy( &m_pNetworkSpectraCUDA[o], &sp->m_Amplitude[Spectra::pixelStart], numBytesPerSpectrum, cudaMemcpyHostToDevice);
		assert(err==cudaSuccess);
		o += numElements;
		m_networkRef.endRead(i);
	}
	
	return true;
}

bool ComputeCUDA::downloadNetwork()
{
	if (!m_bInit)
		return false;

	cudaError_t err = cudaSuccess;

	const size_t numSourceSpectra = m_sourceSpectraRef.getNumSpectra();
	const size_t numNetworkSpectra = m_networkRef.getNumSpectra();
	const size_t numElements = Spectra::pixelEnd-Spectra::pixelStart;
	const size_t numBytesPerSpectrum = numElements*sizeof(float);

	if ( numNetworkSpectra == 0 || numElements == 0 )
		return false;

	err = cudaMemcpy(m_BMUHost, m_BMUCUDA, numNetworkSpectra*sizeof(int), cudaMemcpyDeviceToHost);
	assert(err==cudaSuccess);

	size_t o=0;
	for ( size_t i=0;i<numNetworkSpectra;i++ )
	{
		Spectra *sp = m_networkRef.beginWrite(i);
		err = cudaMemcpy(&sp->m_Amplitude[Spectra::pixelStart], &m_pNetworkSpectraCUDA[o], numBytesPerSpectrum, cudaMemcpyDeviceToHost);
		assert(err==cudaSuccess);
		o += numElements;

		// copy BMU into network, set best match
		sp->m_Index	= m_BMUHost[i];
		
		if ( sp->m_Index >= 0 && sp->m_Index < numSourceSpectra )
		{
			Spectra *spSRC = m_sourceSpectraRef.beginRead( sp->m_Index );

			sp->m_SpecObjID	= spSRC->m_SpecObjID;
			sp->m_version = spSRC->m_version;
			m_sourceSpectraRef.endRead( sp->m_Index );
		}
		else
		{
			sp->m_SpecObjID = 0;
		}
		m_networkRef.endWrite(i);
	}


	return true;
}


void ComputeCUDA::process( size_t *_pSpectraList, float _adaptionThreshold, float _sigmaSqr, float _lRate )
{
	if (!m_bInit)
		return;

	const size_t numSourceSpectra = m_sourceSpectraRef.getNumSpectra();
	const size_t numNetworkSpectra = m_networkRef.getNumSpectra();
	const size_t numElements = Spectra::pixelEnd-Spectra::pixelStart;
	const size_t numBytesPerSpectrum = numElements*sizeof(float);
	const size_t gridSize = sqrtf(numNetworkSpectra);

	::process(
		m_pSourceSpectraCUDA, 
		numSourceSpectra, 
		m_pNetworkSpectraCUDA, 
		numNetworkSpectra, 
		_pSpectraList,
		m_pErrorTempCUDA,
		m_pErrorCUDA, 
		m_BMUCUDA, 
		numElements, 
		gridSize,
		_adaptionThreshold, 
		_sigmaSqr, 
		_lRate );

}
