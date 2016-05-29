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

#include "BoardIO.h"

AFAProcessingParamSW_t	        AFAPP_sw;

// local variables to this file
// ============================

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

    AFAPP_sw.currentStep = 0;
}


#if 0
void calcFluxAndNormalizeInputDS()
{
    AFASpectra_SW *a;
    uint32_t i;

    for ( i = 0; i < AFAPP_sw.m_numSpectra; i++ )
    {
        a = &AFAPP_sw.spectraDataInput[ i ];
        AFASpectraNormalizeByFlux( a );
   }
}
#endif


#if 1
// calculates min and max over all spectra records
void calcMinMaxSp(
    AFASpectra_SW *spectraDataInput,
    uint32_t numSpectra,
    float32_t *outMin,
    float32_t *outMax )
{
    AFASpectra_SW *a;
    uint32_t i;
    *outMin = FLT_MAX;
    *outMax = -FLT_MAX;

    // calc min/max
    for ( i = 0; i < numSpectra; i++ )
    {
        a = &spectraDataInput[ i ];

        if ( *outMin > a->m_Min )
        {
            *outMin = a->m_Min;
        }
        if ( *outMax < a->m_Max)
        {
            *outMax = a->m_Max;
        }
    }
    // for sdds: -560 .. ~20.000 Angström
}
#endif

void initNetwork(
    uint32_t gridSize,  //< edge-length of grid
    float32_t minPeak,
    float32_t maxPeak )
{
    uint32_t i, x, y;
    float32_t strength;
    uint32_t gridSizeSqr = gridSize * gridSize;
    float32_t strengthScale = ( float32_t ) gridSizeSqr * 2.f;
    AFASpectra_SW spec;
    AFASpectra_SW *sp;

    AFASpectraClear( &spec );

    for ( i = 0; i < gridSizeSqr; i++ )
    {
        sp = &AFAPP_sw.spectraDataWorkingSet[ i ];

        x = i % gridSize;
        y = i / gridSize;
        strength = (( float32_t )( x * x + y * y )) * 0.25f / strengthScale;
        AFASpectraRandomize( &spec, minPeak * strength, maxPeak * strength );

        AFASpectraSet( sp, &spec );
    }
}

uint32_t AFACalcGridSize( uint32_t numSpectra )
{
    uint32_t gridSize = (unsigned int)(ceilf(sqrtf((float)numSpectra+1))*1.1f); // gives a factor of 1.1^2=1.21 in total
    return gridSize;
}

