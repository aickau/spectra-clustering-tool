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
//! code taken from : Takuji Nishimura
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  random.cpp
//! \brief Random number generation taken from mt19937-1.c 
//!        should give 2^19937-1 random numbers per seed

#include "include/AFARandom.h"

#include <assert.h>


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


unsigned long m_mt[N]; // the array for the state vector 
int m_mti = N; 



void AFARandomInitRandom( unsigned long _seed )
{
	int i=0;
	for (i=0;i<N;i++) 
	{
		m_mt[i] = _seed & 0xffff0000;
		_seed = 69069 * _seed + 1;
		m_mt[i] |= (_seed & 0xffff0000) >> 16;
		_seed = 69069 * _seed + 1;
	}
	m_mti = N;
}



float AFARandomFloat()
{
	return ( (float)AFARandomDouble() );
}

double AFARandomDouble()
{
	return ( (double)AFARandomInt(31337) * 2.3283064370807974e-10 );
}

double AFARandomDoubleLog( float _ex )
{
	return ((AFARandomDouble()-_ex)*(AFARandomDouble()-_ex)+_ex);
}


unsigned int AFARandomInt()
{
	unsigned long y;
	static unsigned long mag01[2]={0x0, MATRIX_A};    
	if (m_mti >= N) { // generate N words at one time 
		int kk;
		for (kk=0;kk<N-M;kk++) {
			y = (m_mt[kk]&UPPER_MASK)|(m_mt[kk+1]&LOWER_MASK);
			m_mt[kk] = m_mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		for (;kk<N-1;kk++) {
			y = (m_mt[kk]&UPPER_MASK)|(m_mt[kk+1]&LOWER_MASK);
			m_mt[kk] = m_mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		y = (m_mt[N-1]&UPPER_MASK)|(m_mt[0]&LOWER_MASK);
		m_mt[N-1] = m_mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1];
		m_mti = 0;
	}
	y = m_mt[m_mti++];
	y ^= TEMPERING_SHIFT_U(y);
	y ^= TEMPERING_SHIFT_S(y) & TEMPERING_MASK_B;
	y ^= TEMPERING_SHIFT_T(y) & TEMPERING_MASK_C;
	y ^= TEMPERING_SHIFT_L(y);
	return y; 
}

unsigned int AFARandomIntRange( unsigned int _range )
{
	assert(_range != 0x0ffffffff );
	return AFARandomInt() % (_range+1);
}
