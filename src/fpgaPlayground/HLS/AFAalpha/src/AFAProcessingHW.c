#include "AFATypes.h"
#include "AFADefines.h"

#include "AFASpectraHW.h"
#include "AFANetworkSettingsHW.h"
#include "AFAProcessingHW.h"
#include "AFARandomHW.h"

#include <float.h>
#include <math.h>
#include <memory.h>

#define CSIM_JSCJSC

extern uint32_t m_mt_HW[ RANDOM_N ]; // the array for the state vector
extern int m_mti_HW;
extern int pixelStart_HW;		
extern int pixelEnd_HW;		

void
resetBM_HW(
    BestMatch *bmu )
{
    bmu->error = FLT_MAX;
    bmu->index = 0;
}

void
searchBestMatchComplete_HW(
    volatile AFASpectra *spectraDataInput,
    volatile AFASpectra *spectraDataWorkingSet,
    BestMatch *outbm,
    uint32_t m_gridSizeSqr,
    uint32_t spectraIndex )
{
    // see for the same outcome only slightly easier to understand..
    // searchBestMatchCompleteNonBatchMode()
    int j=0;
    int k;
    int i;
    float err[AFA_COMPARE_BATCH_HW];
    BestMatch bestMatch;
    volatile AFASpectra *a;
//    volatile AFASpectra *currentSourceSpectrum = &spectraDataInput[ spectraIndex ];

    resetBM_HW(&bestMatch);


    while ( j < m_gridSizeSqr )
    {
        const int jInc = AFAMIN( AFA_COMPARE_BATCH_HW, (AFAMIN(m_gridSizeSqr, j+AFA_COMPARE_BATCH_HW)-j));

        // calc euclidean distances for spectrum _src and a batch of network spectra starting at m_pNet[j] .. m_pNet[j+jInc-1]
        //a = &AFAPP_HW.m_pNet[j];
        a = &spectraDataWorkingSet[ j ];

        // hint this can run in parallel
        for (i=0;i<jInc;i++)
        {
            float error = 0.0f;

            if ( 0 == a[ i ].m_SpecObjID )
            {
                for ( k = pixelStart_HW; k < pixelEnd_HW; k++ )
                {
                    float d = ( a[ i ].m_Amplitude[ k ]) - ( spectraDataInput[ spectraIndex ].m_Amplitude[ k ]);
                    error += d * d;
                }
                err[ i ] = error;
            }
            else
            {
                err[ i ] = FLT_MAX;
            }
        }

        // find smallest error (i.e. winning neuron/ network spectrum) in batch
        for ( k = 0; k < jInc; k++ )
        {
            if ( err[ k ] < bestMatch.error )
            {
                bestMatch.error = err[ k ];
                bestMatch.index = j+k;
            }
        }

        j += jInc;
    }

    *outbm = bestMatch;
}

void
adaptNetwork_HW_old(
    volatile AFASpectra *_srcSpectrum,
    volatile AFASpectra *spectraDataWorkingSet,
    int _bestMatchIndex,
    float _adaptionThreshold,
    float _sigmaSqr,
    float _lRate,
    uint32_t m_gridSize,
    uint32_t m_gridSizeSqr )
{
    int x,y;
    float distY1, distY1Sqr, distYSqr;
    float distX1, distX1Sqr, distXSqr, distSqr;
    float hxy, lratehsx;
    unsigned int xpBestMatch = _bestMatchIndex % m_gridSize;
    unsigned int ypBestMatch = _bestMatchIndex / m_gridSize;
    float sigmaSqr2 = _sigmaSqr*(1.f/EULER);
    float fGridSizeSqr = (float)m_gridSizeSqr;
    int gridSize = (int) m_gridSize;
    unsigned int spectraAdress;
    volatile AFASpectra *a;

    // hint: this should be parallelized
    // adjust weights of the whole network
    for ( y=0;y<gridSize;y++)
    {
        distY1 = (float)y-(float)ypBestMatch;
        distY1Sqr = distY1*distY1;
        distYSqr = distY1Sqr;

        for ( x=0;x<gridSize;x++)
        {
            distX1 = (float)x-(float)xpBestMatch;
            distX1Sqr = distX1*distX1;
            distXSqr = distX1Sqr;//MIN(distX1Sqr, distX2Sqr);
            distSqr = (distXSqr+distYSqr)/fGridSizeSqr;                 // normalize squared distance with gridsize

            // calculate neighborhood function
            hxy = ( float )exp(-sqrtf(distSqr)/sigmaSqr2);                      // spike
            lratehsx = _lRate*hxy;

            if ( lratehsx > _adaptionThreshold )
            {
                spectraAdress = y*m_gridSize+x;
                a = &spectraDataWorkingSet[spectraAdress];
                AFASpectraAdapt_HW( a, _srcSpectrum, lratehsx );
            }
        }
    }
}

