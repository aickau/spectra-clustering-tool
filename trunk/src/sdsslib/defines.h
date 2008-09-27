#include <float.h>

#ifndef NULL
#define NULL 0
#endif

#define MIN(a,b)    (((a) < (b)) ? (a) : (b))
#define MAX(a,b)    (((a) > (b)) ? (a) : (b))
#define CLAMP(x,min,max) MAX(MIN(x,max),min)

#define POINTER2INDEX(pointer,basepointer,type) (((unsigned int)(pointer)-(unsigned int)basepointer)/sizeof(type))

#ifndef EPS
#define EPS (FLT_MIN*10.f)
#endif 

#ifndef EULER
#define EULER 2.718281828f
#endif 

#ifndef PI
#define PI 3.1415926535897932384626433832795F
#endif  

#ifndef TWOPI
#define TWOPI (PI*2.0f)
#endif 

#ifndef HALFPI
#define HALFPI (PI*0.5f)
#endif 

#ifndef DEG2RAD
#define	DEG2RAD	   0.017453292F
#endif 

#ifndef RAD2DEG
#define	RAD2DEG	   (1.0f/0.017453292F)
#endif 


#ifndef SQR
#define SQR(x) ((x)*(x))
#endif



#define CACHE_LINE 64
#define CACHE_ALIGN __declspec(align(CACHE_LINE))
#define ISSE_ALIGN __declspec(align(16))



// this is Visual c++ .NET 2003
#if _MSC_VER <= 1310
#define sprintf_s sprintf
#endif