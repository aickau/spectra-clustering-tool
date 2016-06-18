#include "AFAConfig.h"

#ifdef AFA_RUN_ON_XILINX_SDK
// Xilinx includes
#include "xafaprocess_hw.h"
#include "xparameters.h"

#include "BoardIO.h"
#endif

// project specific includes
#include "AFATypes.h"
#include "AFADefines.h"
#include "AFAProcessing.h"

// standard includes
#include <stdio.h>
#include <string.h>

#ifdef AFA_RUN_ON_XILINX_SDK
XAfaprocess_hw gXAfaprocess_hwInstancePtr;
#endif

bool_t
AFAProcess_HWWrapper(
    volatile uint32_t param[ 256 ],     // whole block ram used
    volatile uint32_t *baseAddr,        // default starting address in memory
    uint32_t workMode
    )
{
    uint32_t error;
    bool_t rv;

    printf( "AFAProcess_HWWrapper() - Start\r\n" );

    if ( AFA_WRAPPER_WORK_MODE_HW == workMode )
    {
        printf( "* workMode: AFA_WRAPPER_WORK_MODE_HW\r\n" );
#ifdef AFA_RUN_PROCESSHW_HW
        printf( "* AFA_RUN_PROCESSHW_HW\r\n" );
        LEDRGBSet( 0, EVAL_BOARD_LEDRGB_WHITE );        // processing state

        XAfaprocess_hw_Set_baseAddr( &gXAfaprocess_hwInstancePtr, baseAddr );

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
        printf( "* else AFA_RUN_PROCESSHW_HW\r\n" );

        error = AFAProcess_HW(
            param,              // whole block ram used
            baseAddr            // default starting address in memory
            );
#endif
    }
    else
    {
        printf( "* workMode: AFA_WRAPPER_WORK_MODE_SW\r\n" );

        error = AFAProcess_HW(
            param,              // whole block ram used
            baseAddr            // default starting address in memory
            );
    }
    rv = FALSE;
    printf( "AFAProcess_HWWrapper() - End(0x%8.8lx)\r\n", error );
    return rv;
}
