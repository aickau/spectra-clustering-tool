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

#include "AFARandom.h"
#include "AFATypes.h"

#include <assert.h>




uint32_t m_mt[ RANDOM_N ]; // the array for the state vector 
int m_mti = RANDOM_N; 



void AFARandomInitRandom( unsigned long _seed )
{
	int i=0;
	for (i=0;i<RANDOM_N;i++) 
	{
		m_mt[i] = _seed & 0xffff0000;
		_seed = 69069 * _seed + 1;
		m_mt[i] |= (_seed & 0xffff0000) >> 16;
		_seed = 69069 * _seed + 1;
	}
	m_mti = RANDOM_N;
}

void AFARandomGetStateVectorBlockAddress(
	unsigned long **mt, // the array for the state vector 
	int **mti )
{
	*mt = &m_mt[ 0 ]; // the array for the state vector 
	*mti = &m_mti; 
}

float AFARandomFloat()
{
	return ( (float)AFARandomDouble() );
}

double AFARandomDouble()
{
	return ( (double)AFARandomIntRange(31337) * 2.3283064370807974e-10 );
}

double AFARandomDoubleLog( float _ex )
{
	return ((AFARandomDouble()-_ex)*(AFARandomDouble()-_ex)+_ex);
}


unsigned int AFARandomInt()
{
	unsigned long y;
	static unsigned long mag01[2]={0x0, MATRIX_A};    
	if (m_mti >= RANDOM_N) { // generate N words at one time 
		int kk;
		for (kk=0;kk<RANDOM_N-RANDOM_M;kk++) {
			y = (m_mt[kk]&UPPER_MASK)|(m_mt[kk+1]&LOWER_MASK);
			m_mt[kk] = m_mt[kk+RANDOM_M] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		for (;kk<RANDOM_N-1;kk++) {
			y = (m_mt[kk]&UPPER_MASK)|(m_mt[kk+1]&LOWER_MASK);
			m_mt[kk] = m_mt[kk+(RANDOM_M-RANDOM_N)] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		y = (m_mt[RANDOM_N-1]&UPPER_MASK)|(m_mt[0]&LOWER_MASK);
		m_mt[RANDOM_N-1] = m_mt[RANDOM_M-1] ^ (y >> 1) ^ mag01[y & 0x1];
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
