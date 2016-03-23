#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <float.h>
#include <math.h>

#include "AFASpectra.h"
#include "AFAProcessing.h"
#include "AFADefines.h"
#include "AFATypes.h"

//AFAProcessingParamHW_t	        AFAPP_hw;
AFAProcessingParamSW_t	        AFAPP_sw;

// local variables to this file
// ============================

// maximum flux of input spectra
static float m_flux;

// maximum amplitude (before normalization)
static float m_Min;

// minimum amplitude (before normalization)
static float m_Max;

void
AFAProcessSetParamBlockParameters()
{
//	AFAProcessingParam_t	*AFAPP_hw;
    unsigned long			*mt; // the array for the state vector 
    int						*mti;
    int						pStart;
    int						pEnd;

//	AFAPP_hw = AFAProcessGetParamBlockAddress();
    AFARandomGetStateVectorBlockAddress( &mt, &mti );
    AFASpectraPixelStartEndGet( &pStart, &pEnd );

    // no misunderstandings here ...
//    memset( &AFAPP_hw, 0, sizeof( AFAProcessingParamHW_t ));

//	AFAPP_hw.m_currentStep = AFAPP_sw.m_currentStep;
//    AFAPP_hw.m_gridSize = AFAPP_sw.m_gridSize;
//    AFAPP_hw.m_gridSizeSqr = AFAPP_sw.m_gridSizeSqr;
//	AFAPP_hw.m_localSearchErrorVec = AFAPP_sw.m_localSearchErrorVec;
//	AFAPP_hw.m_localSearchIndexVec = AFAPP_sw.m_localSearchIndexVec;
//	AFAPP_hw.m_localSearchSpectraVec = AFAPP_sw.m_localSearchSpectraVec;
//	AFAPP_hw.g_spectraDataInput = AFAPP_sw.g_spectraDataInput;
//	AFAPP_hw.m_pSpectraIndexList = AFAPP_sw.m_pSpectraIndexList;
//    AFAPP_hw.m_numSpectra = AFAPP_sw.m_numSpectra;
//    memcpy( &AFAPP_hw.m_params, &AFAPP_sw.m_params, sizeof( AFAParameters ));
//	AFAPP_hw.spectraDataWorkingSet = AFAPP_sw.spectraDataWorkingSet;

    // the random generator state vector
//    memcpy( &AFAPP_hw.m_mt[ 0 ], mt, sizeof( AFAPP_hw.m_mt ));
//    AFAPP_hw.m_mti = *mti;

    // spectra start/stop
//    AFAPP_hw.m_pStart = pStart;
//    AFAPP_hw.m_pEnd = pEnd;
}

void resetBM( BestMatch *bmu)
{
    bmu->error = FLT_MAX;
    bmu->index = 0;
}

#if 0
// set BMU in the map and source spectrum
// _networkSpectrum artificial spectrum in the map
// _networkIndex network index [0..gridsizesqr-1]
// _bestMatchSpectrum source/input spectrum
// _bestMatchIndex index to input spectrum [0..numspectra-1]
void
setBestMatch(
    volatile AFASpectra *networkSpectrum,
    uint32_t networkIndex,
    volatile AFASpectra *bestMatchSpectrum,
    uint32_t bestMatchIndex )
{
    //assert( networkSpectrum.isEmpty() );
    // set best matching related info.
    networkSpectrum->m_SpecObjID	= bestMatchSpectrum->m_SpecObjID;
    networkSpectrum->m_Index		= ( float32_t ) bestMatchIndex;
//	networkSpectrum->m_version		= bestMatchSpectrum->m_version;

    // remember best match position to NW for faster search
    bestMatchSpectrum->m_Index = ( float32_t ) networkIndex;
}
#endif

void reset( AFAParameters *params )
{
    // input from outside
    AFAPP_sw.m_params = *params;
    AFARandomInitRandom( AFAPP_sw.m_params.randomSeed );

    AFAPP_sw.m_currentStep = 0;
}


