// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2099 Media Design School 
// 
// File Name : Timer.cpp
// Description :  Timer cpp
// Author : Serge Radinovich
// Mail : srad015@hotmail.com

#include "Timer.h"
#include <windows.h>


	/**
	*	Constructor
	*	Find how many seconds there are per processor count
	*	@author Serge Radinovich
	*/
	Timer::Timer()
	{
		__int64 _countsPerSec;
		QueryPerformanceFrequency((LARGE_INTEGER*)&_countsPerSec);
		m_dSecondsPerCount = 1.0 / (double)_countsPerSec;
	}


	/**
	*	Returns the total time elapsed since reset() was called, NOT counting any
	*	time when the clock is stopped.
	*	@author Serge Radinovich
	*	@return float				Total game time
	*/
	float Timer::getGameTime()const
	{
		// If we are stopped, do not count the time that has passed since we stopped.
		//
		// ----*---------------*------------------------------*------> time
		//  mBaseTime       mStopTime                      mCurrTime

		if (m_bStopped)
		{
			return (float)((m_iStopTime - m_iBaseTime)*m_dSecondsPerCount);
		}

		// The distance mCurrTime - mBaseTime includes paused time,
		// which we do not want to count.  To correct this, we can subtract 
		// the paused time from mCurrTime:  
		//
		//  (mCurrTime - mPausedTime) - mBaseTime 
		//
		//                     |<-------d------->|
		// ----*---------------*-----------------*------------*------> time
		//  mBaseTime       mStopTime        startTime     mCurrTime

		else
		{
			return (float)(((m_iCurrTime - m_iPausedTime) - m_iBaseTime)*m_dSecondsPerCount);
		}
	}

	/**
	*	Get change in time from last tick
	*	@author Serge Radinovich
	*	@return float				Change in time
	*/
	float Timer::getDeltaTime()const
	{
		return (float)m_dDeltaTime;
	}

	/**
	*	Restart timer altogether
	*	@author Serge Radinovich
	*/
	void Timer::reset()
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		m_iBaseTime = currTime;
		m_iPrevTime = currTime;
		m_iStopTime = 0;
		m_bStopped = false;
	}

	/**
	*	Start the timer or restart, after a stop
	*	@author Serge Radinovich
	*/
	void Timer::start()
	{
		__int64 startTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&startTime);


		// Accumulate the time elapsed between stop and start pairs.
		//
		//                     |<-------d------->|
		// ----*---------------*-----------------*------------> time
		//  mBaseTime       mStopTime        startTime     

		if (m_bStopped)
		{
			m_iPausedTime += (startTime - m_iStopTime);

			m_iPrevTime = startTime;
			m_iStopTime = 0;
			m_bStopped = false;
		}
	}

	/**
	*	Stop the timer
	*	@author Serge Radinovich
	*/
	void Timer::stop()
	{
		if (!m_bStopped)
		{
			__int64 currTime;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

			m_iStopTime = currTime;
			m_bStopped = true;
		}
	}

	/**
	*	Tick the timer every frame
	*	@author Serge Radinovich
	*/
	void Timer::tick()
	{
		if (m_bStopped)
		{
			m_dDeltaTime = 0.0;
			return;
		}

		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		m_iCurrTime = currTime;

		// Time difference between this frame and the previous.
		m_dDeltaTime = (m_iCurrTime - m_iPrevTime)*m_dSecondsPerCount;

		// Prepare for next frame.
		m_iPrevTime = m_iCurrTime;

		// Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
		// processor goes into a power save mode or we get shuffled to another
		// processor, then mDeltaTime can be negative.
		if (m_dDeltaTime < 0.0)
		{
			m_dDeltaTime = 0.0;
		}
	}
