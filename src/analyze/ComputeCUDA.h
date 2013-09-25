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
//! \file  ComputeCUDA.h
//! \brief 


#ifndef _COMPUTECUDA_H
#define _COMPUTECUDA_H

#include "sdsslib/spectraVFS.h"

#include <fstream>

// SOM CUDA implementation
class ComputeCUDA
{
public:
	ComputeCUDA( SpectraVFS &_sourceSpectra, SpectraVFS &_network, std::ofstream *_logStream=NULL );
	virtual ~ComputeCUDA();

	bool hasCUDADevice() const;

	bool uploadSpectra();

	// _pSpectraList list of indices to source spectra in random order. must contain num source spectra elements
	void process( size_t *_pSpectraList, float _adaptionThreshold, float _sigmaSqr, float _lRate );

	bool downloadNetwork();

private:
	bool initCUDADevice();


	SpectraVFS &m_sourceSpectraRef;
	SpectraVFS &m_networkRef;

	// log output
	std::ofstream *m_pLogStream;

	// CUDA device buffer
	float *m_pSourceSpectraCUDA;			//< source spectra (only amplitudes) size: numsourcespectra*numpixels
	float *m_pNetworkSpectraCUDA;			//< code book vectors of the SOM, size: networksize*numpixels
	float *m_pErrorTempCUDA;				//< euclidean distances. size: networksize*block_size
	float *m_pErrorCUDA;					//< euclidean distances. size: networksize
	int *m_BMUCUDA;							//< indices with best matching units (BMU) within the network. Maps SOM cells to source spectra. Last entry reserved for temporary BMU usage. size: networksize+1
	
	// host buffers
	int *m_BMUHost;							//< indices with best matching units (BMU) within the network on host size. 

	bool m_bInit;
};

#endif