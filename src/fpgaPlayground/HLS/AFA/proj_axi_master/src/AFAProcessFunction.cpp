#include "../../include/AFATypes.h"
#include "../../include/AFADefines.h"
#include "../../include/AFAProcessFunction.h"
#include "../../include/AFASpectra.h"
#include "../../include/AFAProcessing.h"

#include <stdio.h>
#include <string.h>

void
AFAProcessFunction(
    volatile uint32_t *p,
    volatile void *srcNet,
    volatile void *dstNet )
{
#pragma HLS INTERFACE m_axi port=p      depth=64
#pragma HLS INTERFACE m_axi port=srcNet depth=2400
#pragma HLS INTERFACE m_axi port=dstNet depth=2400

  int i;
  int buff[50];
  uint32_t AFAProcessFunctionParamBlock[ 256 / 4 ];
  AFAProcessFunctionParameter_t *paramBlock;

  memcpy(( void * ) &AFAProcessFunctionParamBlock, ( const void * ) p, 64 * sizeof( uint32_t ));
  paramBlock = ( AFAProcessFunctionParameter_t * ) AFAProcessFunctionParamBlock;

  AFAProcessing AFAProc(
      srcNet,
      dstNet,
      paramBlock->numSpectra,
      paramBlock->bContinueComputation );

  memcpy(( void * ) p, ( const void * ) &AFAProcessFunctionParamBlock, 64 * sizeof( uint32_t ));
}

