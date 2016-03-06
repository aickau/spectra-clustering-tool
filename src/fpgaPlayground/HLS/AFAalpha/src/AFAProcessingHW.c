#include "AFATypes.h"
#include "AFADefines.h"

#include "AFASpectraHW.h"
#include "AFANetworkSettingsHW.h"
#include "AFAProcessingHW.h"
#include "AFARandomHW.h"

#include <float.h>
#include <math.h>
#include <memory.h>

//#define JSC_SETBEST_MATCH_HW_INLINE

#if 0
 // temp only
#include <stdio.h>
FILE *f;
#endif

AFAProcessingParamHW_t	AFAPP_hw;

void resetBM_HW( BestMatch *bmu)
{
    bmu->error = FLT_MAX;
    bmu->index = 0;
}

void
adaptNetwork_HW(
    volatile AFASpectra *_srcSpectrum,
    volatile AFASpectra	*spectraDataWorkingSet,
    int _bestMatchIndex,
    float _adaptionThreshold,
    float _sigmaSqr,
    float _lRate )
{
    int x,y;
    float distY1, distY1Sqr, distYSqr;
    float distX1, distX1Sqr, distXSqr, distSqr;
    float hxy, lratehsx;
    unsigned int xpBestMatch = _bestMatchIndex % AFAPP_hw.m_gridSize;
    unsigned int ypBestMatch = _bestMatchIndex / AFAPP_hw.m_gridSize;
    float sigmaSqr2 = _sigmaSqr*(1.f/EULER);
    float fGridSizeSqr = (float)AFAPP_hw.m_gridSizeSqr;
    int gridSize = (int) AFAPP_hw.m_gridSize;
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
            distSqr = (distXSqr+distYSqr)/fGridSizeSqr;					// normalize squared distance with gridsize

            // calculate neighborhood function
            hxy = ( float )exp(-sqrtf(distSqr)/sigmaSqr2);						// spike
            lratehsx = _lRate*hxy;

            if ( lratehsx > _adaptionThreshold )
            {
                spectraAdress = y*AFAPP_hw.m_gridSize+x;
                a = &spectraDataWorkingSet[spectraAdress];
                AFASpectraAdapt_HW( a, _srcSpectrum, lratehsx );
            }
        }
    }
}

#if 0
void compareSpectra_HW(volatile AFASpectra *_a, volatile AFASpectra *_pB, int _nCount, float *_pOutErrors )
{
    //assert( _pB != NULL );
    //assert( _pOutErrors != NULL );
    int i;
    int numElements = (int)_nCount;

    // hint this can run in parallel
    for (i=0;i<numElements;i++)
    {
        if ( AFASpectraIsEmpty_HW( &_pB[i] ) )
        {
            _pOutErrors[i] = AFASpectraCompare_HW( _a, &_pB[i] ); // compares single spectral lines
        }
        else
        {
            _pOutErrors[i] = FLT_MAX;
        }
    }
}
#endif

void compareSpectra_HW( AFASpectra *_a, AFASpectra _pB[ AFA_SPECTRA_CACHE_NUMSPECTRA ], int _nCount, float *_pOutErrors )
{
    //assert( _pB != NULL );
    //assert( _pOutErrors != NULL );
    int i;
    int numElements = (int)_nCount;

    // hint this can run in parallel
    for (i=0;i<numElements;i++)
    {
        if ( AFASpectraIsEmpty_HW( &_pB[i] ) )
        {
            _pOutErrors[i] = AFASpectraCompare_HW( _a, &_pB[i] ); // compares single spectral lines
        }
        else
        {
            _pOutErrors[i] = FLT_MAX;
        }
    }
}

void
searchBestMatchComplete_HW(
    volatile AFASpectra *_src,
    volatile AFASpectra	*spectraDataWorkingSet,
    BestMatch *outbm )
{
    // see for the same outcome only slightly easier to understand..
    // searchBestMatchCompleteNonBatchMode()
    int j=0;
    int k;
    float err[AFA_COMPARE_BATCH_HW];
    BestMatch bestMatch;
    volatile AFASpectra *a;
    AFASpectra localSpectrumSrc1;
    AFASpectra localSpectrumSrc2[ AFA_SPECTRA_CACHE_NUMSPECTRA ];

    resetBM_HW(&bestMatch);

    // generate a local copy for spectrum #1
    memcpy( &localSpectrumSrc1, ( const void * ) _src, sizeof( AFASpectra ));

    while (j<AFAPP_hw.m_gridSizeSqr)
    {
        const int jInc = AFAMIN( AFA_SPECTRA_CACHE_NUMSPECTRA, (AFAMIN(AFAPP_hw.m_gridSizeSqr, j+AFA_SPECTRA_CACHE_NUMSPECTRA)-j));

        // calc euclidean distances for spectrum _src and a batch of network spectra starting at m_pNet[j] .. m_pNet[j+jInc-1]
        a = &spectraDataWorkingSet[ j ];

        // generate a local copy within spectrum #2
        // it contains jInc number of single spectra
        memcpy( &localSpectrumSrc2[ 0 ], ( const void * ) a, jInc * sizeof( AFASpectra ));

        compareSpectra_HW( &localSpectrumSrc1, &localSpectrumSrc2[ 0 ], jInc, err );
//		compareSpectra_HW( _src, a, jInc, err );

        // find smallest error (i.e. winning neuron/ network spectrum) in batch
        for ( k=0;k<jInc;k++ )
        {
            if (err[k] < bestMatch.error )
            {
                bestMatch.error = err[k];
                bestMatch.index = j+k;
            }
        }

        j += AFA_SPECTRA_CACHE_NUMSPECTRA;
    }

    *outbm = bestMatch;
}

