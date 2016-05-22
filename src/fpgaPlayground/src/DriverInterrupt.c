/*
 * interrupt.c
 *
 *  Created on: 08.11.2015
 *      Author: jesko
 */
#include "AFAConfig.h"

#ifdef AFA_RUN_ON_XILINX_SDK
#include "xparameters.h"
#include <xuartlite.h>
#include <xintc.h>

#include "xafaprocess_hw.h"
#include "DriverInterrupt.h"

XIntc InterruptController; // Instance of the Interrupt Controller
extern XAfaprocess_hw gXAfaprocess_hwInstancePtr;


void XAfaprocess_hw_InterruptServiceRoutine( void *interruptParameter )
{

    // lösche wieder den Interrupt:
	XAfaprocess_hw_InterruptClear( &gXAfaprocess_hwInstancePtr, 0xFFFFFFFF);

    // XIntc_mMasterDisable(XPAR_XPS_INTC_0_BASEADDR);

    xil_printf("\n\r --> XExample_InterruptServiceRoutine: Interrupt. \n\r");

    // XIntc_mMasterEnable(XPAR_XPS_INTC_0_BASEADDR);
}

void ErrorLoop()
{
	xil_printf( "ErrorLoop\r\n" );
    for ( ;; )
        ;
}

void
InterruptInit()
{
    int Status;

	// Initialize the interrupt controller driver so that it is ready to use.
	Status = XIntc_Initialize( &InterruptController, XPAR_MICROBLAZE_0_AXI_INTC_DEVICE_ID );
	if ( Status != XST_SUCCESS )
	{
		xil_printf( "failed\r\n" );
		ErrorLoop();
	}
}

void
XAfaprocess_hw_Init()
{
    int Status;
    Status = XAfaprocess_hw_Initialize( &gXAfaprocess_hwInstancePtr, XPAR_AFAPROCESS_HW_0_DEVICE_ID );
	if ( XST_SUCCESS != Status )
	{
		xil_printf( "XAfaprocess_hw(): Failed to initialize: XAfaprocess_hw_Initialize()!\r\n" );
	}

	Status = XIntc_Connect( &InterruptController, XPAR_MICROBLAZE_0_AXI_INTC_AFAPROCESS_HW_0_INTERRUPT_INTR, (XInterruptHandler)XAfaprocess_hw_InterruptServiceRoutine, ( void * )( 0x1234aabb ));
	if ( XST_SUCCESS != Status )
	{
		xil_printf( "ExampleInit(): Failed to connect to IRQ!\r\n" );
	}
}

void
InterruptEnable()
{
    int Status;

    // Initialize the exception table [nothing happens here with microblaze]
	Xil_ExceptionInit();

	// Register the BASIC-STANDARD (from BSP) interrupt controller handler with the exception table.
	// !!! Dirk doesn't need this !!!
	Xil_ExceptionRegisterHandler( XIL_EXCEPTION_ID_INT, ( Xil_ExceptionHandler ) XIntc_DeviceInterruptHandler, ( void* ) 0 /* this is CallBack reference data */);

	// Enable exceptions [microblaze_enable_interrupts();].
	Xil_ExceptionEnable();

	XAfaprocess_hw_InterruptEnable( &gXAfaprocess_hwInstancePtr, 0xFFFFFFFF );
	XAfaprocess_hw_InterruptGlobalEnable( &gXAfaprocess_hwInstancePtr );

	XIntc_Enable( &InterruptController, XPAR_MICROBLAZE_0_AXI_INTC_AFAPROCESS_HW_0_INTERRUPT_INTR );

	// Start the interrupt controller such that interrupts are enabled for all devices that cause interrupts.
	// QUESTION: Too early ???
	Status = XIntc_Start( &InterruptController, XIN_REAL_MODE );
	if ( Status != XST_SUCCESS )
	{
		xil_printf( "failed\r\n" );
		ErrorLoop();
	}
}
#endif