bool_t
AFAProcess_HW(
        uint32_t param[ 512 ],              // whole block ram used
        uint32_t mt_HW[ RANDOM_N ],         // block ram used
        volatile AFASpectra *spectraDataWorkingSet,
        volatile AFASpectra *g_spectraDataInput,
        volatile int *pSpectraIndexList
        )
{
#pragma HLS INTERFACE m_axi     port=spectraDataWorkingSet depth=10000 bundle=MASTER_BUS1
#pragma HLS INTERFACE m_axi     port=pSpectraIndexList     depth=10000 bundle=MASTER_BUS1
#pragma HLS INTERFACE m_axi     port=g_spectraDataInput    depth=10000 bundle=MASTER_BUS1
#pragma HLS INTERFACE bram      port=param                             bundle=BRAM_INTERFACE
//#pragma HLS INTERFACE s_axilite port=spectraDataWorkingSet bundle=CTRL_BUS
//#pragma HLS INTERFACE s_axilite port=pSpectraIndexList     bundle=CTRL_BUS
//#pragma HLS INTERFACE s_axilite port=g_spectraDataInput    bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=return                bundle=CTRL_BUS

    BestMatch bmu;
    volatile AFASpectra *bmuSpectrum=NULL;
    volatile AFASpectra *currentSourceSpectrum;
    volatile AFASpectra *a;
    double avgDist = 0.0;

    int i,j;
    int spectraIndex=0;
    int ind0, ind1, tmp;
    static int paramInitialized = 0;

    // get data from param block
    bool_t bFullSearch = param[ AFA_PARAM_INDICES_FULL_SEARCH ];
    unsigned int searchRadius = param[ AFA_PARAM_INDICES_SEARCH_RADIUS ];

    uint32_t adaptionThreshold_temp = param[ AFA_PARAM_INDICES_ADAPTION_THRESHOLD ];
    float adaptionThreshold = *(( float32_t * )&adaptionThreshold_temp );
    uint32_t sigmaSqr_temp = param[ AFA_PARAM_INDICES_SIGMA_SQR ];
    float sigmaSqr = *(( float32_t * )&sigmaSqr_temp );
    uint32_t lRate_temp = param[ AFA_PARAM_INDICES_LRATE ];
    float lRate = *(( float32_t * )&lRate_temp );
    uint32_t m_gridSize = param[ AFA_PARAM_INDICES_GRID_SIZE ];
    uint32_t m_gridSizeSqr = param[ AFA_PARAM_INDICES_GRID_SIZE_SQR ];
    uint32_t m_numSpectra = param[ AFA_PARAM_INDICES_NUM_SPECTRA ];

    const int spectraCacheSize = AFAMIN( AFA_SPECTRA_CACHE_NUMSPECTRA, ( AFAMIN( m_gridSizeSqr, AFA_COMPARE_BATCH_HW )));

    if ( 0 == paramInitialized )
    {
        uint32_t i;
        for ( i = 0; i < RANDOM_N; ++i )
        {
            m_mt_HW[ i ] = mt_HW[ i ];
        }
        m_mti_HW = param[ AFA_PARAM_INDICES_RNG_MTI ];

        pixelStart_HW = param[ AFA_PARAM_INDICES_PIXEL_START ];
        pixelEnd_HW   = param[ AFA_PARAM_INDICES_PIXEL_END ];

        paramInitialized = 1;
    }


    // select random spectra from spectra dataset

    // store all indicies in a list
    for ( i = 0; i < m_numSpectra; i++ )
    {
        pSpectraIndexList[ i ] = i;
    }

    // shake well
    for ( i = 0; i < m_numSpectra * 2; i++ )
    {
        ind0 = AFARandomIntRange_HW( m_numSpectra - 1 );
        ind1 = AFARandomIntRange_HW( m_numSpectra - 1 );

        // switch indices
        tmp = pSpectraIndexList[ ind0 ];
        pSpectraIndexList[ ind0 ] = pSpectraIndexList[ ind1 ];
        pSpectraIndexList[ ind1 ] = tmp;
    }

    // clear names
    for ( i = 0; i < m_gridSizeSqr; i++ )
    {
        spectraDataWorkingSet[ i ].m_SpecObjID = 0;
        spectraDataWorkingSet[ i ].m_Index = -1;
    }

    // for each training spectra..
    for ( j = 0; j < m_numSpectra; j++ )
    {
        // initialize best match batch
        resetBM_HW( &bmu );

        spectraIndex = pSpectraIndexList[j];
        currentSourceSpectrum = &g_spectraDataInput[ spectraIndex ];

        // retrieve best match neuron for a source spectra
        if (bFullSearch)
        {
            searchBestMatchComplete_HW(
                g_spectraDataInput,
                spectraDataWorkingSet,
                &bmu,
                m_gridSizeSqr,
                spectraIndex );
        }
        else
        {
//            searchBestMatchLocal_HW( currentSourceSpectrum, searchRadius, &bmu );
        }

        // mark best match neuron
        bmuSpectrum = &spectraDataWorkingSet[bmu.index];

        // set BMU in the map and source spectrum
        // _networkSpectrum artificial spectrum in the map
        // _networkIndex network index [0..gridsizesqr-1]
        // _bestMatchSpectrum source/input spectrum
        // _bestMatchIndex index to input spectrum [0..numspectra-1]
        // set best matching related info.
        bmuSpectrum->m_SpecObjID    = currentSourceSpectrum->m_SpecObjID;
        bmuSpectrum->m_Index        = spectraIndex;
        bmuSpectrum->m_version      = currentSourceSpectrum->m_version;

        // remember best match position to NW for faster search
        g_spectraDataInput[ spectraIndex ].m_Index = bmu.index;

        // adapt neighborhood
        // hint: this takes long.
        adaptNetwork_HW_old(
            currentSourceSpectrum,
            spectraDataWorkingSet,
            bmu.index,
            adaptionThreshold,
            sigmaSqr,
            lRate,
            m_gridSize,
            m_gridSizeSqr );
    }

    // clustering not yet finished, need another learning step
    return FALSE;
}
