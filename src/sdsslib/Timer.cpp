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
//! \file  Timer.cpp
//! \brief simple stopwatch

#include "Timer.h"

#include <assert.h>
#ifdef _WIN32
#include <Windows.h>
#else // __linux
#include <sys/time.h>
#endif


Timer::Timer()
:m_DurationTicks( 0 )
,m_bPaused( false )
,m_BoundaryMode( kBoundaryMode_Stop )
{

#ifdef _WIN32
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	m_frequency = static_cast<double>(freq.QuadPart);
#else
	m_frequency = 1000000.0;		// under linux we have microseconds
#endif

	start();
}



void Timer::start()
{
	m_StartTics = Timer::getTicks();
}


void Timer::pause()
{
	if( m_bPaused ) {
		m_StartTics += Timer::getTicks() - m_StartPauseTics;
		m_bPaused = false;
	} else {
		m_StartPauseTics = Timer::getTicks();
		m_bPaused = true;
	}
}


void Timer::setDuration( double _dDuration, EBoundaryMode _Mode )
{
	m_BoundaryMode = _Mode;
	m_DurationTicks = ( _dDuration < 0.0f ) ? 0 : static_cast<uint64_t>(_dDuration * m_frequency);
}


uint64_t Timer::getTicks()
{
#ifdef _WIN32

	LARGE_INTEGER lp;
	QueryPerformanceCounter(&lp);
	return lp.QuadPart;

#else // __linux

	struct timeval end;
	gettimeofday(&end, NULL);
	return ((uint64_t)end.tv_sec)*1000000 + (uint64_t)end.tv_usec;

#endif

}


double Timer::getElapsedSecs()
{
	uint64_t ElapsedTics = ( m_bPaused ) ? m_StartPauseTics-m_StartTics : Timer::getTicks()-m_StartTics;

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

	return (static_cast<double>(ElapsedTics)/m_frequency);
}
