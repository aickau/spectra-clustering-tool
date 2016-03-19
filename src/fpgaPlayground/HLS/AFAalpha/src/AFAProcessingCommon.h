#ifndef AFA_PROCESSING_COMMON_H__
#define AFA_PROCESSING_COMMON_H__

#include "AFARandom.h"

enum
{
	AFA_PARAM_INDICES_RESERVED          = 0,
	AFA_PARAM_INDICES_SEARCH_RADIUS,
	AFA_PARAM_INDICES_FULL_SEARCH,
	AFA_PARAM_INDICES_ADAPTION_THRESHOLD,
	AFA_PARAM_INDICES_SIGMA_SQR,
	AFA_PARAM_INDICES_LRATE,
    AFA_PARAM_INDICES_GRID_SIZE,
    AFA_PARAM_INDICES_GRID_SIZE_SQR,
	AFA_PARAM_INDICES_NUM_SPECTRA,
	AFA_PARAM_INDICES_RNG_MTI,
	AFA_PARAM_INDICES_PIXEL_START,
	AFA_PARAM_INDICES_PIXEL_END
};

typedef struct
{
    // code book spectra
    volatile AFASpectra	*spectraDataWorkingSet;

    // training data
    volatile AFASpectra	*g_spectraDataInput;

    volatile AFASpectra **m_localSearchSpectraVec;
    volatile sint32_t *m_localSearchIndexVec;
    volatile float32_t *m_localSearchErrorVec;

    // determine processing order. must be randomized every learning step
    // contains m_gridSize * m_gridSize  elements
    volatile sint32_t *m_pSpectraIndexList;

    // number of source spectra
    uint32_t        m_numSpectra;

    // current learning step
    uint32_t			    m_currentStep;
    AFAParameters	m_params;

    // grid size in cells of the map
    uint32_t	    m_gridSize;
    // squared grid size, number of neurons
    uint32_t	    m_gridSizeSqr;

    // random
    unsigned long m_mt[ RANDOM_N ]; // the array for the state vector 
    int m_mti;

    int m_pStart;
    int m_pEnd;
} AFAProcessingParamSW_t;

typedef struct
{
    // code book spectra
//    volatile AFASpectra	*spectraDataWorkingSet;

    // training data
    //volatile AFASpectra	*g_spectraDataInput;

    //volatile AFASpectra **m_localSearchSpectraVec;
    //volatile int *m_localSearchIndexVec;
    //volatile float *m_localSearchErrorVec;

    // determine processing order. must be randomized every learning step
    // contains m_gridSize * m_gridSize  elements
    //volatile int *m_pSpectraIndexList;

    // number of source spectra
    uint32_t        m_numSpectra;

    AFAParameters	m_params;

    // grid size in cells of the map
    uint32_t	    m_gridSize;
    // squared grid size, number of neurons
    uint32_t	    m_gridSizeSqr;

    // random
    uint32_t m_mt[ RANDOM_N ]; // the array for the state vector
    int m_mti;

    int m_pStart;
    int m_pEnd;
} AFAProcessingParamHW_t;

// one learning step
// returns true if learning is finished and maximum number of learning steps are reached.
bool_t
AFAProcess_HW(
	uint32_t param[ 256 ],				// whole block ram used
    uint32_t mt[ RANDOM_N ],			// whole block ram used
	volatile AFASpectra	*spectraDataWorkingSet,
	volatile AFASpectra *spectraDataInput,
	volatile sint32_t *m_pSpectraIndexList
    );

#endif
