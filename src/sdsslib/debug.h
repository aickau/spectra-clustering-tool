#ifndef H_ASSERT_124620062006
#define H_ASSERT_124620062006

#include <assert.h>
#include <Windows.h>

#ifndef _DEBUG
	#define ASSERT(x) 
	#define ASSERTM(x,msg)
#else 

	#define ASSERT_IGNORE 0
	#define ASSERT_BREAK 1
	#define ASSERT_QUIT 2

	int DoAssert( const char *szcond, int line, const char *szfile, const char *msg );
	#define ASSERTM(x,msg) {if(!(x)) if(DoAssert (#x,__LINE__,__FILE__,msg)==ASSERT_BREAK) __asm{int 3};}

	//#define ASSERT(x) ASSERTM(x,0)  
	#define ASSERT(x) assert(x);

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