#if 1
void calcFluxAndNormalizeInputDS()
{
    volatile AFASpectra_SW *a;
    uint32_t i;
    m_flux = 0.0f;

    for ( i=0;i<AFAPP_sw.m_numSpectra;i++ )
    {
        a = &AFAPP_sw.g_spectraDataInput[ i ];
        AFASpectraNormalizeByFlux(a);
        m_flux = AFAMAX( a->m_flux, m_flux );
    }
}
#endif


#if 1
void calcMinMaxSp(
    volatile AFASpectra_SW *_vfs,
    float *_outMin,
    float *_outMax )
{
    volatile  AFASpectra_SW *a;
    uint32_t i;
    *_outMin = FLT_MAX;
    *_outMax = -FLT_MAX;

    // calc min/max
    for ( i=0;i<AFAPP_sw.m_numSpectra;i++ )
    {
        a = &AFAPP_sw.g_spectraDataInput[ i ];

        if ( *_outMin > a->m_Min )
        {
            *_outMin = a->m_Min;
        }
        if ( *_outMax < a->m_Max)
        {
            *_outMax = a->m_Max;
        }
    }
    // for sdds: -560 .. ~20.000 Angström
}
#endif

#if 1
void calcMinMaxInputDS()
{
    calcMinMaxSp( AFAPP_sw.g_spectraDataInput, &m_Min, &m_Max );
}
#endif

#if 1
void initNetwork(
    unsigned int _gridSize,
    float _minPeak,
    float _maxPeak )
{
    unsigned int i, x, y;
    float strength;
    unsigned int gridSizeSqr = _gridSize*_gridSize;
    float strengthScale = (float)gridSizeSqr*2.f;
    AFASpectra_SW spec;
    volatile AFASpectra_SW *sp;

    AFASpectraClear( &spec );

    for ( i=0;i<gridSizeSqr;i++)
    {
        sp = &AFAPP_sw.spectraDataWorkingSet[ i ];

        x = i%_gridSize;
        y = i/_gridSize;
        strength = ((float)(x*x+y*y))*0.25f/strengthScale;
        AFASpectraRandomize( &spec, _minPeak*strength, _maxPeak*strength );

        AFASpectraSet( sp, &spec);
    }
}
#endif

uint32_t AFACalcGridSize( uint32_t numSpectra )
{
    uint32_t gridSize = (unsigned int)(ceilf(sqrtf((float)numSpectra+1))*1.1f); // gives a factor of 1.1^2=1.21 in total
    return gridSize;
}

void
AFAHelperStructures_MemAllocate()
{
    uint64_t memoryNeeds = AFAPP_sw.memoryBlockSizeNeeded;
    uint64_t tmpAddrCalc;
    // increase potential memory needs to get space to shift the base address to an aligned place
    memoryNeeds = ( memoryNeeds + AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 );
    AFAPP_sw.memoryBlockBaseAddressAllocated = malloc( memoryNeeds );
    AFAPP_sw.memoryBlockSizeAllocated = memoryNeeds;
    memset( AFAPP_sw.memoryBlockBaseAddressAllocated, 0x76, memoryNeeds );

    // ===========================================================
    // attention: single point of failure - pointer calculation
    // * pointer is converted to uint64_t
    // * then the lower bits are masked out to get the alignment
    // * then the address is stored back into a pointer
    //
    // Jesko Schwarzer 2016-03-20, afa@schwarzers.de
    // ===========================================================
    tmpAddrCalc = ( uint64_t )(( uint8_t * )AFAPP_sw.memoryBlockBaseAddressAllocated );
    tmpAddrCalc = (( tmpAddrCalc + AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 ) & ~( AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 ));
    AFAPP_sw.memoryBlockBaseAddressAligned = ( void * ) tmpAddrCalc;
}

void
AFAHelperStructures_MemFree()
{
    if ( NULL != AFAPP_sw.memoryBlockBaseAddressAllocated )
    {
        free( AFAPP_sw.memoryBlockBaseAddressAllocated );
        AFAPP_sw.memoryBlockBaseAddressAllocated = NULL;
    }
}

