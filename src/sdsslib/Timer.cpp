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