/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_Thread.cpp
**		Threads represents different path of execution. This class can only be derived.
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Core_Thread.h"	  
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace CORE
	{
		//------------------------------------------------------------------------------------------------------------------------
		DWORD WINAPI GlobalThreadProcedure (LPVOID data)
		{
			C_Thread* thread = static_cast<C_Thread*> (data);

			if (thread->IsRunning ())
				thread->Execute ();

			//	After thread's execution
			thread->m_IsRunning = false;

			thread->Signal ();

			return 0;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Thread::C_Thread () :
			m_IsRunning (false),
			m_IsWaiting (false)
		{
			m_ThreadHandle = NULL;
			m_EventHandle = CreateEvent (NULL, false, false, NULL);

			if (!m_EventHandle)
			{
				REDEEMER_LOG << LOG_FATAL << L"Creating a thread's event object failed!" << LOG_ENDMESSAGE;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Thread::~C_Thread ()
		{
			Signal ();	  
			TerminateSafe ();

			CloseHandle (m_ThreadHandle);

			if (!CloseHandle (m_EventHandle))
			{
				REDEEMER_LOG << LOG_FATAL << L"Closing a thread's event object failed!" << LOG_ENDMESSAGE;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Thread::Launch ()
		{
			//	Thread is already run, so exit
			if (m_IsRunning)
				return;

			m_IsRunning = true;
			m_ThreadHandle = CreateThread (NULL, 0, GlobalThreadProcedure, this, 0, NULL);

			if (!m_ThreadHandle)
			{
				REDEEMER_LOG << LOG_FATAL << L"Creating a thread failed!" << LOG_ENDMESSAGE;

			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Thread::Terminate()
		{
			TerminateThread (m_ThreadHandle, static_cast<DWORD>(-1));

			m_IsRunning = false;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Thread::TerminateSafe ()
		{
			// Exit the thread
			if (!m_IsRunning)
				return;

			m_IsWaiting = false;
			m_IsRunning = false;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Thread::Sleep (unsigned long miliseconds)
		{
			::Sleep (miliseconds);

			m_IsRunning = false;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Thread::Suspend ()
		{
			SuspendThread (m_ThreadHandle);

			m_IsRunning = false;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Thread::Resume()
		{
			// Thread is already run, so exit
			if (m_IsRunning)
				return;

			ResumeThread (m_ThreadHandle);

			m_IsRunning = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Thread::Wait ()
		{
			// Wait for the object if necessary
			if (m_IsWaiting || !m_IsRunning)
				return;

			m_IsWaiting = true;

			WaitForSingleObject (m_EventHandle, INFINITE);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Thread::Signal ()
		{
			if (!m_IsWaiting)
				return;

			if (!SetEvent (m_EventHandle))
			{
				REDEEMER_LOG << LOG_FATAL << L"Singaling a thread's event object failed!" << LOG_ENDMESSAGE;
			}

			m_IsWaiting = false;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_Thread::IsRunning () const
		{
			return m_IsRunning;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_Thread::IsWaiting () const
		{
			return m_IsWaiting;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace CORE
}	//	namespace REDEEMER
