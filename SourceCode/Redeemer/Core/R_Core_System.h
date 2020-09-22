/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_System.h
**		Handling system-level tasks
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_CORE_SYSTEM_H_
#define _R_CORE_SYSTEM_H_

#include "R_Core_Exception.h"

#include <list>
#include <vector>
#include <windows.h>

// disable: C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
#pragma warning (disable: 4290)

namespace REDEEMER
{
	namespace CORE
	{
		class C_Window;

		/*!	\brief S_SystemMonitor describes monitor
		*/
		struct S_SystemMonitor
		{
			std::wstring	name;			///<	Monitor name
			std::wstring	adapterName;	///<	Graphics card name
			std::wstring	device;			///<	System name [ \\.\ DISPLAY... ]
			bool			primary;		///<	Is it default monitor
			int				id;				///<	Monitor ID 
			HMONITOR		monitor;		///<	Monitor handle						
			LONG			x;				///<	'X' position
			LONG			y;				///<	'Y' position
			DWORD			width;			///<	Pixel width
			DWORD			height;			///<	Pixel height
			DWORD			depth;			///<	Bit depth
			DWORD			frequency;		///<	Refresh frequency						
			RECT			worksapce;		///<	Workspace rect
		};

		/*!
		**	\brief A C_System class is responsible for dealing with windows, peripherals such as CPU, etc.
		*/
		class C_System
		{
		public:
			///	Constructor
			C_System ();

			///	Destructor
			virtual ~C_System ();

			/*!	Adds new window to the list	and enters it's main loop
			**	\param window an instance of window
			*/
			void LaunchWindow (C_Window* window) throw (C_Excpetion);

			/*!	Adds new window to the list
			**	\param window an instance of window
			*/
			void AddWindow (C_Window* window);

			/*!	Removes given window from the list
			**	\param window an instance of window
			*/
			void RemoveWindow (C_Window* window);

			/*!	Returns main window
			*/
			C_Window* GetMainWindow ();

			///	Returns instance handle of running application
			const HINSTANCE GetInstance () const;

			/*!	Returns monitor info
			**	\param id ID of the monitor
			**	\return a S_SystemMonitor struct describing monitor
			*/
			const S_SystemMonitor* GetSystemMonitor (unsigned id) const;

			/*!	Returns monitor info
			**	\param monitor HMONITOR id of the monitor
			**	\return a S_SystemMonitor struct describing monitor
			*/
			const S_SystemMonitor*	GetSystemMonitor (HMONITOR monitor) const;

			/*!	Returns monitor info
			**	\return a S_SystemMonitor struct describing primary monitor
			*/
			const S_SystemMonitor*	GetPrimarySystemMonitor () const;

			/*!	Grabs icon from resource
			**	\param id ID number of resource
			**	\return HICON resource
			*/
			HICON LoadIconFromResource (int id);

			/*!	Grabs bitmap from resource
			**	\param id ID number of resource
			**	\return HBITMAP resource
			*/
			HBITMAP LoadBitmapFromResource (int id);

			/*!	Grabs cursor from resource
			**	\param id ID number of resource
			**	\return HCURSOR resource
			*/
			HCURSOR	LoadCursorFromResource (int id);

			/*!	Grabs brush from resource
			**	\param color ID number of color
			**	\return HBRUSH resource
			*/
			HBRUSH	LoadBrush (DWORD color);

		private:	
			void EnterMainLoop()  throw (C_Excpetion);
			void DoIdle ();
			bool Idle ();
			void GetSystemMonitorsInfo ();

			std::list <C_Window*>			m_Windows;				///<	A list of running windows
			C_Window*						m_MainWindow;			///<	Main window
			bool							m_IsLoop;				///<	Is main loop activated
			bool							m_IsRunning;			///<	Is window running
			const S_SystemMonitor*			m_PrimarySystemMonitor;	///<	Default monitor
			std::vector<S_SystemMonitor>	m_SystemMonitors;		///<	A list of additional monitors
		};
	}	//	namespace CORE
}	//	namespace REDEEMER

#endif	//	_R_CORE_SYSTEM_H_
