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


Timer::Timer()
: m_bPaused( false ),
m_BoundaryMode( kBoundaryMode_Stop ),
m_DurationTicks( 0 )
{
	QueryPerformanceFrequency(&m_Frequency);
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
	m_DurationTicks = ( _dDuration < 0.0f ) ? 0 : static_cast<TTICKS>(_dDuration * static_cast<double>(m_Frequency.QuadPart));
}
