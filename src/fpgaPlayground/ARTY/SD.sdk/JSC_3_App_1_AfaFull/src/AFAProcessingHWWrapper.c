// Xilinx includes
#include "xafaprocess_hw.h"
#include "xparameters.h"

// project specific includes
#include "AFATypes.h"
#include "AFADefines.h"
#include "AFASpectra.h"
#include "AFAProcessing.h"

#include "BoardIO.h"

// standard includes
#include <string.h>

#define AFA_PARAMBLOCK_STARTADDRESS			( 0xc0000000 )
#define AFA_PARAMBLOCK_SIZE					( 4 * 256 )

XAfaprocess_hw gXAfaprocess_hwInstancePtr;

bool_t
AFAProcess_HWWrapper(
    uint32_t param[ 256 ],              // whole block ram used
    volatile uint32_t *baseAddr 		// default starting address in memory
    )
{
	uint32_t error;
	bool_t rv;

	xil_printf( "AFAProcess_HWWrapper() - Start\r\n" );
#define AFA_RUN_ON_HARDWARE
#ifdef AFA_RUN_ON_HARDWARE
    XAfaprocess_hw_Set_baseAddr( &gXAfaprocess_hwInstancePtr, 0x00000000 );
//    memcpy(( void * ) AFA_PARAMBLOCK_STARTADDRESS, param, 256 * sizeof( uint32_t ));

    XAfaprocess_hw_Start( &gXAfaprocess_hwInstancePtr );
    //> hardware calculation starts here and we wait for done (interrupt is also triggered, but at first we do busy waiting)
    while ( !XAfaprocess_hw_IsDone( &gXAfaprocess_hwInstancePtr ))
    {
    	// busy waiting
    	LEDBinaryShow( param[ AFA_PARAM_INDICES_LED1_OUTPUT ]);
    }

    // read back return value
    error = ( uint32_t )XAfaprocess_hw_Get_return( &gXAfaprocess_hwInstancePtr );
    xil_printf( "Test AXI Master Afaprocess_hw() result: %8.8x [%d]\r\n", error,error );
    error = 0;
#else
    LEDRGBSet( 0, EVAL_BOARD_LEDRGB_WHITE );		// random state

    error = AFAProcess_HW(
        param,              // whole block ram used
        baseAddr 			// default starting address in memory
        );
    error = error == 0xd00fe5e1 ? FALSE : TRUE;
#endif
    rv = error ? TRUE : FALSE;
	xil_printf( "AFAProcess_HWWrapper() - End(0x%8.8x)\r\n", error );
    return rv;
}
