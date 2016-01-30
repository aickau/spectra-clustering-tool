#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <Memory.h>
#include <math.h>

#include "HLS/AFA/include/AFATypes.h"
#include "HLS/AFA/include/AFASpectra.h"
#include "HLS/AFA/include/AFAProcessing.h"

uint32_t
ReadSpectraFileRAW(
    const char *filename,
    uint32_t *spectraCount,
    AFASpectra **spectraData )
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

void generateSineTestSpectra()
{
	const int numSpectra=10;

	const float freqMin = 0.002f;
	const float freqMax = 0.05f;
	const float freqStepSize = (freqMax-freqMin)/static_cast<float>(m_numSpectra);
	float freq = freqMin;

	for (unsigned int i=0;i<m_numSpectra;i++)
	{
		Spectra *a = _pSourceVFS->beginRead(i);
		a->setSine(freq);
		a->m_Z = static_cast<float>(i)/static_cast<float>(m_numSpectra);
		freq += freqStepSize;
		_pSourceVFS->endRead(i);
	}
}



int main(int argc, char* argv[])
{
    uint32_t numSpectra;
    AFASpectra *spectraData;





// 	uint32_t rv = ReadSpectraFileRAW(
//         "allSpectra.bin",
//         &numSpectra,
//         &spectraData );

    printf( "AFASpectraSize=%d\n", sizeof( AFASpectra ));

    unsigned int gridSize = static_cast<unsigned int>(ceilf(sqrtf((float)numSpectra+1))*1.1f); // gives a factor of 1.1^2=1.21 in total
    unsigned int gridSizeSqr = gridSize*gridSize;

    AFASpectra *net = new AFASpectra[ gridSizeSqr ];

    AFAProcessing AFAProc(
        spectraData,
        net,
        numSpectra,
        false );

    free( spectraData );
    delete [] net;
    return ( int ) rv;
}

