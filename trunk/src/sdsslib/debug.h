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
//! \file  debug.h
//! \brief 

#ifndef H_DEBUG_12462006
#define H_DEBUG_12462006

#include <assert.h>

#ifdef _WIN32
#include <Windows.h>
#endif


#ifndef _DEBUG
#ifndef ASSERT
	#define ASSERT(x) 
#endif
	#define ASSERTM(x,msg)
#else 

#ifdef _WIN32
	#define ASSERT_IGNORE 0
	#define ASSERT_BREAK 1
	#define ASSERT_QUIT 2

	int DoAssert( const char *szcond, int line, const char *szfile, const char *msg );
	#define ASSERTM(x,msg) {if(!(x)) if(DoAssert (#x,__LINE__,__FILE__,msg)==ASSERT_BREAK) __asm{int 3};}
#else // __linux
	#define ASSERTM(x,msg) assert(x)  
#endif
#ifndef ASSERT
	#define ASSERT(x) assert(x);
#endif
#endif


// static assert
template<bool> 
struct STATIC_ASSERTION_FAILURE;

template<> 
struct STATIC_ASSERTION_FAILURE<true>{};

#define CONCAT1(a,b) a##b

#define STATIC_ASSERT(X) \
static const int CONCAT1(static_assertion_,__LINE__) = sizeof(STATIC_ASSERTION_FAILURE< bool(X) >);



class Debug
{
public:
	static void PrintConsole(const char *text , ...);
};





#endif