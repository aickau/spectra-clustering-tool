
#define MIN(a,b)    (((a) < (b)) ? (a) : (b))
#define MAX(a,b)    (((a) > (b)) ? (a) : (b))
#define CLAMP(x,min,max) MAX(MIN(x,max),min)

#define POINTER2INDEX(pointer,basepointer,type) (((unsigned int)(pointer)-(unsigned int)basepointer)/sizeof(type))

#define CACHE_LINE 64
#define CACHE_ALIGN __declspec(align(CACHE_LINE))
#define ISSE_ALIGN __declspec(align(16))

#define EULER 2.718281828f

#ifndef NULL
#define NULL 0
#endif


// this is Visual c++ .NET 2003
#if _MSC_VER <= 1310
#define sprintf_s sprintf
#endif