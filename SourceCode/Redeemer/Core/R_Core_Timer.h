/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_Timer.h
**		Timer class used mostly to get a constante movement speed independet on FPS and to calculate the FPS  
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_CORE_TIMER_H_
#define _R_CORE_TIMER_H_

#include <Windows.h>

namespace REDEEMER
{
	namespace CORE
	{
		/*!
		**	\brief Timer class used mostly to get a constante movement speed independet on FPS and to calculate the FPS  
		*/
		class C_Timer
		{
		public:
			/*!	Constructor
			*/
			C_Timer ();

			/*!	Destructor
			*/
			~C_Timer ();

			/*!	The timing function, StartTimer is called before the main loop 
			*/
			void  StartTimer ();

			/*! DoTimer is called inside the main loop
			*/
			void  DoTimer ();

			/*!	Returns the time since the program started
			*/
			float GetTime ();

			/*!	Return the FPS (as a string)
			*/
			char *GetFPS ();

			/*!	Return the movements constante (for constant movement speed on all computers)
			*/
			float CalcMove (float desDistance);

			/*!	Gets time as microseconds
			*/
			INT64 GetMicroseconds(void);

			/*!	Gets time as milliseconds
			*/
			unsigned long GetMiliseconds (void);

		private:
			__int64			m_Frequency;				// Timer Frequency
			float			m_Resolution;				// Timer Resolution
			unsigned long	m_MMTimerStart;				// Multimedia Timer Start Value
			unsigned long	m_MMTimerElapsed;			// Multimedia Timer Elapsed Time
			bool			m_PerformanceTimer;			// Using The Performance Timer?
			__int64			m_PerformanceTimerStart;	// Performance Timer Start Value
			__int64			m_PerformanceTimerElapsed;	// Performance Timer Elapsed Time

			float			m_Time;						// Used to check the current time
			float			m_TimeDiff;					// Used to determine the time elapsed
			float			m_SecsPerFrame;				// Used to hold the value for how many seconds have elapsed between frames
			float			m_LastTime;					// Used to see if it's time to update the FPS
			char			m_Buffer[20];				// Used for printing
			const static unsigned int MILLISECONDS_PER_SECOND = 1000000; //	class constant milliseconds/seconds
		};

	}	//	namespace CORE
}	//	namespace REDEEMER

#endif	//	_R_CORE_TIMER_H_
