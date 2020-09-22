/*
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_Window.h
**		Window handling routines & message loop
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_CORE_WINDOW_H_
#define _R_CORE_WINDOW_H_

#include "R_Core_BaseClass.h"
#include "R_Core_System.h"
#include "R_Core_VideoMode.h"

#include <string>

namespace REDEEMER
{
	namespace CORE
	{
		/*!	Callback 'Window' procedure is used to handle window's messages
		*/
		extern LRESULT CALLBACK GlobalWindowProcedure (HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

		/*!	\brief S_CreateWindowInfo hold all necessary information for creating window
		*/
		struct S_CreateWindowInfo
		{
			std::wstring	className;		///<	Name of the class
			std::wstring	caption;		///<	Title of the window
			bool			clientRect;		///<	Are sizes refering to client rectangle
			int				x;				///<	'x' position on the screen
			int				y;				///<	'y' position on the screen
			int				width;			///<	Window's width
			int				height;			///<	Window's height
			HICON			iconBig;		///<	Icon used as an application icon
			HICON			iconSmall;		///<	Icon used in title bar
			HCURSOR			cursor;			///<	Application cursor
			HBRUSH			brush;			///<	Brush used to paint background
			DWORD			basicStyles;	///<	Window's basic styles
			DWORD			extendedStyles;	///<	Window's extended styles
			UINT			classStyles;	///<	Window's class styles
		};

		/*!	\brief Enumeration of the window styles
		*/
		enum EWindowStyles
		{
			WS_None       = 0,      ///< No border / title bar (this flag and all others are mutually exclusive)
			WS_Titlebar   = 1 << 0, ///< Title bar + fixed border
			WS_Resize     = 1 << 1, ///< Titlebar + resizable border + maximize button
			WS_Close      = 1 << 2, ///< Titlebar + close button
			WS_Fullscreen = 1 << 3, ///< Fullscreen mode (this flag and all others are mutually exclusive)

			WS_Default = WS_Titlebar | WS_Resize | WS_Close ///< Default window style
		};

		/*!
		**	\brief A C_Window class is responsible for system's and user's messages' handling
		*/
		class C_Window : C_BaseClass
		{
			friend LRESULT CALLBACK GlobalWindowProcedure (HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
			friend class C_System;

		public:
			/*!	\brief	Constructor
			*/
			C_Window ();

			/*!	\brief Destructor
			*/
			virtual ~C_Window ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize () = 0;

			/*!	\brief	Finalization code
			*/
			virtual int Finalize () = 0;

			/*!	Easy creation of the window using styles and video modes - as opposite to filling S_CreateWindowInfo struct
			**	\param mode A C_VideoMode type
			**	\param style A WindowStyles type
			**	\param title Window's title
			*/
			void QuickWindow (C_VideoMode mode, EWindowStyles style, const std::wstring& title);

			/*! \brief Change the title of the window.
			**	\param caption New title
			*/
			void SetCaption (const std::wstring& caption);

			/*!	Returns window's caption
			*/
			std::wstring GetCaption() const;

			/*!	Returns window's class name
			*/
			std::wstring GetClassName() const;

			/*!	Returns window's create info data
			*/
			S_CreateWindowInfo* GetCreateInfo ();

			/*!	Shows standard error message box on the screen
			**	\param text text to show
			**	\param caption title of the message box
			*/
			void ErrorMsg (const std::wstring& text, const std::wstring& caption = L"") const;

			/*!	Shows standard warning message box on the screen
			**	\param text text to show
			**	\param caption title of the message box
			*/
			void WarningMsg (const std::wstring& text, const std::wstring& caption = L"") const;

			/*!	Shows standard info message box on the screen
			**	\param text text to show
			**	\param caption title of the message box
			*/
			void InfoMsg (const std::wstring& text, const std::wstring& caption = L"") const;

			/*!	Returns windows' handle
			*/
			const HWND GetHWND () const;

			/*!	Checks, whether window is active or not
			*/	
			bool IsActive () const;

			/*!	Checks, if window is in idle state
			*/
			bool IsIdle () const;

			/*!	Checks, if window is minimized
			*/
			bool IsMinimized () const;

			/*!	Checks, if window is maximized
			*/
			bool IsMaximized () const;

			/*!	Sets width of the window
			**	\param width New width
			**	\param clientArea Is new width referring to client area or whole window
			*/		   
			void SetWidth (LONG width, bool clientArea);

			/*!	Sets height of the window
			**	\param height New height
			**	\param clientArea Is new height referring to client area or whole window
			*/
			void SetHeight (LONG height, bool clientArea);

			/*!	Sets width and height of the window
			**	\param width New width
			**	\param height New height
			**	\param clientArea Is new size referring to client area or whole window
			*/
			void SetSize(LONG width, LONG height, bool clientArea);

			/*!	Returns window's width
			**	\param clientArea Is it width referring to client area or whole window
			*/
			int	GetWidth(bool clientArea) const;

			/*!	Returns window's height
			**	\param clientArea Is it height referring to client area or whole window
			*/
			int	GetHeight (bool clientArea) const;
			  
			/*!	Returns window's rect
			**	\param clientArea Is it width referring to client area or whole window
			*/
			const RECT* GetRect (bool clientArea) const;

			/*!	Sets window's basic styles
			**	\param styles new styles
			**	\param preserveClient should the client area be preserved
			*/
			int	SetBasicStyles(DWORD styles, bool preserveClient);

			/*!	Sets window's extended styles
			**	\param styles new styles
			**	\param preserveClient should the client area be preserved
			*/
			int	SetExtendedStyles(DWORD styles, bool preserveClient);

			/*!	Returns window's basic styles info
			*/
			DWORD GetBasicStyles() const;

			/*!	Returns window's extended styles info
			*/
			DWORD GetExtendedStyles () const;

			/*!	Enable window's 'Idle' state
			*/
			void EnableIdle();

			/*!	Disables window's 'Idle' state
			*/
			void DisableIdle();

			/*!	Enable window's clipping
			*/
			void EnableClipping();

			/*!	Disable window's clipping
			*/
			void DisableClipping();

			/*!	Checks, if window is clipping
			*/
			bool Clipping() const;

			/*!	Enable window's monitor changing
			*/
			void EnableMonitorChanging();

			/*!	Disable window's monitor changing
			*/
			void DisableMonitorChanging();

			/*!	Checks, if window's monitor is changing
			*/
			bool MonitorChanging() const;

			/*!	Shows window on the screen
			*/
			void Show();

			/*!	Hides window from the screen
			*/
			void Hide();

			/*!	Switches to windowed mode
			**	\param mode C_VideMode to switch to
			*/
			void SwitchToWindowed (const C_VideoMode& mode);

			/*!	Switches to fullscreen mode, where hardware accelerated is more useful
			**	\param mode C_VideMode to switch to
			*/
			void SwitchToFullscreen (const C_VideoMode& mode);

			/*!	\brief Show or hide mouse cursor
			**	The cursor is shown by default.
			**	\param show True to show, false otherwise
			*/
			void ShowMouseCursor (bool show = true);

			/*!	\brief Change the position of the mouse cursor
			**	\param x Left coordinate of the cursor, relative to the window.
			**	\param y Top coordinate of the cursor, relative to the window.
			*/
			void SetCursorPosition (unsigned int x, unsigned int y);

			/*!	\brief Change the position of the window on screen.
			**	\param x Left position
			**	\param y Top position
			*/
			void SetPosition (int x, int y);

			/*!	Tells if we are in fullscreen mode
			*/
			bool IsFullScreen () const;

		protected:
			/*!	\brief Function fired after the window has been created
			**	\remarks This function should be overriden in derived classes to perform appropriate tasks.
			*/
			virtual void OnCreate() = 0;

			/*!	\brief Function called after the window has been resized
			**	\remarks This function should be overriden in derived classes to perform appropriate tasks.
			*/
			virtual void OnResize (unsigned width, unsigned height) = 0;

			/*!	\brief Function fired getting message 
			**	\remarks This function should be overriden in derived classes to perform appropriate tasks.
			*/
			virtual bool OnMessage(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;

			/*!	\brief Function fired after window is activated
			**	\remarks This function should be overriden in derived classes to perform appropriate tasks.
			*/
			virtual void OnActivate() = 0;

			/*!	\brief Function fired after window is deactivated
			**	\remarks This function should be overriden in derived classes to perform appropriate tasks.
			*/
			virtual void OnDeactivate() = 0;

			/*!	\brief Function fired when window is in 'idle' state
			**	\remarks This function should be overriden in derived classes to perform appropriate tasks.
			*/
			virtual void OnIdle() = 0;

			/*!	\brief Function fired after monitor change
			**	\remarks This function should be overriden in derived classes to perform appropriate tasks.
			*/
			virtual void OnMonitorChange (const S_SystemMonitor* const monitor) = 0;

			/*!	Centers window on the screen
			*/
			void CenterWindow ();

		protected:
			bool					m_IsFullScreen;		///<	Are we in fullscreen mode

		private:
			LRESULT CALLBACK WindowProcedure (HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

			void PrepareWindow () throw(C_Excpetion);
			void DestroyWindow ();

			void UpdateSizePosition ();
			void ClipWindow ();
			void HandlePossibleMonitorChange ();

			void Activate();
			void Deactivate();
			void DoIdle();

			HWND					m_HWND;				///<	Window's handle
			std::wstring			m_ClassName;		///<	Class name
			DWORD					m_BasicStyles;
			DWORD					m_ExtendedStyles;

			S_CreateWindowInfo		m_CreateInfo;

			const S_SystemMonitor*	m_Monitor;			///<	Monitor which this window is attached to

			RECT					m_WindowRect;		///<	Window's rect
			RECT					m_ClientRect;		///<	Client's rect
			bool					m_IsActive;			///<	Is window active
			bool					m_IsMoving;			
			bool					m_IsMoved;
			bool					m_IsVisible;		///<	Is window visible
			bool					m_IsMonitorChange;
			bool					m_IsClipping;		///<	Clipping window to monitor size
			bool					m_IsIdle;			///<	Should 'Idle' method be called
			bool					m_IsDestroyed;		///<	Is window destroyed			
			std::wstring			m_Title;			///<	Title of the window

			C_VideoMode				m_VideoMode;		///<	Video mode
		};
	}	//	namespace CORE
}	//	namespace REDEEMER

#endif	//	 _R_CORE_WINDOW_H_
