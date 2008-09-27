#ifndef H_RANDOM
#define H_RANDOM

// ij must be in 0..31328
// kl must be in 0..30081
// init random and set seed
void initrandom ( int ij=1802, int kl=9373 );

// seed can be anything except 0
void mt_initrandom ( unsigned long seed = 4357 );
 

// return random number [0..1]
float random ();
// return random number [0..1]
double mt_random_double ();
// return random number with logarithmic scale [0..1], ex = erwartungswert 
double mt_random_double_log (float ex); 
// return random number [0..0xffffffff]
unsigned long mt_random_int ();

#endif






