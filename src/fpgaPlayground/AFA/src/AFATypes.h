#ifndef AFA_TYPES_H__
#define AFA_TYPES_H__

#include <stdint.h>

// prevent clashes with stdint.h, there  uint32_t = unsigned int
#ifndef  _C_STD_BEGIN

typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned long int   uint64_t;

typedef signed char         sint8_t;
typedef signed short        sint16_t;
typedef signed int          sint32_t;
typedef signed long int     sint64_t;

#endif


//typedef long unsigned int   size_t;

typedef float               float32_t;
typedef double              float64_t;
typedef uint32_t			bool_t;
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
