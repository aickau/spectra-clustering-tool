#ifndef AFA_PROCESSING_HW_H__
#define AFA_PROCESSING_HW_H__

#include "include/AFATypes.h"
#include "include/AFAProcessingCommon.h"

// use any large number you like
#define AFA_COMPARE_BATCH_HW 140000


void resetBM_HW( BestMatch *bmu);
int getNetworkIndex_HW( int _cellX, int _cellY );

void adaptNetwork_HW( volatile AFASpectra *_srcSpectrum, int _bestMatchIndex, float _adaptionThreshold, float _sigmaSqr, float _lRate );
void compareSpectra_HW(volatile AFASpectra *_a, volatile AFASpectra *_pB, int _nCount, float *_pOutErrors );
void searchBestMatchComplete_HW( volatile AFASpectra *_src, BestMatch *outbm );
void searchBestMatchLocal_HW( volatile AFASpectra *_src, const int _searchRadius, BestMatch *outbm );
void setBestMatch_HW( volatile AFASpectra *_networkSpectrum, unsigned int _networkIndex, volatile AFASpectra *_bestMatchSpectrum, unsigned int _bestMatchIndex );

// one learning step
// returns true if learning is finished and maximum number of learning steps are reached.
bool_t AFAProcess_HW();

#endif