void
AFAHelperStructures_PrepareDataStructure(
    const uint32_t numSpectra )
{
    uint64_t memoryOffsetInBlock = 0;
    uint64_t memoryBlockSize;
    uint32_t idx = 0;
    uint32_t *mt; // the array for the state vector 
    uint32_t *mti;
    uint32_t pStart;
    uint32_t pEnd;

    // store calculation constants and values
    AFAPP_sw.m_gridSize = AFACalcGridSize( numSpectra );
    AFAPP_sw.m_numSpectra = numSpectra;
    AFAPP_sw.m_gridSizeSqr = AFAPP_sw.m_gridSize * AFAPP_sw.m_gridSize;

    // prepare memory control structure
    strncpy( AFAPP_sw.workData[ idx ].name, "example data", AFA_WORKING_DATA_NAME_LENGTH );
    AFAPP_sw.workData[ idx ].offsetToBaseAddress = memoryOffsetInBlock;
    memoryBlockSize = numSpectra * sizeof( AFASpectra_SW );
    AFAPP_sw.workData[ idx ].size = memoryBlockSize;
    memoryBlockSize = ( memoryBlockSize + AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 ) & ~( AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 );
    AFAPP_sw.workData[ idx ].sizeAllocated = memoryBlockSize;
    memoryOffsetInBlock += memoryBlockSize;
    idx++;

    strncpy( AFAPP_sw.workData[ idx ].name, "m_pNet / SOM", AFA_WORKING_DATA_NAME_LENGTH );
    AFAPP_sw.workData[ idx ].offsetToBaseAddress = memoryOffsetInBlock;
    memoryBlockSize = AFAPP_sw.m_gridSizeSqr * sizeof( AFASpectra_SW );
    AFAPP_sw.workData[ idx ].size = memoryBlockSize;
    memoryBlockSize = ( memoryBlockSize + AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 ) & ~( AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 );
    AFAPP_sw.workData[ idx ].sizeAllocated = memoryBlockSize;
    memoryOffsetInBlock += memoryBlockSize;
    idx++;

    strncpy( AFAPP_sw.workData[ idx ].name, "m_pSpectraIndexList", AFA_WORKING_DATA_NAME_LENGTH );
    AFAPP_sw.workData[ idx ].offsetToBaseAddress = memoryOffsetInBlock;
    memoryBlockSize = AFAPP_sw.m_gridSizeSqr * sizeof( sint32_t );
    AFAPP_sw.workData[ idx ].size = memoryBlockSize;
    memoryBlockSize = ( memoryBlockSize + AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 ) & ~( AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 );
    AFAPP_sw.workData[ idx ].sizeAllocated = memoryBlockSize;
    memoryOffsetInBlock += memoryBlockSize;
    idx++;

    strncpy( AFAPP_sw.workData[ idx ].name, "m_localSearchSpectraVec", AFA_WORKING_DATA_NAME_LENGTH );
    AFAPP_sw.workData[ idx ].offsetToBaseAddress = memoryOffsetInBlock;
    memoryBlockSize = AFAPP_sw.m_gridSizeSqr * sizeof( AFASpectra_SW * );
    AFAPP_sw.workData[ idx ].size = memoryBlockSize;
    memoryBlockSize = ( memoryBlockSize + AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 ) & ~( AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 );
    AFAPP_sw.workData[ idx ].sizeAllocated = memoryBlockSize;
    memoryOffsetInBlock += memoryBlockSize;
    idx++;

    strncpy( AFAPP_sw.workData[ idx ].name, "m_localSearchIndexVec", AFA_WORKING_DATA_NAME_LENGTH );
    AFAPP_sw.workData[ idx ].offsetToBaseAddress = memoryOffsetInBlock;
    memoryBlockSize = AFAPP_sw.m_gridSizeSqr * sizeof( uint32_t );
    AFAPP_sw.workData[ idx ].size = memoryBlockSize;
    memoryBlockSize = ( memoryBlockSize + AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 ) & ~( AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 );
    AFAPP_sw.workData[ idx ].sizeAllocated = memoryBlockSize;
    memoryOffsetInBlock += memoryBlockSize;
    idx++;

    strncpy( AFAPP_sw.workData[ idx ].name, "m_localSearchErrorVec", AFA_WORKING_DATA_NAME_LENGTH );
    AFAPP_sw.workData[ idx ].offsetToBaseAddress = memoryOffsetInBlock;
    memoryBlockSize = AFAPP_sw.m_gridSizeSqr * sizeof( float32_t );
    AFAPP_sw.workData[ idx ].size = memoryBlockSize;
    memoryBlockSize = ( memoryBlockSize + AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 ) & ~( AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 );
    AFAPP_sw.workData[ idx ].sizeAllocated = memoryBlockSize;
    memoryOffsetInBlock += memoryBlockSize;
    idx++;

    // store final memory needs, final values
    AFAPP_sw.memoryBlockSizeNeeded = memoryOffsetInBlock;
}

