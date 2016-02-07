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

#include "include/AFARandomHW.h"

#include <assert.h>

unsigned long m_mt_HW[N]; // the array for the state vector 
int m_mti_HW = N; 



void AFARandomInitRandom_HW( unsigned long _seed )
{
	int i=0;
	for (i=0;i<N;i++) 
	{
		m_mt_HW[i] = _seed & 0xffff0000;
		_seed = 69069 * _seed + 1;
		m_mt_HW[i] |= (_seed & 0xffff0000) >> 16;
		_seed = 69069 * _seed + 1;
	}
	m_mti_HW = N;
}



float AFARandomFloat_HW()
{
	return ( (float)AFARandomDouble_HW() );
}

double AFARandomDouble_HW()
{
	return ( (double)AFARandomIntRange_HW(31337) * 2.3283064370807974e-10 );
}

double AFARandomDoubleLog_HW( float _ex )
{
	return ((AFARandomDouble_HW()-_ex)*(AFARandomDouble_HW()-_ex)+_ex);
}


unsigned int AFARandomInt_HW()
{
	unsigned long y;
	static unsigned long mag01[2]={0x0, MATRIX_A};    
	if (m_mti_HW >= N) { // generate N words at one time 
		int kk;
		for (kk=0;kk<N-M;kk++) {
			y = (m_mt_HW[kk]&UPPER_MASK)|(m_mt_HW[kk+1]&LOWER_MASK);
			m_mt_HW[kk] = m_mt_HW[kk+M] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		for (;kk<N-1;kk++) {
			y = (m_mt_HW[kk]&UPPER_MASK)|(m_mt_HW[kk+1]&LOWER_MASK);
			m_mt_HW[kk] = m_mt_HW[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		y = (m_mt_HW[N-1]&UPPER_MASK)|(m_mt_HW[0]&LOWER_MASK);
		m_mt_HW[N-1] = m_mt_HW[M-1] ^ (y >> 1) ^ mag01[y & 0x1];
		m_mti_HW = 0;
	}
	y = m_mt_HW[m_mti_HW++];
	y ^= TEMPERING_SHIFT_U(y);
	y ^= TEMPERING_SHIFT_S(y) & TEMPERING_MASK_B;
	y ^= TEMPERING_SHIFT_T(y) & TEMPERING_MASK_C;
	y ^= TEMPERING_SHIFT_L(y);
	return y; 
}

unsigned int AFARandomIntRange_HW( unsigned int _range )
{
	assert(_range != 0x0ffffffff );
	return AFARandomInt_HW() % (_range+1);
}
