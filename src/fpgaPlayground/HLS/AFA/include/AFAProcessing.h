#ifndef AFA_PROCESSING_H__
#define AFA_PROCESSING_H__

#include "AFATypes.h"
#include "AFANetworkSettings.h"
#include "AFARandom.h"

#ifndef FLT_MAX
#define FLT_MAX         3.402823466e+38F 
#endif

void AFAInitProcessing(
	volatile void *spectraArray,
	volatile void *spectraArray2,  // "sofmnet.bin"
	uint32_t numSpectra,
	bool_t continueProcessing,
	AFAParameters *params
	);


void AFARandomDeinitProcessing();


// one learning step
// returns true if learning is finished and maximum number of learning steps are reached.
bool_t AFARandomProcess();


#endif