bool_t
AFAHelperStructures_MemAllocate()
{
    uint64_t memoryNeeds = AFAPP_sw.memoryBlockSizeNeeded;
    uint64_t tmpAddrCalc;
    uint8_t clearVal = 0xf9;
    bool_t rv = FALSE;

    // increase potential memory needs to get space to shift the base address to an aligned place
    memoryNeeds = ( memoryNeeds + AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 );
    AFAPP_sw.memoryBlockBaseAddressAllocated = malloc( memoryNeeds );
    rv =     AFAPP_sw.memoryBlockBaseAddressAllocated ? TRUE : FALSE;
    AFAPP_sw.memoryBlockSizeAllocated = memoryNeeds;
    memset( AFAPP_sw.memoryBlockBaseAddressAllocated, clearVal, memoryNeeds );

    // ===========================================================
    // attention: single point of failure - pointer calculation
    // * pointer is converted to uint64_t
    // * then the lower bits are masked out to get the alignment
    // * then the address is stored back into a pointer
    //
    // Jesko Schwarzer 2016-03-20, afa@schwarzers.de
    // ===========================================================
    tmpAddrCalc = ( uint64_t )(( addr_t )(( uint8_t * )AFAPP_sw.memoryBlockBaseAddressAllocated ));
    tmpAddrCalc = (( tmpAddrCalc + AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 ) & ~( AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 ));
    AFAPP_sw.memoryBlockBaseAddressAligned = ( void * ) tmpAddrCalc;

    printf( "  Bytes:                     %lld [0x%16.16llx]\n", memoryNeeds, memoryNeeds );
    printf( "  Clear with value:          %d [0x%2.2x]\n", clearVal, clearVal );
    printf( "  Memory allocation:         0x%16.16llx\n", ( uint64_t )( size_t )AFAPP_sw.memoryBlockBaseAddressAllocated );
    printf( "  Aligned memory allocation: 0x%16.16llx\n", ( uint64_t )( size_t )AFAPP_sw.memoryBlockBaseAddressAligned );

    return rv;
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

    // store calculation constants and values
    AFAPP_sw.m_gridSize = AFACalcGridSize( numSpectra );
    AFAPP_sw.m_numSpectra = numSpectra;
    AFAPP_sw.m_gridSizeSqr = AFAPP_sw.m_gridSize * AFAPP_sw.m_gridSize;

	// TODO: put this into a function

    printf( "Spectra record size (SW): %d [0x%8.8x]\n", sizeof( AFASpectra_SW ), sizeof( AFASpectra_SW ));
    printf( "Spectra record size (HW): %d [0x%8.8x]\n", AFA_SPECTRA_INDEX_SIZE_IN_BYTES, AFA_SPECTRA_INDEX_SIZE_IN_BYTES );
    printf( "Spectra num samples (SW): %d\n", AFA_SPECTRA_NUM_SAMPLES_PROCESS_SW );
    printf( "Spectra num samples (HW): %d\n", AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW );
    printf( "Spectra samples start   : %d\n", AFA_PROCESS_PIXEL_START );
    printf( "Spectra samples end     : %d\n", AFA_PROCESS_PIXEL_END );

    printf( "\n" );
    printf( "%32s: %10s %11s %10s\n", "--------------------------------","----------","------------","----------" );
    printf( "%32s: %10s %11s %10s\n", "Section name                    ","  memory  ","mem. padded ","  offset  " );
    printf( "%32s: %10s %11s %10s\n", "--------------------------------","----------","------------","----------" );

    // prepare memory control structure
    strncpy( AFAPP_sw.workData[ idx ].name, "example data", AFA_WORKING_DATA_NAME_LENGTH );
    AFAPP_sw.workData[ idx ].offsetToBaseAddress = memoryOffsetInBlock;
    memoryBlockSize = numSpectra * sizeof( AFASpectra_SW );
    AFAPP_sw.workData[ idx ].size = memoryBlockSize;
    memoryBlockSize = ( memoryBlockSize + AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 ) & ~( AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 );   // padding at the end of memory block
    AFAPP_sw.workData[ idx ].sizeAllocated = memoryBlockSize;
    printf( "%32s: %10llu [%10llu] 0x%8.8llx\n", AFAPP_sw.workData[ idx ].name, AFAPP_sw.workData[ idx ].size, AFAPP_sw.workData[ idx ].sizeAllocated, memoryOffsetInBlock );
    memoryOffsetInBlock += memoryBlockSize;
    idx++;

    strncpy( AFAPP_sw.workData[ idx ].name, "example data reduced", AFA_WORKING_DATA_NAME_LENGTH );
    AFAPP_sw.workData[ idx ].offsetToBaseAddress = memoryOffsetInBlock;
    memoryBlockSize = numSpectra * AFA_SPECTRA_INDEX_SIZE_IN_BYTES;
    AFAPP_sw.workData[ idx ].size = memoryBlockSize;
    memoryBlockSize = ( memoryBlockSize + AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 ) & ~( AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 );   // padding at the end of memory block
    AFAPP_sw.workData[ idx ].sizeAllocated = memoryBlockSize;
    printf( "%32s: %10llu [%10llu] 0x%8.8llx\n", AFAPP_sw.workData[ idx ].name, AFAPP_sw.workData[ idx ].size, AFAPP_sw.workData[ idx ].sizeAllocated, memoryOffsetInBlock );
    memoryOffsetInBlock += memoryBlockSize;
    idx++;

    strncpy( AFAPP_sw.workData[ idx ].name, "m_pNet / SOM", AFA_WORKING_DATA_NAME_LENGTH );
    AFAPP_sw.workData[ idx ].offsetToBaseAddress = memoryOffsetInBlock;
    memoryBlockSize = AFAPP_sw.m_gridSizeSqr * sizeof( AFASpectra_SW );
    AFAPP_sw.workData[ idx ].size = memoryBlockSize;
    memoryBlockSize = ( memoryBlockSize + AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 ) & ~( AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 );   // padding at the end of memory block
    AFAPP_sw.workData[ idx ].sizeAllocated = memoryBlockSize;
    printf( "%32s: %10llu [%10llu] 0x%8.8llx\n", AFAPP_sw.workData[ idx ].name, AFAPP_sw.workData[ idx ].size, AFAPP_sw.workData[ idx ].sizeAllocated, memoryOffsetInBlock );
    memoryOffsetInBlock += memoryBlockSize;
    idx++;

    strncpy( AFAPP_sw.workData[ idx ].name, "m_pNet / SOM reduced", AFA_WORKING_DATA_NAME_LENGTH );
    AFAPP_sw.workData[ idx ].offsetToBaseAddress = memoryOffsetInBlock;
    memoryBlockSize = AFAPP_sw.m_gridSizeSqr * AFA_SPECTRA_INDEX_SIZE_IN_BYTES;
    AFAPP_sw.workData[ idx ].size = memoryBlockSize;
    memoryBlockSize = ( memoryBlockSize + AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 ) & ~( AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 );   // padding at the end of memory block
    AFAPP_sw.workData[ idx ].sizeAllocated = memoryBlockSize;
    printf( "%32s: %10llu [%10llu] 0x%8.8llx\n", AFAPP_sw.workData[ idx ].name, AFAPP_sw.workData[ idx ].size, AFAPP_sw.workData[ idx ].sizeAllocated, memoryOffsetInBlock );
    memoryOffsetInBlock += memoryBlockSize;
    idx++;

    strncpy( AFAPP_sw.workData[ idx ].name, "m_pSpectraIndexList", AFA_WORKING_DATA_NAME_LENGTH );
    AFAPP_sw.workData[ idx ].offsetToBaseAddress = memoryOffsetInBlock;
    memoryBlockSize = AFAPP_sw.m_gridSizeSqr * sizeof( sint32_t );
    AFAPP_sw.workData[ idx ].size = memoryBlockSize;
    memoryBlockSize = ( memoryBlockSize + AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 ) & ~( AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 );   // padding at the end of memory block
    AFAPP_sw.workData[ idx ].sizeAllocated = memoryBlockSize;
    printf( "%32s: %10llu [%10llu] 0x%8.8llx\n", AFAPP_sw.workData[ idx ].name, AFAPP_sw.workData[ idx ].size, AFAPP_sw.workData[ idx ].sizeAllocated, memoryOffsetInBlock );
    memoryOffsetInBlock += memoryBlockSize;
    idx++;

    strncpy( AFAPP_sw.workData[ idx ].name, "m_localSearchSpectraVec", AFA_WORKING_DATA_NAME_LENGTH );
    AFAPP_sw.workData[ idx ].offsetToBaseAddress = memoryOffsetInBlock;
    memoryBlockSize = AFAPP_sw.m_gridSizeSqr * sizeof( AFASpectra_SW * );
    AFAPP_sw.workData[ idx ].size = memoryBlockSize;
    memoryBlockSize = ( memoryBlockSize + AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 ) & ~( AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 );   // padding at the end of memory block
    AFAPP_sw.workData[ idx ].sizeAllocated = memoryBlockSize;
    printf( "%32s: %10llu [%10llu] 0x%8.8llx\n", AFAPP_sw.workData[ idx ].name, AFAPP_sw.workData[ idx ].size, AFAPP_sw.workData[ idx ].sizeAllocated, memoryOffsetInBlock );
    memoryOffsetInBlock += memoryBlockSize;
    idx++;

    strncpy( AFAPP_sw.workData[ idx ].name, "m_localSearchIndexVec", AFA_WORKING_DATA_NAME_LENGTH );
    AFAPP_sw.workData[ idx ].offsetToBaseAddress = memoryOffsetInBlock;
    memoryBlockSize = AFAPP_sw.m_gridSizeSqr * sizeof( uint32_t );
    AFAPP_sw.workData[ idx ].size = memoryBlockSize;
    memoryBlockSize = ( memoryBlockSize + AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 ) & ~( AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 );   // padding at the end of memory block
    AFAPP_sw.workData[ idx ].sizeAllocated = memoryBlockSize;
    printf( "%32s: %10llu [%10llu] 0x%8.8llx\n", AFAPP_sw.workData[ idx ].name, AFAPP_sw.workData[ idx ].size, AFAPP_sw.workData[ idx ].sizeAllocated, memoryOffsetInBlock );
    memoryOffsetInBlock += memoryBlockSize;
    idx++;

    strncpy( AFAPP_sw.workData[ idx ].name, "m_localSearchErrorVec", AFA_WORKING_DATA_NAME_LENGTH );
    AFAPP_sw.workData[ idx ].offsetToBaseAddress = memoryOffsetInBlock;
    memoryBlockSize = AFAPP_sw.m_gridSizeSqr * sizeof( float32_t );
    AFAPP_sw.workData[ idx ].size = memoryBlockSize;
    memoryBlockSize = ( memoryBlockSize + AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 ) & ~( AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS - 1 );   // padding at the end of memory block
    AFAPP_sw.workData[ idx ].sizeAllocated = memoryBlockSize;
    printf( "%32s: %10llu [%10llu] 0x%8.8llx\n", AFAPP_sw.workData[ idx ].name, AFAPP_sw.workData[ idx ].size, AFAPP_sw.workData[ idx ].sizeAllocated, memoryOffsetInBlock );
    memoryOffsetInBlock += memoryBlockSize;
    idx++;

    AFAPP_sw.workDataNumRecords = idx;
    printf( "%32s: %10s %11s %10s\n", "--------------------------------","----------","------------","----------" );
    printf( "\n" );

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
    uint32_t idx;
    uint8_t *offset;

    printf( "\n" );
    printf( "%32s: %10s %11s %39s\n", "--------------------------------","----------","------------","---------------------------------------" );
    printf( "%32s: %10s %11s %18s\n", "Section name                    ","  memory  ","   memory   ","            memory address             " );
    printf( "%32s: %10s %11s %18s\n", "                                ","          ","   padded   ","       from        -         to        " );
    printf( "%32s: %10s %11s %18s\n", "--------------------------------","----------","------------","---------------------------------------" );

    offset = ( uint8_t * )( AFAPP_sw.memoryBlockBaseAddressAligned );
    for ( idx = 0; idx < AFAPP_sw.workDataNumRecords; idx++ )
    {
        AFAPP_sw.workData[ idx ].address = ( void * )offset;
        printf( "%32s: %10llu [%10llu] 0x%16.16llx - 0x%16.16llx\n",
        	AFAPP_sw.workData[ idx ].name,
			AFAPP_sw.workData[ idx ].size,
			AFAPP_sw.workData[ idx ].sizeAllocated,
			( uint64_t )( size_t )offset,
		    ( uint64_t )( size_t )( offset + AFAPP_sw.workData[ idx ].sizeAllocated ));
        offset += AFAPP_sw.workData[ idx ].sizeAllocated;             // add the padded size of the block to get correct alignment
    }
    printf( "%32s: %10s %11s %18s\n", "--------------------------------","----------","------------","---------------------------------------" );
}

