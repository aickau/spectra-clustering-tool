//! \verbatim
//! ###########################################################################
//! # SDSS sorting prototype - Stage I
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

#include "windows.h"

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
	typedef unsigned __int64 TTICKS;
	static TTICKS getTicks();

	TTICKS m_StartTics;
	TTICKS m_StartPauseTics;
	TTICKS m_DurationTicks;
	bool m_bPaused;
	EBoundaryMode m_BoundaryMode;
	LARGE_INTEGER m_Frequency;
};


inline 
Timer::TTICKS Timer::getTicks()
{
	LARGE_INTEGER lp;
	QueryPerformanceCounter(&lp);
	return lp.QuadPart;
}


inline 
double Timer::getElapsedSecs()
{
	TTICKS ElapsedTics = ( m_bPaused ) ? m_StartPauseTics-m_StartTics : Timer::getTicks()-m_StartTics;

	if ( m_DurationTicks > 0 )
	{
		if( ElapsedTics > m_DurationTicks ) {
			switch( m_BoundaryMode ) {
case kBoundaryMode_Loop :
				ElapsedTics = 0;
				m_StartTics = Timer::getTicks();
				break;
			default:
				ElapsedTics = m_DurationTicks;
			}
		}
	}

	return (static_cast<double>(ElapsedTics)/static_cast<double>(m_Frequency.QuadPart));
}


#endif 
