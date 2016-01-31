#ifndef AFA_PROCESSING_H__
#define AFA_PROCESSING_H__

#include "AFATypes.h"
#include "AFANetworkSettings.h"
#include "AFARandom.h"

#ifndef FLT_MAX
#define FLT_MAX         3.402823466e+38F 
#endif

// calculate memory space to allocate in bytes needed for helper structures, including the SOM
uint64_t AFACalcAllocSpaceForHelperStructures( uint32_t numSpectra );

// returns true if system could be successfully initialized
bool_t AFAInitProcessing(
	volatile void *spectraArray,
	volatile void *helperStucturesMem,  // memory region for helper structures. Use AFACalcAllocSpaceForHelpStructures() to calculate needed size in bytes
	uint32_t numSpectra,
	bool_t continueProcessing,
	AFAParameters *params
	);



// one learning step
// returns true if learning is finished and maximum number of learning steps are reached.
bool_t AFAProcess();


#endif
