/*
 * DriverAFAProcessing.c
 *
 *  Created on: 04.06.2016
 *      Author: jesko
 */
// must be the first include everywhere - this basic information is needed
#include "AFAConfig.h"

#ifdef AFA_RUN_ON_XILINX_SDK
#include "AFATypes.h"
#include "DriverInterrupt.h"
#include "DriverAFAProcessing.h"

#include "xparameters.h"
#include <xuartlite.h>
#include <xintc.h>

#include "xafaprocess_hw.h"

extern XIntc DriverInterruptControllerHandle; // Instance of the Interrupt Controller
XAfaprocess_hw gXAfaprocess_hwInstancePtr;

void
DriverAFAProcessingInterruptServiceRoutine(
	void *interruptParameter )
{
    // clear the interrupt flags
	XAfaprocess_hw_InterruptClear(
		&gXAfaprocess_hwInstancePtr,
		0xFFFFFFFF );

    // XIntc_mMasterDisable(XPAR_XPS_INTC_0_BASEADDR);

    xil_printf("\n\rIRQ-IRQ-IRQ: DriverAFAProcessingInterruptServiceRoutine()\n\r" );

    // XIntc_mMasterEnable(XPAR_XPS_INTC_0_BASEADDR);
}

void
DriverAFAProcessingInit()
{
    int Status;
    Status = XAfaprocess_hw_Initialize(
    	&gXAfaprocess_hwInstancePtr,
		XPAR_AFATESTHW_DEVICE_ID );
	if ( XST_SUCCESS != Status )
	{
		xil_printf( "XAfaprocess_hw(): Failed to initialize: XAfaprocess_hw_Initialize()!\r\n" );
	}

	Status = XIntc_Connect(
		&DriverInterruptControllerHandle,
		XPAR_MICROBLAZE_0_AXI_INTC_AFATESTHW_INTERRUPT_INTR,
		( XInterruptHandler )DriverAFAProcessingInterruptServiceRoutine,
		( void * )( 0x1234aabb ));
	if ( XST_SUCCESS != Status )
	{
		xil_printf( "ExampleInit(): Failed to connect to IRQ!\r\n" );
	}

	XAfaprocess_hw_InterruptEnable(
		&gXAfaprocess_hwInstancePtr,
		0xFFFFFFFF );

	XAfaprocess_hw_InterruptGlobalEnable(
		&gXAfaprocess_hwInstancePtr );

	XIntc_Enable(
		&DriverInterruptControllerHandle,
		XPAR_MICROBLAZE_0_AXI_INTC_AFATESTHW_INTERRUPT_INTR );
}

#endif
