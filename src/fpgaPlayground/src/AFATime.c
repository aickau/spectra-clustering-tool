#include "AFAConfig.h"
#include "AFATypes.h"
#include "AFATime.h"

#ifndef AFA_RUN_ON_VISUAL_STUDIO

clock_t
clock()
{
	uint32_t tc = 0;//DriverTimerReadCounter();
	return ( clock_t ) tc;
}
#endif

