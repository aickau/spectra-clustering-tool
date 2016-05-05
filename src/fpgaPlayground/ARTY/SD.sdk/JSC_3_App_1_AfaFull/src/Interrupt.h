/*
 * interrupt.h
 *
 *  Created on: 26.04.2016
 *      Author: jesko
 */

#ifndef INTERRUPT_H__
#define INTERRUPT_H__

void XAfaprocess_hw_InterruptServiceRoutine( void *interruptParameter );
void ErrorLoop();
void InterruptInit();
void XAfaprocess_hw_Init();
void InterruptEnable();

#endif /* INTERRUPTS_H_ */
