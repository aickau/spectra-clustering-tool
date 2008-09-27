#include "Timer.h"
#include <assert.h>


Timer::Timer()
: m_bPaused( false ),
m_BoundaryMode( kBoundaryMode_Stop ),
m_DurationTicks( 0 )
{
	QueryPerformanceFrequency(&m_Frequency);
	Start();
}



void Timer::Start()
{
	m_StartTics = Timer::GetTicks();
}


void Timer::Pause()
{
	if( m_bPaused ) {
		m_StartTics += Timer::GetTicks() - m_StartPauseTics;
		m_bPaused = false;
	} else {
		m_StartPauseTics = Timer::GetTicks();
		m_bPaused = true;
	}
}


void Timer::SetDuration( double _dDuration, EBoundaryMode _Mode )
{
	m_BoundaryMode = _Mode;
	m_DurationTicks = ( _dDuration < 0.0f ) ? 0 : static_cast<TTICKS>(_dDuration * static_cast<double>(m_Frequency.QuadPart));
}
