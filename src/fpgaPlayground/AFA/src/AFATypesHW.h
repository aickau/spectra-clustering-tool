#ifndef AFA_TYPES_HW_H__
#define AFA_TYPES_HW_H__

#include "AFAConfig.h"
#include "AFATypesCommon.h"
#ifdef __SYNTHESIS__
#include <ap_cint.h>

typedef uint1				uint1_t;

#else
#include "AFATypes.h"

#endif // #ifdef __SYNTHESIS__

#endif
