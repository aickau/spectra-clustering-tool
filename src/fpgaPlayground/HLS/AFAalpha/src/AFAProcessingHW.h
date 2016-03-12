#ifndef AFA_PROCESSING_HW_H__
#define AFA_PROCESSING_HW_H__

#include "AFATypes.h"
#include "AFAProcessingCommon.h"

// use any large number you like
#define AFA_COMPARE_BATCH_HW 140000

// spectra cache size has direct impact on used HW resources on FPGA
#define AFA_SPECTRA_CACHE_NUMSPECTRA	( 16 ) /* COMPARE_BATCH_HW 140000 */


void resetBM_HW( BestMatch *bmu);
int getNetworkIndex_HW( int _cellX, int _cellY );

void
adaptNetwork_HW(
    volatile AFASpectra *_srcSpectrum,
    volatile AFASpectra	*spectraDataWorkingSet,
    int _bestMatchIndex,
    float _adaptionThreshold,
    float _sigmaSqr,
    float _lRate );

//void compareSpectra_HW(volatile AFASpectra *_a, volatile AFASpectra *_pB, int _nCount, float *_pOutErrors );
void
compareSpectra_HW(
    volatile AFASpectra *_a,
    volatile AFASpectra _pB[ AFA_SPECTRA_CACHE_NUMSPECTRA ],
    int _nCount,
    float *_pOutErrors );
#if 0
void
searchBestMatchComplete_HW(
    volatile AFASpectra *_src,
    volatile AFASpectra	*spectraDataWorkingSet,
    BestMatch *outbm,
    uint32_t m_gridSizeSqr,
    uint32_t spectraIndex );
#endif

void searchBestMatchLocal_HW( volatile AFASpectra *_src, const int _searchRadius, BestMatch *outbm );
void setBestMatch_HW( volatile AFASpectra *_networkSpectrum, unsigned int _networkIndex, volatile AFASpectra *_bestMatchSpectrum, unsigned int _bestMatchIndex );



#endif