int getNetworkIndex_HW( int _cellX, int _cellY )
{
    return _cellX+_cellY*AFAPP_hw.m_gridSize;
}

#ifndef JSC_SETBEST_MATCH_HW_INLINE
// set BMU in the map and source spectrum
// _networkSpectrum artificial spectrum in the map
// _networkIndex network index [0..gridsizesqr-1]
// _bestMatchSpectrum source/input spectrum
// _bestMatchIndex index to input spectrum [0..numspectra-1]
void setBestMatch_HW( volatile AFASpectra *_networkSpectrum, unsigned int _networkIndex, volatile AFASpectra *_bestMatchSpectrum, unsigned int _bestMatchIndex )
{
    //assert( _networkSpectrum.isEmpty() );
    // set best matching related info.
    _networkSpectrum->m_SpecObjID	= _bestMatchSpectrum->m_SpecObjID;
    _networkSpectrum->m_Index		= _bestMatchIndex;
    _networkSpectrum->m_version		= _bestMatchSpectrum->m_version;

    // remember best match position to NW for faster search
    _bestMatchSpectrum->m_Index = _networkIndex;

}
#endif

void searchBestMatchLocal_HW( volatile AFASpectra *_src, const int _searchRadius, BestMatch *outbm )
{
#if 0
    //assert(_searchRadius > 0);
    int i,x,y;
    int xpBestMatchOld, ypBestMatchOld;
    int xMin, yMin, xMax, yMax;
    int numSpectraToSearch;
    unsigned int c;
    int spectraIndex;
    float err;

    BestMatch bestMatch;
    resetBM_HW(&bestMatch);


    if ( _src->m_Index < 0 )
    {
        // no old position, due to continue computation, we have to use complete search!
        searchBestMatchComplete_HW( _src, outbm );
        return;
    }

    xpBestMatchOld = _src->m_Index % AFAPP_HW.m_gridSize;
    ypBestMatchOld = _src->m_Index / AFAPP_HW.m_gridSize;

    // calc boundaries
    xMin = AFAMAX( xpBestMatchOld-_searchRadius, 0 );
    yMin = AFAMAX( ypBestMatchOld-_searchRadius, 0 );
    xMax = AFAMIN( xpBestMatchOld+_searchRadius+1, AFAPP_HW.m_gridSize );
    yMax = AFAMIN( ypBestMatchOld+_searchRadius+1, AFAPP_HW.m_gridSize );

    numSpectraToSearch = ( (xMax-xMin)*(yMax-yMin) );


    // read spectra from vfs
    c=0;
    for ( y=yMin;y<yMax;y++ )
    {
        for ( x=xMin;x<xMax;x++ )
        {
            spectraIndex = getNetworkIndex_HW( x, y );
            AFAPP_HW.m_localSearchIndexVec[c] = spectraIndex;
            AFAPP_HW.m_localSearchSpectraVec[c] = &AFAPP_HW.m_pNet[spectraIndex];
            c++;
        }
    }

    // calculate errors/distances
    compareSpectra_HW( _src, *AFAPP_HW.m_localSearchSpectraVec, c, &AFAPP_HW.m_localSearchErrorVec[0] );

    // find bmu from error vector
    for ( i=0;i<numSpectraToSearch;i++ )
    {
        spectraIndex = AFAPP_HW.m_localSearchIndexVec[i];
        err = AFAPP_HW.m_localSearchErrorVec[i];
        if (err < bestMatch.error )
        {
            bestMatch.error = err;
            bestMatch.index = spectraIndex;
        }
    }


    *outbm = bestMatch;
#endif
}

float newpow( float base, float exponent )
{
    return expf( exponent * logf( base ));
}

