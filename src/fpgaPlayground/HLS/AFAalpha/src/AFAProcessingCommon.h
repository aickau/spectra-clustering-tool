#ifndef AFA_PROCESSING_COMMON_H__
#define AFA_PROCESSING_COMMON_H__

#include "AFARandom.h"

//#ifndef FLT_MAX
//#define FLT_MAX         3.402823466e+38F 
//#endif

typedef struct  
{
	// code book spectra
	volatile AFASpectra	*m_pNet;

	// training data
	volatile AFASpectra	*m_pSourceSpectra;

	volatile AFASpectra **m_localSearchSpectraVec;
	int *m_localSearchIndexVec;
	float *m_localSearchErrorVec;

	// number of source spectra
	int             m_numSpectra;

	// current learning step
	int			    m_currentStep;
	AFAParameters	m_params;

	// grid size in cells of the map
	int			    m_gridSize;
	// squared grid size, number of neurons
	int			    m_gridSizeSqr;

	// determine processing order. must be randomized every learning step
	// contains m_gridSize * m_gridSize  elements
	volatile int *m_pSpectraIndexList;

	// random
	unsigned long m_mt[ RANDOM_N ]; // the array for the state vector 
	int m_mti;

	int m_pStart;
	int m_pEnd;
} AFAProcessingParam_t;


#endif
