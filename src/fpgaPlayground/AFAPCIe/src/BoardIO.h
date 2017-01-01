/*
 * BoardIO.h
 *
 *  Created on: 02.05.2016
 *      Author: jesko
 */

#ifndef BOARDIO_H_
#define BOARDIO_H_

#include "AFATypes.h"

#define GPIO_CHANNEL_LED_MONO	( 1 )
#define GPIO_CHANNEL_LED		( 2 )

typedef enum
{
	EVAL_BOARD_LEDRGB_BLACK = 0,	// 0000b
	EVAL_BOARD_LEDRGB_DARK = 0,		// 0000b
	EVAL_BOARD_LEDRGB_BLUE,			// 0001b
	EVAL_BOARD_LEDRGB_GREEN,		// 0010b
	EVAL_BOARD_LEDRGB_CYAN,			// 0011b
	EVAL_BOARD_LEDRGB_RED,			// 0100b
	EVAL_BOARD_LEDRGB_VIOLETT,		// 0101b
	EVAL_BOARD_LEDRGB_YELLOW,		// 0110b
	EVAL_BOARD_LEDRGB_WHITE,		// 0111b
} LED_COLOR_T;

bool_t LEDInit();
void LEDSet( uint32_t idx );
void LEDReset( uint32_t idx );
void LEDBinaryShow(	uint32_t number );

void LEDRGBSet(	uint32_t idx, LED_COLOR_T col );
void LEDRGBBinaryShow( uint32_t number,	LED_COLOR_T col );

#endif /* BOARDIO_H_ */
