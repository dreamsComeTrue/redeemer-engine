/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Input_Keyboard.cpp
**		Base class for keyboard device
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Input_Keyboard.h"

// disable:  forcing value to bool 'true' or 'false' (performance warning)
#pragma warning (disable: 4800)

namespace REDEEMER
{
	namespace INPUT
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_KeyboardEvent::C_KeyboardEvent (EKeyCode keyCode, unsigned int character) :
			Key (keyCode),
			Text (character)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_KeyboardEvent::~C_KeyboardEvent()
		{
			//	EMPTY	
		}


		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_KeyboardListener::~C_KeyboardListener ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_KeyboardDevice::C_KeyboardDevice (const std::string &vendorName, bool isBuffered, C_InputSystem* inputSystem) :
			C_InputDevice (IDT_Keyboard, vendorName, isBuffered, inputSystem),
			m_Modifiers (0)
		{
			m_Listeners.clear();
		}

		//------------------------------------------------------------------------------------------------------------------------
	   
		C_KeyboardDevice::~C_KeyboardDevice ()
		{
			if (!m_Listeners.empty())
				m_Listeners.clear();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_KeyboardDevice::AttachKeyboardListener (C_KeyboardListener *keyboardListener)
		{
			for (unsigned int i = 0; i < m_Listeners.size(); i ++)
			{
				if (m_Listeners[i] == keyboardListener)
					return;
			}	

			m_Listeners.push_back(keyboardListener);
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::vector<C_KeyboardListener*>& C_KeyboardDevice::GetKeyboardListeners ()
		{
			return m_Listeners;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_KeyboardDevice::IsModifierDown (EKeyboardModifier modifier)
		{
			return (m_Modifiers & modifier);
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace INPUT
}	//	namespace REDEEMER

