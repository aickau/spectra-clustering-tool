#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <Memory.h>
#include <math.h>

#include "include/AFADefines.h"
#include "include/AFATypes.h"
#include "include/AFASpectra.h"
#include "include/AFAProcessing.h"

// HW-Section
#include "include/AFAProcessingHW.h"

//#define JSCDBG_ITER_SPECIAL

sint16_t golden_data[12][12]=
{
#ifndef JSCDBG_ITER_SPECIAL
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
#else
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
#endif
};

/*
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
*/


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
		sp->m_Z = (float)i/numTestSpectra;
		freq += freqStepSize;
      sp->m_JSCIndex = i;
	}
}



int main(int argc, char* argv[])
{
	int numSpectra = 100;
	void *helperStructureSpace;
	uint64_t neededSystemSpace;
    AFASpectra *spectraData;
	int xp, yp, gridSize;
	int idx;
   int rv = 0;

	AFAParameters params;
	size_t spectraDataSize = numSpectra * sizeof( AFASpectra );

	spectraData = ( AFASpectra * )malloc( spectraDataSize );
	memset( spectraData, 0xe7, spectraDataSize );

	AFASetDefaultParameters( &params );
	params.searchMode = AFANET_SETTINGS_SEARCHMODE_global;
	params.numSteps = 200;

	generateSineTestSpectra( numSpectra, spectraData );

	neededSystemSpace = AFACalcAllocSpaceForHelperStructures( numSpectra );

	helperStructureSpace = malloc( neededSystemSpace );
	memset( helperStructureSpace, 0x9e, neededSystemSpace );

	AFAInitProcessing( spectraData, helperStructureSpace, numSpectra, FALSE, &params );

	AFAProcessSetParamBlockParameters();

	while ( !AFAProcess_HW() )
    {
		printf( "." );
#if 1
    }
    {
#else
#endif
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
                if ( idx < 0 )
	                printf(" -1, ");
                else
	                printf("%3d, ", idx);
                idx = ( idx < 0 ) ? -1 : idx;
                if ( idx != golden_data[ yp ][ xp ])
                {
                    rv = 1000000 + xp + yp * gridSize;
                }
            }
            printf("},\n");
	    }
	    printf("\n");
    }


	free( spectraData );
	free( helperStructureSpace );

    if ( rv )
    {
        printf( "ERROR!!!: %d\n", rv - 1000000 );
    }
    else
    {
        printf( "==> Fehlerfrei !!!\n" );
    }

    return rv;
}



#if 0
#include <stdio.h>
#include "AFAProcessing.h"

unsigned char someMem[ 100 ];
int main()
{
	AFAProcessing( someMem, 55, 100 );
	for ( int idx = 0; idx < 100; idx++ )
	{
		printf("Value someMem[%d]=0x%x (%d)\n", idx, someMem[ idx ], someMem[ idx ]);
	}

	return 0;
}
#endif

