#include "AFAConfig.h"

#ifdef AFA_RUN_ON_XILINX_SDK
#include "AFATypes.h"
#include "DriverError.h"
#include "DriverTimer.h"

#include "xparameters.h"
#include "xil_io.h"
#include "xtmrctr.h"
#include "xtmrctr_i.h"

#include <xintc.h>

#include "DriverInterruptController.h"

#define TIMER_CNTR_0			0
#define RESET_VALUE	 			JSC_INTERRUPT_PERIOD_1MS

//extern XIntc DriverInterruptControllerHandle; // Instance of the Interrupt Controller
XTmrCtr DriverTimerHandle; /* The instance of the timer counter */

static uint32_t countMSecSinceStart = 0;

uint32_t
DriverTimerGetMSec()
{
	return countMSecSinceStart;
}

uint32_t
DriverTimerReadCounter()
{
	u32 TimerCount = 0;
	u8 TmrCtrNumber = 0;

	TimerCount = XTmrCtr_ReadReg(
		DriverTimerHandle.BaseAddress,
		TmrCtrNumber,
		XTC_TCR_OFFSET);
	return ( uint32_t ) TimerCount;
}

void
DriverTimerInterruptHandlerDefault(
	void *CallbackRef,
	u8 TmrCtrNumber )
{
	countMSecSinceStart++;
}


void
DriverTimerSet(
	uint32_t timerCounter,
	uint32_t resetValue )
{
	XTmrCtr_SetHandler(
		&DriverTimerHandle,
		DriverTimerInterruptHandlerDefault,
		&DriverTimerHandle );
	XTmrCtr_SetOptions(
		&DriverTimerHandle,
		timerCounter,
		XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION );
	XTmrCtr_SetResetValue(
		&DriverTimerHandle,
		timerCounter,
		resetValue );
	XTmrCtr_Start(
		&DriverTimerHandle,
		timerCounter);
}

uint32_t
DriverTimerInit()
{
	int Status;
	u8 TmrCtrNumber = 0;

	/*
	 * Initialize the TmrCtr driver so that it is ready to use
	 */
	Status = XTmrCtr_Initialize(
		&DriverTimerHandle,
		XPAR_AXI_TIMER_0_DEVICE_ID );
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	Status = XTmrCtr_SelfTest(
		&DriverTimerHandle,
		TmrCtrNumber);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	Status = DriverInterruptControllerRegister(
		XPAR_INTC_0_TMRCTR_0_VEC_ID,
		( XInterruptHandler )XTmrCtr_InterruptHandler,
		( void * ) &DriverTimerHandle );

	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	DriverTimerSet(
		0,
		DRIVER_TIMER_RELOAD_FOR_INTERRUPT_PERIOD_1MS );

	return EC_SUCCESS;
}

void
DriverTimerDeInit()
{
	/*
	 * Disable the interrupt for the timer counter
	 */
	DriverInterruptControllerDeRegister(
		XPAR_INTC_0_TMRCTR_0_VEC_ID );
}

#endif
