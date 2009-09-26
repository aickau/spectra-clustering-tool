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
//! \file  SOFMNetwork.h


#ifndef _SOFMNETWORK_H
#define _SOFMNETWORK_H

#include "sdsslib/spectra.h"
#include "sdsslib/random.h"

#include <fstream>


class SpectraVFS;

// Builds, processes and exports a Self Organizing Feature Map of a given training spectra data set which you want to cluster.
// See T. Kohonen. Self-organized formation of topologically correct feature maps. Biological Cybernetics, 43:-69, 1982. 
// .. or just use Google to get a hell lot of material which explain the algorithm.
//
// Good code names for a prototype:
//
// Arzamas-16
// Chelyabinsk-70 
// Krasnoyarsk-26
// Tomsk-7
// Penza-19
// Semipalatinsk-16
// Sverdlovsk-44
// Zlatoust-36
// Chita-46 
class SOFMNetwork
{
public:

	// number of columns and rows of spectra output table per HTML page.
	static const size_t s_outputPlanSize = 5;

	// fraction of global search vs. local search
	//static const size_t s_globalSearchFraction = 10;

	// search radius in cells for local search
	//static const int s_searchRadius = 4;

	// SOFM parameters
	class Parameters
	{
	public:
		Parameters( size_t _numSteps, size_t _randomSeed, float _lRateBegin, float _lRateEnd, float _radiusBegin, float _radiusEnd);

		size_t numSteps;											// number of learn steps. reasonable: 10..500
		size_t randomSeed;											// zero is not allowed
		float lRateBegin;											// learn rate begin 0.0 .. 1.0
		float lRateEnd;												// learn rate end (0.0 .. 1.0)
		float radiusBegin;											// radius begin (0.0 .. grid size)
		float radiusEnd;											// radius end (0.0 .. grid size)
		bool exportSubPage;											// if true add subpages to HTML output
		bool waitForUser;											// if true wait for user input after each calculation step
		std::string sstrSearchMode;									// search mode to control quality vs. computation time
																	// SOFMNET_SETTINGS_SEARCHMODE_global     < uses global search for BMUs, best quality, runtime is O(n²)
																	// SOFMNET_SETTINGS_SEARCHMODE_local      < uses local search for BMUs if possible.
																	// SOFMNET_SETTINGS_SEARCHMODE_localfast  < uses very fast local search for BMUs if possible, runtime is O(n) 
		Spectra::SpectraNormalization normaliziationType;			// type of normalization of spectra
		

		static Parameters defaultParameters;
	};



	// _pSourceVFS is your input data which you want to cluster
	SOFMNetwork( SpectraVFS *_pSourceVFS, bool bContinueComputation, std::ofstream *_logStream );

	~SOFMNetwork();

	// resets the network
	void reset( const Parameters &_params );

	// one learning step
	void process();

	// get spectra from SOFM at a given cell.
	// if a given cell contains a match (thus it is occupied by some input spectra) can be seen by its objectID
	// For unoccupied cells: objectID=0.
	// Also beware that the spectral data for matches is not equal to its original in the input (You always get the footprint). 
	void getSOFMSpectra( size_t _cellX, size_t _cellY, Spectra &_outSpectrum );

	SpectraVFS &getNetwork();

	// export to HTML.
	// _sstrFilename absolute or relative filename without (!) extension.
	// _fullExport if set to true generates UMatrix, DifferenceMaps and search result lists (containing top 100 similar spectra) for input each spectrum
	void exportToHTML( const std::string &_sstrFilename, bool _fullExport = true );
	
	// Calculate unified distance matrix (Ultsch 1989) of the SOM for the current calculation step. 
	// sstrFilenName path + filename (no extension)
	// _bUseLogScale use logarithmic scaling for matrix display
	// _bShowEmpty show intensities for empty cells (no input data in there), otherwise empty cells are filled with black pixels
	// _bNormalize normalize map for comparison
	void calcUMatrix( const std::string &_sstrFilenName, bool _bUseLogScale=true, bool _bShowEmpty=true, bool _bNormalize=false );

	// Calculate difference map of the SOM codebook vectors and the input data assigned to a neuron  for the current calculation step. 
	// sstrFilenName path + filename (no extension)
	// _bUseLogScale use logarithmic scaling for matrix display
	// _bNormalize normalize map for comparison
	// bOutputAsTextFile if true, we write the difference map as text file too (then no log scale).
	void calcDifferenceMap( const std::string &_sstrFilenName, bool _bUseLogScale, bool _bNormalize, bool _bOutputAsTextFile = false );

	// Calculate z map of th input data assigned to a neuron  for the current calculation step. 
	// sstrFilenName path + filename (no extension)
	// _bUseLogScale use logarithmic scaling for matrix display
	void calcZMap( const std::string &_sstrFilenName, bool _bUseLogScale );


	// grid size in cells of the map
	size_t			m_gridSize;

	// squared gridsize, number of neurons
	size_t			m_gridSizeSqr;

	// current learning step
	size_t			m_currentStep;

	Parameters		m_params;

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
		size_t index;
		float error;	
	};

	// calculate min/max values for a given SpectraVFS
	void calcMinMax( SpectraVFS &_vfs, float &_outMin, float &_outMax );

	// calculate min/max values for the input data set.
	void calcMinMaxInputDS();

	// calculate flux values for the input data set and normalize
	void calcFluxAndNormalizeInputDS( Spectra::SpectraNormalization _normalizationType );


	// render icons for input data set to disk.
	void renderIcons();

	// export an energy and z histograms for the current input data set.
	void exportHistograms( const std::string &_sstrExportDirectory );

	// plot neighborhood functions for 0%, 25%, 50%, 75% and 100% of the learning process.
	// x-axis = distance from BMU in grid cells, y-axis = network adaption
	void exportNeighbourHoodFunction( const std::string &_sstrFilenName );
	
	// write SOM cluster settings to file
	void writeSettings( const std::string &_sstrFileName );

	// read SOM cluster settings from file
	bool readSettings( const std::string &_sstrFileName, std::string &_sstrSOFMFileName );

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

	// generate HTML info pages for each source spectrum.
	// _sstrMapBaseName base name of the exported HTML maps, see exportToHTML()
	void generateHTMLInfoPages( const std::string &_sstrMapBaseName );

	static std::string spectraNormalizationToString( Spectra::SpectraNormalization _type );	 
	static Spectra::SpectraNormalization spectraNormalizationFromString( const std::string &_sstrSpectraNormalization );


	// code book spectra
	SpectraVFS		*m_pNet;	

	// training data
	SpectraVFS		*m_pSourceVFS;

	// number of source spectra
	const size_t	m_numSpectra;

	// random number generator
	Rnd				m_Random;

	// log output
	std::ofstream	*m_pLogStream;

	// average distance/error to BMU per learning step
	float			*m_pAvgDistanceToBMU;
};


#endif