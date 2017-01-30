#ifndef DRIVER_AFA_PROCESSING_H__
#define DRIVER_AFA_PROCESSING_H__

// must be the first include everywhere - this basic information is needed
#include "AFAConfig.h"

#ifdef AFA_RUN_ON_XILINX_SDK
#ifdef AFA_RUN_PROCESSHW_HW_INTERFACE_AP

#include "AFATypes.h"
#include "DriverInterruptController.h"

#include "xparameters.h"
#include <xuartlite.h>
#include <xintc.h>

#include "xafaprocess_hw.h"

void DriverAFAProcessingInterruptServiceRoutine( void *interruptParameter );
void DriverAFAProcessingInit();

#else 	// AFA_RUN_PROCESSHW_HW_INTERFACE_AP

void DriverAFAProcessingInit();

#endif	// AFA_RUN_PROCESSHW_HW_INTERFACE_AP
#endif	// AFA_RUN_ON_XILINX_SDK
#endif  // #ifndef DRIVER_AFA_PROCESSING_H__
