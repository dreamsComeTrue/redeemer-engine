/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Input_DirectXKeyboard.h
**		DirectX implementation of keyboard device
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_INPUT_DIRECTXKEYBOARD_H_
#define _R_INPUT_DIRECTXKEYBOARD_H_

#include "R_Input_Keyboard.h"

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

namespace REDEEMER
{
	namespace INPUT
	{
		/*!
		**	\brief DirectX implementation of keyboard device
		*/
		class C_DirectXKeyboardDevice : public C_KeyboardDevice
		{
		public:
			/*!	Constructor
			*/
			C_DirectXKeyboardDevice (C_InputSystem* inputSystem, IDirectInput8* inputDevice, bool isBuffered, DWORD cooperationSettings);

			/*!	Destructor
			*/
			virtual ~C_DirectXKeyboardDevice ();

			/*!	Checks if key is pressed
			**	\param key A KeyCode to check
			*/
			virtual bool IsKeyDown (EKeyCode key);

			/*!	Returns key's string representation - for example, KC_ENTER will be "Enter".
			**	\param key KeyCode to convert
			*/
			virtual const std::string& GetKeyCodeAsString (EKeyCode key);

			/*!	\brief	Initialization code
			*/
			virtual int Initialize ();

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Sets device's buffered mode	
			*/
			virtual void SetBuffered (bool buffered);

			/*!	If available, get an interface to write to some devices.
			**	\param type The type of interface you are looking for
			*/
			virtual C_InputDeviceInterface* QueryInterface (C_InputDeviceInterface::EInputDeviceInterfaceType type);

			/*!	Used for retrieving device's data
			*/
			virtual void RetrieveData ();

		private:	
			/*!	Internal method for translating KeyCodes to Text
			*/
			int TranslateText (EKeyCode keyCode);

			IDirectInput8*			m_DirectInput;			///<	DirectX input device
			IDirectInputDevice8*	m_KeyboardDevice;		///<	DirectX keyboard device
			DWORD					m_CooperationSettings;	///<	Coops settings

			unsigned char			m_KeyBuffer[256];		///<	Buffer holding states of all keys

			WCHAR					m_DeadKey;				///<	Stored dead key from last translation
			std::string				m_GetString;			///<	Used with GetKeyCodeAsString
		};

	}	//	namespace INPUT
}	//	namespace REDEEMER

#endif	//	_R_INPUT_DIRECTXKEYBOARD_H_
