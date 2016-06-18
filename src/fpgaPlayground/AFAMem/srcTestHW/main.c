#include "AFAConfig.h"
#include "AFADefines.h"
#include "AFATypes.h"
#include "AFAProcessing.h"
#include "AFATime.h"

#ifdef AFA_RUN_ON_XILINX_SDK
// BSP Includes
#include "xparameters.h"
#include "xil_cache.h"
#include "xtmrctr.h"

// Driver includes
#include "DriverInterrupt.h"
#include "DriverTimer.h"
#include "DriverAFAProcessing.h"
#endif

#include "BoardIO.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef AFA_RUN_ON_XILINX_SDK
void
TestClockFunction()
{
    clock_t timeIterationStart;
    clock_t timeIterationEnd;
    sint32_t numSecs = 10;

    timeIterationStart = clock();
	while( numSecs >= 0 )
	{
		timeIterationEnd = clock();
		if (((( double )( timeIterationEnd - timeIterationStart )) / (( double ) CLOCKS_PER_SEC ) )>= 1.0 )
		{
			printf( "t=%ld\n", numSecs );
			timeIterationStart = timeIterationEnd;
			numSecs--;
		}
	}
}

void AssertCallback(
	const char8 *File,
	s32 Line )
{
	printf( "ASSERTION: File=%s, Line=%ld\n", File, Line );
	LEDRGBSet( 0, EVAL_BOARD_LEDRGB_RED );		// min/max state
}
#endif

#ifdef AFA_RUN_PROCESSHW_HW
uint32_t *param = ( uint32_t * ) XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR;
#else
uint32_t param[ 256 ];
#endif

int main(
    int argc,
    char* argv[])
{
    uint32_t *baseAddr = NULL;  // that's a dummy address pointing to the start of work area of HW in memory
    int rv = 0;
    bool_t rc;

    uint32_t testSize = 16 * 1024 * 1024;
    uint64_t inputAddressHW_OffsetToBaseAddress  = (uint32_t)malloc( testSize );
    uint64_t outputAddressHW_OffsetToBaseAddress = (uint32_t)malloc( testSize );
	uint32_t someSize = testSize / sizeof( uint32_t);;

	void *outputAddressSW = ( void * )outputAddressHW_OffsetToBaseAddress;

    clock_t timeGlobalStart;
    clock_t timeGlobalEnd;
    clock_t timeIterationStart;
    clock_t timeIterationEnd;

    // processor and other HW preparations --- start ------------------------------------------------------

#ifdef AFA_RUN_ON_XILINX_SDK
    Xil_ICacheEnable();
//	Xil_DCacheEnable();

    Xil_AssertSetCallback( AssertCallback );

    DriverInterruptInit();
    DriverTimerInit();
    DriverAFAProcessingInit();
	DriverInterruptEnable();
	LEDInit();

	TestClockFunction();
#endif

	// processor and other HW preparations --- end --------------------------------------------------------

	printf( "Starting main() ...\n" );
    LEDRGBSet( 0, EVAL_BOARD_LEDRGB_GREEN );		// power on
    if ( !AFATypesOK())
    {
        printf( "Error with AFATypes.h\n" );
        exit( 1 );
    }

//    memset( outputAddressSW, 0xdc, someSize * sizeof( uint32_t ));

	// ==================================================
	//
	// prepare parameter block
	//
	// ==================================================

	param[ AFA_PARAM_INDICES_SIZE                 ] = someSize;
	param[ AFA_PARAM_INDICES_INPUT_HW_ADDR_LOW    ] = ( uint32_t )((             inputAddressHW_OffsetToBaseAddress  )       );
	param[ AFA_PARAM_INDICES_INPUT_HW_ADDR_HIGH   ] = ( uint32_t )((( uint64_t ) inputAddressHW_OffsetToBaseAddress  ) >> 32 );
	param[ AFA_PARAM_INDICES_OUTPUT_HW_ADDR_LOW   ] = ( uint32_t )((             outputAddressHW_OffsetToBaseAddress )       );
	param[ AFA_PARAM_INDICES_OUTPUT_HW_ADDR_HIGH  ] = ( uint32_t )((( uint64_t ) outputAddressHW_OffsetToBaseAddress ) >> 32 );

	// ==================================================
	//
	// call HW calculation
	//
	// ==================================================

	printf( "* HW Test starts ...\n" );
	timeGlobalStart = clock();
	timeIterationStart = clock();
	rc = AFAProcess_HWWrapper(
		param,                              // whole block ram used
		baseAddr,
		AFA_WRAPPER_WORK_MODE_HW
		);
	timeIterationEnd = clock();
    printf( "InterationTime: %ld [%7.2fsec.]\n",
    		timeIterationEnd - timeIterationStart,
		(( double )( timeIterationEnd - timeIterationStart )) / (( double ) CLOCKS_PER_SEC ));
    timeGlobalEnd = clock();
    printf( "GlobalTime: %ld [%7.2fsec.]\n",
		timeGlobalEnd - timeGlobalStart,
		(( double )( timeGlobalEnd - timeGlobalStart )) / (( double ) CLOCKS_PER_SEC ));
	printf( "* HW Test ends ...\n" );

	printf( "Ending main() ...\n" );

    return rv;
}
