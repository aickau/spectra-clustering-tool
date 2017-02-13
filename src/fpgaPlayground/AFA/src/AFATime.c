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

void sleep(
	uint32_t sleepTime )
{
	clock_t t = clock();
	uint32_t h = ( uint32_t ) t;
	if ( h > (( uint32_t ) ( 0 - sleepTime )))
	{
		h = ( uint32_t ) ( sleepTime - (( uint32_t )( 0 - h )));
	}
	else
	{
		h = h + sleepTime;
	}
	while ( h != (( uint32_t )clock()));
}
#endif

