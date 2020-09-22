/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Input_InputManager.cpp
**		Engine's input system communication layer
*/

//------------------------------------------------------------------------------------------------------------------------
#include "..\\RedeemerEngine.h"

#include "R_Input_InputManager.h"

namespace REDEEMER
{
	namespace INPUT
	{
		//------------------------------------------------------------------------------------------------------------------------

		C_InputManager::C_InputManager () :
			m_InputSystem (NULL),
			m_KeyboardDevice (NULL),
			m_MouseDevice (NULL)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_InputManager::~C_InputManager ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_InputManager::Initialize (DeviceParameter &paramList, HWND hwnd)
		{
			C_BaseClass::Initialize();

			m_InputSystem = C_InputSystem::CreateInputSystem (paramList, hwnd);

			m_KeyboardDevice = (INPUT::C_KeyboardDevice*)m_InputSystem->CreateInputDevice (INPUT::IDT_Keyboard, true);
			m_MouseDevice = (INPUT::C_MouseDevice*)m_InputSystem->CreateInputDevice (INPUT::IDT_Mouse, true);

			REDEEMER_LOG << LOG_INFO << L"Input Manager initialized!" << LOG_ENDMESSAGE;

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_InputManager::Finalize ()
		{
			m_InputSystem->DestroyInputDevice (m_MouseDevice);
			m_InputSystem->DestroyInputDevice (m_KeyboardDevice);

			C_InputSystem::DestroyInputSystem(m_InputSystem);

			REDEEMER_LOG << LOG_INFO << L"Input Manager finalized!" << LOG_ENDMESSAGE;

			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_InputSystem& C_InputManager::GetInputSystem ()
		{
			return *m_InputSystem;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_KeyboardDevice& C_InputManager::GetKeyboard ()
		{
			return *m_KeyboardDevice;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_MouseDevice& C_InputManager::GetMouse ()
		{
			return *m_MouseDevice;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_InputManager::RetrieveData ()
		{
			if (m_KeyboardDevice)
				m_KeyboardDevice->RetrieveData();

			if (m_MouseDevice)
				m_MouseDevice->RetrieveData();
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace INPUT
}	//	namespace REDEEMER

