#ifndef AFA_PROCESS_FUNCTION_H__
#define AFA_PROCESS_FUNCTION_H__

#include "AFATypes.h"

typedef struct
{
	uint32_t numSpectra;
	bool bContinueComputation;
} AFAProcessFunctionParameter_t;

void
AFAProcessFunction(
    volatile uint32_t *p,
    volatile void *srcNet,
    volatile void *dstNet );

#endif
