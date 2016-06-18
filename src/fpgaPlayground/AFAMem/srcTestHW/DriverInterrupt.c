/*
 * interrupt.c
 *
 *  Created on: 08.11.2015
 *      Author: jesko
 */
// must be the first include everywhere - this basic information is needed
#include "AFAConfig.h"

#ifdef AFA_RUN_ON_XILINX_SDK
#include "AFATypes.h"
#include "DriverInterrupt.h"

#include "xparameters.h"
#include <xuartlite.h>
#include <xintc.h>

#include "xafaprocess_hw.h"

XIntc DriverInterruptControllerHandle; // Instance of the Interrupt Controller

bool_t
DriverInterruptInit()
{
    int Status;
    bool_t rv = TRUE;
	// Initialize the interrupt controller driver so that it is ready to use.
	Status = XIntc_Initialize(
		&DriverInterruptControllerHandle,
		XPAR_MICROBLAZE_0_AXI_INTC_DEVICE_ID );
	if ( Status != XST_SUCCESS )
	{
		xil_printf( "InterruptInit(): failed\r\n" );
		rv = FALSE;
	}
	return rv;
}

void
DriverInterruptEnable()
{
    int Status;

	// Start the interrupt controller such that interrupts are enabled for all devices that cause interrupts.
	Status = XIntc_Start(
		&DriverInterruptControllerHandle,
		XIN_REAL_MODE );
	if ( Status != XST_SUCCESS )
	{
		xil_printf( "InterruptEnable(): failed\r\n" );
	}

	/*
	 * Enable the interrupt for the timer counter
	 */
	XIntc_Enable(
		&DriverInterruptControllerHandle,
		XPAR_INTC_0_TMRCTR_0_VEC_ID);

	// Initialize the exception table [nothing happens here with microblaze]
	Xil_ExceptionInit();

	// Register the BASIC-STANDARD (from BSP) interrupt controller handler with the exception table.
	// !!! Dirk doesn't need this !!!
	Xil_ExceptionRegisterHandler(
		XIL_EXCEPTION_ID_INT,
		( Xil_ExceptionHandler ) XIntc_DeviceInterruptHandler,
		( void* ) 0 /* this is CallBack reference data */);

	// Enable exceptions [microblaze_enable_interrupts();].
	Xil_ExceptionEnable();

}
#endif