void *
AFAHelperStructures_GetAddressOf(
    const char *dataName )
{
    uint32_t i;

    for ( i = 0; i < sizeof( AFAPP_sw.workData ) / sizeof( AFAPP_sw.workData[ 0 ]); i++ )
    {
        if ( 0 == strncmp( AFAPP_sw.workData[ i ].name, dataName, sizeof( AFAPP_sw.workData[ i ].name )))
        {
            return ( void * )AFAPP_sw.workData[ i ].address;
        }
    }
    return NULL;
}

uint64_t
AFAHelperStructures_GetOffsetToBaseAddress(
    const char *dataName )
{
    uint32_t i;

    for ( i = 0; i < sizeof( AFAPP_sw.workData ) / sizeof( AFAPP_sw.workData[ 0 ]); i++ )
    {
        if ( 0 == strncmp( AFAPP_sw.workData[ i ].name, dataName, sizeof( AFAPP_sw.workData[ i ].name )))
        {
            return ( void * )AFAPP_sw.workData[ i ].offsetToBaseAddress;
        }
    }
    return -1;
}

void
AFAHelperStructures_UpdateAddressData()
{
    uint32_t i;
    uint8_t *offset;

    offset = ( uint8_t * )( AFAPP_sw.memoryBlockBaseAddressAligned );
    for ( i = 0; i < sizeof( AFAPP_sw.workData ) / sizeof( AFAPP_sw.workData[ 0 ]); i++ )
    {
        AFAPP_sw.workData[ i ].address = ( void * )offset;
        offset += AFAPP_sw.workData[ i ].size;
    }
}

bool_t
AFAInitProcessingNew(
    bool_t continueProcessing
    )
{
    uint32_t i;
    uint32_t xp, yp;
    uint32_t inset, spectraIndex;
    volatile AFASpectra_SW *a, *b;
    uint64_t memOffset;
    AFAPP_sw.m_currentStep = 0;

    AFAPP_sw.spectraDataWorkingSet = ( AFASpectra_SW * ) AFAHelperStructures_GetAddressOf( "m_pNet / SOM" );
    AFAPP_sw.g_spectraDataInput    = ( AFASpectra_SW * ) AFAHelperStructures_GetAddressOf( "example data" );
    AFAPP_sw.m_pSpectraIndexList   = ( sint32_t *      ) AFAHelperStructures_GetAddressOf( "m_pSpectraIndexList" );

    reset( &AFAPP_sw.m_params );

    AFASpectraSetOperationRange(
        AFAPP_sw.m_params.useBOSSWavelengthRange );
    calcFluxAndNormalizeInputDS();
    calcMinMaxInputDS();

    if ( !continueProcessing )
    {
        //
        // start new computation
        //

        // generate random filled cluster and load it. (AFA changes: do not load it, generate it in memory only)
        initNetwork( AFAPP_sw.m_gridSize, m_Min, m_Max*0.01f );


        // fill network with random spectra, this improves the convergence times.
        // it does not matter if some spectra are inserted multiple times or other may missing since this is just for initialization purposes.
        AFARandomInitRandom( AFAPP_sw.m_params.randomSeed );

        for ( i=0;i<AFAPP_sw.m_gridSizeSqr;i++ )
        {
            xp = i % AFAPP_sw.m_gridSize;
            yp = i / AFAPP_sw.m_gridSize;
            inset = 0;
            if ( xp >= inset && xp <= (AFAPP_sw.m_gridSize-inset) && yp >= inset && yp <= (AFAPP_sw.m_gridSize-inset) )
            {
                a = &AFAPP_sw.spectraDataWorkingSet[ i ];

                spectraIndex = AFARandomIntRange( AFAPP_sw.m_numSpectra - 1 );
                b = &AFAPP_sw.g_spectraDataInput[ spectraIndex ];

                AFASpectraSet( a, b );
            }
        }
    }
    else
    {
#if 0
        //
        // continue old computation
        //

        m_pNet = new SpectraVFS( sstrSOFMFileName, false );

        m_gridSizeSqr = m_pNet->getNumSpectra();
        m_gridSize = sqrtf(m_gridSizeSqr);

        if ( m_gridSizeSqr == 0 )
        {
            // error
            assert(0);
            exit(0);
        }
#endif
    }
    return TRUE;
}

