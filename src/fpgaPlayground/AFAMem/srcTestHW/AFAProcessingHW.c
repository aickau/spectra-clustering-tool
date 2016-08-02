#include "AFATypes.h"
#include "AFADefines.h"

#include "AFAProcessingHW.h"

#include <float.h>
#include <math.h>
#include <string.h>

uint32_t
AFAProcess_HW(
    volatile uint32_t *baseAddr,			// default starting address in memory (normally 0x00000000)
	const uint32_t paramIndex
    )
{
#pragma HLS INTERFACE m_axi     port=baseAddr              depth=10000
#pragma HLS INTERFACE s_axilite port=paramIndex                        bundle=INTERFACE_AXILITE_SLAVE
#pragma HLS INTERFACE s_axilite port=baseAddr                          bundle=INTERFACE_AXILITE_SLAVE
#pragma HLS INTERFACE s_axilite port=return                            bundle=INTERFACE_AXILITE_SLAVE

	volatile uint32_t *param = &baseAddr[ paramIndex ];
	volatile uint32_t *psrc;
	volatile uint32_t *pdst;
	uint32_t buffer[ 1024 ];
    uint64_t inputAddressHW_OffsetToBaseAddress;
    uint64_t outputAddressHW_OffsetToBaseAddress;
	unsigned int someSize;
	uint32_t idx;
	uint32_t idx2;
	uint32_t dataWord = 3344ee00;
	
    // define the offsets to access external memory through arrays
    uint64_t inputAddressHW_IndexToMem;
    uint64_t outputAddressHW_IndexToMem;

//	for ( idx = 0; idx < 256; idx++ )
//	{
//		param[ idx ] = baseAddr[ paramIndex + idx ];
//	}

    param[ 0 ] = 0x11223345;						// address 0xc0000000 is written
	// while ( 0x11223346 != param[ 0 ])
		// baseAddr[( 0x88000080 / 4 )] = param[ 0 ];  // address 0xc0000080 is read and 0x88000080 is written !
    // param[ 10 ] = 0x55667789;

    // while ( param[ 1 ] != 2 )
		// ;
	// param[ 10 ] = 0x5566778b;

    // get data from param block
    // =========================
	someSize = param[ AFA_PARAM_INDICES_SIZE ];

    // get offsets to different memory locations as 2 32-bit words combining them to 64bit offsets
    inputAddressHW_OffsetToBaseAddress  = ( param[ AFA_PARAM_INDICES_INPUT_HW_ADDR_LOW  ] | (( uint64_t ) param[ AFA_PARAM_INDICES_INPUT_HW_ADDR_HIGH  ] << 32 ));
    outputAddressHW_OffsetToBaseAddress = ( param[ AFA_PARAM_INDICES_OUTPUT_HW_ADDR_LOW ] | (( uint64_t ) param[ AFA_PARAM_INDICES_OUTPUT_HW_ADDR_HIGH ] << 32 ));

    // these offsets are used as array indices into an uint32_t array. So divide them up by the byte width
    inputAddressHW_IndexToMem  = inputAddressHW_OffsetToBaseAddress  / sizeof( uint32_t );
    outputAddressHW_IndexToMem = outputAddressHW_OffsetToBaseAddress / sizeof( uint32_t );

	param[ AFA_PARAM_INDICES_LED1_OUTPUT ] = someSize;

	// do some memory operation
	param[ 13 ] = AFA_PARAM_INDICES_SIZE;
	param[ 14 ] = someSize;
#if 0
	// initial version: takes 10.56s (64MB)
    for ( idx = 0; idx < someSize; idx++ )
    {
		dataWord = baseAddr[ inputAddressHW_IndexToMem + idx ];
		baseAddr[ outputAddressHW_IndexToMem + idx ] = dataWord;
    }
#endif

#if 0
	// first optimized version: takes 0.37s (64MB)
    for ( idx = 0; idx < someSize; idx += 1024 )
    {
		psrc = &baseAddr[  inputAddressHW_IndexToMem + idx ];
		pdst = &baseAddr[ outputAddressHW_IndexToMem + idx ];
		memcpy(( void * )buffer, ( void * ) psrc, 1024 * sizeof( uint32_t ));
		memcpy(( void * )pdst, ( void * ) buffer, 1024 * sizeof( uint32_t ));
	}
#endif

#if 0
	// 3rd try, omit memcpy version: takes 3.54s (64MB) - mistake in write (idx2 missing)
    for ( idx = 0; idx < someSize; idx += 1024 )
    {
		for ( idx2 = 0; idx2 < 1024; ++idx2 )
		{
			buffer[ idx2 ] = baseAddr[ inputAddressHW_IndexToMem + idx + idx2 ];
		}
		for ( idx2 = 0; idx2 < 1024; ++idx2 )
		{
			baseAddr[ outputAddressHW_IndexToMem + idx ] = buffer[ idx2 ];
		}
	}
#endif

#if 0
	// 4th try, omit memcpy version: takes 1.02s (64MB)
    for ( idx = 0; idx < someSize; idx += 1024 )
    {
		// this is with burst read
		for ( idx2 = 0; idx2 < 1024; ++idx2 )
		{
			buffer[ idx2 ] = baseAddr[ inputAddressHW_IndexToMem + idx + idx2 ];
		}
		// this is with burst write
		for ( idx2 = 0; idx2 < 1024; ++idx2 )
		{
			baseAddr[ outputAddressHW_IndexToMem + idx + idx2 ] = buffer[ idx2 ];
		}
	}
#endif

#if 1
	// 5th try, omit memcpy version: takes 1.02s (64MB)
    for ( idx = 0; idx < someSize; idx += 1024 )
    {
		// this is with burst read
		psrc = &baseAddr[  inputAddressHW_IndexToMem + idx ];
		pdst = buffer;
		for ( idx2 = 0; idx2 < 1024; ++idx2 )
		{
			*pdst++ = *psrc++;
		}

		// this is with burst write
		psrc = buffer;
		pdst = &baseAddr[ outputAddressHW_IndexToMem + idx ];
		for ( idx2 = 0; idx2 < 1024; ++idx2 )
		{
			*pdst++ = *psrc++;
		}
	}
#endif
	param[ 31 ] = idx;

    // clustering not yet finished, need another learning step
	return dataWord;
}
