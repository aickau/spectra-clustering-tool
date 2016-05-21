#//define AFA_RUN_ON_HARDWARE

#ifdef AFA_RUN_ON_HARDWARE
// Xilinx includes
#include "xafaprocess_hw.h"
#include "xparameters.h"

#include "BoardIO.h"
#endif

// project specific includes
#include "AFATypes.h"
#include "AFADefines.h"
#include "AFASpectra.h"
#include "AFAProcessing.h"

// standard includes
#include <stdio.h>
#include <string.h>

//#define AFA_PARAMBLOCK_STARTADDRESS         ( 0xc0000000 )
//#define AFA_PARAMBLOCK_SIZE                 ( 4 * 256 )

#ifdef AFA_RUN_ON_HARDWARE
XAfaprocess_hw gXAfaprocess_hwInstancePtr;
#endif

bool_t
AFAProcess_HWWrapper(
    volatile uint32_t param[ 256 ],              // whole block ram used
    volatile uint32_t *baseAddr         // default starting address in memory
    )
{
    uint32_t error;
    bool_t rv;

//    printf( "AFAProcess_HWWrapper() - Start\r\n" );

#ifdef AFA_RUN_ON_HARDWARE
    LEDRGBSet( 0, EVAL_BOARD_LEDRGB_WHITE );        // processing state

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
    xil_printf( "Test AXI Master Afaprocess_hw() result: %8.8lx [%ld]\r\n", error,error );
#else
    error = AFAProcess_HW(
        param,              // whole block ram used
        baseAddr            // default starting address in memory
        );
#endif
    rv = FALSE;
//    printf( "AFAProcess_HWWrapper() - End(0x%8.8lx)\r\n", error );
    return rv;
}
