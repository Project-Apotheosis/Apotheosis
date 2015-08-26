// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2099 Media Design School 
// 
// File Name : Timer.h
// Description :  Timer header
// Author : Serge Radinovich
// Mail : srad015@hotmail.com

#ifndef __TIMER_H__
#define ___TIMER_H__



	class Timer
	{
	public: //Construction
		Timer();

		float getGameTime()const;  // in seconds
		float getDeltaTime()const; // in seconds

	private: //Data

		double	m_dSecondsPerCount{ 0.0 };
		double	m_dDeltaTime{ -1.0 };

		__int64 m_iBaseTime{ 0 };
		__int64 m_iPausedTime{ 0 };
		__int64 m_iStopTime{ 0 };
		__int64 m_iPrevTime{ 0 };
		__int64 m_iCurrTime{ 0 };

		bool	m_bStopped{ false };

	public: //Interface

		// Call before message loop.
		void reset();
		// Call when unpaused.
		void start();
		// Call when paused.
		void stop();
		// Call every frame.
		void tick();
	};

#endif // ___TIMER_H__