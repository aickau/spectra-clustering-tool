#ifndef AFA_PROCESSING_H__
#define AFA_PROCESSING_H__

#include "AFATypes.h"
#include "AFANetworkSettings.h"
#include "AFARandom.h"
#include "AFAProcessingCommon.h"

// use any large number you like
#define AFA_COMPARE_BATCH 140000

AFAProcessingParamHW_t *
AFAProcessGetParamBlockAddress();

void
AFAProcessSetParamBlockParameters();

// calculate SOM grid size based on number of input spectra
uint32_t
AFACalcGridSize( uint32_t numSpectra );

bool_t
AFAHelperStructures_MemAllocate();
void
AFAHelperStructures_MemFree();

// calculate memory space to allocate in bytes needed for helper structures, including the SOM
void
AFAHelperStructures_PrepareDataStructure(
    uint32_t numSpectra );
void
AFAHelperStructures_UpdateAddressData();

void *
AFAHelperStructures_GetAddressOf(
    const char *dataName );

uint64_t
AFAHelperStructures_GetSizeOf(
    const char *dataName );

uint64_t
AFAHelperStructures_GetOffsetToBaseAddress(
    const char *dataName );

// returns true if system could be successfully initialized
bool_t
AFAInitProcessingNew(
    bool_t continueProcessing
    );

bool_t
AFAInitProcessing(
    volatile void *spectraArray,		// source spectra to cluster
    volatile void *helperStucturesMem,  // memory region for helper structures. Use AFACalcAllocSpaceForHelpStructures() to calculate needed size in bytes
    uint32_t numSpectra,				// number of source spectra in spectraArray
    bool_t continueProcessing,			// continue computation, hint: currently not supported
    AFAParameters *params				// spectra clustering params
    );



// one learning step
// returns true if learning is finished and maximum number of learning steps are reached.
bool_t
AFAProcess();

// get index to source spectrum in SOM 
// xp = x grid pos [0..gridsize-1]
// yp = y grid pos [0..gridsize-1]
// returns index to src spectrum [0..numSpectra] or -1 if cell is empty or index out of range
uint32_t
AFAGetSpectraIndex(
    uint32_t xp,
    uint32_t yp ); 


#endif
