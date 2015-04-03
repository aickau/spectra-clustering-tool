#ifndef AFA_DEFINES_H__
#define AFA_DEFINES_H__

//const float FLT_MAX = 3.40282347e+38f;


#define AFAMIN(a,b)    (((a) < (b)) ? (a) : (b))
#define AFAMAX(a,b)    (((a) > (b)) ? (a) : (b))
#define AFACLAMP(x,min,max) AFAMAX(AFAMIN(x,max),min)

#ifndef EULER
#define EULER 2.718281828f
#endif 


#endif
