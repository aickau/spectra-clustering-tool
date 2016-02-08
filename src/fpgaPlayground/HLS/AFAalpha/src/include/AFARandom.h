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

#ifndef AFA_RANDOM_H__
#define AFA_RANDOM_H__

#include "include/AFARandomCommon.h"

void AFARandomGetStateVectorBlockAddress(
	unsigned long **mt, // the array for the state vector 
	int **mti );

// reset random number generation to a given seed
void AFARandomInitRandom( unsigned long _seed );

// return random number [0..1]
float AFARandomFloat();

// return random number [0..1]
double AFARandomDouble();

// return random number with logarithmic scale [0..1], ex = erwartungswert 
double AFARandomDoubleLog( float _ex );

// return random number [0..0xffffffff]
unsigned int AFARandomInt();

// return random number [0.._range]
unsigned int AFARandomIntRange( unsigned int _range );

#endif
