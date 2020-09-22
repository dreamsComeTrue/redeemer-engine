/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Input_DirectXMouse.h
**		DirectX implementation of mouse device
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_INPUT_DIRECTXMOUSE_H_
#define _R_INPUT_DIRECTXMOUSE_H_

#include "R_Input_Mouse.h"

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

namespace REDEEMER
{
	namespace INPUT
	{
		/*!
		**	\brief DirectX implementation of mouse device
		*/
		class C_DirectXMouseDevice : public C_MouseDevice
		{
		public:
			/*!	Constructor
			*/
			C_DirectXMouseDevice (C_InputSystem* inputSystem, IDirectInput8* inputDevice, bool isBuffered, DWORD cooperationSettings);

			/*!	Destructor
			*/
			virtual ~C_DirectXMouseDevice ();

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

			/*!	Shows or hides mouse cursor
			*/
			virtual void ShowCursor (bool show = true);

			/*!	Sets mouse position on the screen
			*/
			virtual void SetPosition (long x, long y);

		private:	
			/*!	Simulate mouse click
			*/
			bool DoMouseClick (int mouseButton, DIDEVICEOBJECTDATA& di);

			IDirectInput8*			m_DirectInput;
			IDirectInputDevice8*	m_MouseDevice;
			DWORD					m_CooperationSettings;
			HWND					m_HWND;
		};

	}	//	namespace INPUT
}	//	namespace REDEEMER

#endif	//	_R_INPUT_DIRECTXMOUSE_H_
