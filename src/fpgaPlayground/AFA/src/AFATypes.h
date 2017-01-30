#ifndef AFA_TYPES_H__
#define AFA_TYPES_H__

#include <stdint.h>

// prevent clashes with stdint.h, there  uint32_t = unsigned int
#ifndef _STDINT_H
#ifndef  _C_STD_BEGIN

typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned long long  uint64_t;

#endif
#endif

typedef signed char         sint8_t;
typedef signed short        sint16_t;
typedef signed int          sint32_t;
typedef signed long long    sint64_t;


//typedef long unsigned int   size_t;

typedef float               float32_t;
typedef double              float64_t;
typedef uint32_t			bool_t;

// this is OS-environment-dependent:
// * if we have a 32-bit OS its uint32_t
// * if we have a 64-bit OS its uint64_t
typedef uint32_t			addr_t;



typedef enum
{
    SN_NONE,
    SN_AMPLITUDE,
    SN_FLUX
} SpectraNormalization;



typedef struct BestMatch_
{
    sint32_t index;				//< index in the map
    float32_t error;			//< euclidean distance
} BestMatch;

bool_t
AFATypesOK();



#endif
