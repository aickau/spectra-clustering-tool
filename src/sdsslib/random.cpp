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

#include "random.h"

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


Rnd::Rnd( unsigned long _seed )
:m_mti(N)
{
	initRandom( _seed );
}


void Rnd::initRandom( unsigned long _seed )
{
	for (int i=0;i<N;i++) 
	{
		m_mt[i] = _seed & 0xffff0000;
		_seed = 69069 * _seed + 1;
		m_mt[i] |= (_seed & 0xffff0000) >> 16;
		_seed = 69069 * _seed + 1;
	}
	m_mti = N;
}



float Rnd::randomFloat()
{
	return ( (float)randomDouble() );
}

double Rnd::randomDouble()
{
	return ( (double)randomInt() * 2.3283064370807974e-10 );
}

double Rnd::randomDouleLog( float _ex )
{
	return ((randomDouble()-_ex)*(randomDouble()-_ex)+_ex);
}


unsigned int Rnd::randomInt()
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

unsigned int Rnd::randomInt( unsigned int _range )
{
	assert(_range != 0x0ffffffff );
	return randomInt() % (_range+1);
}






