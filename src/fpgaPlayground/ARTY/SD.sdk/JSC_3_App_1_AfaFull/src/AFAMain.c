#if 0
#include <string.h>

#include "AFATypes.h"

#include "xafaprocess_hw.h"
#include "xparameters.h"

#define EXTMEM_C0_64BIT_BASEADDR						( 0x040000000 )
#define EXTMEM_C0_64BIT_SIZE							( 0x040000000 )	// 1GB
#define EXTMEM_C1_64BIT_BASEADDR						( 0x080000000 )
#define EXTMEM_C1_64BIT_SIZE							( 0x040000000 )	// 1GB

#define EXTMEM_C0_32BIT_WINDOW_BASE						( 0x40000000 )
#define EXTMEM_C0_32BIT_WINDOW_SIZE						( 0x40000000 )	// 1GB
#define EXTMEM_C1_32BIT_WINDOW_BASE						( 0x80000000 )
#define EXTMEM_C1_32BIT_WINDOW_SIZE						( 0x40000000 )	// 1GB

#define DEBUG_SEARCHSTEP								( 0x00010000 )

#define SETMEM_WRITE_SIZE								( 100 )
#define SETMEM_WRITE_OFFSET								( 0x10000000 )
#define SETMEM_BUSMASTER_ADDRESS_START					( EXTMEM_C0_64BIT_BASEADDR    + SETMEM_WRITE_OFFSET )
#define SETMEM_MICROBLAZE_ADDRESS_START					( EXTMEM_C0_32BIT_WINDOW_BASE + SETMEM_WRITE_OFFSET )

XAfaprocess_hw gXAfaprocess_hwInstancePtr;

unsigned char *someMem = ( unsigned char * ) SETMEM_MICROBLAZE_ADDRESS_START;
unsigned char *memMonitor = ( unsigned char * ) NULL;

#define AFA_PARAMBLOCK_STARTADDRESS			( 0xc0000000 )
#define AFA_PARAMBLOCK_SIZE					( 4 * 1024 )

volatile uint32_t *paramBlock = ( uint32_t * ) ( AFA_PARAMBLOCK_STARTADDRESS );

