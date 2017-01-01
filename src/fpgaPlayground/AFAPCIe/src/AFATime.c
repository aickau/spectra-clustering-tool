#include "AFAConfig.h"
#include "AFATypes.h"
#include "AFATime.h"

#include "DriverTimer.h"

#ifdef AFA_RUN_ON_XILINX_SDK

clock_t
clock()
{
	uint32_t tc = DriverTimerGetMSec();
	return ( clock_t ) tc;
}
#endif

