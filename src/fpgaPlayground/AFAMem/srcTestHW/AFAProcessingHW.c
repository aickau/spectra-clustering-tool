#include "AFATypes.h"
#include "AFADefines.h"

#include "AFAProcessingHW.h"

#include <float.h>
#include <math.h>
#include <string.h>

uint32_t
AFAProcess_HW(
    volatile uint32_t param[ 256 ],     // whole block ram used
    volatile uint32_t *baseAddr 		// default starting address in memory
    )
{
#pragma HLS RESOURCE            variable=param             core=RAM_1P_BRAM
#pragma HLS INTERFACE bram      port=param
#pragma HLS INTERFACE m_axi     port=baseAddr              depth=10000
#pragma HLS INTERFACE s_axilite port=baseAddr                          bundle=INTERFACE_AXILITE_SLAVE
#pragma HLS INTERFACE s_axilite port=return                            bundle=INTERFACE_AXILITE_SLAVE

    uint64_t inputAddressHW_OffsetToBaseAddress;
    uint64_t outputAddressHW_OffsetToBaseAddress;
	uint32_t someSize;
	uint32_t idx;
	uint32_t dataWord;
	
    // define the offsets to access external memory through arrays
    uint64_t inputAddressHW_IndexToMem;
    uint64_t outputAddressHW_IndexToMem;

    // get data from param block
    // =========================
	someSize = param[ AFA_PARAM_INDICES_SIZE ];

    // get offsets to different memory locations as 2 32-bit words combining them to 64bit offsets
    inputAddressHW_OffsetToBaseAddress  = ( param[ AFA_PARAM_INDICES_INPUT_HW_ADDR_LOW  ] | (( uint64_t ) param[ AFA_PARAM_INDICES_INPUT_HW_ADDR_HIGH  ] << 32 ));
    outputAddressHW_OffsetToBaseAddress = ( param[ AFA_PARAM_INDICES_OUTPUT_HW_ADDR_LOW ] | (( uint64_t ) param[ AFA_PARAM_INDICES_OUTPUT_HW_ADDR_HIGH ] << 32 ));

    // these offsets are used as array indices into an uint32_t array. So divide them up by the byte width
    inputAddressHW_IndexToMem  = inputAddressHW_OffsetToBaseAddress  / sizeof( uint32_t );
    outputAddressHW_IndexToMem = outputAddressHW_OffsetToBaseAddress / sizeof( uint32_t );

	// do some memory operation
    for ( idx = 0; idx < someSize; idx++ )
    {
//		param[ AFA_PARAM_INDICES_LED1_OUTPUT ] = idx;
		dataWord = baseAddr[ inputAddressHW_IndexToMem + idx ];
		baseAddr[ outputAddressHW_IndexToMem + idx ] = dataWord;
    }

    // clustering not yet finished, need another learning step
	return 0xd00fe5e1;
}
