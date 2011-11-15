//! \verbatim
//! ###########################################################################
//! # SDSS sorting prototype - Stage I
//! #
//! # Copyright (c) 2009 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 1/19/2009
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  keymap.h


#ifndef _KEYMAP_H
#define _KEYMAP_H

#include "keycodes.h"

typedef struct TKEYMAP_TAG
{
	int		scancode;
	void	(*ptProc)();
} TKEYMAP;



extern void ArrowLeft();
extern void ArrowRight();
extern void ArrowUp();
extern void ArrowDown();
extern void Space();
extern void Export();
extern void DisableOutput();

extern void KeyOne();
extern void KeyTwo();

TKEYMAP fr_keymap[] = {
							{KB_LEFT,	&ArrowLeft},
							{KB_RIGHT,	&ArrowRight},
							{KB_UP,		&ArrowUp},
							{KB_DOWN,	&ArrowDown},
							{KB_SPACE,	&Space},
							{KB_E,		&Export},
							{KB_V,		&DisableOutput},
							{KB_1,		&KeyOne},			
							{KB_2,		&KeyTwo},

};

#endif