void
AfaTestApp()
{
	return;
	uint32_t error;
	uint32_t ii;
    xil_printf( "AfaTestApp() - Start\r\n" );

#if 0
    int error = 0;
    int clean;

    if ( gEnableClearC1 )
    {
    	xil_printf( "Clear C1\r\n" );
    	memset(( void * )( EXTMEM_C1_32BIT_WINDOW_BASE ), 0xaa, EXTMEM_C1_32BIT_WINDOW_SIZE );
    }

    xil_printf( "Parameter:\r\n" );
    xil_printf( "64-Bit Bus: C0: 0x%8.8x_%8.8x\r\n", ( uint32_t )(( uint64_t )EXTMEM_C0_64BIT_BASEADDR >> 32 ), EXTMEM_C0_64BIT_BASEADDR & 0xffffffff );
    xil_printf( "            C1: 0x%8.8x_%8.8x\r\n", ( uint32_t )(( uint64_t )EXTMEM_C1_64BIT_BASEADDR >> 32 ), EXTMEM_C1_64BIT_BASEADDR & 0xffffffff );
    xil_printf( "32-Bit Bus: C0: 0x%8.8x_%8.8x\r\n", 0, EXTMEM_C0_32BIT_WINDOW_BASE );
    xil_printf( "            C1: 0x%8.8x_%8.8x\r\n", 0, EXTMEM_C1_32BIT_WINDOW_BASE );

    xil_printf( "Clear area:        0x%8.8x - 0x%8.8x\r\n", SETMEM_MICROBLAZE_ADDRESS_START, SETMEM_MICROBLAZE_ADDRESS_START + SETMEM_WRITE_SIZE - 1 );
    for ( int idx = 0; idx < SETMEM_WRITE_SIZE; idx++ )
    {
    	someMem[ idx ] = 0;
    }
    xil_printf( "Config BUS-Master: 0x%8x - 0x%8.8x\r\n", ( uint32_t )(( uint64_t )SETMEM_BUSMASTER_ADDRESS_START >> 32 ), SETMEM_BUSMASTER_ADDRESS_START & 0xffffffff );
#endif

    XAfaprocess_hw_Set_baseAddr( &gXAfaprocess_hwInstancePtr, 0x00000000 );

    memset(( void * ) paramBlock, 0x7a, AFA_PARAMBLOCK_SIZE );
#if 1
    paramBlock[ 0 ] = 0x00000000;

    XAfaprocess_hw_Start( &gXAfaprocess_hwInstancePtr );
    while ( !XAfaprocess_hw_IsDone( &gXAfaprocess_hwInstancePtr ));
    error = ( uint32_t )XAfaprocess_hw_Get_return( &gXAfaprocess_hwInstancePtr );
    xil_printf( "Test AXI Master Afaprocess_hw() result: %8.8x [%d]\r\n", error,error );
#endif
    for ( ii = 0; ii < AFA_PARAMBLOCK_SIZE / sizeof( paramBlock[ 0 ]); ii += 4 )
    {
    	xil_printf( "0x%8.8x: %8.8x, %8.8x, %8.8x, %8.8x\r\n",
    			0x80000000 + ii * 4,
    			paramBlock[ ii + 0 ],
    			paramBlock[ ii + 1 ],
    			paramBlock[ ii + 2 ],
    			paramBlock[ ii + 3 ]
    			);
    }
#if 0
    xil_printf( "Show area:         0x%8.8x - 0x%8.8x\r\n", SETMEM_MICROBLAZE_ADDRESS_START, SETMEM_MICROBLAZE_ADDRESS_START + SETMEM_WRITE_SIZE - 1 );
    xil_printf( "someMem addr:                   0x%8.8x\r\n", ( uint32_t )someMem );
    for ( int idx = 0; idx < SETMEM_WRITE_SIZE; idx++ )
    {
    	xil_printf( "Value someMem[%d] = %d\r\n", idx, someMem[ idx ]);
    }

    xil_printf( "Search value in window C0:\r\n" );
    for ( uint32_t idx = EXTMEM_C0_32BIT_WINDOW_BASE; ( idx >= ( uint32_t )EXTMEM_C0_32BIT_WINDOW_BASE ) && ( idx < ( uint32_t )(( uint32_t )EXTMEM_C0_32BIT_WINDOW_BASE + ( uint32_t )EXTMEM_C0_32BIT_WINDOW_SIZE )); idx += DEBUG_SEARCHSTEP )
    {
    	char p = memMonitor[ idx ];
    	if ( 55 == p )
    	{
    		clean = 1;
    		xil_printf( "0x%8.8x = %d\r\n    ", idx, memMonitor[ idx ]);
    	    for ( uint32_t i = 0; i < 10; i++ )
    	    {
    	    	if ( 55 != memMonitor[ idx + i ])
    	    	{
    	    		clean = 0;
    	    	}
        		xil_printf( "%d, ", memMonitor[ idx + i ]);
    	    }
    		xil_printf( "\r\n" );
    		if ( clean )
    		{
        		xil_printf( "success#1!!!!\r\n" );
        		for(;;);
    		}
    	}
    }

    xil_printf( "Search value in window C1:\r\n" );
    for ( uint32_t idx = EXTMEM_C1_32BIT_WINDOW_BASE; ( uint32_t )( idx >= ( uint32_t )EXTMEM_C1_32BIT_WINDOW_BASE ) && ( idx < ( uint32_t )(( uint32_t )EXTMEM_C1_32BIT_WINDOW_BASE + ( uint32_t )EXTMEM_C1_32BIT_WINDOW_SIZE )); idx += DEBUG_SEARCHSTEP )
    {
    	char p = memMonitor[ idx ];
    	if ( 55 == p )
    	{
    		clean = 1;
    		xil_printf( "0x%8.8x = %d\r\n    ", idx, memMonitor[ idx ]);
    	    for ( uint32_t i = 0; i < 10; i++ )
    	    {
    	    	if ( 55 != memMonitor[ idx + i ])
    	    	{
    	    		clean = 0;
    	    	}
        		xil_printf( "%d, ", memMonitor[ idx + i ]);
    	    }
    		xil_printf( "\r\n" );
    		if ( clean )
    		{
        		xil_printf( "success#2!!!!\r\n" );
        		for(;;);
    		}
    	}
    }
#endif
    xil_printf( "AfaTestApp() - Stop\r\n" );
}
#endif
