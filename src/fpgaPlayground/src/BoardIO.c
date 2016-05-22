#include "AFAConfig.h"
#include "AFATypes.h"
#include "AFADefines.h"
#include "BoardIO.h"

#include <string.h>

#ifdef AFA_RUN_ON_XILINX_SDK
#include "xparameters.h"
#include "xgpio.h"
#include "stdio.h"
#include "xstatus.h"
#endif

#ifdef AFA_RUN_ON_XILINX_SDK
XGpio GpioOutput; /* The driver instance for GPIO Device configured as O/P */
#endif

static uint32_t gMonoLEDState = 0x00000000;
static uint32_t gRGBLEDState = 0x00000000;

#ifdef AFA_RUN_ON_XILINX_SDK
bool_t
LEDInit()
{
	int Status;
	/*
	 * Initialize the GPIO driver so that it's ready to use,
	 * specify the device ID that is generated in xparameters.h
	 */
	Status = XGpio_Initialize(
		&GpioOutput, XPAR_AXI_GPIO_0_DEVICE_ID );
	if ( Status != XST_SUCCESS )
	{
		return FALSE;
	}

	/*
	* Set the direction for all signals to be outputs
	*/
	XGpio_SetDataDirection( &GpioOutput, GPIO_CHANNEL_LED_MONO, 0x00000000 );
	XGpio_SetDataDirection( &GpioOutput, GPIO_CHANNEL_LED, 0x00000000 );
	
	gMonoLEDState = 0x00000000;
	gRGBLEDState = 0x00000000;
	XGpio_DiscreteWrite( &GpioOutput, GPIO_CHANNEL_LED_MONO, gMonoLEDState );
	XGpio_DiscreteWrite( &GpioOutput, GPIO_CHANNEL_LED, gRGBLEDState );
	
	return TRUE;
}

void
LEDSet(
	uint32_t idx )
{
	gMonoLEDState |= 1 << idx;
	XGpio_DiscreteWrite( &GpioOutput, GPIO_CHANNEL_LED_MONO, gMonoLEDState );
}

void
LEDReset(
	uint32_t idx )
{
	gMonoLEDState &= ~( 1 << idx );
	XGpio_DiscreteWrite( &GpioOutput, GPIO_CHANNEL_LED_MONO, gMonoLEDState );
}

void
LEDRGBSet(
	uint32_t idx,
	LED_COLOR_T col )
{
	gRGBLEDState &= ~( 7 << ( idx * 3 ));
	gRGBLEDState |= (( uint32_t ) col ) << ( idx * 3 );
	XGpio_DiscreteWrite( &GpioOutput, GPIO_CHANNEL_LED, gRGBLEDState );
}

#else
bool_t
LEDInit()
{
	gMonoLEDState = 0x00000000;
	gRGBLEDState = 0x00000000;
	return TRUE;
}

void
LEDSet(
	uint32_t idx )
{
	gMonoLEDState |= 1 << idx;
}

void
LEDReset(
	uint32_t idx )
{
	gMonoLEDState &= ~( 1 << idx );
}

void
LEDRGBSet(
	uint32_t idx,
	LED_COLOR_T col )
{
	gRGBLEDState &= ~( 7 << ( idx * 3 ));
	gRGBLEDState |= (( uint32_t ) col ) << ( idx * 3 );
}
#endif

void
LEDBinaryShow(
	uint32_t number )
{
	number & 0x00000001 ? LEDSet( 0 ) : LEDReset( 0 );
	number & 0x00000002 ? LEDSet( 1 ) : LEDReset( 1 );
	number & 0x00000004 ? LEDSet( 2 ) : LEDReset( 2 );
	number & 0x00000008 ? LEDSet( 3 ) : LEDReset( 3 );
}

void
LEDRGBBinaryShow(
	uint32_t number,
	LED_COLOR_T col )
{
	number & 0x00000001 ? LEDRGBSet( 0, col ) : LEDRGBSet( 0, EVAL_BOARD_LEDRGB_BLACK );
	number & 0x00000002 ? LEDRGBSet( 1, col ) : LEDRGBSet( 1, EVAL_BOARD_LEDRGB_BLACK );
	number & 0x00000004 ? LEDRGBSet( 2, col ) : LEDRGBSet( 2, EVAL_BOARD_LEDRGB_BLACK );
	number & 0x00000008 ? LEDRGBSet( 3, col ) : LEDRGBSet( 3, EVAL_BOARD_LEDRGB_BLACK );
}
