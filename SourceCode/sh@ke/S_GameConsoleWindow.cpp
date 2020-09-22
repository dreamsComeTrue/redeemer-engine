/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	S_GameConsoleWindow.cpp
**		Game console window class
*/

//------------------------------------------------------------------------------------------------------------------------
#include "S_GameConsoleWindow.h"
#include "..\\Redeemer\\RedeemerEngine.h"

#include <Richedit.h>
#include <commctrl.h>

namespace REDEEMER
{
	namespace SHAKE
	{
		S_GameConsoleWindow* g_ConsoleWindow;

		//------------------------------------------------------------------------------------------------------------------------

		LRESULT CALLBACK GlobalConsoleCommandLineProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			return g_ConsoleWindow->ConsoleCommandLineProc (hwnd, msg, wParam, lParam);
		}

		//------------------------------------------------------------------------------------------------------------------------

		S_GameConsoleWindow::S_GameConsoleWindow () :
			m_HistoryIndex (0),
			m_RichEditModule (0)
		{
			InitCommonControls();

			m_RichEditModule = LoadLibrary(L"Riched20.dll");

			if (m_RichEditModule == 0)
				throw SystemException (L"Failed to load Riched20.dll");
		}

		//------------------------------------------------------------------------------------------------------------------------

		S_GameConsoleWindow::~S_GameConsoleWindow ()
		{
			SetWindowLong (m_ConsoleCommandLine, GWL_WNDPROC, reinterpret_cast<LONG>(m_OldCommandlineProc));			

			if (m_RichEditModule)
				FreeLibrary (m_RichEditModule);		
		}

		//------------------------------------------------------------------------------------------------------------------------

