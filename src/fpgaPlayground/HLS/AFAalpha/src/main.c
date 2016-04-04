#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "AFADefines.h"
#include "AFATypes.h"
#include "AFASpectra.h"
#include "AFAProcessing.h"

#define JSCDBG_ITER_SPECIAL

extern AFAProcessingParamSW_t	    AFAPP_sw;
extern uint32_t m_mt[ RANDOM_N ]; // the array for the state vector 
extern int m_mti; 

// current learning step
sint32_t currentStep;

sint16_t golden_data[ 12 ][ 12 ]=
{
#ifdef JSCDBG_ITER_SPECIAL
// params.numSteps = 1;
    {   8,    2,   12,   14,   17,   18,   52,   49,   51,   48,   47,   40, },
    {  90,    3,    7,   15,   11,   16,   53,   55,   54,   46,   38,   45, },
    {  92,   89,    5,   10,    9,   13,   -1,   56,   50,   41,   44,   35, },
    {  91,   87,    1,    4,   93,   -1,   -1,   -1,   -1,   43,   42,   36, },
    {  95,   94,   96,    0,    6,   -1,   -1,   -1,   -1,   -1,   37,   39, },
    {  97,   98,   99,   86,   -1,   -1,   -1,   -1,   -1,   -1,   32,   34, },
    {  84,   88,   85,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   20,   33, },
    {  83,   80,   -1,   -1,   -1,   -1,   -1,   57,   -1,   -1,   23,   30, },
    {  81,   77,   -1,   -1,   -1,   -1,   61,   58,   -1,   22,   29,   31, },
    {  78,   76,   -1,   71,   67,   66,   62,   59,   60,   24,   25,   28, },
    {  82,   74,   75,   70,   68,   65,   64,   63,   -1,   -1,   27,   21, },
    {  79,   73,   72,   69,   -1,   -1,   -1,   -1,   -1,   -1,   19,   26, }
#else

///////////////////////////////////////////////////////////
#define JSCDBG_ACCEPT_LITTLE_INACCURACIES
///////////////////////////////////////////////////////////

#ifdef JSCDBG_ACCEPT_LITTLE_INACCURACIES
// params.numSteps = 200;
    {  10,   11,   13,   15,   18,   19,   25,   27,   29,   30,   33,   34, },
    {   8,    9,   12,   14,   17,   20,   24,   26,   31,   32,   35,   36, },
    {   7,    4,   -1,   -1,   16,   21,   23,   28,   -1,   -1,   37,   38, },
    {   6,    2,    5,   -1,   -1,   22,   -1,   -1,   -1,   39,   40,   41, },
    {   0,    1,    3,   -1,   -1,   -1,   -1,   -1,   -1,   43,   42,   44, },
    {  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   47,   46,   45, },
    {  97,   95,   98,   99,   -1,   -1,   -1,   -1,   -1,   51,   49,   48, },
    {  93,   94,   92,   96,   -1,   -1,   -1,   -1,   -1,   50,   53,   52, },
    {  90,   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   54,   55, },
    {  88,   89,   -1,   81,   -1,   76,   72,   70,   65,   60,   57,   56, },
    {  87,   82,   84,   79,   78,   74,   73,   69,   66,   62,   59,   58, },
    {  86,   85,   83,   80,   77,   75,   71,   68,   67,   64,   63,   61, }
#else
// params.numSteps = 200;
    {  10,   11,   13,   15,   18,   19,   25,   27,   29,   30,   33,   34, },
    {   8,    9,   12,   14,   17,   20,   24,   26,   31,   32,   35,   36, },
    {   7,    4,   -1,   -1,   16,   21,   23,   28,   -1,   -1,   37,   38, },
    {   6,    2,    5,   -1,   -1,   22,   -1,   -1,   -1,   39,   40,   41, },
    {   0,    1,    3,   -1,   -1,   -1,   -1,   -1,   -1,   43,   42,   44, },
    {  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   47,   46,   45, },
    {  97,   95,   98,   99,   -1,   -1,   -1,   -1,   -1,   51,   49,   48, },
    {  93,   94,   92,   96,   -1,   -1,   -1,   -1,   -1,   50,   53,   52, },
    {  90,   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   54,   55, },
    {  88,   89,   -1,   81,   -1,   76,   72,   70,   65,   60,   57,   56, },
    {  87,   82,   84,   79,   78,   74,   73,   69,   66,   62,   59,   58, },
    {  86,   85,   83,   80,   77,   75,   71,   68,   67,   64,   63,   61, }
#endif
#endif
};

