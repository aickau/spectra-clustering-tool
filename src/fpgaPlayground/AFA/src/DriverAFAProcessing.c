/*
 * DriverAFAProcessing.c
 *
 *  Created on: 04.06.2016
 *      Author: jesko
 */
// must be the first include everywhere - this basic information is needed
#include "AFAConfig.h"

#ifdef AFA_RUN_ON_XILINX_SDK

#ifdef AFA_RUN_PROCESSHW_HW_INTERFACE_AP

#include "AFATypes.h"
#include "DriverInterruptController.h"
#include "DriverAFAProcessing.h"

#include "xparameters.h"
#include <xuartlite.h>
#include <xintc.h>

#include "xafaprocess_hw.h"

//extern XIntc DriverInterruptControllerHandle; // Instance of the Interrupt Controller
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
		XPAR_AFAPROCESSHW_DEVICE_ID );
	if ( XST_SUCCESS != Status )
	{
		xil_printf( "XAfaprocess_hw(): Failed to initialize: XAfaprocess_hw_Initialize()!\r\n" );
	}
return;
	Status = DriverInterruptControllerRegister(
		XPAR_MICROBLAZE_0_AXI_INTC_AFAPROCESSHW_INTERRUPT_INTR,
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

	DriverInterruptControllerRegisteredEnable(
		XPAR_MICROBLAZE_0_AXI_INTC_AFAPROCESSHW_INTERRUPT_INTR );
}

void
DriverAFAProcessingEnable()
{
	XAfaprocess_hw_InterruptEnable(
		&gXAfaprocess_hwInstancePtr,
		0xFFFFFFFF );

	XAfaprocess_hw_InterruptGlobalEnable(
		&gXAfaprocess_hwInstancePtr );

	DriverInterruptControllerRegisteredEnable(
		XPAR_MICROBLAZE_0_AXI_INTC_AFAPROCESSHW_INTERRUPT_INTR );
}

void
DriverAFAProcessingDisable()
{
	XAfaprocess_hw_InterruptDisable(
		&gXAfaprocess_hwInstancePtr,
		0xFFFFFFFF );

	XAfaprocess_hw_InterruptGlobalDisable(
		&gXAfaprocess_hwInstancePtr );

	DriverInterruptControllerRegisteredEnable(
		XPAR_MICROBLAZE_0_AXI_INTC_AFAPROCESSHW_INTERRUPT_INTR );
}
#else  //AFA_RUN_PROCESSHW_HW_INTERFACE_AP
void
DriverAFAProcessingInit()
{
}
#endif //AFA_RUN_PROCESSHW_HW_INTERFACE_AP
#endif
