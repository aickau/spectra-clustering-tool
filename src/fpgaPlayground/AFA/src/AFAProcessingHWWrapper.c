#include "AFAConfig.h"

#ifdef AFA_RUN_ON_XILINX_SDK
// Xilinx includes
//#include "xafaprocess_hw.h"
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

#ifdef AFA_RUN_ON_XILINX_SDK
//XAfaprocess_hw gXAfaprocess_hwInstancePtr;
#endif

bool_t
AFAProcess_HWWrapper(
    uint32_t param[ AFA_PARAM_BLOCK_SIZE_IN_BYTES / sizeof( uint32_t )],              // whole block ram used
    volatile uint32_t *baseAddr         // default starting address in memory
    )
{
    uint32_t error;
    bool_t rv;
    uint32_t status = 0;
//    printf( "AFAProcess_HWWrapper() - Start\r\n" );

#ifdef AFA_RUN_PROCESSHW_HW
    LEDRGBSet( 0, EVAL_BOARD_LEDRGB_WHITE );        // processing state

#ifdef AFA_RUN_PROCESSHW_HW_INTERFACE_AP
    XAfaprocess_hw_Set_baseAddr( &gXAfaprocess_hwInstancePtr, 0x00000000 );
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
#endif

#ifdef AFA_RUN_PROCESSHW_HW_INTERFACE_RAW
    // set processing to STOP
    baseAddr[ AFA_PARAM_BLOCK_ADDRESS_INDEX + AFA_PARAM_INDICES_STARTSTOP ] = 0xd00fd00f;

    // prepare start, but stop module
    param[ AFA_PARAM_INDICES_STARTSTOP ] = 0xd00fd00f;

    // wait for design being IDLE
    do
    {
        status = baseAddr[ AFA_PARAM_BLOCK_ADDRESS_INDEX + AFA_PARAM_INDICES_STATUS ];
    } while ( 0x01 != ( status & 0x01 ));

    // now it's IDLE and waiting to be started

    // copy configuration with STOP code in it to prevent running before all data is copied
    memcpy(( void * ) ( &baseAddr[ AFA_PARAM_BLOCK_ADDRESS_INDEX + 2 ]), &param[ 2 ], AFA_PARAM_BLOCK_SIZE_IN_BYTES - 2 * 4 );

    // now we start the design by writing something different than 0xd00fd00f
    baseAddr[ AFA_PARAM_BLOCK_ADDRESS_INDEX + AFA_PARAM_INDICES_STARTSTOP ] = 0x12345678;

    //> hardware calculation starts here and we wait for done (interrupt is also triggered, but at first we do busy waiting)
    do
    {
        status = baseAddr[ AFA_PARAM_BLOCK_ADDRESS_INDEX + AFA_PARAM_INDICES_STATUS ];
        // busy waiting
        LEDBinaryShow( status );
    } while ( 0x02 == ( status & 0x02 ));

	memcpy( param, ( void * ) ( &baseAddr[ AFA_PARAM_BLOCK_ADDRESS_INDEX ]), AFA_PARAM_BLOCK_SIZE_IN_BYTES );
	error = status & 0x02 ? 0x00000000 /* no error */ : 0xffffffff /* something went wrong */;
    xil_printf( "Test AXI Master Afaprocess_hw() result: %8.8lx [%ld]\r\n", error,error );
#endif


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
