/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_Thread.h
**		Threads represents different path of execution. This class can only be derived.
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_CORE_THREAD_H_
#define _R_CORE_THREAD_H_

#include <Windows.h>

namespace REDEEMER
{
	namespace CORE
	{
		DWORD WINAPI GlobalThreadProcedure (LPVOID data);

		/*!
		**	\brief Threads represents different path of execution. This class can only be derived.
		*/
		class C_Thread
		{
			friend DWORD WINAPI GlobalThreadProcedure (LPVOID data);

		public:
			/*!	Constructor
			*/
			C_Thread ();

			/*!	Destructor
			*/
			virtual ~C_Thread ();

			/*!	Begins executing thread
			*/
			void Launch ();

			/*!	Terminate the thread
			**	This function immediately stops the thread, without waiting for its function to finish.
			**	Terminating a thread with this function is not safe, and can lead to local variables not being destroyed
			**	on some operating systems. You should rather try to make the thread function terminate by itself.
			*/
			void Terminate();

			/*!	Safe version of terminate mehod
			*/
			void TerminateSafe ();

			/*!	Sleeps the thread for given time
			*/
			void Sleep (unsigned long miliseconds);

			/*!	Suspends thread execution
			*/
			void Suspend ();

			/*!	Awakes the thread.
			*/
			void Resume();

			/*!	Waits for the object to be released.
			*/
			void Wait ();

			/*!	Signals thread object
			*/
			void Signal ();

			/*!	Checks, if thread is executing
			*/
			bool IsRunning () const;

			/*!	Checks, if thread is waiting
			*/
			bool IsWaiting () const;

		protected:
			/*!	This method should be overriden in derived classes, and filled with thread's execution code
			*/
			virtual void Execute () = 0;

		private:
			bool m_IsRunning;
			bool m_IsWaiting;

			HANDLE m_ThreadHandle;
			HANDLE m_EventHandle;
		};

	}	//	namespace CORE
}	//	namespace REDEEMER

#endif	//	_R_CORE_THREAD_H_
