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


	void Start();
	void Pause();

	// -1.0 for no duration
	void SetDuration( double _dDuration, EBoundaryMode _Mode );

	double GetElapsedSecs();

private:
	typedef unsigned __int64 TTICKS;
	static TTICKS GetTicks();

	TTICKS m_StartTics;
	TTICKS m_StartPauseTics;
	TTICKS m_DurationTicks;
	bool m_bPaused;
	EBoundaryMode m_BoundaryMode;
	LARGE_INTEGER m_Frequency;
};


inline 
Timer::TTICKS Timer::GetTicks()
{
	LARGE_INTEGER lp;
	QueryPerformanceCounter(&lp);
	return lp.QuadPart;
}


inline 
double Timer::GetElapsedSecs()
{
	TTICKS ElapsedTics = ( m_bPaused ) ? m_StartPauseTics-m_StartTics : Timer::GetTicks()-m_StartTics;

	if ( m_DurationTicks > 0 )
	{
		if( ElapsedTics > m_DurationTicks ) {
			switch( m_BoundaryMode ) {
case kBoundaryMode_Loop :
				ElapsedTics = 0;
				m_StartTics = Timer::GetTicks();
				break;
			default:
				ElapsedTics = m_DurationTicks;
			}
		}
	}

	return (static_cast<double>(ElapsedTics)/static_cast<double>(m_Frequency.QuadPart));
}


#endif 
