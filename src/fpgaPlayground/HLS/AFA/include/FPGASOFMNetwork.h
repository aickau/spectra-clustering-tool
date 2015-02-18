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
//! \file  SOFMNetwork.h


#ifndef _FPGASOFMNETWORK_H
#define _FPGASOFMNETWORK_H

#include "../analyze/SOFMNetworkSettings.h"

#include "sdsslib/spectra.h"
#include "sdsslib/random.h"

#include <fstream>


class SpectraVFS;

// striped down version for FPGA project.
class FPGASOFMNetwork
{
public:

	// _pSourceVFS is your input data which you want to cluster
	FPGASOFMNetwork( SpectraVFS *_pSourceVFS, bool bContinueComputation );

	~FPGASOFMNetwork();

	// resets the network
	void reset( const SOFMParameters &_params );

	// one learning step
	// returns true if learning is finished and maximum number of learning steps are reached.
	bool process();

	// grid size in cells of the map
	size_t			m_gridSize;

	// squared gridsize, number of neurons
	size_t			m_gridSizeSqr;

	// current learning step
	size_t			m_currentStep;

	SOFMParameters	m_params;

	// maximum amplitude (before normalization)
	float			m_Min;

	// minimum amplitude (before normalization)
	float			m_Max;

	// maximum flux of input spectra
	float			m_flux;

protected:

	struct BestMatch
	{
		void reset();
		size_t index;	//< index in the map
		float error;	//< euclidean distance
	};

	// calculate min/max values for a given SpectraVFS
	void calcMinMax( SpectraVFS &_vfs, float &_outMin, float &_outMax );

	// calculate min/max values for the input data set.
	void calcMinMaxInputDS();

	// calculate flux values for the input data set and normalize
	void calcFluxAndNormalizeInputDS();

	// search for best matching spectrum/neuron in the network
	// this version will perform a brute-force full search in the entire network
	// _src source spectra
	// returns best spectra in the network
	BestMatch searchBestMatchComplete( const Spectra &_src );

	// search for best matching spectrum/neuron in the network using only a local window ( s_searchRadius )
	// _src source spectra
	// returns best spectra in the network
	BestMatch searchBestMatchLocal( const Spectra &_src, const int _searchRadius );

	// adapt network for a given neuron/spectrum
	// _spectrum source spectrum to adapt
	// _bestMatchIndex index into network where it best matches
	// _adaptionThreshold adaption threshold so we do not need to go through the whole network
	// _sigmaSqr sigma squared
	// _lRate current learning rate for the given processing step
	void adaptNetwork( const Spectra &_spectrum, size_t _bestMatchIndex, float _adaptionThreshold, float _sigmaSqr, float _lRate );

	// calculate index from cell positions
	size_t getIndex( size_t _cellX, size_t _cellY );




	// code book spectra
	SpectraVFS		*m_pNet;	

	// training data
	SpectraVFS		*m_pSourceVFS;

	// number of source spectra
	const size_t	m_numSpectra;

	// random number generator
	Rnd				m_Random;	
};


#endif