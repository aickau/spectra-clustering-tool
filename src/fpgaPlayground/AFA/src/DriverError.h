#ifndef DRIVER_ERROR_H__
#define DRIVER_ERROR_H__

enum
{	// error codes (EC_...)
	EC_SUCCESS														= 0x00000000,
	EC_OUT_OF_MEMORY												= 0x00001000,
	EC_FAILURE_TOO_LAZY												= 0x0fffff77
};

#endif