		int S_GameConsoleWindow::Initialize ()
		{
			CORE::S_CreateWindowInfo* cwi = GetCreateInfo();

			cwi->caption		= L"sh@ke Console [Powered by Redeemer (C) Engine]";
			cwi->className		= L"sh@keConsole";
			cwi->basicStyles	= WS_CAPTION | WS_VISIBLE;
			cwi->extendedStyles	= 0;
			cwi->classStyles	= 0;
			cwi->width			= 620;
			cwi->height			= 520;
			cwi->x				= 0;
			cwi->y				= 0;
			cwi->brush			= C_RedeemerEngine::GetSingleton().GetSystem()->LoadBrush(12345608);
			cwi->cursor			= C_RedeemerEngine::GetSingleton().GetSystem()->LoadCursorFromResource(0);
			cwi->iconBig		= C_RedeemerEngine::GetSingleton().GetSystem()->LoadIconFromResource(101);
			cwi->iconSmall		= C_RedeemerEngine::GetSingleton().GetSystem()->LoadIconFromResource(101);
			cwi->clientRect		= true;

			try
			{
				C_RedeemerEngine::GetSingleton().GetSystem()->AddWindow(this);

				g_ConsoleWindow = this;

				// copy button
				m_ConsoleButtonCopy	= CreateWindow(L"button", L"copy", WS_CHILD | WS_VISIBLE,
					10,480,72,22, GetHWND(), (HMENU)222, C_RedeemerEngine::GetSingleton().GetSystem()->GetInstance(), NULL);

				if(!m_ConsoleButtonCopy)
					throw SystemException(L"Failed to create copy button");

				// clear button
				m_ConsoleButtonClear = CreateWindow(L"button", L"clear", WS_CHILD | WS_VISIBLE,
					92,480,72,22, GetHWND(), (HMENU)223, C_RedeemerEngine::GetSingleton().GetSystem()->GetInstance(), NULL);

				if(!m_ConsoleButtonClear)
					throw SystemException(L"Failed to create clear button");

				// execute button
				m_ConsoleButtonExecute = CreateWindow(L"button", L"execute", WS_CHILD | WS_VISIBLE,
					538,480,72,22, GetHWND(), (HMENU)224, C_RedeemerEngine::GetSingleton().GetSystem()->GetInstance(), NULL);

				if(!m_ConsoleButtonExecute)
					throw SystemException(L"Failed to create execute button");

				// command line edit box
				m_ConsoleCommandLine = CreateWindowEx(WS_EX_CLIENTEDGE,L"EDIT", L"", ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE,
					10,446,600,22, GetHWND(), (HMENU)225, C_RedeemerEngine::GetSingleton().GetSystem()->GetInstance(), NULL);

				if(!m_ConsoleCommandLine)
					throw SystemException(L"Failed to create commandline editbox");

				// limit text to 512 chars
				SendMessage(m_ConsoleCommandLine, EM_LIMITTEXT, 512, 0);

				// console edit box window
				m_ConsoleEditBox = CreateWindowEx(WS_EX_CLIENTEDGE, RICHEDIT_CLASS, L"",
					ES_READONLY | WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE,
					10, 10, 600, 426,
					GetHWND(), (HMENU)226, C_RedeemerEngine::GetSingleton().GetSystem()->GetInstance(), 0);

				if(!m_ConsoleEditBox)
					throw SystemException(L"Failed to create console editbox");

				SendMessage(m_ConsoleEditBox, EM_SETBKGNDCOLOR, 0, RGB(245,245,245));
				SendMessage(m_ConsoleEditBox, EM_LIMITTEXT, -1, 0);
				SendMessage(m_ConsoleEditBox, EM_SETLANGOPTIONS, 0, 0);

				SendMessage(m_ConsoleEditBox, EM_REPLACESEL, 0, (LPARAM)L"[SIMPLE SAMPLE OF CONSOLE]\r\n\r\n");

				// device context for fonts creation
				m_ConsoleDC	= GetDC(GetHWND());

				if(!m_ConsoleDC)
					throw SystemException(L"Failed to create console device context");

				// Verdana font 8pt
				m_FontVerdana = CreateFont(-MulDiv(8, GetDeviceCaps(m_ConsoleDC, LOGPIXELSY), 72),
					0,0,0,FW_NORMAL,false,false,false,DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
					FF_DONTCARE,L"Verdana");

				if(!m_FontVerdana)
					throw SystemException(L"Failed to create font \"Verdana\"");

				// Courier New font 8pt
				m_FontLucidaConsole	= CreateFont(-MulDiv(9, GetDeviceCaps(m_ConsoleDC, LOGPIXELSY), 72),
					0,0,0,FW_NORMAL,false,false,false,DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
					FF_DONTCARE,L"Lucida Console");

				if(!m_FontLucidaConsole)
					throw SystemException(L"Failed to create font \"Lucida Console\"");

				// changing fonts in controls
				SendMessage(GetDlgItem(GetHWND(), 222), WM_SETFONT, (WPARAM)m_FontVerdana, TRUE); 
				SendMessage(GetDlgItem(GetHWND(), 223), WM_SETFONT, (WPARAM)m_FontVerdana, TRUE); 
				SendMessage(GetDlgItem(GetHWND(), 224), WM_SETFONT, (WPARAM)m_FontVerdana, TRUE); 
				SendMessage(GetDlgItem(GetHWND(), 225), WM_SETFONT, (WPARAM)m_FontLucidaConsole, TRUE); 
				SendMessage(GetDlgItem(GetHWND(), 226), WM_SETFONT, (WPARAM)m_FontLucidaConsole, TRUE); 

				m_OldCommandlineProc = reinterpret_cast<WNDPROC>(SetWindowLong(m_ConsoleCommandLine, GWL_WNDPROC, reinterpret_cast<LONG>(GlobalConsoleCommandLineProc)));

			}
			catch (CORE::C_Excpetion& exception)
			{
				C_RedeemerEngine::GetSingleton().GetLogger()->FlushToFileWithError (exception.File(), exception.Function(), exception.Line(), exception.Description());

				return -1;
			}

			return 0;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int S_GameConsoleWindow::Finalize ()
		{
			C_RedeemerEngine::GetSingleton().GetSystem()->RemoveWindow(this);

			return 0;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void S_GameConsoleWindow::OnCreate()
		{

		}

		//------------------------------------------------------------------------------------------------------------------------

		void S_GameConsoleWindow::OnResize (unsigned width, unsigned height)
		{

		}

		//------------------------------------------------------------------------------------------------------------------------

		bool S_GameConsoleWindow::OnMessage(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			switch (msg)
			{
			case WM_CREATE:
				DisableIdle();

				return false;

			case WM_COMMAND:
				switch (LOWORD (wParam))
				{
				case 222:
				case 223:
				case 224:
					InfoMsg (L"To tylko przykladowa aplikacja.. Nie chcialo mi sie implementowac wszystkiego ;p");
				};

			case WM_DESTROY:
				return false;

			case WM_CLOSE:
				return false;

			default:
				return true;
			}

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void S_GameConsoleWindow::OnActivate()
		{

		}

		//------------------------------------------------------------------------------------------------------------------------

		void S_GameConsoleWindow::OnDeactivate()
		{

		}

		//------------------------------------------------------------------------------------------------------------------------

		void S_GameConsoleWindow::OnIdle()
		{

		}

		//------------------------------------------------------------------------------------------------------------------------

		void S_GameConsoleWindow::OnMonitorChange (const CORE::S_SystemMonitor* const monitor)
		{

		}

		//------------------------------------------------------------------------------------------------------------------------

		void S_GameConsoleWindow::Write(const std::wstring& message)
		{				
			SendMessage (m_ConsoleEditBox, EM_SETSEL, -1, -1);
			SendMessage (m_ConsoleEditBox, EM_REPLACESEL, 0, (LPARAM)message.c_str());
			SendMessage (m_ConsoleEditBox, EM_SCROLL, SB_BOTTOM, 0);
		}

		//------------------------------------------------------------------------------------------------------------------------

		LRESULT CALLBACK S_GameConsoleWindow::ConsoleCommandLineProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			switch (msg)
			{
			case WM_KEYDOWN:
				switch(wParam)
				{
				case VK_RETURN:
					{
						wchar_t command[512];

						GetWindowText (m_ConsoleCommandLine, command, 512);

						if (command[0] == '\0')
							return 0;

						Write (command);
						Write (L"\r\n");

						m_History.push_back(command);
						m_HistoryIndex = 0;

						SetWindowText (m_ConsoleCommandLine, L"");
					}
					return 0;

				case VK_DOWN:
					{
						int t = m_History.size();

						if (m_HistoryIndex <= 0 || t == 0)
						{
							SetWindowText (m_ConsoleCommandLine, L"");
							m_HistoryIndex = t;
						}
						else
						{
							SetWindowText (m_ConsoleCommandLine, m_History[m_HistoryIndex - 1].c_str());
							--m_HistoryIndex;
						}
					}
					return 0;

				case VK_UP:
					{
						int t = m_History.size();

						if (m_HistoryIndex >= t || t == 0)
						{
							SetWindowText (m_ConsoleCommandLine, L"");
							m_HistoryIndex = 0;
						}
						else
						{
							SetWindowText (m_ConsoleCommandLine, m_History[t - 1 - m_HistoryIndex].c_str());
							++m_HistoryIndex;
						}
					}
					return 0;
				}
				break;
			}

			return CallWindowProc (m_OldCommandlineProc, hwnd, msg, wParam, lParam);
		}

	}	//	namespace SHAKE
}	//	namespace REDEEMER