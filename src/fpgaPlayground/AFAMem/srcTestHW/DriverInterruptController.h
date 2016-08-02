/*
 * DriverInterruptController.h
 *
 *  Created on: 26.04.2016
 *      Author: jesko
 */

#ifndef DRIVER_INTERRUPT_CONTROLLER_H__
#define DRIVER_INTERRUPT_CONTROLLER_H__

#include <xintc.h>

bool_t DriverInterruptControllerInit();
void DriverInterruptControllerEnable();
void DriverInterruptControllerRegisteredEnable(
	uint8_t Id );
void DriverInterruptControllerRegisteredDisable(
	uint8_t Id );
uint32_t DriverInterruptControllerRegister(
	uint8_t Id,
	XInterruptHandler Handler,
	void *CallBackRef );
void DriverInterruptControllerDeRegister(
	uint8_t Id );

#endif /* DRIVER_INTERRUPT_CONTROLLER_H__ */
