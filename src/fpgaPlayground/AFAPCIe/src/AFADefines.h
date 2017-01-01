#ifndef AFA_DEFINES_H__
#define AFA_DEFINES_H__

//const float FLT_MAX = 3.40282347e+38f;

#define AFAMIN(a,b)    (((a) < (b)) ? (a) : (b))
#define AFAMAX(a,b)    (((a) > (b)) ? (a) : (b))
#define AFACLAMP(x,min,max) AFAMAX(AFAMIN(x,max),min)

#ifndef AFA_CONST_EULER
#define AFA_CONST_EULER			( 2.718281828f )
#endif 

#define AFA_MAX( a, b )		(( a ) > ( b ) ? ( a ) : ( b ))

#ifndef NULL
#define NULL			( 0 )
#endif 

#ifndef TRUE
#define TRUE			( 1 )
#endif 

#ifndef FALSE
#define FALSE			( 0 )
#endif 


#endif