bool_t AFAInitProcessing(
    volatile void *spectraArrayInput,
    volatile void *helperStucturesMem,
    uint32_t numSpectra,
    bool_t continueProcessing,
    AFAParameters *params
    )
{
    uint32_t i;
    uint32_t xp, yp;
    uint32_t inset, spectraIndex;
    volatile AFASpectra_SW *a, *b;
    uint64_t memOffset;


    if ( helperStucturesMem == NULL || spectraArrayInput == NULL )
        return FALSE;

    AFAPP_sw.m_gridSize = AFACalcGridSize(numSpectra);
    AFAPP_sw.m_gridSizeSqr = AFAPP_sw.m_gridSize*AFAPP_sw.m_gridSize;
    AFAPP_sw.m_currentStep = 0;
    AFAPP_sw.m_numSpectra = numSpectra;
    AFAPP_sw.g_spectraDataInput = ( volatile AFASpectra_SW * ) spectraArrayInput;

    memOffset = 0;
    AFAPP_sw.spectraDataWorkingSet = ( volatile AFASpectra_SW * )  &((unsigned char*)helperStucturesMem)[memOffset];
    memOffset += AFAPP_sw.m_gridSizeSqr*sizeof(AFASpectra_SW);

    AFAPP_sw.m_pSpectraIndexList = ( volatile sint32_t * ) &((unsigned char*)helperStucturesMem)[memOffset];
    memOffset += AFAPP_sw.m_gridSizeSqr * sizeof(int);

    AFAPP_sw.m_localSearchSpectraVec = ( volatile AFASpectra_SW ** ) &((unsigned char*)helperStucturesMem)[memOffset];
    memOffset += AFAPP_sw.m_gridSizeSqr * sizeof( AFASpectra_SW * );

    AFAPP_sw.m_localSearchIndexVec = ( sint32_t * ) &((unsigned char*)helperStucturesMem)[memOffset];
    memOffset += AFAPP_sw.m_gridSizeSqr*sizeof(int);

    AFAPP_sw.m_localSearchErrorVec = ( float32_t *)  &(( unsigned char * )helperStucturesMem)[memOffset];


    reset(params);

    AFASpectraSetOperationRange( AFAPP_sw.m_params.useBOSSWavelengthRange );
    calcFluxAndNormalizeInputDS();
    calcMinMaxInputDS();

    if ( !continueProcessing )
    {
        //
        // start new computation
        //

        // generate random filled cluster and load it. (AFA changes: do not load it, generate it in memory only)
        initNetwork( AFAPP_sw.m_gridSize, m_Min, m_Max*0.01f );


        // fill network with random spectra, this improves the convergence times.
        // it does not matter if some spectra are inserted multiple times or other may missing since this is just for initialization purposes.
        AFARandomInitRandom( AFAPP_sw.m_params.randomSeed );

        for ( i=0;i<AFAPP_sw.m_gridSizeSqr;i++ )
        {
            xp = i % AFAPP_sw.m_gridSize;
            yp = i / AFAPP_sw.m_gridSize;
            inset = 0;
            if ( xp >= inset && xp <= (AFAPP_sw.m_gridSize-inset) && yp >= inset && yp <= (AFAPP_sw.m_gridSize-inset) )
            {
                a = &AFAPP_sw.spectraDataWorkingSet[ i ];

                spectraIndex = AFARandomIntRange( AFAPP_sw.m_numSpectra - 1 );
                b = &AFAPP_sw.g_spectraDataInput[ spectraIndex ];

                AFASpectraSet( a, b );
            }
        }
    }
    else
    {
#if 0
        //
        // continue old computation
        //

        m_pNet = new SpectraVFS( sstrSOFMFileName, false );

        m_gridSizeSqr = m_pNet->getNumSpectra();
        m_gridSize = sqrtf(m_gridSizeSqr);

        if ( m_gridSizeSqr == 0 )
        {
            // error
            assert(0);
            exit(0);
        }
#endif
    }
    return TRUE;
}




