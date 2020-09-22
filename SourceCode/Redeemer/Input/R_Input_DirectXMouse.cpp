/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Input_DirectXMouse.cpp
**		DirectX implementation of mouse device
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Input_DirectXMouse.h"
#include "R_Input_InputSystem.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace INPUT
	{
		#define INPUTDEVICE_MOUSE_DIRECTX_BUFFERSIZE 64

		//------------------------------------------------------------------------------------------------------------------------
		C_DirectXMouseDevice::C_DirectXMouseDevice (C_InputSystem* inputSystem, IDirectInput8* inputDevice, bool isBuffered, DWORD cooperationSettings) :
			C_MouseDevice (inputSystem->GetInputSystemName(), isBuffered, inputSystem),
			m_DirectInput (inputDevice),
			m_MouseDevice (NULL),
			m_CooperationSettings (cooperationSettings),
			m_HWND (NULL)
		{
			inputSystem->SetMouseUsed (true);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DirectXMouseDevice::~C_DirectXMouseDevice ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_DirectXMouseDevice::Initialize ()
		{
			C_BaseClass::Initialize();

			DIPROPDWORD dipdw;

			//	Clear old state
			m_State.Clear();

			dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
			dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			dipdw.diph.dwObj        = 0;
			dipdw.diph.dwHow        = DIPH_DEVICE;
			dipdw.dwData            = INPUTDEVICE_MOUSE_DIRECTX_BUFFERSIZE;

			if (FAILED(m_DirectInput->CreateDevice(GUID_SysMouse, &m_MouseDevice, NULL)))
			{
				REDEEMER_LOG << LOG_FATAL << L"Failed to create DirectX mouse device" << LOG_ENDMESSAGE;

				return RESULT_FAIL;
			}

			if (FAILED(m_MouseDevice->SetDataFormat(&c_dfDIMouse2)))
			{
				REDEEMER_LOG << LOG_FATAL << L"Failed to set format for DirectX mouse" << LOG_ENDMESSAGE;

				return RESULT_FAIL;
			}

			m_HWND = m_InputSystem->GetWindowHandle ();

			if (FAILED(m_MouseDevice->SetCooperativeLevel (m_HWND, m_CooperationSettings)))
			{
				REDEEMER_LOG << LOG_FATAL << L"Failed to set cooperation level for DirectX mouse" << LOG_ENDMESSAGE;

				return RESULT_FAIL;
			}

			if (FAILED (m_MouseDevice->SetProperty (DIPROP_BUFFERSIZE, &dipdw.diph)))
			{
				REDEEMER_LOG << LOG_FATAL << L"Failed to set buffer size property for DirectX mouse" << LOG_ENDMESSAGE;

				return RESULT_FAIL;
			}

			HRESULT hr = m_MouseDevice->Acquire ();
			
			if (FAILED(hr) && hr != DIERR_OTHERAPPHASPRIO)
			{
				REDEEMER_LOG << LOG_FATAL << L"Failed to set acquire DirectX mouse" << LOG_ENDMESSAGE;

				return RESULT_FAIL;
			}

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_DirectXMouseDevice::Finalize ()
		{
			if (m_MouseDevice)
			{
				m_MouseDevice->Unacquire();

				REDEEMER_SAFE_RELEASE (m_MouseDevice);
			}

			m_InputSystem->SetMouseUsed (false);

			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectXMouseDevice::SetBuffered (bool buffered)
		{
			m_IsBuffered = buffered;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_InputDeviceInterface* C_DirectXMouseDevice::QueryInterface (C_InputDeviceInterface::EInputDeviceInterfaceType type)
		{
			return NULL;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectXMouseDevice::RetrieveData ()
		{
			//	Clear old relative values
			m_State.X.Rel = m_State.Y.Rel = m_State.Z.Rel = 0;

			DIDEVICEOBJECTDATA diBuff[INPUTDEVICE_MOUSE_DIRECTX_BUFFERSIZE];
			DWORD entries = INPUTDEVICE_MOUSE_DIRECTX_BUFFERSIZE;

			HRESULT hr = m_MouseDevice->GetDeviceData (sizeof(DIDEVICEOBJECTDATA), diBuff, &entries, 0);

			if (hr != DI_OK)
			{
				hr = m_MouseDevice->Acquire();

				while (hr == DIERR_INPUTLOST) 
					hr = m_MouseDevice->Acquire ();

				hr = m_MouseDevice->GetDeviceData (sizeof(DIDEVICEOBJECTDATA), diBuff, &entries, 0);

				//Perhaps the user just tabbed away, and coop settings are nonexclusive..so just ignore
				if (FAILED (hr))
					return;
			}

			bool axesMoved = false;

			//	Accumulate all axis movements for one axesMove message..	Buttons are fired off as they are found
			for (unsigned int i = 0; i < entries; ++i)
			{
				switch( diBuff[i].dwOfs )
				{
				case DIMOFS_BUTTON0:
					if(!DoMouseClick(0, diBuff[i])) return;
					break;

				case DIMOFS_BUTTON1:
					if(!DoMouseClick(1, diBuff[i])) return;
					break;

				case DIMOFS_BUTTON2:
					if(!DoMouseClick(2, diBuff[i])) return;
					break;

				case DIMOFS_BUTTON3:
					if(!DoMouseClick(3, diBuff[i])) return;
					break;

				case DIMOFS_BUTTON4:
					if(!DoMouseClick(4, diBuff[i])) return;
					break;

				case DIMOFS_BUTTON5:
					if(!DoMouseClick(5, diBuff[i])) return;
					break;

				case DIMOFS_BUTTON6:
					if(!DoMouseClick(6, diBuff[i])) return;
					break;

				case DIMOFS_BUTTON7:
					if(!DoMouseClick(7, diBuff[i])) return;
					break;

				case DIMOFS_X:
					m_State.X.Rel += diBuff[i].dwData;
					axesMoved = true;
					break;

				case DIMOFS_Y:
					m_State.Y.Rel += diBuff[i].dwData;
					axesMoved = true;
					break;

				case DIMOFS_Z:
					m_State.Z.Rel += diBuff[i].dwData;
					axesMoved = true;
					break;

				default: 
					break;
				} 
			}

			if (axesMoved)
			{
				if (m_CooperationSettings & DISCL_NONEXCLUSIVE)
				{
					//	DirectInput provides us with meaningless values, so correct that
					POINT point;

					GetCursorPos (&point);
					ScreenToClient (m_HWND, &point);

					m_State.X.Abs = point.x;
					m_State.Y.Abs = point.y;
				}
				else
				{
					m_State.X.Abs +=  m_State.X.Rel;
					m_State.Y.Abs +=  m_State.Y.Rel;
				}

				m_State.Z.Abs +=  m_State.Z.Rel;

				//Clip values to window
				if (m_State.X.Abs < 0)
					m_State.X.Abs = 0;
				else 
					if (m_State.X.Abs > m_State.DisplayAreaWidth)
						m_State.X.Abs = m_State.DisplayAreaWidth;
				
				if (m_State.Y.Abs < 0 )
					m_State.Y.Abs = 0;
				else 
					if (m_State.Y.Abs > m_State.DisplayAreaHeight)
						m_State.Y.Abs = m_State.DisplayAreaHeight;

				//	Do the move
				if (!m_Listeners.empty() && m_IsBuffered)
				{
					for (unsigned int i = 0; i < m_Listeners.size(); i ++)
						m_Listeners[i]->MouseMoved (C_MouseEvent (m_State));
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_DirectXMouseDevice::DoMouseClick (int mouseButton, DIDEVICEOBJECTDATA& di)
		{
			bool result = true;

			if (di.dwData & 0x80)
			{
				m_State.Buttons |= 1 << mouseButton; //turn the bit flag on

				if (!m_Listeners.empty() && m_IsBuffered)
				{
					for (unsigned int i = 0; i < m_Listeners.size(); i ++)
						result = m_Listeners[i]->MousePressed (C_MouseEvent (m_State), (EMouseButtonID)mouseButton);
				}
			}
			else
			{
				m_State.Buttons &= ~(1 << mouseButton); //turn the bit flag off
				
				if (!m_Listeners.empty() && m_IsBuffered)
				{
					for (unsigned int i = 0; i < m_Listeners.size(); i ++)
						result = m_Listeners[i]->MouseReleased (C_MouseEvent (m_State), (EMouseButtonID)mouseButton);
				}
			}

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectXMouseDevice::ShowCursor (bool show)
		{
			SetCursor (NULL);

			if (show)
			{
				HCURSOR hCursor = LoadCursor(NULL, IDC_ARROW);

				SetCursor (hCursor);
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectXMouseDevice::SetPosition (long x, long y)
		{
			SetCursorPos (x, y);
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace INPUT
}	//	namespace REDEEMER

