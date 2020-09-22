/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_Timer.cpp
**		Timer class used mostly to get a constante movement speed independet on FPS and to calculate the FPS  
*/

//------------------------------------------------------------------------------------------------------------------------

#include "R_Core_Timer.h"

#include <cstdio>

// disable: deprecation warnings when using CRT calls in VC8
#pragma warning (disable: 4996)

namespace REDEEMER
{
	namespace CORE
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_Timer::C_Timer ()
		{
			// Clear the object
			memset (this, 0, sizeof(this));

			//	Check To See If A Performance Counter Is Available
			//	If One Is Available The Timer Frequency Will Be Updated
			if (!QueryPerformanceFrequency((LARGE_INTEGER *) &m_Frequency))
			{
				// No Performance Counter Available
				m_PerformanceTimer = false;			// Set Performance Timer To FALSE
				m_MMTimerStart = timeGetTime();		// Use timeGetTime() To Get Current Time
				m_Resolution = 1.0f/1000.0f;		// Set Our Timer Resolution To .001f
				m_Frequency = 1000;					// Set Our Timer Frequency To 1000
				m_MMTimerElapsed = m_MMTimerStart;	// Set The Elapsed Time To The Current Time
			}
			else
			{
				// Performance Counter Is Available, Use It Instead Of The Multimedia Timer
				// Get The Current Time And Store It In performance_timer_start
				QueryPerformanceCounter((LARGE_INTEGER *) &m_PerformanceTimerStart);

				m_PerformanceTimer = true;				// Set Performance Timer To TRUE

				// Calculate The Timer Resolution Using The Timer Frequency
				m_Resolution = (float) (((double)1.0f)/((double)m_Frequency));
				// Set The Elapsed Time To The Current Time
				m_PerformanceTimerElapsed = m_PerformanceTimerStart;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Timer::~C_Timer ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Timer::StartTimer ()
		{
			// Get the current time
			m_Time = GetTime ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Timer::DoTimer ()
		{
			// First determine how many seconds elapsed
			m_TimeDiff = GetTime() - m_Time;

			// The function reads in milliseconds so convert to seconds by dividing by 1000
			m_SecsPerFrame = (float)(m_TimeDiff / 1000.0f);

			// Get the new time
			m_Time = GetTime();
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_Timer::GetTime ()
		{
			__int64 time;				// Time will hold A 64 bit integer

			if (m_PerformanceTimer)		// Are We Using The Performance Timer?
			{
				QueryPerformanceCounter ((LARGE_INTEGER *) &time);	// Grab The Current Performance Time

				// Return The Current Time Minus The Start Time Multiplied By The Resolution And 1000 (To Get MS)
				return ((float) (time - m_PerformanceTimerStart) * m_Resolution) * 1000.0f;
			}
			else
			{
				// Return The Current Time Minus The Start Time Multiplied By The Resolution And 1000 (To Get MS)
				return ((float) (timeGetTime() - m_MMTimerStart) * m_Resolution) * 1000.0f;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		INT64 C_Timer::GetMicroseconds(void)
		{
			__int64 t;

			QueryPerformanceCounter((LARGE_INTEGER *)&t);

			return (t - m_PerformanceTimerStart) * MILLISECONDS_PER_SECOND / m_Frequency;
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned long C_Timer::GetMiliseconds (void)
		{
			return timeGetTime() - m_MMTimerStart;
		}

		//------------------------------------------------------------------------------------------------------------------------

		char* C_Timer::GetFPS ()
		{
			// Print the FPS once every second
			if (m_Time - m_LastTime > 1000.0f) 
			{

				// Print the FPS to the buffer
				sprintf (m_Buffer, "%7.02f ", 1 / m_SecsPerFrame);

				// Update the last time
				m_LastTime = m_Time;
			}

			return m_Buffer;
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_Timer::CalcMove (float desDistance)
		{
			return desDistance * m_SecsPerFrame;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace CORE
}	//	namespace REDEEMER
