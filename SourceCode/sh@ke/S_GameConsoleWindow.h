/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	S_GameConsoleWindow.h
**		Game console window class
*/

//------------------------------------------------------------------------------------------------------------------------
#include "..\\Redeemer\\Core\\R_Core_Window.h"

namespace REDEEMER
{
	namespace SHAKE
	{
		extern LRESULT CALLBACK GlobalConsoleCommandLineProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		/*!
		**	\brief A S_GameConsoleWindow class derives from C_Window and is used as communication layer between game and an user
		*/
		class S_GameConsoleWindow : public CORE::C_Window
		{
			friend LRESULT CALLBACK GlobalConsoleCommandLineProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		public:
			///	Constructor
			S_GameConsoleWindow ();

			///	Destructor
			virtual ~S_GameConsoleWindow ();

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
			  
			/*!	Adds message to console
			**	\param message message to write
			*/
			void Write (const std::wstring& message);

		private:	
			LRESULT CALLBACK ConsoleCommandLineProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

			WNDPROC						m_OldCommandlineProc;
			HDC							m_ConsoleDC;

			HWND						m_ConsoleButtonCopy;
			HWND						m_ConsoleButtonClear;
			HWND						m_ConsoleButtonExecute;
			HWND						m_ConsoleEditBox;
			HWND						m_ConsoleCommandLine;

			HFONT						m_FontVerdana;
			HFONT						m_FontLucidaConsole;

			int							m_HistoryIndex;
			std::vector<std::wstring>	m_History;
			HMODULE						m_RichEditModule;
		};
	}	//	namespace SHAKE
}	//	namespace REDEEMER