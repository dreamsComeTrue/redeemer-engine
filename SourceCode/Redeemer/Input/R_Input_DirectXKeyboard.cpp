/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Input_DirectXKeyboard.cpp
**		DirectX implementation of keyboard device
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Input_DirectXKeyboard.h"
#include "R_Input_InputSystem.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace INPUT
	{
		#define INPUTDEVICE_KEYBOARD_DIRECTX_BUFFERSIZE 17

		//------------------------------------------------------------------------------------------------------------------------
		C_DirectXKeyboardDevice::C_DirectXKeyboardDevice (C_InputSystem* inputSystem, IDirectInput8* inputDevice, bool isBuffered, DWORD cooperationSettings) :
			C_KeyboardDevice (inputSystem->GetInputSystemName(), isBuffered, inputSystem),
			m_KeyboardDevice (NULL),
			m_DirectInput (inputDevice),
			m_CooperationSettings (cooperationSettings)
		{
			m_DeadKey = '\0';

			//Clear our keyboard state buffer
			memset (&m_KeyBuffer, 0, 256);

			inputSystem->SetKeyboardUsed(true);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DirectXKeyboardDevice::~C_DirectXKeyboardDevice ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_DirectXKeyboardDevice::Initialize ()
		{
			C_BaseClass::Initialize();

			m_Modifiers = 0;
			m_DeadKey = '\0';

			if (FAILED (m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_KeyboardDevice, NULL)))
			{
				REDEEMER_LOG << LOG_FATAL << L"Could not initialize DirectX keyboard device!" << LOG_ENDMESSAGE;

				return RESULT_FAIL;
			}

			if (FAILED (m_KeyboardDevice->SetDataFormat(&c_dfDIKeyboard)))
			{
				REDEEMER_LOG << LOG_FATAL << L"Could not set proper format for DirectX keyboard device!" << LOG_ENDMESSAGE;

				return RESULT_FAIL;
			}

			HWND hwin = m_InputSystem->GetWindowHandle ();

			if (FAILED (m_KeyboardDevice->SetCooperativeLevel( hwin, m_CooperationSettings)))
			{
				REDEEMER_LOG << LOG_FATAL << L"Could not set cooperation level for DirectX keyboard device!" << LOG_ENDMESSAGE;

				return RESULT_FAIL;
			}

			if (m_IsBuffered)
			{
				DIPROPDWORD dipdw;
				dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
				dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
				dipdw.diph.dwObj        = 0;
				dipdw.diph.dwHow        = DIPH_DEVICE;
				dipdw.dwData            = INPUTDEVICE_KEYBOARD_DIRECTX_BUFFERSIZE;

				if (FAILED (m_KeyboardDevice->SetProperty (DIPROP_BUFFERSIZE, &dipdw.diph)))
				{
					REDEEMER_LOG << LOG_FATAL << L"Buffer error in DirectX keyboard device!" << LOG_ENDMESSAGE;

					return RESULT_FAIL;
				}
			}

			HRESULT hr = m_KeyboardDevice->Acquire ();

			if (FAILED(hr) && hr != DIERR_OTHERAPPHASPRIO)
			{
				REDEEMER_LOG << LOG_FATAL << L"DirectX keyboard device acquire error occured!" << LOG_ENDMESSAGE;

				return RESULT_FAIL;
			}

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_DirectXKeyboardDevice::Finalize ()
		{
			if (m_KeyboardDevice)
			{
				m_KeyboardDevice->Unacquire();

				REDEEMER_SAFE_RELEASE (m_KeyboardDevice);
			}

			m_InputSystem->SetKeyboardUsed(false);

			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_DirectXKeyboardDevice::IsKeyDown (EKeyCode key)
		{
			return (m_KeyBuffer[key] & 0x80) != 0;
		}

		//------------------------------------------------------------------------------------------------------------------------

		const std::string& C_DirectXKeyboardDevice::GetKeyCodeAsString (EKeyCode key)
		{
			char temp[256];

			DIPROPSTRING prop;

			prop.diph.dwSize = sizeof(DIPROPSTRING);
			prop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			prop.diph.dwObj = static_cast<DWORD>(key);
			prop.diph.dwHow = DIPH_BYOFFSET;

			if (SUCCEEDED (m_KeyboardDevice->GetProperty (DIPROP_KEYNAME, &prop.diph)))
			{
				// convert the WCHAR in "wsz" to multibyte
				if (WideCharToMultiByte (CP_ACP, 0, prop.wsz, -1, temp, sizeof(temp), NULL, NULL))
					return m_GetString.assign (temp);
			}

			std::stringstream ss;

			ss << "KEY_" << (int)key;

			return m_GetString.assign (ss.str());
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectXKeyboardDevice::SetBuffered (bool buffered)
		{
			if (buffered != m_IsBuffered)
			{
				if (m_KeyboardDevice)
				{
					m_KeyboardDevice->Unacquire();

					REDEEMER_SAFE_RELEASE (m_KeyboardDevice);
				}

				m_IsBuffered = buffered;

				Initialize ();
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_InputDeviceInterface* C_DirectXKeyboardDevice::QueryInterface (C_InputDeviceInterface::EInputDeviceInterfaceType type)
		{
			return NULL;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectXKeyboardDevice::RetrieveData ()
		{
			if (m_IsBuffered)
			{
				DIDEVICEOBJECTDATA diBuff[INPUTDEVICE_KEYBOARD_DIRECTX_BUFFERSIZE];
				DWORD entries = INPUTDEVICE_KEYBOARD_DIRECTX_BUFFERSIZE;
				HRESULT hr;

				hr = m_KeyboardDevice->GetDeviceData (sizeof(DIDEVICEOBJECTDATA), diBuff, &entries, 0);

				if (hr != DI_OK)
				{
					hr = m_KeyboardDevice->Acquire ();

					while (hr == DIERR_INPUTLOST)
						hr = m_KeyboardDevice->Acquire ();

					return;
				}

				if (FAILED (hr))
				{
					REDEEMER_LOG << LOG_FATAL << "Error within buffer reading from DirectX keyboard device" << LOG_ENDMESSAGE;

					return;
				}

				//	Update keyboard and modifier states.. And, if mListener, fire events
				for (unsigned int i = 0; i < entries; ++i )
				{
					//If the mListener returns false, that means that we are probably deleted...
					//send no more events and just leave as the this pointer is invalid now...
					bool ret = true;
					EKeyCode kc = (EKeyCode)diBuff[i].dwOfs;

					//Store result in our keyBuffer too
					m_KeyBuffer[kc] = static_cast<unsigned char>(diBuff[i].dwData);

					if (diBuff[i].dwData & 0x80)
					{
						//Turn on modifier
						if (kc == KEY_LCONTROL || kc == KEY_RCONTROL)
							m_Modifiers |= KM_Ctrl;

						else if (kc == KEY_LSHIFT || kc == KEY_RSHIFT)
							m_Modifiers |= KM_Shift;

						else if (kc == KEY_LMENU || kc == KEY_RMENU)
							m_Modifiers |= KM_Alt;

						if (!m_Listeners.empty())
						{
							for (unsigned int i = 0; i < m_Listeners.size(); i ++)
								ret = m_Listeners[i]->KeyPressed (C_KeyboardEvent (kc, TranslateText (kc)));
						}
					}
					else
					{
						//	Turn off modifier
						if (kc == KEY_LCONTROL || kc == KEY_RCONTROL)
							m_Modifiers &= ~KM_Ctrl;

						else if (kc == KEY_LSHIFT || kc == KEY_RSHIFT)
							m_Modifiers &= ~KM_Shift;

						else if (kc == KEY_LMENU || kc == KEY_RMENU)
							m_Modifiers &= ~KM_Alt;

						//	Fire off event
						if (!m_Listeners.empty())
						{
							for (unsigned int i = 0; i < m_Listeners.size(); i ++)
								ret = m_Listeners[i]->KeyReleased (C_KeyboardEvent (kc, 0));
						}
					}

					if(ret == false)
						break;
				}
			}
			else
			{
				HRESULT  hr = m_KeyboardDevice->GetDeviceState (sizeof(m_KeyBuffer), &m_KeyBuffer);

				if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
				{
					hr = m_KeyboardDevice->Acquire ();

					if (hr != DIERR_OTHERAPPHASPRIO)
						m_KeyboardDevice->GetDeviceState (sizeof (m_KeyBuffer), &m_KeyBuffer);
				}

				//Set Shift, Ctrl, Alt
				m_Modifiers = 0;

				if (IsKeyDown (KEY_LCONTROL) || IsKeyDown (KEY_RCONTROL))
					m_Modifiers |= KM_Ctrl;

				if (IsKeyDown (KEY_LSHIFT) || IsKeyDown (KEY_RSHIFT))
					m_Modifiers |= KM_Shift;

				if (IsKeyDown (KEY_LMENU) || IsKeyDown (KEY_RMENU))
					m_Modifiers |= KM_Alt;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_DirectXKeyboardDevice::TranslateText (EKeyCode keyCode)
		{
			BYTE keyState[256];
			HKL  layout = GetKeyboardLayout (0);

			if (GetKeyboardState (keyState) == 0)
				return 0;

			unsigned int vk = MapVirtualKeyEx (keyCode, 3, layout);

			if (vk == 0)
				return 0;

			unsigned char buff[3] = {0,0,0};
			int ascii = ToAsciiEx(vk, keyCode, keyState, (LPWORD) buff, 0, layout);

			//WCHAR wide[3];
			//int ascii = ToUnicodeEx(vk, kc, keyState, wide, 3, 0, layout);
			if (ascii == 1 && m_DeadKey != '\0')
			{
				// A dead key is stored and we have just converted a character key
				// Combine the two into a single character
				WCHAR wcBuff[3] = {buff[0], m_DeadKey, '\0'};
				WCHAR out[3];

				m_DeadKey = '\0';

				if (FoldStringW (MAP_PRECOMPOSED, (LPWSTR)wcBuff, 3, (LPWSTR)out, 3))
					return out[0];
			}
			else 
				if (ascii == 1)
				{	
					// We have a single character
					m_DeadKey = '\0';
				
					return buff[0];
				}
				else
					if(ascii == 2)
					{
						// Convert a non-combining diacritical mark into a combining diacritical mark
						// Combining versions range from 0x300 to 0x36F; only 5 (for French) have been mapped below
						// http://www.fileformat.info/info/unicode/block/combining_diacritical_marks/images.htm
						switch (buff[0])	
						{
						case 0x5E: // Circumflex accent: â
							m_DeadKey = 0x302;
							
							break;

						case 0x60: // Grave accent: à
							m_DeadKey = 0x300; 
							
							break;

						case 0xA8: // Diaeresis: ü
							m_DeadKey = 0x308; 
							
							break;

						case 0xB4: // Acute accent: é
							m_DeadKey = 0x301; 
							
							break;

						case 0xB8: // Cedilla: ç
							m_DeadKey = 0x327; 
							
							break;

						default:
							m_DeadKey = buff[0]; 
							
							break;
						}
					}

			return 0;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace INPUT
}	//	namespace REDEEMER