/*
int AFAProcessing::getSrcSpectraIndex( int index ) const
{
    if ( m_pNet[index].isEmpty() )
        return -1;

    return m_pNet[index].m_Index;
}

volatile AFASpectra *AFAProcessing::getSrcSpectrum( int index ) const
{
    if ( m_pNet[index].isEmpty() )
        return NULL;

    const int srcIndex = m_pNet[index].m_Index;
    if ( srcIndex < 0 || srcIndex >= m_numSpectra ) {
        // should not happen
        return NULL;
    }
    return &m_pSourceSpectra[srcIndex];
}
*/




int getNetworkIndex( int _cellX, int _cellY )
{
    return _cellX+_cellY*AFAPP_sw.m_gridSize;
}



void adaptNetwork( volatile AFASpectra_SW *_srcSpectrum, int _bestMatchIndex, float _adaptionThreshold, float _sigmaSqr, float _lRate )
{
    int x,y;
    float distY1, distY1Sqr, distYSqr;
    float distX1, distX1Sqr, distXSqr, distSqr;
    float hxy, lratehsx;
    unsigned int xpBestMatch = _bestMatchIndex % AFAPP_sw.m_gridSize;
    unsigned int ypBestMatch = _bestMatchIndex / AFAPP_sw.m_gridSize;
    float sigmaSqr2 = _sigmaSqr*(1.f/EULER);
    float fGridSizeSqr = (float)AFAPP_sw.m_gridSizeSqr;
    int gridSize = (int) AFAPP_sw.m_gridSize;
    unsigned int spectraAdress;
    volatile AFASpectra_SW *a;

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
                spectraAdress = y*AFAPP_sw.m_gridSize+x;
                a = &AFAPP_sw.spectraDataWorkingSet[spectraAdress];
                AFASpectraAdapt( a, _srcSpectrum, lratehsx );
            }
        }
    }
}



void compareSpectra(volatile AFASpectra_SW *_a, volatile AFASpectra_SW *_pB, int _nCount, float *_pOutErrors )
{
    //assert( _pB != NULL );
    //assert( _pOutErrors != NULL );
    int i;
    int numElements = (int)_nCount;

    // hint this can run in parallel
    for (i=0;i<numElements;i++)
    {
        if ( AFASpectraIsEmpty( &_pB[i] ) )
        {
            _pOutErrors[i] = AFASpectraCompare( _a, &_pB[i] );
        }
        else
        {
            _pOutErrors[i] = FLT_MAX;
        }
    }
}



/*

// Code just for illustration purposes how the search for the best match works in trivial way:
BestMatch AFAProcessing::searchBestMatchCompleteNonBatchMode( const AFASpectra &_src )
{
    BestMatch bestMatch;
    bestMatch.reset();

    float err;

    for (int i=0;i<m_gridSizeSqr;i++)
    {
        err = FLT_MAX;
        if ( m_pNet[i].isEmpty() )
        {
            err = _src.compare( m_pNet[i] );
        }

        if (err < bestMatch.error )
        {
            bestMatch.error = err[k];
            bestMatch.index = i;
        }
    }
}
*/

