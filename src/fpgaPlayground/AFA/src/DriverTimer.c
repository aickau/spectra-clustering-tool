#include "AFAConfig.h"

#ifdef AFA_RUN_ON_XILINX_SDK
#include "AFATypes.h"
#include "DriverError.h"

#include "xparameters.h"
#include "xil_io.h"
#include "xtmrctr.h"
#include "xtmrctr_i.h"

XTmrCtr TimerCounter; /* The instance of the timer counter */

uint32_t
DriverTimerReadCounter()
{
	u32 TimerCount = 0;
	u8 TmrCtrNumber = 0;
	XTmrCtr *TmrCtrInstancePtr = &TimerCounter;

	TimerCount = XTmrCtr_ReadReg(
		TmrCtrInstancePtr->BaseAddress,
		TmrCtrNumber,
		XTC_TCR_OFFSET);
	return ( uint32_t ) TimerCount;
}

uint32_t
DriverTimerInit()
{
	int Status;
	u32 TimerCount1 = 0;
	u32 TimerCount2 = 0;
	u16 Count = 0;
	u8 TmrCtrNumber = 0;
	XTmrCtr *TmrCtrInstancePtr = &TimerCounter;

	/*
	 * Initialize the TmrCtr driver so that it iss ready to use
	 */
	Status = XTmrCtr_Initialize(TmrCtrInstancePtr, XPAR_AXI_TIMER_0_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Set the Capture register to 0
	 */
	XTmrCtr_WriteReg(
		TmrCtrInstancePtr->BaseAddress,
		TmrCtrNumber,
		XTC_TLR_OFFSET,
		0);

	/*
	 * Reset the timer and the interrupt
	 */
	XTmrCtr_WriteReg(
		TmrCtrInstancePtr->BaseAddress,
		TmrCtrNumber,
		XTC_TCSR_OFFSET,
		XTC_CSR_INT_OCCURED_MASK | XTC_CSR_LOAD_MASK);

	/*
	 * Set the control/status register to enable timer
	 */
	XTmrCtr_WriteReg(TmrCtrInstancePtr->BaseAddress, TmrCtrNumber,
			  XTC_TCSR_OFFSET, XTC_CSR_ENABLE_TMR_MASK);
#if 0
	/*
	 * Read the timer
	 */
	TimerCount1 = XTmrCtr_ReadReg(
		TmrCtrInstancePtr->BaseAddress,
		TmrCtrNumber,
		XTC_TCR_OFFSET);
	/*
	 * Make sure timer is incrementing if the Count rolls over to zero
	 * and the timer still has not incremented an error is returned
	 */

	do {
		TimerCount2 = XTmrCtr_ReadReg(
			TmrCtrInstancePtr->BaseAddress,
			TmrCtrNumber,
			XTC_TCR_OFFSET);
		Count++;
	}
	while ((TimerCount1 == TimerCount2) && (Count != 0));

	/*
	 * Reset the timer and the interrupt
	 */
	XTmrCtr_WriteReg(TmrCtrInstancePtr->BaseAddress, TmrCtrNumber,
			  XTC_TCSR_OFFSET,
			  XTC_CSR_INT_OCCURED_MASK | XTC_CSR_LOAD_MASK);

	/*
	 * Set the control/status register to 0 to complete initialization
	 * this disables the timer completely and allows it to be used again
	 */

	XTmrCtr_WriteReg(TmrCtrInstancePtr->BaseAddress, TmrCtrNumber,
			  XTC_TCSR_OFFSET, 0);

	if (TimerCount1 == TimerCount2) {
		return XST_FAILURE;
	}
	else {
		return XST_SUCCESS;
	}
	return XST_SUCCESS;
#endif
	return EC_SUCCESS;
}
#endif
