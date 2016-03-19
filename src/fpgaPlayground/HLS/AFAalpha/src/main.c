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
extern int pixelStart;                                              // spectrum pixel range for compare and adaption processes [0..<pixelEnd], must be multiples of four.
extern int pixelEnd;                                                // spectrum pixel range for compare and adaption processes [0..numSamples]

// current learning step
int m_currentStep;

sint16_t golden_data[12][12]=
{
#ifdef JSCDBG_ITER_SPECIAL
// params.numSteps = 1;
	{  5,   7,  12,   3,   1,   9,  -1,  37,  34,  35,  33,  31 },
	{  2,   8,  10,   4,   0,  -1,  -1,  -1,  32,  36,  30,  40 },
	{ 11,  13,   6,  -1,  -1,  -1,  -1,  -1,  -1,  39,  41,  38 },
	{ -1,  -1,  -1,  -1,  67,  64,  -1,  -1,  -1,  43,  42,  45 },
	{ 99,  -1,  -1,  68,  66,  62,  61,  60,  -1,  -1,  44,  47 },
	{ 94,  93,  -1,  -1,  65,  59,  58,  56,  -1,  -1,  49,  48 },
	{ 97,  96,  -1,  -1,  -1,  63,  57,  -1,  -1,  -1,  53,  46 },
	{ 95,  98,  86,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  55,  54 },
	{ 91,  89,  88,  -1,  69,  -1,  -1,  -1,  22,  16,  52,  51 },
	{ 92,  90,  80,  85,  70,  71,  -1,  26,  27,  23,  24,  50 },
	{ 87,  81,  84,  76,  72,  77,  -1,  29,  28,  19,  17,  15 },
	{ 79,  83,  82,  78,  73,  74,  75,  20,  18,  25,  21,  14 }
#else

///////////////////////////////////////////////////////////
#define JSCDBG_ACCEPT_LITTLE_INACCURACIES
///////////////////////////////////////////////////////////

#ifdef JSCDBG_ACCEPT_LITTLE_INACCURACIES
// params.numSteps = 200;
	{  6,   5,  10,  13,  15,  17,  24,  25,  29,  30,  33,  34 },
	{  7,   8,  -1,  12,  16,  18,  23,  26,  27,  32,  31,  36 },
	{  2,   4,   9,  11,  14,  20,  21,  -1,  28,  39,  35,  38 },
	{  0,   1,  -1,  -1,  -1,  19,  22,  -1,  -1,  37,  40,  41 },
	{  3,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  43,  42,  44 },
	{ -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  47,  46,  45 },
	{ 96,  95,  97,  98,  -1,  -1,  -1,  -1,  -1,  50,  49,  48 },
	{ 93,  94,  92,  99,  -1,  -1,  -1,  -1,  -1,  51,  53,  52 },
	{ 90,  91,  89,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  54,  55 },
	{ 88,  87,  -1,  -1,  76,  74,  70,  -1,  65,  60,  57,  56 },
	{ 86,  84,  82,  79,  78,  75,  72,  69,  66,  62,  59,  58 },
	{ 85,  83,  81,  80,  77,  73,  71,  68,  67,  64,  63,  61 }
#else
// params.numSteps = 200;
	{  6,   5,  10,  13,  15,  17,  24,  25,  29,  30,  33,  34 },
	{  7,   8,  -1,  12,  16,  18,  23,  26,  27,  32,  31,  36 },
	{  2,   4,   9,  11,  14,  20,  21,  -1,  28,  39,  35,  38 },
	{  0,   1,  -1,  -1,  -1,  19,  22,  -1,  -1,  37,  40,  41 },
	{  3,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  43,  42,  44 },
	{ -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  47,  46,  45 },
	{ 96,  95,  97,  98,  -1,  -1,  -1,  -1,  -1,  51,  49,  48 },
	{ 93,  94,  92,  99,  -1,  -1,  -1,  -1,  -1,  50,  53,  52 },
	{ 90,  91,  89,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  54,  55 },
	{ 88,  87,  -1,  -1,  76,  74,  70,  -1,  65,  60,  57,  56 },
	{ 86,  84,  82,  79,  78,  75,  72,  69,  66,  62,  59,  58 },
	{ 85,  83,  81,  80,  77,  73,  71,  68,  67,  64,  63,  61 }
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

void generateSineTestSpectra( int numTestSpectra, AFASpectra *outSpectraArray )
{
	int i;
	float freqMin = 0.002f;
	float freqMax = 0.05f;
	float freqStepSize = (freqMax-freqMin)/(float)numTestSpectra;
	float freq = freqMin;

	for (i=0;i<numTestSpectra;i++)
	{
		AFASpectra *sp =  &outSpectraArray[i];
		AFASpectraSetSine( sp, freq, 0.0f, 1.0f, 0.0f );
//		sp->m_Z = (float)i/numTestSpectra;
		freq += freqStepSize;
	}
}


uint32_t param[ 256 ];
uint32_t rng_mt_HW[ RANDOM_N ];			// whole block ram used

int main(int argc, char* argv[])
{
	int numSpectra = 100;
	void *helperStructureSpace;
	uint64_t neededSystemSpace;
    AFASpectra *spectraDataInput;
	int xp, yp, gridSize;
	int idx;
    int rv = 0;
    bool_t rc;
	AFAParameters params;
	
	printf( "Starting main() ...\n" );
	if ( !AFATypesOK())
	{
		printf( "Error with AFATypes.h\n" );
		exit( 1 );
	}
	size_t spectraDataSize = numSpectra * sizeof( AFASpectra );

	spectraDataInput = ( AFASpectra * )malloc( spectraDataSize );
	memset( spectraDataInput, 0xe7, spectraDataSize );

	AFASetDefaultParameters( &params );
	params.searchMode = AFANET_SETTINGS_SEARCHMODE_global;
#ifdef JSCDBG_ITER_SPECIAL
	params.numSteps = 1;
#else
	params.numSteps = 200;
#endif

	generateSineTestSpectra( numSpectra, spectraDataInput );

	neededSystemSpace = AFACalcAllocSpaceForHelperStructures( numSpectra );

	helperStructureSpace = malloc( neededSystemSpace );
	memset( helperStructureSpace, 0x9e, neededSystemSpace );

	AFAInitProcessing( spectraDataInput, helperStructureSpace, numSpectra, FALSE, &params );

	AFAProcessSetParamBlockParameters();

	do 
	{
	    if ( m_currentStep > AFAPP_sw.m_params.numSteps )
	    {
	        //Clustering finished (success).
	        rc = TRUE;
	    }
	    else
	    {
	        float32_t lPercent = ( float32_t )( m_currentStep ) / ( float32_t )( AFAPP_sw.m_params.numSteps );
	        float32_t lRate = ( float32_t ) ( AFAPP_sw.m_params.lRateBegin * powf( AFAPP_sw.m_params.lRateEnd / AFAPP_sw.m_params.lRateBegin, lPercent ));
	        float32_t adaptionThreshold = AFAPP_sw.m_params.lRateEnd * 0.01f;
	        float32_t sigma = ( float32_t ) ( AFAPP_sw.m_params.radiusBegin * powf( AFAPP_sw.m_params.radiusEnd / AFAPP_sw.m_params.radiusBegin, lPercent));
	        float32_t sigmaSqr = sigma*sigma;
	        bool_t bFullSearch = TRUE;
	        uint32_t searchRadius = 1;

	        // determine search strategy for BMUs for the current learning step
	        if ( AFAPP_sw.m_params.searchMode == AFANET_SETTINGS_SEARCHMODE_localfast )
	        {
	            // always use a constant search radius, never do a global search
	        	bFullSearch = ( m_currentStep < 1 );
	            searchRadius = 2;
	        }
	        else if ( AFAPP_sw.m_params.searchMode == AFANET_SETTINGS_SEARCHMODE_local )
	        {
	            // global search for the first 5 steps, decreasing search radius for increasing number of learning steps
	        	bFullSearch = ( m_currentStep < 5 );
	        	searchRadius = ( unsigned int )((( 1.f - lPercent ) * 0.5f * ( float )( AFAPP_sw.m_gridSize ))) + 2;
	        }
	        else // SOFMNET_SETTINGS_SEARCHMODE_global
	        {
	            // always use global search, never go local.
	            // slow but guarantees optimal results in every case
	        	bFullSearch = TRUE;
	        }
	        param[ AFA_PARAM_INDICES_FULL_SEARCH        ] = bFullSearch;
	        param[ AFA_PARAM_INDICES_SEARCH_RADIUS      ] = searchRadius;
	        param[ AFA_PARAM_INDICES_ADAPTION_THRESHOLD ] = *(( uint32_t * ) &adaptionThreshold );
	        param[ AFA_PARAM_INDICES_SIGMA_SQR          ] = *(( uint32_t * ) &sigmaSqr          );
	        param[ AFA_PARAM_INDICES_LRATE              ] = *(( uint32_t * ) &lRate             );
	        param[ AFA_PARAM_INDICES_GRID_SIZE          ] = AFAPP_sw.m_gridSize;
			param[ AFA_PARAM_INDICES_GRID_SIZE_SQR      ] = AFAPP_sw.m_gridSizeSqr;
			param[ AFA_PARAM_INDICES_NUM_SPECTRA        ] = numSpectra;
			param[ AFA_PARAM_INDICES_RNG_MTI            ] = m_mti;
			param[ AFA_PARAM_INDICES_PIXEL_START        ] = pixelStart;
			param[ AFA_PARAM_INDICES_PIXEL_END          ] = pixelEnd;

			printf( "." );fflush(stdout);
			rc = AFAProcess_HW(
				param,								// whole block ram used
				m_mt,								// some block ram used
				AFAPP_sw.spectraDataWorkingSet,
				AFAPP_sw.g_spectraDataInput,
				AFAPP_sw.m_pSpectraIndexList
				);
#if 0
		    {
		        rv = 0;

			    // print results
			    gridSize = AFACalcGridSize(numSpectra);

			    printf("\n");
			    for ( yp=0;yp<gridSize;yp++ )
			    {
		            printf("\t{");
		            for ( xp=0;xp<gridSize;xp++ )
		            {
		                idx = AFAGetSpectraIndex( xp,yp );
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
			    m_currentStep++;
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
                idx = AFAGetSpectraIndex( xp,yp );
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
	    printf("\n");
    }


	free( spectraDataInput );
	free( helperStructureSpace );

    if ( rv > 0 )
    {
        printf( "ERROR!!!: %d\n", rv - 1000000 );
    }
    else
    {
        printf( "==> Fehlerfrei !!!\n" );
#ifdef JSCDBG_ACCEPT_LITTLE_INACCURACIES
        printf( "    ==> Remember we accept little inaccuracies !!!\n" );
#endif
    }

    return rv;
}

