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
//! \file  random.h
//! \brief random number genration

#ifndef H_RANDOM
#define H_RANDOM

// random number generation class
class Rnd
{
public:

	// random seed, zero is not allowed.
	Rnd( unsigned long _seed = 4357 );

	// reset random number generation to a given seed
	void initRandom( unsigned long _seed = 4357 );

	// return random number [0..1]
	float randomFloat();

	// return random number [0..1]
	double randomDouble();

	// return random number with logarithmic scale [0..1], ex = erwartungswert 
	double randomDouleLog( float _ex );

	// return random number [0..0xffffffff]
	unsigned int randomInt();

	// return random number [0.._range]
	unsigned int randomInt( unsigned int _range );


private:

	static const int N=624;

	unsigned long m_mt[N]; // the array for the state vector 
	int m_mti; 
};


#endif






