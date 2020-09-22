/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Input_InputSystem.cpp
**		Engine's input system communication layer
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Input_InputSystem.h"
#include "R_Input_DirectXKeyboard.h"
#include "R_Input_DirectXMouse.h"
#include "R_Input_DirectXJoyStick.h"
#include "..\\Core\\R_Core_Convert.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace INPUT
	{
#define INPUTSYSTEM_NAME	"RedeemerInputSystem"

		//------------------------------------------------------------------------------------------------------------------------
		C_InputSystem::C_InputSystem () :
			m_DirectInput (NULL),
			m_KeyboardParameters (0),
			m_MouseParameters (0),
			m_JoyStickParameters (0),
			m_KeyboardUsed (false),
			m_MouseUsed (false),
			m_JoySticksCount (0),
			m_InputSystemName (INPUTSYSTEM_NAME)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_InputSystem::~C_InputSystem ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_InputSystem::Initialize (DeviceParameter& parameters)
		{
			C_BaseClass::Initialize();

			HINSTANCE hInstance = 0;
			HRESULT hr;

			hInstance = GetModuleHandle (0);

			//	Create DirectX input device
			hr = DirectInput8Create (hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_DirectInput, NULL);

			if (FAILED (hr))	
			{
				REDEEMER_LOG << LOG_FATAL << L"InputManager: Can not initialize DirectX Input system!" << LOG_ENDMESSAGE;
				
				return RESULT_FAIL;
			}

			//	Enumerate all attached devices 
			m_DirectInput->EnumDevices (NULL, EnumerateDevicesCallback, this, DIEDFL_ATTACHEDONLY); 

			//Here we pick up settings such as a device's cooperation mode
			std::map<EInputDeviceParamater, DWORD> temp;
			temp[IDP_DISCL_BACKGROUND]	= DISCL_BACKGROUND;
			temp[IDP_DISCL_EXCLUSIVE]		= DISCL_EXCLUSIVE;
			temp[IDP_DISCL_FOREGROUND]	= DISCL_FOREGROUND;
			temp[IDP_DISCL_NONEXCLUSIVE]	= DISCL_NONEXCLUSIVE;
			temp[IDP_DISCL_NOWINKEY]		= DISCL_NOWINKEY;

			//	Check for appropriate pairs			
			for (std::multimap<EInputDeviceType, EInputDeviceParamater>::iterator it = parameters.begin(); it != parameters.end(); it ++) 
			{
				if (it->first == IDT_Keyboard)
					m_KeyboardParameters |= temp[it->second];
				else 
					if (it->first == IDT_Mouse)
						m_MouseParameters |= temp[it->second];
					else
						if (it->first == IDT_JoyStick)
						m_JoyStickParameters |= temp[it->second];
			}

			//	If we haven't set any of them
			if (m_KeyboardParameters == 0)
				m_KeyboardParameters = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY;

			if (m_MouseParameters == 0)
				m_MouseParameters = DISCL_FOREGROUND | DISCL_EXCLUSIVE;

			if (m_JoyStickParameters == 0)
				m_JoyStickParameters = DISCL_FOREGROUND | DISCL_EXCLUSIVE;

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		BOOL CALLBACK C_InputSystem::EnumerateDevicesCallback (LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
		{
			C_InputSystem *inputSystem = static_cast<C_InputSystem*>(pvRef);

			//	Get info about attached joysticks
			if (GET_DIDEVICE_TYPE (lpddi->dwDevType) == DI8DEVTYPE_JOYSTICK || GET_DIDEVICE_TYPE (lpddi->dwDevType) == DI8DEVTYPE_GAMEPAD ||
				GET_DIDEVICE_TYPE (lpddi->dwDevType) == DI8DEVTYPE_1STPERSON || GET_DIDEVICE_TYPE (lpddi->dwDevType) == DI8DEVTYPE_DRIVING ||
				GET_DIDEVICE_TYPE (lpddi->dwDevType) == DI8DEVTYPE_FLIGHT)
			{
				S_JoyStickInfo info;

				info.DeviceID = inputSystem->m_JoySticksCount;
				info.DeviceGUID = lpddi->guidInstance;

				CORE::C_Convert::ConvertUnicodeToChars(&info.VendorName, lpddi->tszInstanceName);

				inputSystem->m_JoySticksCount ++;

				inputSystem->m_UnusedJoysticks.push_back (info);
			}

			return DIENUM_CONTINUE;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_InputSystem::Finalize ()
		{
			REDEEMER_SAFE_RELEASE (m_DirectInput);

			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_InputSystem::GetNumberOfDevices (EInputDeviceType deviceType)
		{
			switch (deviceType)
			{
			case IDT_Keyboard: 
				return 1;

			case IDT_Mouse: 
				return 1;

			case IDT_JoyStick: 
				return m_JoySticksCount;

			default: return 0;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_InputSystem* C_InputSystem::CreateInputSystem (DeviceParameter &paramList, HWND hwnd)
		{
			C_InputSystem* inputSystem = new C_InputSystem ();

			inputSystem->m_HWND = hwnd;

			if (inputSystem->Initialize (paramList) == RESULT_FAIL)
			{
				REDEEMER_SAFE_DELETE (inputSystem);
			}

			return inputSystem;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_InputSystem::DestroyInputSystem (C_InputSystem* inputSystem)
		{
			if (inputSystem == 0)
				return;

			//	Cleanup before deleting...
			for (unsigned int i = 0; i < inputSystem->m_InputDevices.size(); i ++)
				inputSystem->DestroyInputDevice (inputSystem->m_InputDevices[i]);

			REDEEMER_SAFE_DELETE (inputSystem);
		}

		//------------------------------------------------------------------------------------------------------------------------

		const std::string& C_InputSystem::GetInputSystemName ()
		{
			return m_InputSystemName;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_InputDevice* C_InputSystem::CreateInputDevice (EInputDeviceType type, bool isBuffered, const std::string &vendor)
		{
			C_InputDevice* device = NULL;

			switch (type)
			{
			case IDT_Keyboard: 
				{
					if (m_KeyboardUsed == false)
						device = new C_DirectXKeyboardDevice (this, m_DirectInput, isBuffered, m_KeyboardParameters);
					break;
				}
			case IDT_Mouse:
				{
					if (m_MouseUsed == false)
						device = new C_DirectXMouseDevice (this, m_DirectInput, isBuffered, m_MouseParameters);
					break;
				}
			case IDT_JoyStick:
				{
					for (std::vector<S_JoyStickInfo>::iterator it = m_UnusedJoysticks.begin(); it != m_UnusedJoysticks.end(); it ++)
					{
						if (vendor == "" || it->VendorName == vendor)
						{
							device = new C_DirectXJoyStickDevice (this, m_DirectInput, isBuffered, m_JoyStickParameters, *it);
							m_UnusedJoysticks.erase (it);
							break;
						}
					}
					break;
				}
			default:
				break;
			}

			if (device == 0)
				REDEEMER_LOG << LOG_FATAL << "No input devices match requested type." << LOG_ENDMESSAGE;

			m_InputDevices.push_back (device);

			//	Initialize device
			if (device->Initialize() == RESULT_FAIL)
			{
				//	Some kind of error, cleanup and rethrow
				DestroyInputDevice (device);
			}

			return device;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_InputSystem::DestroyInputDevice (C_InputDevice* device)
		{
			if (device == 0)
				return;

			for (std::vector<C_InputDevice*>::iterator it = m_InputDevices.begin(); it != m_InputDevices.end (); it ++)
			{
				if ((*it) == device)
				{
					delete device;

					m_InputDevices.erase (it);

					return;
				}
			}

			REDEEMER_LOG << LOG_FATAL << "Input device's creator not known." << LOG_ENDMESSAGE;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_InputSystem::SetKeyboardUsed (bool used)
		{
			m_KeyboardUsed = used;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_InputSystem::SetMouseUsed (bool used)
		{
			m_MouseUsed = used;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_InputSystem::ReturnJoyStick (const S_JoyStickInfo& joystick)
		{
			 m_UnusedJoysticks.push_back (joystick);
		}

		//------------------------------------------------------------------------------------------------------------------------

		HWND C_InputSystem::GetWindowHandle ()
		{
			return m_HWND;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace INPUT
}	//	namespace REDEEMER

