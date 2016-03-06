#ifndef AFA_PROCESSING_COMMON_H__
#define AFA_PROCESSING_COMMON_H__

#include "AFARandom.h"

//#ifndef FLT_MAX
//#define FLT_MAX         3.402823466e+38F 
//#endif

enum
{
	AFA_PARAM_INDICES_RESERVED          = 0,
	AFA_PARAM_INDICES_SEARCH_RADIUS,
	AFA_PARAM_INDICES_FULL_SEARCH,
	AFA_PARAM_INDICES_ADAPTION_THRESHOLD,
	AFA_PARAM_INDICES_SIGMA_SQR,
	AFA_PARAM_INDICES_LRATE
};

typedef struct
{
    // code book spectra
    volatile AFASpectra	*spectraDataWorkingSet;

    // training data
    volatile AFASpectra	*g_spectraDataInput;

    volatile AFASpectra **m_localSearchSpectraVec;
    volatile int *m_localSearchIndexVec;
    volatile float *m_localSearchErrorVec;

    // determine processing order. must be randomized every learning step
    // contains m_gridSize * m_gridSize  elements
    volatile int *m_pSpectraIndexList;

    // number of source spectra
    int             m_numSpectra;

    // current learning step
    int			    m_currentStep;
    AFAParameters	m_params;

    // grid size in cells of the map
    int			    m_gridSize;
    // squared grid size, number of neurons
    int			    m_gridSizeSqr;

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
    int             m_numSpectra;

    AFAParameters	m_params;

    // grid size in cells of the map
    int			    m_gridSize;
    // squared grid size, number of neurons
    int			    m_gridSizeSqr;

    // random
    unsigned long m_mt[ RANDOM_N ]; // the array for the state vector 
    int m_mti;

    int m_pStart;
    int m_pEnd;
} AFAProcessingParamHW_t;

// one learning step
// returns true if learning is finished and maximum number of learning steps are reached.
bool_t
AFAProcess_HW(
	uint32_t param[ 512 ],				// whole block ram used
	volatile AFASpectra	*spectraDataWorkingSet,
	volatile int *m_pSpectraIndexList,
	volatile AFASpectra	*g_spectraDataInput
    );

#endif
