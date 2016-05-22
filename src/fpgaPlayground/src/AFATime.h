#ifndef AFA_TIME_H__
#define AFA_TIME_H__

#include "AFAConfig.h"
#include "AFATypes.h"

#ifdef AFA_RUN_ON_VISUAL_STUDIO
#include <time.h>
#else

typedef uint32_t
	clock_t;

clock_t clock();

#define CLOCKS_PER_SEC	(( 1000000000.0f / 3.0f ) / 2.0f / 2.0f ) /* DDR3 clock is 333.33333Mhz, AXI Bus is 1/4 of it */

#endif

#endif