#if 0
uint32_t ReadSpectraFileRAW( const char *filename, uint32_t *spectraCount, AFASpectra **spectraData )
{
    FILE *f;
    uint32_t rv = 0xffffffff;
    long size = 0;

    if ( spectraData )
    {
        f = fopen( filename, "r+b" );
        if ( f )
        {
            fseek( f, 0, SEEK_END );

            const unsigned int fileSize = ( unsigned int ) ftell( f );
            const unsigned int fileCount = fileSize / sizeof( AFASpectra );
            const unsigned int remainder = fileSize % sizeof( AFASpectra );

            if ( 0 == fileSize )
            {
                // file illegal
                rv = 3;
            }
            else
            {
                if  ( fileSize < sizeof( AFASpectra ) || remainder != 0 )
                {
                    // wrong size
                    rv = 4;
                }
                else
                {
                    *spectraData = ( AFASpectra * ) malloc( fileSize );
                    if ( *spectraData )
                    {
                        // clear memory
                        memset(( void * )*spectraData, 0x00, fileSize );
                        //
                        *spectraCount = ( unsigned long ) fileCount;
                        fseek( f, 0, SEEK_SET );
                        unsigned int n = fread(( void * )( *spectraData ), fileSize, 1, f );
                        if ( 1 == n )
                        {
                            // everything is fine
                            rv = 0;
                        }
                        else
                        {
                            // read failed
                            rv = 6;
                        }
                    }
                    else
                    {
                        // no mem avail
                        rv = 5;
                    }
                }
            }
            fclose( f );
        }
        else
        {
            // cannot open
            rv = 2;
        }
    }
    else
    {
        // illegal pointer
        rv = 1;
    }
    return rv;
}
#endif

void generateSineTestSpectra( uint32_t numTestSpectra, AFASpectra_SW *outSpectraArray )
{
    uint32_t i;
    float freqMin = 0.002f;
    float freqMax = 0.05f;
    float freqStepSize = (freqMax-freqMin)/(float)numTestSpectra;
    float freq = freqMin;

    for (i=0;i<numTestSpectra;i++)
    {
        AFASpectra_SW *sp =  &outSpectraArray[i];
        AFASpectraSetSine( sp, freq, 0.0f, 1.0f, 0.0f );
        freq += freqStepSize;
    }
}


// translates sw spectra to hw spectra. Can be used for input spectra as well as network spectra.
// spectraArraySw incoming sw spectra
// outHWAddr allocated hardware memory where we write AFA_SPECTRA_INDEX_SIZE_IN_BYTES*numSpectra bytes
void
swSpectraToHwSpectra(
    AFASpectra_SW *spectraArraySw,
    uint32_t *outHWAddr,
    uint32_t numSpectra )
{
	uint64_t spectraAdress;
	uint32_t i,j;
	AFASpectra_SW *sp;

	for ( i = 0; i < numSpectra; i++ )
	{
		sp = &spectraArraySw[ i ];

		spectraAdress =  i * AFA_SPECTRA_INDEX_SIZE_IN_UINT32;

		for ( j = 0; j < AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW; j++ )
		{
			// cast to float ptr to write float values
			float32_t *tmpValFloatPtr = (( float32_t * ) &outHWAddr[ spectraAdress + AFA_SPECTRA_INDEX_AMPLITUDE + j ]);
			( *tmpValFloatPtr ) = sp->m_Amplitude[ j + AFA_PROCESS_PIXEL_START ];
		}

		outHWAddr[ spectraAdress + AFA_SPECTRA_INDEX_INDEX ] = sp->m_Index;
		outHWAddr[ spectraAdress + AFA_SPECTRA_INDEX_SPEC_OBJ_ID_LOW ]  = ( uint32_t )( sp->m_SpecObjID & 0x0ffffffff );
		outHWAddr[ spectraAdress + AFA_SPECTRA_INDEX_SPEC_OBJ_ID_HIGH ] = ( uint32_t )( sp->m_SpecObjID >> 32 );
	}
}

