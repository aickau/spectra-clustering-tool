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
#include "DriverInterruptController.h"
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
    sint32_t numSecs = 3;

    timeIterationStart = clock();
    if ( timeIterationStart )
    {
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
    else
    {
    	printf( "timer not working (not initialized?)\n" );
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
#else
#endif

int
main(
    int argc,
    char* argv[])
{
    uint32_t *baseAddr = NULL;  // that's a dummy address pointing to the start of work area of HW in memory
    int rv = 0;
    uint32_t *param;
    uint32_t paramIndexIntoMem = 0;

    uint32_t testSize = 64 * 1024 * 1024;
    uint64_t inputAddressHW_OffsetToBaseAddress;
    uint64_t outputAddressHW_OffsetToBaseAddress;
	uint32_t someSize = testSize / sizeof( uint32_t);;

	void *outputAddressSW = ( void * )outputAddressHW_OffsetToBaseAddress;

    clock_t timeGlobalStart;
    clock_t timeGlobalEnd;
    clock_t timeIterationStart;
    clock_t timeIterationEnd;

	printf( "Starting main() ...\n" );

	// processor and other HW preparations --- start ------------------------------------------------------

#ifdef AFA_RUN_ON_XILINX_SDK
    Xil_ICacheEnable();
	Xil_DCacheEnable();

    Xil_AssertSetCallback( AssertCallback );

    DriverInterruptControllerInit();
    DriverTimerInit();
    DriverAFAProcessingInit();
    DriverInterruptControllerEnable();
	LEDInit();

	TestClockFunction();
#endif

	// processor and other HW preparations --- end --------------------------------------------------------

    LEDRGBSet( 0, EVAL_BOARD_LEDRGB_GREEN );		// power on
    if ( !AFATypesOK())
    {
        printf( "Error with AFATypes.h\n" );
        exit( 1 );
    }

    inputAddressHW_OffsetToBaseAddress  = (uint32_t)malloc( testSize );
    Xil_AssertVoid( NULL != inputAddressHW_OffsetToBaseAddress );
    outputAddressHW_OffsetToBaseAddress = (uint32_t)malloc( testSize );
    Xil_AssertVoid( NULL != outputAddressHW_OffsetToBaseAddress );

    param = ( uint32_t * )malloc( 256 * sizeof( uint32_t ));

    printf( "* Initialize memory block 'input'\n" );
    memset( inputAddressHW_OffsetToBaseAddress,  0xdc, someSize * sizeof( uint32_t ));
    printf( "* Initialize memory block 'output'\n" );
    memset( outputAddressHW_OffsetToBaseAddress, 0x67, someSize * sizeof( uint32_t ));
    printf( "* Initialize memory block 'param'\n" );
    memset( param,                               0xe3, 256      * sizeof( uint32_t ));

	// ==================================================
	//
	// prepare parameter block
	//
	// ==================================================
    paramIndexIntoMem = ( uint32_t )param;
    paramIndexIntoMem /= sizeof( uint32_t );

//someSize=1*1024*1024;
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
	AFAProcess_HWWrapper(
		baseAddr,
		paramIndexIntoMem,
		AFA_WRAPPER_WORK_MODE_HW
		);
	timeIterationEnd = clock();
    printf( "IterationTime: %ld [%7.2fsec.]\n",
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
