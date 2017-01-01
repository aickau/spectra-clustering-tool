#ifndef DRIVER_TIMER_H__
#define DRIVER_TIMER_H__

#include "AFATypes.h"

#define DRIVER_TIMER_TIMER_INPUT_CLOCK				    ( 100000000 )
#define DRIVER_TIMER_IRQ_PER_SEC 						( 1000 )
#define DRIVER_TIMER_RELOAD_FOR_INTERRUPT_PERIOD_1MS	(( uint32_t )(-( DRIVER_TIMER_TIMER_INPUT_CLOCK / DRIVER_TIMER_IRQ_PER_SEC )))

uint32_t DriverTimerGetMSec();
uint32_t DriverTimerReadCounter();
uint32_t DriverTimerInit();
void DriverTimerDeInit();

#endif