void searchBestMatchComplete( volatile AFASpectra_SW *_src, BestMatch *outbm )
{
    // see for the same outcome only slightly easier to understand..
    // searchBestMatchCompleteNonBatchMode()
    uint32_t j=0;
    uint32_t k;
    float err[AFA_COMPARE_BATCH];
    BestMatch bestMatch;
    volatile AFASpectra_SW *a;
    resetBM(&bestMatch);


    while (j<AFAPP_sw.m_gridSizeSqr)
    {
        const uint32_t jInc = AFAMIN( AFA_COMPARE_BATCH, (AFAMIN(AFAPP_sw.m_gridSizeSqr, j+AFA_COMPARE_BATCH)-j));

        // calc euclidean distances for spectrum _src and a batch of network spectra starting at m_pNet[j] .. m_pNet[j+jInc-1]
        a = &AFAPP_sw.spectraDataWorkingSet[j];
        compareSpectra( _src, a, jInc, err );

        // find smallest error (i.e. winning neuron/ network spectrum) in batch
        for ( k=0;k<jInc;k++ )
        {
            if (err[k] < bestMatch.error )
            {
                bestMatch.error = err[k];
                bestMatch.index = j+k;
            }
        }

        j += jInc;
    }

    *outbm = bestMatch;
}

#if 0
void searchBestMatchLocal( volatile AFASpectra *_src, const int _searchRadius, BestMatch *outbm )
{
    //assert(_searchRadius > 0);
    int i,x,y;
    int xpBestMatchOld, ypBestMatchOld;
    int xMin, yMin, xMax, yMax;
    int numSpectraToSearch;
    unsigned int c;
    int spectraIndex;
    float err;

    BestMatch bestMatch;
    resetBM(&bestMatch);


    if ( _src->m_Index < 0 )
    {
        // no old position, due to continue computation, we have to use complete search!
        searchBestMatchComplete( _src, outbm );
        return;
    }

    xpBestMatchOld = _src->m_Index % AFAPP_sw.m_gridSize;
    ypBestMatchOld = _src->m_Index / AFAPP_sw.m_gridSize;

    // calc boundaries
    xMin = AFAMAX( xpBestMatchOld-_searchRadius, 0 );
    yMin = AFAMAX( ypBestMatchOld-_searchRadius, 0 );
    xMax = AFAMIN( xpBestMatchOld+_searchRadius+1, AFAPP_sw.m_gridSize );
    yMax = AFAMIN( ypBestMatchOld+_searchRadius+1, AFAPP_sw.m_gridSize );

    numSpectraToSearch = ( (xMax-xMin)*(yMax-yMin) );


    // read spectra from vfs
    c=0;
    for ( y=yMin;y<yMax;y++ )
    {
        for ( x=xMin;x<xMax;x++ )
        {
            spectraIndex = getNetworkIndex( x, y );
            AFAPP_sw.m_localSearchIndexVec[c] = spectraIndex;
            AFAPP_sw.m_localSearchSpectraVec[c] = &AFAPP_sw.spectraDataWorkingSet[spectraIndex];
            c++;
        }
    }

    // calculate errors/distances
    compareSpectra( _src, *AFAPP_sw.m_localSearchSpectraVec, c, &AFAPP_sw.m_localSearchErrorVec[0] );

    // find bmu from error vector
    for ( i=0;i<numSpectraToSearch;i++ )
    {
        spectraIndex = AFAPP_sw.m_localSearchIndexVec[i];
        err = AFAPP_sw.m_localSearchErrorVec[i];

        if (err < bestMatch.error )
        {
            bestMatch.error = err;
            bestMatch.index = spectraIndex;
        }
    }


    *outbm = bestMatch;
}
#endif

uint32_t
AFAGetSpectraIndex(
    uint32_t xp,
    uint32_t yp )
{
    uint32_t adr = xp + yp * AFAPP_sw.m_gridSize;

    if ( adr < 0 || adr >= AFAPP_sw.m_gridSizeSqr )
        return ( uint32_t ) ( -1 );

    return ( uint32_t )AFAPP_sw.spectraDataWorkingSet[ adr ].m_Index;
}