// translates hw spectra back to sw spectra. Can be used for inputs spectra as well as network spectra.
// spectraArrayHw incoming hw spectra with AFA_SPECTRA_INDEX_SIZE_IN_BYTES*numSpectra data
// outSwArray allocated software memory where we write spectra to
void
hwSpectraToSwSpectra(
	uint32_t *spectraArrayHw,
	AFASpectra_SW *outSwArray,
	uint32_t numSpectra )
{
	uint64_t spectraAdress;
	uint32_t i,j;
	AFASpectra_SW *outSp;
	float32_t *tmpValFloatPtr;


	for ( i = 0; i < numSpectra; i++ )
	{
		outSp = &outSwArray[ i ];

		spectraAdress =  i * AFA_SPECTRA_INDEX_SIZE_IN_UINT32;
		tmpValFloatPtr = (( float32_t * ) &spectraArrayHw[ spectraAdress + AFA_SPECTRA_INDEX_AMPLITUDE ]);

		for ( j = 0; j < AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW; j++ )
		{
			// cast to float ptr to read float values
			outSp->m_Amplitude[ j + AFA_PROCESS_PIXEL_START ] = tmpValFloatPtr[ j ];
		}

		outSp->m_Index = spectraArrayHw[ spectraAdress + AFA_SPECTRA_INDEX_INDEX ];
		outSp->m_SpecObjID = (uint64_t)spectraArrayHw[ spectraAdress + AFA_SPECTRA_INDEX_SPEC_OBJ_ID_LOW ] | ((uint64_t)spectraArrayHw[ spectraAdress + AFA_SPECTRA_INDEX_SPEC_OBJ_ID_LOW ] << 32);
	}
}

uint32_t
AFAGetSpectraIndexNew(
    uint32_t xp,
    uint32_t yp )
{
    uint32_t adr = xp + yp * AFAPP_sw.m_gridSize;

    if ( adr >= AFAPP_sw.m_gridSizeSqr )
        return ( uint32_t ) ( -1 );

    return ( uint32_t )AFAPP_sw.spectraDataWorkingSetHW[ adr * AFA_SPECTRA_INDEX_SIZE_IN_UINT32 + AFA_SPECTRA_INDEX_INDEX ];
}




uint32_t param[ 256 ];

