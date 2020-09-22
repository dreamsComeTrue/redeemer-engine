/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	S_GameWindow.h
**		Game window class
*/

//------------------------------------------------------------------------------------------------------------------------
#include "Core\\R_Core_SmartPointer.h"

#include "S_GameConsoleWindow.h"

namespace REDEEMER
{
	namespace SHAKE
	{
		/*!
		**	\brief A S_GameWindow class derives from C_Window and has basic functionality for game loop
		*/
		class S_GameWindow : public CORE::C_Window
		{
		public:
			///	Constructor
			S_GameWindow ();

			///	Destructor
			virtual ~S_GameWindow ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize ();

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	\brief Function fired after the window has been created
			*/
			virtual void OnCreate();

			/*!	\brief Function called after the window has been resized
			*/
			virtual void OnResize (unsigned width, unsigned height);

			/*!	\brief Function fired getting message 
			*/
			virtual bool OnMessage(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

			/*!	\brief Function fired after window is activated
			*/
			virtual void OnActivate();

			/*!	\brief Function fired after window is deactivated
			*/
			virtual void OnDeactivate();

			/*!	\brief Function fired when window is in 'idle' state
			*/
			virtual void OnIdle();

			/*!	\brief Function fired after monitor change
			*/
			virtual void OnMonitorChange (const CORE::S_SystemMonitor* const monitor);

		private:
			CORE::C_SmartPointer<S_GameConsoleWindow>	m_GameConsoleWindow;	///<	Game console window
		};
	}	//	namespace SHAKE
}	//	namespace REDEEMER