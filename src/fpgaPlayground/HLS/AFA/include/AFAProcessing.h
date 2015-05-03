#ifndef AFA_PROCESSING_H__
#define AFA_PROCESSING_H__

#include "AFATypes.h"
#include "AFANetworkSettings.h"
#include "AFARandom.h"

#ifndef FLT_MAX
#define FLT_MAX         3.402823466e+38F 
#endif

struct BestMatch
{
	void reset()
	{
		error = FLT_MAX;
		index = 0;
	}

	size_t index;	//< index in the map
	float error;	//< euclidean distance
};

class AFAProcessing
{
public:
    AFAProcessing(
        volatile void *spectraArray,
        volatile void *spectraArray2,  // "sofmnet.bin"
        uint32_t numSpectra,
        bool continueProcessing = false );
    ~AFAProcessing();

private:

    // resets the network
    void reset( const AFAParameters &_params );

    // one learning step
    // returns true if learning is finished and maximum number of learning steps are reached.
    bool process();

    // grid size in cells of the map
    int			    m_gridSize;

    // squared grid size, number of neurons
    int			    m_gridSizeSqr;

    // current learning step
    int			    m_currentStep;

    AFAParameters	m_params;

    // maximum amplitude (before normalization)
    float			m_Min;

    // minimum amplitude (before normalization)
    float			m_Max;

    // maximum flux of input spectra
    float			m_flux;

	// determine processing order. must be randomized every learning step
	// contains m_gridSize * m_gridSize  elements
	int				*m_pSpectraIndexList;

private:

    // calculate min/max values for a given SpectraVFS
    void calcMinMax( volatile AFASpectra *_vfs, float &_outMin, float &_outMax );

    // calculate min/max values for the input data set.
    void calcMinMaxInputDS();

    // calculate flux values for the input data set and normalize
    void calcFluxAndNormalizeInputDS();

	void initNetwork( size_t _gridSize, float _minPeak, float _maxPeak );


    // search for best matching spectrum/neuron in the network
    // this version will perform a brute-force full search in the entire network
    // _src source spectra
    // returns best spectra in the network
    BestMatch searchBestMatchComplete( volatile AFASpectra *_src );

    // search for best matching spectrum/neuron in the network using only a local window ( s_searchRadius )
    // _src source spectra
    // returns best spectra in the network
    BestMatch searchBestMatchLocal( volatile AFASpectra *_src, const int _searchRadius );

    // adapt network for a given neuron/spectrum
    // _spectrum source spectrum to adapt
    // _bestMatchIndex index into network where it best matches
    // _adaptionThreshold adaption threshold so we do not need to go through the whole network
    // _sigmaSqr sigma squared
    // _lRate current learning rate for the given processing step
    void adaptNetwork( volatile AFASpectra *_spectrum, int _bestMatchIndex, float _adaptionThreshold, float _sigmaSqr, float _lRate );

    // calculate index from cell positions
    int getIndex( int _cellX, int _cellY );

	static void compareSpectra( volatile AFASpectra *_a, volatile AFASpectra *_pB, int _nCount, float *_pOutErrors ); 


    // code book spectra
    volatile AFASpectra	*m_pNet;

    // training data
    volatile AFASpectra	*m_pSourceSpectra;

	volatile AFASpectra **m_localSearchSpectraVec;
	int *m_localSearchIndexVec;
	float *m_localSearchErrorVec;

    // number of source spectra
    const int m_numSpectra;

    // random number generator
    AFARnd m_Random;       //<<<< initialized twice!
};

#endif