int main(int argc, char* argv[])
{
    AFASpectra_SW *spectraDataInput;
    uint32_t *spectraDataInputHW;
	AFASpectra_SW *spectraDataWorkingSet;
	uint32_t *spectraDataWorkingSetHW;
    uint32_t *baseAddr = NULL;	// that's a dummy address pointing to the start of work area of HW in memory
    uint32_t numSpectra = 100;  // do we have more than 4bn spectra ... ? not in THESE times
    uint32_t xp, yp, gridSize, gridSizeSqr;
    sint32_t idx;
    sint32_t idx_golden;
    int rv = 0;
    bool_t rc;

    uint64_t spectraDataInputHW_OffsetToBaseAddress;
    uint64_t spectraDataWorkingSetHW_OffsetToBaseAddress;
    uint64_t pSpectraIndexList_OffsetToBaseAddress;

    printf( "Starting main() ...\n" );
    if ( !AFATypesOK())
    {
        printf( "Error with AFATypes.h\n" );
        exit( 1 );
    }

    AFASetDefaultParameters( &AFAPP_sw.m_params );
#ifdef JSCDBG_ITER_SPECIAL
    AFAPP_sw.m_params.numSteps = 1;
#else
    AFAPP_sw.m_params.numSteps = 200;
#endif
    AFAPP_sw.m_params.searchMode = AFANET_SETTINGS_SEARCHMODE_global;
    
    AFAHelperStructures_PrepareDataStructure(
        numSpectra );
    AFAHelperStructures_MemAllocate();
    AFAHelperStructures_UpdateAddressData();
    spectraDataInput = ( AFASpectra_SW * ) AFAHelperStructures_GetAddressOf(
        "example data" );
    spectraDataInputHW = ( uint32_t * ) AFAHelperStructures_GetAddressOf(
        "example data reduced" );


	spectraDataWorkingSet = ( AFASpectra_SW * ) AFAHelperStructures_GetAddressOf(
		"m_pNet / SOM" );
	spectraDataWorkingSetHW = ( uint32_t * ) AFAHelperStructures_GetAddressOf(
		"m_pNet / SOM reduced" );
	gridSize = AFAPP_sw.m_gridSize;
	gridSizeSqr = AFAPP_sw.m_gridSizeSqr;

    generateSineTestSpectra(
        numSpectra,
        spectraDataInput );
    AFAInitProcessingNew(
        FALSE );

    swSpectraToHwSpectra(
        spectraDataInput,
        spectraDataInputHW,
        numSpectra );

    swSpectraToHwSpectra(
        spectraDataWorkingSet,
        spectraDataWorkingSetHW,
        gridSizeSqr );


    // here we convert pointer differences to byte offsets (baseAddr is NULL)
    spectraDataInputHW_OffsetToBaseAddress      = ( char * ) AFAPP_sw.spectraDataInputHW      - (( char * ) baseAddr );
    spectraDataWorkingSetHW_OffsetToBaseAddress = ( char * ) AFAPP_sw.spectraDataWorkingSetHW - (( char * ) baseAddr );
    pSpectraIndexList_OffsetToBaseAddress       = ( char * ) AFAPP_sw.m_pSpectraIndexList     - (( char * ) baseAddr );

    do
    {
        if ( currentStep > AFAPP_sw.m_params.numSteps )
        {
            //Clustering finished (success).
            rc = TRUE;
        }
        else
        {
            float32_t lPercent = ( float32_t )( currentStep ) / ( float32_t )( AFAPP_sw.m_params.numSteps );
            float32_t lRate = ( float32_t ) ( AFAPP_sw.m_params.lRateBegin * powf( AFAPP_sw.m_params.lRateEnd / AFAPP_sw.m_params.lRateBegin, lPercent ));
            float32_t adaptionThreshold = AFAPP_sw.m_params.lRateEnd * 0.01f;
            float32_t sigma = ( float32_t ) ( AFAPP_sw.m_params.radiusBegin * powf( AFAPP_sw.m_params.radiusEnd / AFAPP_sw.m_params.radiusBegin, lPercent));
            float32_t sigmaSqr = sigma*sigma;
            uint32_t i;
            bool_t bFullSearch = TRUE;
            uint32_t searchRadius = 1;

            // determine search strategy for BMUs for the current learning step
            if ( AFAPP_sw.m_params.searchMode == AFANET_SETTINGS_SEARCHMODE_localfast )
            {
                // always use a constant search radius, never do a global search
                bFullSearch = ( currentStep < 1 );
                searchRadius = 2;
            }
            else if ( AFAPP_sw.m_params.searchMode == AFANET_SETTINGS_SEARCHMODE_local )
            {
                // global search for the first 5 steps, decreasing search radius for increasing number of learning steps
                bFullSearch = ( currentStep < 5 );
                searchRadius = ( unsigned int )((( 1.f - lPercent ) * 0.5f * ( float )( AFAPP_sw.m_gridSize ))) + 2;
            }
            else // SOFMNET_SETTINGS_SEARCHMODE_global
            {
                // always use global search, never go local.
                // slow but guarantees optimal results in every case
                bFullSearch = TRUE;
            }

            // ==================================================
            //
            // prepare HW data structures (swizzle indices, etc.)
            //
            // ==================================================

            // store all indices in a list
            for ( i = 0; i < numSpectra; i++ )
            {
                AFAPP_sw.m_pSpectraIndexList[ i ] = i;
            }

            // shake well
            for ( i = 0; i < numSpectra * 2; i++ )
            {
                uint32_t ind0 = AFARandomIntRange( numSpectra - 1 );
                uint32_t ind1 = AFARandomIntRange( numSpectra - 1 );

                // switch indices
                sint32_t tmp = AFAPP_sw.m_pSpectraIndexList[ ind0 ];
                AFAPP_sw.m_pSpectraIndexList[ ind0 ] = AFAPP_sw.m_pSpectraIndexList[ ind1 ];
                AFAPP_sw.m_pSpectraIndexList[ ind1 ] = tmp;
            }

            // clear names
            for ( i = 0; i < gridSizeSqr * AFA_SPECTRA_INDEX_SIZE_IN_UINT32; i += AFA_SPECTRA_INDEX_SIZE_IN_UINT32 )
            {
                AFAPP_sw.spectraDataWorkingSetHW[ i + AFA_SPECTRA_INDEX_SPEC_OBJ_ID_LOW  ] = 0;
                AFAPP_sw.spectraDataWorkingSetHW[ i + AFA_SPECTRA_INDEX_SPEC_OBJ_ID_HIGH ] = 0;
                AFAPP_sw.spectraDataWorkingSetHW[ i + AFA_SPECTRA_INDEX_INDEX            ] = ( uint32_t ) ( -1 );
            }

            // ==================================================
            //
            // prepare parameter block
            //
            // ==================================================

            param[ AFA_PARAM_INDICES_FULL_SEARCH                       ] = bFullSearch;
            param[ AFA_PARAM_INDICES_SEARCH_RADIUS                     ] = searchRadius;
            param[ AFA_PARAM_INDICES_ADAPTION_THRESHOLD                ] = *(( uint32_t * ) &adaptionThreshold );
            param[ AFA_PARAM_INDICES_SIGMA_SQR                         ] = *(( uint32_t * ) &sigmaSqr          );
            param[ AFA_PARAM_INDICES_LRATE                             ] = *(( uint32_t * ) &lRate             );
            param[ AFA_PARAM_INDICES_GRID_SIZE                         ] = gridSize;
            param[ AFA_PARAM_INDICES_GRID_SIZE_SQR                     ] = gridSizeSqr;
            param[ AFA_PARAM_INDICES_NUM_SPECTRA                       ] = numSpectra;
            param[ AFA_PARAM_INDICES_RNG_MTI                           ] = m_mti;
            param[ AFA_PARAM_INDICES_PIXEL_START                       ] = 0;
            param[ AFA_PARAM_INDICES_PIXEL_END                         ] = 0;
            param[ AFA_PARAM_INDICES_SPECTRA_DATA_INPUT_HW_ADDR_LOW    ] = ( uint32_t )((             spectraDataInputHW_OffsetToBaseAddress      )       );
            param[ AFA_PARAM_INDICES_SPECTRA_DATA_INPUT_HW_ADDR_HIGH   ] = ( uint32_t )((( uint64_t ) spectraDataInputHW_OffsetToBaseAddress      ) >> 32 );
            param[ AFA_PARAM_INDICES_SPECTRA_DATA_WS_HW_ADDR_LOW       ] = ( uint32_t )((             spectraDataWorkingSetHW_OffsetToBaseAddress )       );
            param[ AFA_PARAM_INDICES_SPECTRA_DATA_WS_HW_ADDR_HIGH      ] = ( uint32_t )((( uint64_t ) spectraDataWorkingSetHW_OffsetToBaseAddress ) >> 32 );
            param[ AFA_PARAM_INDICES_SPECTRA_DATA_INDEX_LIST_ADDR_LOW  ] = ( uint32_t )((             pSpectraIndexList_OffsetToBaseAddress       )       );
            param[ AFA_PARAM_INDICES_SPECTRA_DATA_INDEX_LIST_ADDR_HIGH ] = ( uint32_t )((( uint64_t ) pSpectraIndexList_OffsetToBaseAddress       ) >> 32 );

            // ==================================================
            //
            // call HW calculation
            //
            // ==================================================

            printf( "." );fflush(stdout);    // print out a dot
            rc = AFAProcess_HW(
                param,								// whole block ram used
                baseAddr
                );
#if 0
            {
                printf( "after\n" );fflush(stdout);
                rv = 0;

 
                printf("\n"); 
                for ( yp=0;yp<gridSize;yp++ )
                {
                    printf("\t{");
                    for ( xp=0;xp<gridSize;xp++ )
                    {
                        idx = AFAGetSpectraIndexNew( xp,yp );
                        idx = ( idx < 0 ) ? -1 : idx;
                        if ( idx != golden_data[ yp ][ xp ])
                        {
                            rv = 1000000 + xp + yp * gridSize;

                            if ( idx < 0 )
                                printf("* -1* ");
                            else
                                printf("*%3d* ", idx);
                        }
                        else
                        {
                            if ( idx < 0 )
                                printf("  -1, ");
                            else
                                printf(" %3d, ", idx);
                        }
                    }
                    printf("},\n");
                }
                printf("\n");fflush(stdout);
            }

#endif
            if ( !rc )
            {
                currentStep++;
            }
        }
    } while ( !rc );

    rv = 0;
    if ( 1 )
    {
        // print results
        gridSize = AFACalcGridSize(numSpectra);

        printf("\nFinal result !\n");
        for ( yp=0;yp<gridSize;yp++ )
        {
            printf("\t{");
            for ( xp=0;xp<gridSize;xp++ )
            {
                idx = AFAGetSpectraIndexNew( xp,yp );
                idx = ( idx < 0 ) ? -1 : idx;
                idx_golden = golden_data[ yp ][ xp ];
                if ( idx != idx_golden )
                {
                    rv = 1000000 + xp + yp * gridSize;

                    if ( idx < 0 )
                        printf("* -1* ");
                    else
                        printf("*%3ld* ", idx);
                }
                else
                {
                    if ( idx < 0 )
                        printf("  -1, ");
                    else
                        printf(" %3ld, ", idx);
                }
            }
            printf("},\n");
        }
        printf("\n");
    }

    AFAHelperStructures_MemFree();

    if ( rv > 0 )
    {
        printf( "ERROR!!!: %d\n", rv - 1000000 );
    }
    else
    {
        printf( "==> Error free !!!\n" );
#ifdef JSCDBG_ACCEPT_LITTLE_INACCURACIES
        printf( "    ==> Remember we accept little inaccuracies !!!\n" );
#endif
    }

    return rv;
}

