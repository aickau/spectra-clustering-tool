#include "random.h"

// taken from random.c in faqsys
// credits:
// "This C language version was written by Jim Butler, and was based on a
//  FORTRAN program posted by David LaSalle of Florida State University."
//
// should give at least 2^30 random numbers per seed


static float u[98], c, cd, cm;
static int i97, j97;

void initrandom ( int ij, int kl ) 
{
	int i, j, k, l, ii, jj, m;
	float s, t;		
	i = (ij/177)%177 + 2;
	j = ij%177 + 2;
	k = (kl/169)%178 + 1;
	l = kl%169;	
	for (ii=1; ii<=97; ii++) {
		s = 0.0;
		t = 0.5;
		for (jj=1; jj<=24; jj++) {
			m = (((i*j)%179)*k) % 179;
			i = j;
			j = k;
			k = m;
			l = (53*l + 1) % 169;
			if ((l*m)%64 >= 32) s += t;
			t *= 0.5;
		}
		u[ii] = s;
	}	
	c = 362436.0 / 16777216.0;
	cd = 7654321.0 / 16777216.0;
	cm = 16777213.0 / 16777216.0;	
	i97 = 97;
	j97 = 33;		
}

float random () {
	float uni = u[i97] - u[j97];
	if (uni < 0.0) uni += 1.0;
	u[i97] = uni;
	i97--;
	if (i97==0) i97 = 97;
	j97--;
	if (j97==0) j97 = 97;
	c -= cd;
	if (c<0.0) c += cm;
	uni -= c;
	if (uni<0.0) uni += 1.0;
	return uni;			
}

// taken from mt19937-1.c 
// credits:
// "Coded by Takuji Nishimura"
// 
// should give 2^19937-1 random numbers per seed

#define N 624
#define M 397
#define MATRIX_A 0x9908b0df   
#define UPPER_MASK 0x80000000 
#define LOWER_MASK 0x7fffffff 
#define TEMPERING_MASK_B 0x9d2c5680
#define TEMPERING_MASK_C 0xefc60000
#define TEMPERING_SHIFT_U(y)  (y >> 11)
#define TEMPERING_SHIFT_S(y)  (y << 7)
#define TEMPERING_SHIFT_T(y)  (y << 15)
#define TEMPERING_SHIFT_L(y)  (y >> 18)

static unsigned long mt[N]; /* the array for the state vector  */
static int mti=N+1; /* mti==N+1 means mt[N] is not initialized */

/* Initializing the array with a seed */
void mt_initrandom ( unsigned long seed ) {    
    for (int i=0;i<N;i++) {
         mt[i] = seed & 0xffff0000;
         seed = 69069 * seed + 1;
         mt[i] |= (seed & 0xffff0000) >> 16;
         seed = 69069 * seed + 1;
    }
    mti = N;
}

unsigned long mt_random_int () {
    unsigned long y;
    static unsigned long mag01[2]={0x0, MATRIX_A};    
    if (mti >= N) { // generate N words at one time 
        int kk;
        if (mti == N+1) mt_initrandom(4357); 
        for (kk=0;kk<N-M;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1];
        }
        for (;kk<N-1;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1];
        }
        y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
        mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1];
        mti = 0;
    }
    y = mt[mti++];
    y ^= TEMPERING_SHIFT_U(y);
    y ^= TEMPERING_SHIFT_S(y) & TEMPERING_MASK_B;
    y ^= TEMPERING_SHIFT_T(y) & TEMPERING_MASK_C;
    y ^= TEMPERING_SHIFT_L(y);
    return y; 
}

// returns randoms 0..1
double mt_random_double () {	
	return ( (double)mt_random_int () * 2.3283064370807974e-10 );
}


// returns randoms 0..1 (logarithmic probability)
double mt_random_double_log (float ex) {	
	return ((mt_random_double ()-ex)*(mt_random_double ()-ex)+ex);
}





