#include "AFATypes.h"
#include "AFADefines.h"

#include "AFASpectraHW.h"
#include "AFANetworkSettingsHW.h"
#include "AFAProcessingHW.h"
#include "AFARandomHW.h"

#include <float.h>
#include <math.h>
#include <string.h>

extern uint32_t m_mt_HW[ RANDOM_N ]; // the array for the state vector
extern uint32_t m_mti_HW;

void
searchBestMatchComplete_HW(
    volatile uint32_t *baseAddr, 		// default starting address in memory
    uint64_t spectraDataInputHW_IndexToMem,
    uint64_t spectraDataWorkingSetHW_IndexToMem,
    BestMatch *outbm,
    uint32_t m_gridSizeSqr,
    uint32_t spectraIndex )
{
    // see for the same outcome only slightly easier to understand..
    // searchBestMatchCompleteNonBatchMode()
    uint32_t j = 0;
    uint32_t k;
    uint32_t i;
    float32_t err[ AFA_COMPARE_BATCH_HW ];
    BestMatch bestMatch;

    //resetBM_HW(&bestMatch);
    bestMatch.error = FLT_MAX;
    bestMatch.index = 0;

    while ( j < m_gridSizeSqr )
    {
        const uint32_t jInc = AFAMIN( AFA_COMPARE_BATCH_HW, ( AFAMIN( m_gridSizeSqr, j + AFA_COMPARE_BATCH_HW ) - j ));

        // calculate euclidean distances for spectrum _src and a batch of network spectra starting at m_pNet[j] .. m_pNet[j+jInc-1]
        //a = &AFAPP_HW.m_pNet[j];
        //a = &spectraDataWorkingSet[ j * AFA_SPECTRA_INDEX_SIZE_IN_FLOAT32 ];

        // hint this can run in parallel
        for ( i = 0; i < jInc; i++ )
        {
            float32_t error = 0.0f;

            uint32_t objIDLow  = baseAddr[ spectraDataWorkingSetHW_IndexToMem + ( j + i ) * AFA_SPECTRA_INDEX_SIZE_IN_UINT32 + AFA_SPECTRA_INDEX_SPEC_OBJ_ID_LOW  ];
            uint32_t objIDHigh = baseAddr[ spectraDataWorkingSetHW_IndexToMem + ( j + i ) * AFA_SPECTRA_INDEX_SIZE_IN_UINT32 + AFA_SPECTRA_INDEX_SPEC_OBJ_ID_HIGH ];

            if ( objIDLow == 0 && objIDHigh == 0 )
            {
                for ( k = AFA_SPECTRA_INDEX_AMPLITUDE; k < AFA_SPECTRA_INDEX_AMPLITUDE + AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW; k++ )
                {
                    uint32_t tmpVal1 = baseAddr[ spectraDataInputHW_IndexToMem + spectraIndex * AFA_SPECTRA_INDEX_SIZE_IN_UINT32 + k ];
                    uint32_t tmpVal2 = baseAddr[ spectraDataWorkingSetHW_IndexToMem + ( j + i ) * AFA_SPECTRA_INDEX_SIZE_IN_UINT32 + k ];
                    float32_t tmpVal1Float = *(( float32_t * ) &tmpVal1 );
                    float32_t tmpVal2Float = *(( float32_t * ) &tmpVal2 );
                    float32_t d = ( tmpVal2Float - tmpVal1Float );
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
                bestMatch.index = j + k;
            }
        }

        j += jInc;
    }

    *outbm = bestMatch;
}

void
adaptNetwork_HW_integrated(
    volatile uint32_t *baseAddr, 		// default starting address in memory
    uint64_t currentSourceSpectrumIndex,
    uint64_t spectraDataWorkingSetIndex,
    sint32_t bestMatchIndex,
    float32_t adaptionThreshold,
    float32_t sigmaSqr,
    float32_t lRate,
    uint32_t gridSize,
    uint32_t gridSizeSqr )
{
    uint32_t x,y,i;
    float32_t distY1, distY1Sqr, distYSqr;
    float32_t distX1, distX1Sqr, distXSqr, distSqr;
    float32_t hxy, lratehsx;
    uint32_t xpBestMatch = bestMatchIndex % gridSize;
    uint32_t ypBestMatch = bestMatchIndex / gridSize;
    float32_t sigmaSqr2 = sigmaSqr * ( 1.f / AFA_CONST_EULER );
    float32_t fGridSizeSqr = ( float32_t )gridSizeSqr;
    uint64_t spectraAdress;

    // hint: this should be parallelized
    // adjust weights of the whole network
    for ( y = 0; y < gridSize; y++ )
    {
        distY1 = ( float32_t )y - ( float32_t )ypBestMatch;
        distY1Sqr = distY1 * distY1;
        distYSqr = distY1Sqr;

        for ( x = 0; x < gridSize; x++ )
        {
            distX1 = ( float32_t )x - ( float32_t )xpBestMatch;
            distX1Sqr = distX1 * distX1;
            distXSqr = distX1Sqr;//MIN(distX1Sqr, distX2Sqr);

            // JSC 2016-03-27: may not totally correct as fGridSizeSqr is the square of side length
            // whereas ( distXSqr + distYSqr ) may be twice squared side length, ==> [0..2] not [0..1]
            distSqr = ( distXSqr + distYSqr ) / fGridSizeSqr;                 // normalize squared distance with gridsize

            // calculate neighborhood function
            hxy = expf( -sqrtf( distSqr ) / sigmaSqr2 );              // spike
            lratehsx = lRate * hxy;

            if ( lratehsx > adaptionThreshold )
            {
                spectraAdress = ( y * gridSize + x ) * AFA_SPECTRA_INDEX_SIZE_IN_UINT32;

                for ( i = AFA_SPECTRA_INDEX_AMPLITUDE; i < AFA_SPECTRA_INDEX_AMPLITUDE + AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW; i++ )
                {
                    uint32_t aTmp = baseAddr[ spectraDataWorkingSetIndex + spectraAdress + i ];
                    uint32_t cTmp = baseAddr[ currentSourceSpectrumIndex + i ];
                    float32_t aTmpFloat = *(( float32_t * ) &aTmp );
                    float32_t cTmpFloat = *(( float32_t * ) &cTmp );

                    aTmpFloat += lratehsx * ( cTmpFloat - aTmpFloat );

                    aTmp = *(( uint32_t * ) &aTmpFloat );

                    baseAddr[ spectraDataWorkingSetIndex + spectraAdress + i ] = aTmp;
                }
            }
        }
    }
}

bool_t
AFAProcess_HW(
    uint32_t param[ 256 ],              // whole block ram used
    uint32_t mt_HW[ RANDOM_N ],         // block ram used
    volatile uint32_t *baseAddr 		// default starting address in memory
    //volatile AFASpectra *spectraDataWorkingSet,
    //volatile AFASpectra *g_spectraDataInput,
    //volatile sint32_t *pSpectraIndexList
    )
{
#pragma HLS RESOURCE            variable=param             core=RAM_1P_BRAM
#pragma HLS INTERFACE bram      port=param
#pragma HLS INTERFACE bram      port=mt_HW
#pragma HLS INTERFACE m_axi     port=baseAddr              depth=10000
#pragma HLS INTERFACE s_axilite port=baseAddr                          bundle=INTERFACE_AXILITE_SLAVE
#pragma HLS INTERFACE s_axilite port=return                            bundle=INTERFACE_AXILITE_SLAVE

    BestMatch bmu;

    uint32_t i,j;
    sint32_t ind0, ind1, tmp;

    sint32_t spectraIndex = 0;

    static bool_t paramNotInitialized = TRUE;

    // get data from param block
    // =========================
    bool_t bFullSearch = param[ AFA_PARAM_INDICES_FULL_SEARCH ];
    uint32_t adaptionThreshold_temp = param[ AFA_PARAM_INDICES_ADAPTION_THRESHOLD ];
    float adaptionThreshold = *(( float32_t * )&adaptionThreshold_temp );
    uint32_t sigmaSqr_temp = param[ AFA_PARAM_INDICES_SIGMA_SQR ];
    float sigmaSqr = *(( float32_t * )&sigmaSqr_temp );
    uint32_t lRate_temp = param[ AFA_PARAM_INDICES_LRATE ];
    float lRate = *(( float32_t * )&lRate_temp );
    uint32_t m_gridSize = param[ AFA_PARAM_INDICES_GRID_SIZE ];
    uint32_t m_gridSizeSqr = param[ AFA_PARAM_INDICES_GRID_SIZE_SQR ];
    uint32_t m_numSpectra = param[ AFA_PARAM_INDICES_NUM_SPECTRA ];
    //uint64_t g_spectraDataInputIndexToMem;  // in future obsolete
    uint64_t g_spectraDataInputHWIndexToMem;
//    uint64_t spectraDataWorkingSetIndexToMem;
    uint64_t spectraDataWorkingSetHWIndexToMem;
    uint64_t pSpectraIndexListIndexToMem;
#ifdef LOCAL_SEARCH_ENABLED
    const int spectraCacheSize = AFAMIN( AFA_SPECTRA_CACHE_NUMSPECTRA, ( AFAMIN( m_gridSizeSqr, AFA_COMPARE_BATCH_HW )));
    const sint32_t AFAConstantM1 = -1;
    const uint32_t searchRadius = param[ AFA_PARAM_INDICES_SEARCH_RADIUS ];
    const float32_t AFAConstantM1F = -1.0f;
    const uint32_t AFAConstant0  = 0;
#endif

    // get offsets to different memory locations as 2 32-bit words combining them to 64bit offsets
//    g_spectraDataInputIndexToMem    = ( param[ AFA_PARAM_INDICES_SPECTRA_DATA_INPUT_ADDR_LOW      ] | (( uint64_t ) param[ AFA_PARAM_INDICES_SPECTRA_DATA_INPUT_ADDR_HIGH      ] << 32 ));
    g_spectraDataInputHWIndexToMem    = ( param[ AFA_PARAM_INDICES_SPECTRA_DATA_INPUT_HW_ADDR_LOW   ] | (( uint64_t ) param[ AFA_PARAM_INDICES_SPECTRA_DATA_INPUT_HW_ADDR_HIGH   ] << 32 ));
//    spectraDataWorkingSetIndexToMem   = ( param[ AFA_PARAM_INDICES_SPECTRA_DATA_WS_ADDR_LOW         ] | (( uint64_t ) param[ AFA_PARAM_INDICES_SPECTRA_DATA_WS_ADDR_HIGH         ] << 32 ));
    spectraDataWorkingSetHWIndexToMem = ( param[ AFA_PARAM_INDICES_SPECTRA_DATA_WS_HW_ADDR_LOW      ] | (( uint64_t ) param[ AFA_PARAM_INDICES_SPECTRA_DATA_WS_ADDR_HIGH         ] << 32 ));
    pSpectraIndexListIndexToMem       = ( param[ AFA_PARAM_INDICES_SPECTRA_DATA_INDEX_LIST_ADDR_LOW ] | (( uint64_t ) param[ AFA_PARAM_INDICES_SPECTRA_DATA_INDEX_LIST_ADDR_HIGH ] << 32 ));

    // these offsets are used as array indices into an uint32_t array. So divide them up by the byte width
//    g_spectraDataInputIndexToMem    /= sizeof( uint32_t );
    g_spectraDataInputHWIndexToMem    /= sizeof( uint32_t );
//    spectraDataWorkingSetIndexToMem   /= sizeof( uint32_t );
    spectraDataWorkingSetHWIndexToMem /= sizeof( uint32_t );
    pSpectraIndexListIndexToMem       /= sizeof( uint32_t );

    if ( paramNotInitialized )
    {
        for ( i = 0; i < RANDOM_N; ++i )
        {
            m_mt_HW[ i ] = mt_HW[ i ];
        }
        m_mti_HW = param[ AFA_PARAM_INDICES_RNG_MTI ];

        paramNotInitialized = FALSE;
    }

    // select random spectra from spectra dataset

    // store all indices in a list
    for ( i = 0; i < m_numSpectra; i++ )
    {
        baseAddr[ pSpectraIndexListIndexToMem + i ] = i;
    }

    // shake well
    for ( i = 0; i < m_numSpectra * 2; i++ )
    {
        ind0 = AFARandomIntRange_HW( m_numSpectra - 1 );
        ind1 = AFARandomIntRange_HW( m_numSpectra - 1 );

        // switch indices
        tmp = baseAddr[ pSpectraIndexListIndexToMem + ind0 ];
        baseAddr[ pSpectraIndexListIndexToMem + ind0 ] = baseAddr[ pSpectraIndexListIndexToMem + ind1 ];
        baseAddr[ pSpectraIndexListIndexToMem + ind1 ] = tmp;
    }

    // clear names
    for ( i = 0; i < m_gridSizeSqr * AFA_SPECTRA_INDEX_SIZE_IN_UINT32; i += AFA_SPECTRA_INDEX_SIZE_IN_UINT32 )
    {
        baseAddr[ spectraDataWorkingSetHWIndexToMem + i + AFA_SPECTRA_INDEX_SPEC_OBJ_ID_LOW  ] = 0;
        baseAddr[ spectraDataWorkingSetHWIndexToMem + i + AFA_SPECTRA_INDEX_SPEC_OBJ_ID_HIGH ] = 0;
        baseAddr[ spectraDataWorkingSetHWIndexToMem + i + AFA_SPECTRA_INDEX_INDEX            ] = ( uint32_t ) ( -1 );
    }

    // for each training spectra..
    for ( j = 0; j < m_numSpectra; j++ )
    {
        // initialize best match batch
        bmu.error = FLT_MAX;
        bmu.index = 0;

        spectraIndex = baseAddr[ pSpectraIndexListIndexToMem + j ];

        // retrieve best match neuron for a source spectra
        if ( bFullSearch )
        {
            searchBestMatchComplete_HW(
                baseAddr, 		// default starting address in memory
                g_spectraDataInputHWIndexToMem,
                spectraDataWorkingSetHWIndexToMem,
                &bmu,
                m_gridSizeSqr,
                spectraIndex );
        }
        else
        {
#ifdef LOCAL_SEARCH_ENABLED
            searchBestMatchLocal_HW( currentSourceSpectrum, searchRadius, &bmu );
#endif
        }
        // mark best match neuron

        // set BMU in the map and source spectrum
        // _networkSpectrum artificial spectrum in the map
        // _networkIndex network index [0..gridsizesqr-1]
        // _bestMatchSpectrum source/input spectrum
        // _bestMatchIndex index to input spectrum [0..numspectra-1]
        // set best matching related info.
        {
            uint64_t bmuSpectrumIndex = spectraDataWorkingSetHWIndexToMem + bmu.index * AFA_SPECTRA_INDEX_SIZE_IN_UINT32;
            baseAddr[ bmuSpectrumIndex + AFA_SPECTRA_INDEX_SPEC_OBJ_ID_LOW ] = baseAddr[ g_spectraDataInputHWIndexToMem + spectraIndex * AFA_SPECTRA_INDEX_SIZE_IN_UINT32 + AFA_SPECTRA_INDEX_SPEC_OBJ_ID_LOW ];
            baseAddr[ bmuSpectrumIndex + AFA_SPECTRA_INDEX_SPEC_OBJ_ID_HIGH ] = baseAddr[ g_spectraDataInputHWIndexToMem + spectraIndex * AFA_SPECTRA_INDEX_SIZE_IN_UINT32 + AFA_SPECTRA_INDEX_SPEC_OBJ_ID_HIGH ];
            baseAddr[ bmuSpectrumIndex + AFA_SPECTRA_INDEX_INDEX ] = spectraIndex;

            // remember best match position to NW for faster search
            baseAddr[ g_spectraDataInputHWIndexToMem + spectraIndex * AFA_SPECTRA_INDEX_SIZE_IN_UINT32 + AFA_SPECTRA_INDEX_INDEX ] = bmu.index;
        }

        // adapt neighborhood
        // hint: this takes long.
        adaptNetwork_HW_integrated(
            baseAddr, 		// default starting address in memory
            g_spectraDataInputHWIndexToMem + spectraIndex * AFA_SPECTRA_INDEX_SIZE_IN_UINT32, // this is the "currentSourceSpectrumIndex"
            spectraDataWorkingSetHWIndexToMem, // this is the spectraDataWorkingSetHWIndexToMem
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
