/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_System.cpp
**		Handling system-level tasks
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Core_System.h"
#include "R_Core_Window.h"
#include "R_Core_Convert.h"
#include "..\RedeemerEngine.h"

// disable: deprecation warnings when using CRT calls in VC8
#pragma warning (disable: 4996)

namespace REDEEMER
{
	namespace CORE
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_System::C_System () :
			m_IsLoop (false),
			m_IsRunning (false),
			m_MainWindow (NULL)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_System::~C_System ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_System::LaunchWindow (C_Window* window)
		{
			if (m_IsRunning)
				throw SystemException (L"System is already launched");

			if (window == 0)
				throw SystemException (L"'window' is NULL");			

			GetSystemMonitorsInfo ();

			m_MainWindow = window;

			AddWindow (window);

			EnterMainLoop ();

			RemoveWindow (window);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_System::AddWindow (C_Window* window)
		{	
			window->PrepareWindow();

			m_Windows.push_back (window);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_System::RemoveWindow (C_Window* window)
		{
			using std::list;

			list<C_Window*>::iterator it = m_Windows.begin();

			for (; it != m_Windows.end(); ++it)
			{
				if ((*it) == window)
				{
					window->DestroyWindow();
					m_Windows.erase (it);

					return;
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Window* C_System::GetMainWindow()
		{
			return m_MainWindow;
		}

		//------------------------------------------------------------------------------------------------------------------------

		const HINSTANCE C_System::GetInstance () const
		{
			return GetModuleHandle (0);
		}

		//------------------------------------------------------------------------------------------------------------------------

		const S_SystemMonitor* C_System::GetSystemMonitor (unsigned id) const
		{
			for (unsigned i = 0; i < m_SystemMonitors.size (); ++i)
				if (m_SystemMonitors[i].id == id)
					return &m_SystemMonitors[i];

			REDEEMER_LOG << LOG_WARNING << "Specified monitor has not been found [ using primary monitor ]"
				<< LOG_ENDLINE << LOG_DEBUG_INFO << LOG_ENDMESSAGE;

			return GetPrimarySystemMonitor ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		const S_SystemMonitor* C_System::GetSystemMonitor (HMONITOR monitor) const
		{
			for (unsigned i = 0; i < m_SystemMonitors.size (); ++i)
				if (m_SystemMonitors[i].monitor == monitor)
					return &m_SystemMonitors[i];

			REDEEMER_LOG << LOG_WARNING << "Specified monitor has not been found [ using primary monitor ]"
				<< LOG_ENDLINE << LOG_DEBUG_INFO << LOG_ENDMESSAGE;

			return GetPrimarySystemMonitor ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		const S_SystemMonitor* C_System::GetPrimarySystemMonitor () const
		{
			return m_PrimarySystemMonitor;
		}

		//------------------------------------------------------------------------------------------------------------------------

		HICON C_System::LoadIconFromResource (int id)
		{
			HICON icon = LoadIcon (GetInstance (), MAKEINTRESOURCE (id));

			if (!icon)
			{
				LOGERROR (REDEEMER_LOG, L"Loading icon failed");
			}

			return icon;
		}

		//------------------------------------------------------------------------------------------------------------------------

		HBITMAP C_System::LoadBitmapFromResource (int id)
		{
			HBITMAP bitmap = LoadBitmap (GetInstance (), MAKEINTRESOURCE (id));

			if (!bitmap)
			{
				LOGERROR (REDEEMER_LOG, L"Loading bitmap failed");
			}

			return bitmap;
		}

		//------------------------------------------------------------------------------------------------------------------------

		HCURSOR	C_System::LoadCursorFromResource (int id)
		{
			HCURSOR cursor = 0;

			if (id == 0)
			{
				cursor = LoadCursor (0, IDC_ARROW);
			}
			else
				cursor = LoadCursor (GetInstance (), MAKEINTRESOURCE (id));


			if (!cursor)
			{
				LOGERROR (REDEEMER_LOG, L"Loading cursor failed");
			}

			return cursor;
		}

		//------------------------------------------------------------------------------------------------------------------------

		HBRUSH	C_System::LoadBrush (DWORD color)
		{
			HBRUSH brush = CreateSolidBrush (color);

			if (!brush)
			{
				LOGERROR (REDEEMER_LOG, L"Failed to create brush");
			}

			return brush;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_System::EnterMainLoop()  throw (C_Excpetion)
		{
			MSG		msg;
			BOOL	ret;

			msg.message = WM_NULL;

			m_IsLoop = true;
			m_IsRunning = true;

			while (m_IsLoop)
			{
				if (Idle ())
				{
					if (PeekMessage (&msg, 0, 0, 0, PM_REMOVE))
					{
						if (msg.message == WM_QUIT)
						{
							m_IsLoop = false;
						}
						else
						{
							TranslateMessage (&msg);
							DispatchMessage (&msg);
						}
					}
					else
					{
						//	Update all engine's systems
						C_RedeemerEngine::GetSingleton().UpdateSystems();

						DoIdle();
					}
				}
				else
				{	
					if ((ret = GetMessage (&msg, 0, 0, 0)) == -1)
					{
						throw SystemException (L"Main loop error");
					}
					else
					{
						if (!ret)
						{
							m_IsLoop = false;
						}
						else
						{
							TranslateMessage (&msg);
							DispatchMessage (&msg);
						}
					}
				}				
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_System::DoIdle ()
		{
			for (std::list<C_Window*>::iterator it = m_Windows.begin(); it != m_Windows.end(); ++it)
			{
				if ((*it)->IsIdle ())
					(*it)->DoIdle ();
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_System::Idle ()
		{
			for (std::list<C_Window*>::iterator it = m_Windows.begin(); it != m_Windows.end(); ++it)
			{
				if((*it)->IsIdle ())
					return true;
			}

			return false;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_System::GetSystemMonitorsInfo ()
		{
			m_SystemMonitors.clear ();

			DISPLAY_DEVICE dd;
			dd.cb = sizeof (dd);

			DWORD dev = 0;	// device index
			int id = 1;		// monitor number, as used by Display Properties > Settings

			S_SystemMonitor monitor;

			REDEEMER_LOG << LOG_INFO << L"Monitors info:" << LOG_ENDLINE << LOG_ENDLINE;

			while (EnumDisplayDevices (0, dev, &dd, 0))
			{
				if (!(dd.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER))
				{
					// ignore virtual mirror displays

					// get information about the monitor attached to this display adapter. dualhead cards
					// and laptop video cards can have multiple monitors attached

					DISPLAY_DEVICE	ddMon;

					ZeroMemory(&ddMon, sizeof(ddMon));

					ddMon.cb		= sizeof(ddMon);
					DWORD devMon	= 0;

					// please note that this enumeration may not return the correct monitor if multiple monitors
					// are attached. this is because not all display drivers return the ACTIVE flag for the monitor
					// that is actually active
					while (EnumDisplayDevices (dd.DeviceName, devMon, &ddMon, 0))
					{
						if (ddMon.StateFlags & DISPLAY_DEVICE_ACTIVE)
							break;

						devMon++;
					}

					if (!*ddMon.DeviceString)
					{
						EnumDisplayDevices(dd.DeviceName, 0, &ddMon, 0);

						if (!*ddMon.DeviceString)
							wcscpy (ddMon.DeviceString, L"Default Monitor");
					}

					// get information about the display's position and the current display mode
					DEVMODE	dm;

					ZeroMemory(&dm, sizeof(dm));

					dm.dmSize	= sizeof(dm);

					if (EnumDisplaySettingsEx(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm, 0) == FALSE)
						EnumDisplaySettingsEx(dd.DeviceName, ENUM_REGISTRY_SETTINGS, &dm, 0);
									 
					// get the monitor handle and workspace
					HMONITOR	hm = 0;
					MONITORINFO	mi;

					ZeroMemory(&mi, sizeof(mi));

					mi.cbSize	= sizeof(mi);

					if (dd.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP)
					{
						// display is enabled. only enabled displays have a monitor handle
						POINT	pt = { dm.dmPosition.x, dm.dmPosition.y };

						hm = MonitorFromPoint(pt, MONITOR_DEFAULTTONULL);

						if( hm)
							GetMonitorInfo(hm, &mi);
					}

					//MyMonitor on MyVideoCard
					std::string tmp;

					C_Convert::ConvertUnicodeToChars (&tmp, ddMon.DeviceString);

					REDEEMER_LOG << id << ". " << tmp << L" on " << dd.DeviceString << LOG_ENDLINE;

					monitor.monitor		= hm;
					monitor.name		= ddMon.DeviceString;
					monitor.adapterName	= dd.DeviceString;
					monitor.primary		= false;
					monitor.id			= id;

					// status flags: primary, disabled, removable
					if(!(dd.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP))
						REDEEMER_LOG << L"disabled" << LOG_ENDLINE;
					else if(dd.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)
					{
						monitor.primary	= true;
						REDEEMER_LOG << L"primary" << LOG_ENDLINE;
					}
					if(dd.StateFlags & DISPLAY_DEVICE_REMOVABLE)
						REDEEMER_LOG << L"removable" << LOG_ENDLINE;

					// width x height @ x,y - bpp - refresh rate
					// note that refresh rate information is not available on Win9x

					REDEEMER_LOG << dm.dmPelsWidth
						<< L" x " 
						<< dm.dmPelsHeight 
						<< L" @ " 
						<< dm.dmPosition.x 
						<< L"," 
						<< dm.dmPosition.y 
						<< L" - " 
						<< dm.dmBitsPerPel 
						<< L"-bit - " 
						<< dm.dmDisplayFrequency 
						<< L"Hz" << LOG_ENDLINE;

					monitor.width		= dm.dmPelsWidth;
					monitor.height		= dm.dmPelsHeight;
					monitor.x			= dm.dmPosition.x;
					monitor.y			= dm.dmPosition.y;
					monitor.depth		= dm.dmBitsPerPel;
					monitor.frequency	= dm.dmDisplayFrequency;

					if(hm)
					{
						monitor.worksapce	= mi.rcWork;
					}

					// device name
					if(*ddMon.DeviceName)
					{
						REDEEMER_LOG << L"Device: " << ddMon.DeviceName << LOG_ENDLINE << LOG_ENDLINE;
						monitor.device	= ddMon.DeviceName;
					}
					else
					{
						REDEEMER_LOG << L"Device: " << dd.DeviceName << LOG_ENDLINE << LOG_ENDLINE;
						monitor.device	= dd.DeviceName;
					}

					id++;

					m_SystemMonitors.push_back(monitor);
				}

				dev++;
			}

			REDEEMER_LOG << LOG_ENDMESSAGE;

			for (unsigned i = 0; i < m_SystemMonitors.size (); ++i)
				if (m_SystemMonitors[i].primary)
				{
					m_PrimarySystemMonitor = &m_SystemMonitors[i];
					break;
				}
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace CORE
}	//	namespace REDEEMER

