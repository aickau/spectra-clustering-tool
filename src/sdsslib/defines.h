//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
//! #
//! # Copyright (c) 2009 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 1/19/2009
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  defines.h
//! \brief 

#include <float.h>
#include <stddef.h> 
#include <stdint.h>


#ifndef NULL
#define NULL 0
#endif

#define MIN(a,b)    (((a) < (b)) ? (a) : (b))
#define MAX(a,b)    (((a) > (b)) ? (a) : (b))
#define CLAMP(x,min,max) MAX(MIN(x,max),min)

#define CALC_ADRESS(x,y,xsize) ((x)+((y)*(xsize)))
#define CALC_ADRESS_SAFE(x,y,xsize,ysize) ((CLAMP(x,0,xsize-1))+((CLAMP(y,0,ysize-1))*(xsize)))

#define POINTER2INDEX(pointer,basepointer,type) (((unsigned int)(pointer)-(unsigned int)basepointer)/sizeof(type))

#define CALC_MAPADRESS(x,y,xsize) ((CLAMP(x,0,xsize-1))+((CLAMP(y,0,xsize-1))*(xsize)))
#define SETPIXEL(ptr,x,y,xsize,val) ptr[CALC_MAPADRESS(x,y,xsize)*3] = val; \
	ptr[CALC_MAPADRESS(x,y,xsize)*3+1] = val; \
	ptr[CALC_MAPADRESS(x,y,xsize)*3+2] = val; 

#define SETPIXELRGB(ptr,x,y,xsize,r,g,b) ptr[CALC_MAPADRESS(x,y,xsize)*3] = r; \
	ptr[CALC_MAPADRESS(x,y,xsize)*3+1] = g; \
	ptr[CALC_MAPADRESS(x,y,xsize)*3+2] = b; 


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

#define FWHM_GAUSS	2.3548200450309493820231386529194f


#define CACHE_LINE 64

#ifdef _WIN32
#define CACHE_ALIGN __declspec(align(CACHE_LINE))
#define SSE_ALIGN __declspec(align(16))
#else
#define CACHE_ALIGN __attribute__ ((aligned (CACHE_LINE)))
#define SSE_ALIGN __attribute__ ((aligned (16)))
#endif


#ifdef _WIN32
#define isnan(x) _isnan(x)
#define isinf(x) (!_finite(x))
#endif

// make sure to add in the compiler flags -D_FILE_OFFSET_BITS=64  for larger files than 2GB.
#ifdef __linux
#define _ftelli64(f) ftell(f)
#define _fseeki64(f, pos, origin) fseek(f, pos, origin)
#endif

// this is Visual c++ .NET 2003
#if _MSC_VER <= 1310
#define sprintf_s sprintf
#endif


// disable specific warnings for ms visual studio
#ifdef _MSC_VER
#pragma warning( disable : 4267 )		// conversion from 'size_t' to 'type', possible loss of data
#pragma warning( disable : 4244 )		// conversion from 'type1' to 'type2', possible loss of data
#pragma warning( disable : 4018 )		// signed/unsigned mismatch
#pragma warning( disable : 4305 )		// truncation from 'type1' to 'type2'
#endif

#ifdef __linux
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif


#ifdef __x86_64__
#define X64								// to have X64 define under linux
#endif