bool_t
AFAInitProcessingNew(
    bool_t continueProcessing
    )
{
    uint32_t i;
    uint32_t xp, yp;
    uint32_t inset, spectraIndex;
    AFASpectra_SW *a, *b;
    float32_t m_Min;
    float32_t m_Max;

    // store addresses new to ease accesses
    AFAPP_sw.spectraDataInput        = ( AFASpectra_SW * ) AFAHelperStructures_GetAddressOf( "example data" );
    AFAPP_sw.spectraDataInputHW      = ( uint32_t *      ) AFAHelperStructures_GetAddressOf( "example data reduced" );
    AFAPP_sw.spectraDataWorkingSet   = ( AFASpectra_SW * ) AFAHelperStructures_GetAddressOf( "m_pNet / SOM" );
    AFAPP_sw.spectraDataWorkingSetHW = ( uint32_t *      ) AFAHelperStructures_GetAddressOf( "m_pNet / SOM reduced" );
    AFAPP_sw.m_pSpectraIndexList     = ( sint32_t *      ) AFAHelperStructures_GetAddressOf( "m_pSpectraIndexList" );

    // * reset global loop counter.
    // * initialize params
    // * initialize RNG
    reset( &AFAPP_sw.m_params );

    // set the start and end of amplitude range
    //AFASpectraSetOperationRange(
    //    AFAPP_sw.m_params.useBOSSWavelengthRange );

    // normalizes all INPUT spectra records to have a flux of 1.0 ...
    // little below because of 0.001 in function AFASpectraNormalizeByFlux()
	if ( AFAPP_sw.m_params.normaliziationType == SN_FLUX )
	{
	    LEDRGBSet( 0, EVAL_BOARD_LEDRGB_CYAN );		// normalization state
		for ( i = 0; i < AFAPP_sw.m_numSpectra; i++ )
		{
	    	LEDBinaryShow( i );

			a = &AFAPP_sw.spectraDataInput[ i ];
			AFASpectraNormalizeByFlux( a );
		}
	}

    // calculates the min and max of the complete INPUT spectra grid and give back values
	LEDRGBSet( 0, EVAL_BOARD_LEDRGB_BLUE );		// min/max state
    calcMinMaxSp(
        AFAPP_sw.spectraDataInput,
        AFAPP_sw.m_numSpectra,
        &m_Min,
        &m_Max );

    if ( !continueProcessing )
    {
        //
        // start new computation
        //

        // generate random filled cluster and load it.
        // ==> (AFA changes: do not load it, generate it in memory only)
        // -------------------------------------------------------------

        // =======================================================================
        // fill network with random spectra, this improves the convergence times.
        // it does not matter if some spectra are inserted multiple times or other
        // may missing since this is just for initialization purposes.
        // =======================================================================

        // 1.) generate random spectra with increasing amplitude
        //     from lower left to upper right corner based on
        //     min/max from INPUT spectra
        initNetwork( AFAPP_sw.m_gridSize, m_Min, m_Max * 0.01f );

        // 2.) initialize the random function, second time so far (see reset()). Is this intended ? e.g. seed etc. ?
        AFARandomInitRandom( AFAPP_sw.m_params.randomSeed );

        // 3.) now select randomly for each network cell an input one, this improves the convergence times.
		//     it does not matter if some spectra are inserted multiple times or other may missing since this is just for initialization purposes.
        // -------------------------------------------------------------------------------
	    LEDRGBSet( 0, EVAL_BOARD_LEDRGB_VIOLETT );		// random state

        for ( i = 0; i < AFAPP_sw.m_gridSizeSqr; i++ )
        {
        	LEDBinaryShow( i );

            xp = i % AFAPP_sw.m_gridSize;
            yp = i / AFAPP_sw.m_gridSize;
            inset = 0;
            if ( xp >= inset && xp <= (AFAPP_sw.m_gridSize-inset) && yp >= inset && yp <= (AFAPP_sw.m_gridSize-inset) )
            {
                a = &AFAPP_sw.spectraDataWorkingSet[ i ];

                spectraIndex = AFARandomIntRange( AFAPP_sw.m_numSpectra - 1 );
                b = &AFAPP_sw.spectraDataInput[ spectraIndex ];

                AFASpectraSet( a, b );
                a->m_pad2 = i;
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
