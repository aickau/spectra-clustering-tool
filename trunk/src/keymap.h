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

TKEYMAP fr_keymap[] = {
							{KB_LEFT,	&ArrowLeft},
							{KB_RIGHT,	&ArrowRight},
							{KB_UP,		&ArrowUp},
							{KB_DOWN,	&ArrowDown},
							{KB_SPACE,	&Space},
							{KB_E,		&Export},
							{KB_V,		&DisableOutput},
							
};

#endif