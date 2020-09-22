/*
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_Window.cpp
**		Window handling routines & message loop
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Core_Window.h"
#include "..\\RedeemerEngine.h"

// disable:  forcing value to bool 'true' or 'false' (performance warning)
#pragma warning (disable: 4800)

#define PASS_MESSAGE_ON def = false; break

namespace REDEEMER
{
	namespace CORE
	{
		//------------------------------------------------------------------------------------------------------------------------
		LRESULT CALLBACK GlobalWindowProcedure (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			REDEEMER::CORE::C_Window* wnd = 0;

			if (msg == WM_CREATE)
			{
				wnd = reinterpret_cast<REDEEMER::CORE::C_Window*>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);

				SetWindowLong (hwnd, GWL_USERDATA, reinterpret_cast<LONG>(wnd));
			}

			wnd = reinterpret_cast<REDEEMER::CORE::C_Window*>(GetWindowLong (hwnd, GWL_USERDATA));

			if (wnd)
				return wnd->WindowProcedure (hwnd, msg, wParam, lParam);
			else
				return DefWindowProc (hwnd, msg, wParam, lParam);
		}

		//------------------------------------------------------------------------------------------------------------------------
		C_Window::C_Window ()  :
			m_IsDestroyed (false),
			m_HWND (0),
			m_IsFullScreen (false)
		{
			m_VideoMode = C_VideoMode (800, 600, 16);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Window::~C_Window ()
		{
			DestroyWindow ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::QuickWindow (C_VideoMode mode, EWindowStyles style, const std::wstring& title)
		{
			// Choose the window style according to the Style parameter
			DWORD win32Style = WS_VISIBLE;

			if (style == WS_None)
			{
				win32Style |= WS_POPUP;
			}
			else
			{
				if (style & WS_Titlebar) win32Style |= WS_CAPTION | WS_MINIMIZEBOX;
				if (style & WS_Resize)   win32Style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
				if (style & WS_Close)    win32Style |= WS_SYSMENU;
			}

			m_IsFullScreen = (style & WS_Fullscreen) != 0;

			m_CreateInfo.basicStyles		= win32Style;
			m_CreateInfo.width				= mode.m_Width;
			m_CreateInfo.height				= mode.m_Height;
			m_CreateInfo.caption			= title;
			m_CreateInfo.className			= title;
			m_CreateInfo.basicStyles		= win32Style;
			m_CreateInfo.extendedStyles		= 0;
			m_CreateInfo.classStyles		= 0;
			m_CreateInfo.x					= 0;
			m_CreateInfo.y					= 0;
			m_CreateInfo.brush				= C_RedeemerEngine::GetSingleton().GetSystem()->LoadBrush(12345608);
			m_CreateInfo.cursor				= NULL;
			m_CreateInfo.iconBig			= NULL;
			m_CreateInfo.iconSmall			= NULL;
			m_CreateInfo.clientRect			= true;

			m_VideoMode						= mode;
		}	

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::SetCaption (const std::wstring& caption)
		{
			SetWindowText (m_HWND, caption.c_str());

			m_Title = caption;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::wstring C_Window::GetCaption() const
		{
			return m_Title;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::wstring C_Window::GetClassName() const
		{
			return m_ClassName;
		}

		//------------------------------------------------------------------------------------------------------------------------

		S_CreateWindowInfo* C_Window::GetCreateInfo ()
		{
			return &m_CreateInfo;			
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::ErrorMsg (const std::wstring& text, const std::wstring& caption) const
		{
			if (caption.length () == 0)
				::MessageBox (m_HWND, text.c_str(), m_Title.c_str(), MB_OK | MB_ICONERROR);
			else
				::MessageBox (m_HWND, text.c_str(), caption.c_str(), MB_OK | MB_ICONERROR);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::WarningMsg (const std::wstring& text, const std::wstring& caption) const
		{
			if (caption.length () == 0)
				::MessageBox (m_HWND, text.c_str(), m_Title.c_str(), MB_OK | MB_ICONWARNING);
			else
				::MessageBox (m_HWND, text.c_str(), caption.c_str(), MB_OK | MB_ICONWARNING);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::InfoMsg (const std::wstring& text, const std::wstring& caption) const
		{
			if (caption.length () == 0)
				::MessageBox (m_HWND, text.c_str(), m_Title.c_str(), MB_OK | MB_ICONINFORMATION);
			else
				::MessageBox (m_HWND, text.c_str(), caption.c_str(), MB_OK | MB_ICONINFORMATION);
		}

		//------------------------------------------------------------------------------------------------------------------------

		const HWND C_Window::GetHWND () const
		{
			return m_HWND;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_Window::IsActive () const
		{
			return m_IsActive;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_Window::IsIdle () const
		{
			return m_IsIdle;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_Window::IsMinimized () const
		{
			return IsIconic (m_HWND);
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_Window::IsMaximized () const
		{
			return IsZoomed (m_HWND);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::SetWidth (LONG width, bool clientArea)
		{
			RECT r = { 0, 0, 0, 0 };

			int height = m_WindowRect.bottom - m_WindowRect.top;

			if (clientArea)
			{
				AdjustWindowRectEx (&r, m_BasicStyles, false, m_ExtendedStyles);

				width += r.right - r.left;
			}

			SetWindowPos (m_HWND, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

			UpdateSizePosition ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::SetHeight (LONG height, bool clientArea)
		{
			RECT r = { 0, 0, 0, 0 };
			int width = m_WindowRect.right - m_WindowRect.left;

			if (clientArea)
			{
				AdjustWindowRectEx (&r, m_BasicStyles, false, m_ExtendedStyles);

				height += r.bottom - r.top;
			}

			SetWindowPos (m_HWND, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

			UpdateSizePosition ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::SetSize (LONG width, LONG height, bool clientArea)
		{
			RECT r = { 0, 0, 0, 0 };

			if (clientArea)
			{
				AdjustWindowRectEx (&r, m_BasicStyles, false, m_ExtendedStyles);

				width += r.right - r.left;
				height += r.bottom - r.top;
			}

			SetWindowPos (m_HWND, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

			UpdateSizePosition ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int	C_Window::GetWidth (bool clientArea) const
		{
			if (clientArea)
				return m_ClientRect.right - m_ClientRect.left;
			else
				return m_WindowRect.right - m_WindowRect.left;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int	C_Window::GetHeight (bool clientArea) const
		{
			if (clientArea)
				return m_ClientRect.bottom - m_ClientRect.top;
			else
				return m_WindowRect.bottom - m_WindowRect.top;
		}

		//------------------------------------------------------------------------------------------------------------------------

		const RECT* C_Window::GetRect (bool clientArea) const
		{
			return clientArea ? &m_ClientRect : &m_WindowRect;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int SetStyle (EWindowStyles style, bool preserveClient);

		//------------------------------------------------------------------------------------------------------------------------

		int	C_Window::SetBasicStyles (DWORD styles, bool preserveClient)
		{
			if (styles == m_BasicStyles)
			{
				REDEEMER_LOG << LOG_WARNING << "New basic styles are same as old ones [" << styles << "]" << LOG_ENDMESSAGE;

				return 0;
			}

			// WARNING:	first hide the window, because when we attempt to change styles in showed window, strange things can happen
			ShowWindow (m_HWND, SW_HIDE);

			SetWindowLong (m_HWND, GWL_STYLE, styles);

			if (preserveClient)
			{
				RECT r;

				AdjustWindowRectEx (&r, styles, false, m_ExtendedStyles);

				int width = (m_ClientRect.right - m_ClientRect.left) + (r.right - r.left);
				int height = (m_ClientRect.bottom - m_ClientRect.top) + (r.bottom - r.top);

				SetWindowPos (m_HWND, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
			}		
			else
				SetWindowPos (m_HWND, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

			m_BasicStyles = styles;

			if (m_IsClipping)
				ClipWindow();
			else
				UpdateSizePosition ();

			if (m_IsVisible)
				Show ();

			return 0;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int	C_Window::SetExtendedStyles (DWORD styles, bool preserveClient)
		{
			if (styles == m_ExtendedStyles)
			{
				REDEEMER_LOG << LOG_WARNING << "New extended styles are same as old ones [" << styles << "]" << LOG_ENDMESSAGE;
				return 0;
			}

			// WARNING:	first hide the window, because when we attempt to change styles in showed window, strange things can happen
			ShowWindow (m_HWND, SW_HIDE);

			SetWindowLong (m_HWND, GWL_EXSTYLE, styles);

			if (preserveClient)
			{
				RECT r;

				AdjustWindowRectEx (&r, m_BasicStyles, false, styles);

				int width = (m_ClientRect.right - m_ClientRect.left) + (r.right - r.left);
				int height = (m_ClientRect.bottom - m_ClientRect.top) + (r.bottom - r.top);

				SetWindowPos(m_HWND, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
			}		
			else
				SetWindowPos(m_HWND, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

			m_ExtendedStyles = styles;

			if (m_IsClipping)
				ClipWindow();
			else
				UpdateSizePosition ();

			if (m_IsVisible)
				Show ();

			return 0;
		}

		//------------------------------------------------------------------------------------------------------------------------

		DWORD C_Window::GetBasicStyles() const
		{
			return m_BasicStyles;
		}

		//------------------------------------------------------------------------------------------------------------------------

		DWORD C_Window::GetExtendedStyles() const
		{
			return m_ExtendedStyles;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::EnableIdle()
		{
			m_IsIdle = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::DisableIdle()
		{
			m_IsIdle = false;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::EnableClipping()
		{
			m_IsClipping = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::DisableClipping()
		{
			m_IsClipping = false;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_Window::Clipping() const
		{
			return m_IsClipping;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::EnableMonitorChanging()
		{
			m_IsMonitorChange = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::DisableMonitorChanging()
		{
			m_IsMonitorChange = false;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_Window::MonitorChanging() const
		{
			return m_IsMonitorChange;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::Show()
		{
			ShowWindow (m_HWND, SW_SHOW);

			m_IsVisible = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::Hide()
		{
			ShowWindow (m_HWND, SW_HIDE);

			m_IsVisible = false;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::SwitchToWindowed (const C_VideoMode& mode)
		{
		   	if (!m_IsFullScreen)	
				return;

			DEVMODE devMode;
			devMode.dmSize       = sizeof(devMode);
			devMode.dmPelsWidth  = mode.m_Width;
			devMode.dmPelsHeight = mode.m_Height;
			devMode.dmBitsPerPel = mode.m_BitsPerPixel;
			devMode.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

			//	Change the screen resolution
			if (ChangeDisplaySettings(&devMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL )
			{
				REDEEMER_LOG << LOG_ERROR << "Failed to change display mode for windowed" << LOG_ENDMESSAGE;
				return;
			}

			RECT windowRect;
			windowRect.left = (long)0;
			windowRect.right = (long)mode.m_Width;
			windowRect.top = (long)0;
			windowRect.bottom = (long)mode.m_Height;
									
			AdjustWindowRectEx (&windowRect, WS_OVERLAPPEDWINDOW, false, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
			
			m_IsFullScreen = false;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::SwitchToFullscreen (const C_VideoMode& mode)
		{
			DEVMODE devMode;
			devMode.dmSize       = sizeof(devMode);
			devMode.dmPelsWidth  = mode.m_Width;
			devMode.dmPelsHeight = mode.m_Height;
			devMode.dmBitsPerPel = mode.m_BitsPerPixel;
			devMode.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

			// Apply fullscreen mode
			if (ChangeDisplaySettings (&devMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			{
				REDEEMER_LOG << LOG_ERROR << "Failed to change display mode for fullscreen" << LOG_ENDMESSAGE;
				return;
			}

			// Make the window flags compatible with fullscreen mode
			SetWindowLong (m_HWND, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
			SetWindowLong (m_HWND, GWL_EXSTYLE, WS_EX_APPWINDOW);

			// Resize the window so that it fits the entire screen
			SetWindowPos (m_HWND, HWND_TOP, 0, 0, mode.m_Width, mode.m_Height, SWP_FRAMECHANGED);
			ShowWindow (m_HWND, SW_SHOW);

			m_IsFullScreen = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::ShowMouseCursor (bool show)
		{
			if (show)
				m_CreateInfo.cursor = LoadCursor(NULL, IDC_ARROW);
			else
				m_CreateInfo.cursor = NULL;

			SetCursor (m_CreateInfo.cursor);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::SetCursorPosition (unsigned int x, unsigned int y)
		{
			POINT position = {x, y};

			ClientToScreen (m_HWND, &position);
			SetCursorPos (position.x, position.y);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::SetPosition (int x, int y)
		{
			SetWindowPos (m_HWND, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}

		//------------------------------------------------------------------------------------------------------------------------

		LRESULT CALLBACK C_Window::WindowProcedure (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			bool def = true;

			switch (msg)
			{
			case WM_ACTIVATE:
				{
					switch (LOWORD (wParam))
					{
					case WA_ACTIVE:
					case WA_CLICKACTIVE:
						if((BOOL)(HIWORD(wParam)))
							Deactivate ();
						else
							Activate();
						break;

					case WA_INACTIVE:
						Deactivate ();
						break;
					}
				}
				PASS_MESSAGE_ON;

			case WM_ENTERSIZEMOVE:
				m_IsMoving = true;
				m_IsMoved = false;
				PASS_MESSAGE_ON;

			case WM_EXITSIZEMOVE:
				m_IsMoving = false;

				if (m_IsMoved)
				{
					UpdateSizePosition ();
					HandlePossibleMonitorChange ();

					if (m_IsClipping)
						ClipWindow ();

					m_IsMoved = false;

					OnResize (m_ClientRect.right - m_ClientRect.left, m_ClientRect.bottom - m_ClientRect.top);
				}
				PASS_MESSAGE_ON;

			case WM_MOVE:
				m_IsMoved = true;
				PASS_MESSAGE_ON;

			case WM_SIZE:
				switch (wParam)
				{
				case SIZE_MAXIMIZED:
				case SIZE_MINIMIZED:
					UpdateSizePosition ();
					HandlePossibleMonitorChange();

					if (m_IsClipping)
						ClipWindow ();

					OnResize (m_ClientRect.right - m_ClientRect.left, m_ClientRect.bottom - m_ClientRect.top);
					break;

				case SIZE_RESTORED:
					if (m_IsMoving)
						m_IsMoved = true;
					else
					{
						UpdateSizePosition ();
						HandlePossibleMonitorChange();

						if (m_IsClipping)
							ClipWindow ();

						OnResize (m_ClientRect.right - m_ClientRect.left, m_ClientRect.bottom - m_ClientRect.top);
					}
					break;
				}
				PASS_MESSAGE_ON;

			case WM_CREATE:
				//	If something goes wrong here, '-1' will be returned which turns out to be a zero handle to 'CreateWindow*'
				m_HWND = hwnd;

				if (OnMessage (hwnd, msg, wParam, lParam))
				{
					LOGERROR (REDEEMER_LOG, L"Failed to create window");

					return -1;
				}

				UpdateSizePosition();

				if (REDEEMER::C_RedeemerEngine::GetSingleton().GetSystem()->GetMainWindow())
					REDEEMER::C_RedeemerEngine::GetSingleton().CreateDependantSystems();

				OnCreate ();

				return 0;

			default:
				def = true;
			}

			if (OnMessage (hwnd, msg, wParam, lParam) && def)
				return DefWindowProc (hwnd, msg, wParam, lParam);

			return 0;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::PrepareWindow () throw(C_Excpetion)
		{
			WNDCLASSEX wcx;

			wcx.cbSize			= sizeof(WNDCLASSEX);
			wcx.style			= m_CreateInfo.classStyles;
			wcx.lpfnWndProc		= GlobalWindowProcedure;
			wcx.cbClsExtra		= 0;
			wcx.cbWndExtra		= 0;
			wcx.hInstance		= C_RedeemerEngine::GetSingleton().GetSystem()->GetInstance();
			wcx.hIcon			= m_CreateInfo.iconBig;
			wcx.hCursor			= m_CreateInfo.cursor;
			wcx.hbrBackground	= m_CreateInfo.brush;
			wcx.lpszMenuName	= 0;
			wcx.lpszClassName	= m_CreateInfo.className.c_str();
			wcx.hIconSm			= m_CreateInfo.iconSmall;

			if (!RegisterClassEx(&wcx))
				throw SystemException (L"RegisterClassEx failed");

			RECT r;

			r.left		= 0;
			r.right		= m_CreateInfo.width;
			r.top		= 0;
			r.bottom	= m_CreateInfo.height;

			// If 'clientRect' is true - those sizes are referring only to client area of the window
			//	without title bar and frame

			if (m_CreateInfo.clientRect)
				AdjustWindowRectEx(&r, m_CreateInfo.basicStyles, false, m_CreateInfo.extendedStyles);

			m_IsActive			= false;
			m_IsMoving			= false;
			m_IsMoved			= false;

			m_ClassName			= m_CreateInfo.className;
			m_Title				= m_CreateInfo.caption;
			m_IsClipping		= false;
			m_IsIdle			= true;
			m_BasicStyles		= m_CreateInfo.basicStyles;
			m_ExtendedStyles	= m_CreateInfo.extendedStyles;
			m_Monitor			= C_RedeemerEngine::GetSingleton().GetSystem()->GetPrimarySystemMonitor();
			m_IsMonitorChange	= true;

			m_IsVisible = static_cast<bool>(m_BasicStyles & WS_VISIBLE);

			m_HWND = CreateWindowEx (m_ExtendedStyles, m_ClassName.c_str(), m_Title.c_str(), m_BasicStyles, m_CreateInfo.x + m_Monitor->x, 
				m_CreateInfo.y + m_Monitor->y, r.right - r.left, r.bottom - r.top, 0, 0, C_RedeemerEngine::GetSingleton().GetSystem()->GetInstance(), this);

			if (!m_HWND)
				throw SystemException (m_Title.c_str());

			UpdateSizePosition ();

			if (m_IsVisible)
				Show ();

			// Switch to fullscreen if requested
			if (m_IsFullScreen)
				SwitchToFullscreen (m_VideoMode);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::DestroyWindow ()
		{
			if (m_IsDestroyed)
				return;

			if (IsWindow (m_HWND))
			{
				if (!::DestroyWindow (m_HWND))
				{
					LOGERROR(REDEEMER_LOG, L"DestroyWindow failed");
				}
			}
			else
			{
				LOGERROR(REDEEMER_LOG, L"DestroyWindow failed");
			}

			if (!UnregisterClass(m_ClassName.c_str(), C_RedeemerEngine::GetSingleton ().GetSystem()->GetInstance()))
			{
				LOGERROR(REDEEMER_LOG, L"UnregisterClass failed");
			}

			m_IsDestroyed = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::UpdateSizePosition ()
		{
			GetWindowRect (m_HWND, &m_WindowRect);
			GetClientRect (m_HWND, &m_ClientRect);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::ClipWindow ()
		{
			RECT	winrc	= m_WindowRect;
			RECT	rc		= m_Monitor->worksapce;

			if (!(winrc.left < rc.left || winrc.top < rc.top || winrc.right > rc.right || winrc.bottom > rc.bottom))
				return;

			int		w		= m_WindowRect.right - m_WindowRect.left;
			int		h		= m_WindowRect.bottom - m_WindowRect.top;

			int		w_w		= m_Monitor->worksapce.right - m_Monitor->worksapce.left;
			int		w_h		= m_Monitor->worksapce.bottom - m_Monitor->worksapce.top;

			winrc.left   = max(rc.left, min(rc.right-w,  winrc.left));
			winrc.top    = max(rc.top,  min(rc.bottom-h, winrc.top));
			winrc.right  = winrc.left + w;
			winrc.bottom = winrc.top  + h;

			if (w > w_w)
			{
				w = w_w;
				winrc.left   = rc.left;
				winrc.right  = rc.right;
			}

			if (h > w_h)
			{
				h = w_h;
				winrc.top		= rc.top;
				winrc.bottom	= rc.bottom;
			}

			if(IsWindow (m_HWND))
			{
				if (!SetWindowPos (m_HWND, NULL, winrc.left, winrc.top, w, h, SWP_NOZORDER | SWP_NOACTIVATE))
				{
					LOGERROR (REDEEMER_LOG, L"SetWindowPos failed");
				}
				else
					UpdateSizePosition ();
			}
			else
			{
				LOGERROR (REDEEMER_LOG, L"IsWindow returned false");
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::CenterWindow ()
		{
			RECT	winrc	= m_WindowRect;
			RECT	rc		= m_Monitor->worksapce;
			int		w		= m_WindowRect.right - m_WindowRect.left;
			int		h		= m_WindowRect.bottom - m_WindowRect.top;

			winrc.left   = rc.left + (rc.right  - rc.left - w) / 2;
			winrc.top    = rc.top  + (rc.bottom - rc.top  - h) / 2;

			if (IsWindow (m_HWND))
			{
				if (!SetWindowPos (m_HWND, NULL, winrc.left, winrc.top, w, h, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE))
				{
					LOGERROR (REDEEMER_LOG, L"SetWindowPos failed");
				}
				else
					UpdateSizePosition ();
			}
			else
			{
				LOGERROR (REDEEMER_LOG, L"IsWindow returned false");
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::HandlePossibleMonitorChange ()
		{
			if (!m_IsMonitorChange)
				return;

			HMONITOR hm = MonitorFromWindow (m_HWND, MONITOR_DEFAULTTONEAREST);

			if (hm == m_Monitor->monitor)
				return;	// no monitor change

			const S_SystemMonitor* mon = C_RedeemerEngine::GetSingleton ().GetSystem()->GetSystemMonitor (hm);

			if (!mon)
			{
				LOGERROR (REDEEMER_LOG, L"GetSystemMonitor returned 0");
				return;
			}

			REDEEMER_LOG << LOG_DEBUG_INFO << "Monitor change [ current monitor: " << mon->device << " ]" << LOG_ENDMESSAGE;

			m_Monitor = mon;

			OnMonitorChange (mon);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::Activate()
		{
			if (!m_IsActive)
			{
				m_IsActive = true;

				OnActivate ();
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::Deactivate()
		{
			if (m_IsActive)
			{
				m_IsActive = false;

				OnDeactivate();
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Window::DoIdle()
		{
			OnIdle();
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_Window::IsFullScreen () const
		{
			return m_IsFullScreen;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace CORE
}	//	namespace REDEEMER
