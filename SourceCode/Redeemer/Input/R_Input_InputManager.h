/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Input_InputManager.h
**		Engine's input system communication layer
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_INPUT_INPUTMANAGER_H_
#define _R_INPUT_INPUTMANAGER_H_

#include "..\\Core\\R_Core_Singleton.h"
#include "..\\Core\\R_Core_BaseClass.h"

#include "R_Input_InputSystem.h"
#include "R_Input_Keyboard.h"
#include "R_Input_Mouse.h"

namespace REDEEMER
{
	namespace INPUT
	{
		/*!
		**	\brief C_InputManager provides communication layer between engine and DirectX's input system
		*/
		class C_InputManager : public CORE::C_Singleton<C_InputManager>, public CORE::C_BaseClass
		{
		public:
			/*!	Constructor
			*/
			C_InputManager ();

			/*!	Destructor
			*/
			virtual ~C_InputManager ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize (DeviceParameter &paramList, HWND hwnd);

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Returns reference to input system object
			*/
			C_InputSystem& GetInputSystem ();

			/*!	Returns reference to input keyboard object
			*/
			C_KeyboardDevice& GetKeyboard ();

			/*!	Returns reference to input mouse object
			*/
			C_MouseDevice& GetMouse ();

			/*!	Retrieves input data
			*/
			void RetrieveData ();

		private:			
			C_InputSystem*		m_InputSystem;		///<	Input system object
			C_KeyboardDevice*	m_KeyboardDevice;	///<	Input keyboard device
			C_MouseDevice*		m_MouseDevice;		///<	Input mouse device
		};

	}	//	namespace INPUT
}	//	namespace REDEEMER

#endif	//	_R_INPUT_INPUTMANAGER_H_