bool_t
AFAProcess_HW(
        uint32_t param[ 512 ],				// whole block ram used
        volatile AFASpectra	*spectraDataWorkingSet,
        volatile int *pSpectraIndexList,
        volatile AFASpectra	*g_spectraDataInput
        )
{
#pragma HLS INTERFACE m_axi     port=spectraDataWorkingSet depth=10000 bundle=MASTER_BUS
#pragma HLS INTERFACE m_axi     port=pSpectraIndexList     depth=10000 bundle=MASTER_BUS
#pragma HLS INTERFACE m_axi     port=g_spectraDataInput    depth=10000 bundle=MASTER_BUS
#pragma HLS INTERFACE bram      port=param                             bundle=BRAM_INTERFACE
#pragma HLS INTERFACE s_axilite port=spectraDataWorkingSet bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=pSpectraIndexList     bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=g_spectraDataInput    bundle=CTRL_BUS
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
    const int spectraCacheSize = AFAMIN( AFA_SPECTRA_CACHE_NUMSPECTRA, ( AFAMIN( AFAPP_hw.m_gridSizeSqr, AFA_COMPARE_BATCH_HW )));

    // get data from param block
    bool_t bFullSearch = param[ AFA_PARAM_INDICES_FULL_SEARCH ];
    unsigned int searchRadius = param[ AFA_PARAM_INDICES_SEARCH_RADIUS ];
    float adaptionThreshold = *(( float32_t * )&param[ AFA_PARAM_INDICES_ADAPTION_THRESHOLD ]);
    float sigmaSqr = *(( float32_t * )&param[ AFA_PARAM_INDICES_SIGMA_SQR ]);
    float lRate = *(( float32_t * )&param[ AFA_PARAM_INDICES_LRATE ]);

    if ( 0 == paramInitialized )
    {
        AFARandomGetInit(
            AFAPP_hw.m_mt, // the array for the state vector 
            AFAPP_hw.m_mti );
        AFASpectraPixelStartEndSet_HW(
            AFAPP_hw.m_pStart,
            AFAPP_hw.m_pEnd );
        paramInitialized = 1;
    }


    // select random spectra from spectra dataset

    // store all indicies in a list
    for ( i = 0; i < AFAPP_hw.m_numSpectra; i++ )
    {
        pSpectraIndexList[ i ] = i;
    }

    // shake well
    for ( i = 0; i < AFAPP_hw.m_numSpectra * 2; i++ )
    {
        ind0 = AFARandomIntRange_HW( AFAPP_hw.m_numSpectra - 1 );
        ind1 = AFARandomIntRange_HW( AFAPP_hw.m_numSpectra - 1 );

        // switch indices
        tmp = pSpectraIndexList[ ind0 ];
        pSpectraIndexList[ ind0 ] = pSpectraIndexList[ ind1 ];
        pSpectraIndexList[ ind1 ] = tmp;
    }

    // clear names
    for ( i = 0; i < AFAPP_hw.m_gridSizeSqr; i++ )
    {
        a = &spectraDataWorkingSet[ i ];
        a->m_SpecObjID = 0;
        a->m_Index = -1;
    }


    // for each training spectra..
    for ( j = 0; j < AFAPP_hw.m_numSpectra; j++ )
    {
        // initialize best match batch
        resetBM_HW( &bmu );

        spectraIndex = pSpectraIndexList[j];
        currentSourceSpectrum = &g_spectraDataInput[ spectraIndex ];

        // retrieve best match neuron for a source spectra
        if (bFullSearch)
        {
        	searchBestMatchComplete_HW(
                currentSourceSpectrum,
                spectraDataWorkingSet,
                &bmu );
        }
        else
        {
//            searchBestMatchLocal_HW( currentSourceSpectrum, searchRadius, &bmu );
        }

        // mark best match neuron
        bmuSpectrum = &spectraDataWorkingSet[bmu.index];
#ifdef JSC_SETBEST_MATCH_HW_INLINE
// set BMU in the map and source spectrum
// _networkSpectrum artificial spectrum in the map
// _networkIndex network index [0..gridsizesqr-1]
// _bestMatchSpectrum source/input spectrum
// _bestMatchIndex index to input spectrum [0..numspectra-1]

    // set best matching related info.
	bmuSpectrum->m_SpecObjID	= currentSourceSpectrum->m_SpecObjID;
	bmuSpectrum->m_Index		= spectraIndex;
	bmuSpectrum->m_version		= currentSourceSpectrum->m_version;

    // remember best match position to NW for faster search
	currentSourceSpectrum->m_Index = bmu.index;
#else
		setBestMatch_HW(
            bmuSpectrum,
            bmu.index,
            currentSourceSpectrum,
            spectraIndex );
#endif
        // adapt neighborhood
        // hint: this takes long.
        adaptNetwork_HW(
            currentSourceSpectrum,
            spectraDataWorkingSet,
            bmu.index,
            adaptionThreshold,
            sigmaSqr,
            lRate );
    }

    // clustering not yet finished, need another learning step
    return FALSE;
}

