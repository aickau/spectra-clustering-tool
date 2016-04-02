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

AFAProcessingParamSW_t	        AFAPP_sw;

// local variables to this file
// ============================

// maximum flux of input spectra
static float m_flux;

// maximum amplitude (before normalization)
static float m_Min;

// minimum amplitude (before normalization)
static float m_Max;

void resetBM( BestMatch *bmu)
{
    bmu->error = FLT_MAX;
    bmu->index = 0;
}

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
        a = &AFAPP_sw.spectraDataInput[ i ];
        AFASpectraNormalizeByFlux( a );
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
        a = &AFAPP_sw.spectraDataInput[ i ];

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
    calcMinMaxSp( AFAPP_sw.spectraDataInput, &m_Min, &m_Max );
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
    memoryBlockSize = ( memoryBlockSize + AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 ) & ~( AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 );   // padding at the end of memory block
    AFAPP_sw.workData[ idx ].sizeAllocated = memoryBlockSize;
    memoryOffsetInBlock += memoryBlockSize;
    printf( "%32s: %10llu [%10llu]\n", AFAPP_sw.workData[ idx ].name, AFAPP_sw.workData[ idx ].size, AFAPP_sw.workData[ idx ].sizeAllocated );
    idx++;

    strncpy( AFAPP_sw.workData[ idx ].name, "example data reduced", AFA_WORKING_DATA_NAME_LENGTH );
    AFAPP_sw.workData[ idx ].offsetToBaseAddress = memoryOffsetInBlock;
    memoryBlockSize = numSpectra * AFA_SPECTRA_INDEX_SIZE_IN_UINT32;
    AFAPP_sw.workData[ idx ].size = memoryBlockSize;
    memoryBlockSize = ( memoryBlockSize + AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 ) & ~( AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 );   // padding at the end of memory block
    AFAPP_sw.workData[ idx ].sizeAllocated = memoryBlockSize;
    memoryOffsetInBlock += memoryBlockSize;
    printf( "%32s: %10llu [%10llu]\n", AFAPP_sw.workData[ idx ].name, AFAPP_sw.workData[ idx ].size, AFAPP_sw.workData[ idx ].sizeAllocated );
    idx++;

    strncpy( AFAPP_sw.workData[ idx ].name, "m_pNet / SOM", AFA_WORKING_DATA_NAME_LENGTH );
    AFAPP_sw.workData[ idx ].offsetToBaseAddress = memoryOffsetInBlock;
    memoryBlockSize = AFAPP_sw.m_gridSizeSqr * sizeof( AFASpectra_SW );
    AFAPP_sw.workData[ idx ].size = memoryBlockSize;
    memoryBlockSize = ( memoryBlockSize + AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 ) & ~( AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 );   // padding at the end of memory block
    AFAPP_sw.workData[ idx ].sizeAllocated = memoryBlockSize;
    memoryOffsetInBlock += memoryBlockSize;
    printf( "%32s: %10llu [%10llu]\n", AFAPP_sw.workData[ idx ].name, AFAPP_sw.workData[ idx ].size, AFAPP_sw.workData[ idx ].sizeAllocated );
    idx++;

    strncpy( AFAPP_sw.workData[ idx ].name, "m_pSpectraIndexList", AFA_WORKING_DATA_NAME_LENGTH );
    AFAPP_sw.workData[ idx ].offsetToBaseAddress = memoryOffsetInBlock;
    memoryBlockSize = AFAPP_sw.m_gridSizeSqr * sizeof( sint32_t );
    AFAPP_sw.workData[ idx ].size = memoryBlockSize;
    memoryBlockSize = ( memoryBlockSize + AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 ) & ~( AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 );   // padding at the end of memory block
    AFAPP_sw.workData[ idx ].sizeAllocated = memoryBlockSize;
    memoryOffsetInBlock += memoryBlockSize;
    printf( "%32s: %10llu [%10llu]\n", AFAPP_sw.workData[ idx ].name, AFAPP_sw.workData[ idx ].size, AFAPP_sw.workData[ idx ].sizeAllocated );
    idx++;

    strncpy( AFAPP_sw.workData[ idx ].name, "m_localSearchSpectraVec", AFA_WORKING_DATA_NAME_LENGTH );
    AFAPP_sw.workData[ idx ].offsetToBaseAddress = memoryOffsetInBlock;
    memoryBlockSize = AFAPP_sw.m_gridSizeSqr * sizeof( AFASpectra_SW * );
    AFAPP_sw.workData[ idx ].size = memoryBlockSize;
    memoryBlockSize = ( memoryBlockSize + AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 ) & ~( AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 );   // padding at the end of memory block
    AFAPP_sw.workData[ idx ].sizeAllocated = memoryBlockSize;
    memoryOffsetInBlock += memoryBlockSize;
    printf( "%32s: %10llu [%10llu]\n", AFAPP_sw.workData[ idx ].name, AFAPP_sw.workData[ idx ].size, AFAPP_sw.workData[ idx ].sizeAllocated );
    idx++;

    strncpy( AFAPP_sw.workData[ idx ].name, "m_localSearchIndexVec", AFA_WORKING_DATA_NAME_LENGTH );
    AFAPP_sw.workData[ idx ].offsetToBaseAddress = memoryOffsetInBlock;
    memoryBlockSize = AFAPP_sw.m_gridSizeSqr * sizeof( uint32_t );
    AFAPP_sw.workData[ idx ].size = memoryBlockSize;
    memoryBlockSize = ( memoryBlockSize + AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 ) & ~( AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 );   // padding at the end of memory block
    AFAPP_sw.workData[ idx ].sizeAllocated = memoryBlockSize;
    memoryOffsetInBlock += memoryBlockSize;
    printf( "%32s: %10llu [%10llu]\n", AFAPP_sw.workData[ idx ].name, AFAPP_sw.workData[ idx ].size, AFAPP_sw.workData[ idx ].sizeAllocated );
    idx++;

    strncpy( AFAPP_sw.workData[ idx ].name, "m_localSearchErrorVec", AFA_WORKING_DATA_NAME_LENGTH );
    AFAPP_sw.workData[ idx ].offsetToBaseAddress = memoryOffsetInBlock;
    memoryBlockSize = AFAPP_sw.m_gridSizeSqr * sizeof( float32_t );
    AFAPP_sw.workData[ idx ].size = memoryBlockSize;
    memoryBlockSize = ( memoryBlockSize + AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 ) & ~( AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 );   // padding at the end of memory block
    AFAPP_sw.workData[ idx ].sizeAllocated = memoryBlockSize;
    memoryOffsetInBlock += memoryBlockSize;
    printf( "%32s: %10llu [%10llu]\n", AFAPP_sw.workData[ idx ].name, AFAPP_sw.workData[ idx ].size, AFAPP_sw.workData[ idx ].sizeAllocated );
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
            return AFAPP_sw.workData[ i ].offsetToBaseAddress;
        }
    }
    return ( uint64_t ) -1;
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

    AFAPP_sw.spectraDataInput      = ( AFASpectra_SW * ) AFAHelperStructures_GetAddressOf( "example data" );
    AFAPP_sw.spectraDataInputHW    = ( uint32_t *      ) AFAHelperStructures_GetAddressOf( "example data reduced" );
    AFAPP_sw.spectraDataWorkingSet = ( AFASpectra_SW * ) AFAHelperStructures_GetAddressOf( "m_pNet / SOM" );
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
                b = &AFAPP_sw.spectraDataInput[ spectraIndex ];

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

int getNetworkIndex( int _cellX, int _cellY )
{
    return _cellX+_cellY*AFAPP_sw.m_gridSize;
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

    if ( adr >= AFAPP_sw.m_gridSizeSqr )
        return ( uint32_t ) ( -1 );

    return ( uint32_t )AFAPP_sw.spectraDataWorkingSet[ adr ].m_Index;
}
