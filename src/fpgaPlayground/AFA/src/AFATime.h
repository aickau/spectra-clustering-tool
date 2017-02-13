#ifndef AFA_TIME_H__
#define AFA_TIME_H__

#include "AFAConfig.h"
#include "AFATypes.h"

#ifdef AFA_RUN_ON_VISUAL_STUDIO
#include <time.h>
#else

#ifdef AFA_RUN_ON_VIVADO_HLS_CSIM
#include <time.h>
#else

typedef uint32_t
	clock_t;

clock_t clock();
void sleep(	uint32_t sleepTime );

#define CLOCKS_PER_SEC						DRIVER_TIMER_IRQ_PER_SEC

#endif  // #ifdef AFA_RUN_ON_VIVADO_HLS_CSIM
#endif  // #ifdef AFA_RUN_ON_VISUAL_STUDIO

#endif
