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
//! \file  Timer.h
//! \brief simple stopwatch


#ifndef TIMER_H
#define TIMER_H

#include "defines.h"

class Timer  
{
public:
	Timer();

	enum EBoundaryMode {
		kBoundaryMode_Stop,
		kBoundaryMode_Loop
	};


	void start();
	void pause();

	// -1.0 for no duration
	void setDuration( double _dDuration, EBoundaryMode _Mode );

	double getElapsedSecs();

private:
	static uint64_t getTicks();

	uint64_t m_StartTics;
	uint64_t m_StartPauseTics;
	uint64_t m_DurationTicks;
	bool m_bPaused;
	EBoundaryMode m_BoundaryMode;
	double m_frequency;
};



#endif 
