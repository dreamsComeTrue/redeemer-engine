/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_Mutex.cpp
**		Mutex blocks concurrent access to shared resources from multiple threads
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Core_Mutex.h"

namespace REDEEMER
{
	namespace CORE
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_Mutex::C_Mutex ()
		{
			InitializeCriticalSection (&m_Mutex);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Mutex::~C_Mutex ()
		{
			DeleteCriticalSection (&m_Mutex);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Mutex::Lock ()
		{
			EnterCriticalSection (&m_Mutex);
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_Mutex::LockSafe ()
		{
			return (TryEnterCriticalSection (&m_Mutex) != 0);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Mutex::Unlock ()
		{
			LeaveCriticalSection (&m_Mutex);
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace CORE
}	//	namespace REDEEMER

