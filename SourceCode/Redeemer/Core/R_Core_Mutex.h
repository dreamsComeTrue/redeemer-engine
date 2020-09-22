/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_Mutex.h
**		Mutex blocks concurrent access to shared resources from multiple threads
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_CORE_MUTEX_H_
#define _R_CORE_MUTEX_H_

#include <Windows.h>

namespace REDEEMER
{
	namespace CORE
	{
		/*!
		**	\brief Mutex blocks concurrent access to shared resources from multiple threads
		*/
		class C_Mutex
		{
		public:
			/*!	Constructor
			*/
			C_Mutex ();

			/*!	Destructor
			*/
			virtual ~C_Mutex ();

			/*!	Lock the mutex
			**	If the mutex is already locked in another thread, this call will block the execution until the mutex
			**	is released.
			*/
			void Lock ();

			/*!	Tries to enter critical section.
			**	\remarks If succeeds in entering critical section, it locks mutex and returns true. Otherwise it returns false without blocking.
			*/
			bool LockSafe ();

			/*!	Unlock the mutex
			*/
			void Unlock ();

		private:
			CRITICAL_SECTION m_Mutex;
		};

	}	//	namespace CORE
}	//	namespace REDEEMER

#endif	//	_R_CORE_MUTEX_H_
