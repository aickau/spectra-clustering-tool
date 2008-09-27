#ifndef _SOFMNETWORK_H
#define _SOFMNETWORK_H

#include "sdsslib/spectra.h"

#include <fstream>


class SpectraVFS;

// Builds, processes and exports a Self Organizing Feature Map of a given training spectra data set which you want to cluster.
// See T. Kohonen. Self-organized formation of topologically correct feature maps. Biological Cybernetics, 43:-69, 1982. 
// .. or just use Google to get a hell lot of material which explain the algorithm.
class SOFMNetwork
{
public:

	// number of columns and rows of spectra output table per html page.
	static const size_t OutputPlanSize = 5;

	// SOFM parameters
	class Parameters
	{
	public:
		Parameters( size_t _numSteps, size_t _randomSeed, float _lRateBegin, float _lRateEnd, float _radiusBegin, float _radiusEnd );
		size_t numSteps;											// number of learn steps. reasonable: 10..500
		size_t randomSeed;											// zero is not allowed
		float lRateBegin;											// learn rate begin 0.0 .. 1.0
		float lRateEnd;												// learn rate end (0.0 .. 1.0)
		float radiusBegin;											// radius begin (0.0 .. grid size)
		float radiusEnd;											// radius end (0.0 .. grid size)
		bool exportSubPage;											// if true add subpages to html output
		bool waitForUser;											// if true wait for user input after each calculation step

		static Parameters defaultParameters;
	};



	// _pSourceVFS is your input data which you want to cluster
	SOFMNetwork( SpectraVFS *_pSourceVFS, bool bContinueComputation = false );

	~SOFMNetwork();

	// resets the network
	void Reset( const Parameters &_params );

	// one learning step
	void Process();

	// get spectra from SOFM at a given cell.
	// if a given cell contains a match (thus it is occupied by some input spectra) can be seen by its objectID
	// For unoccupied cells: objectID=0.
	// Also beware that the spectral data for matches is not equal to its original in the input (You always get the footprint). 
	Spectra getSOFMSpectra( size_t _cellX, size_t _cellY );

	// export to HTML.
	// _sstrFilename absolute or relative filename without (!) extension.
	void Export( const std::string &_sstrFilename );

	size_t			m_gridSize;
	size_t			m_gridSizeSqr;
	size_t			m_currentStep;

	Parameters		m_params;

	float			m_Min;
	float			m_Max;

protected:

	struct BestMatch
	{
		size_t index;
		float error;	
	};

	void CalcMinMax();
	void RenderIcons();
	void ExportEnergyMap();
	void WriteSettings( const std::string &_sstrFileName );
	bool ReadSettings( const std::string &_sstrFileName, std::string &_sstrSOFMFileName );

	size_t getIndex( size_t _cellX, size_t _cellY );

	SpectraVFS		*m_pNet;	

	// training data
	SpectraVFS		*m_pSourceVFS;
	size_t			m_numSpectra;

	std::ofstream	m_logFile;
};


#endif