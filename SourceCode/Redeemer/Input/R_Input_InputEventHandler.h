/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Input_InputEventHandler.h
**		Interface for event handling for keyboards, mouses and joysticks
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_INPUT_INPUTEVENTHANDLER_H_
#define _R_INPUT_INPUTEVENTHANDLER_H_

#include "R_Input_Keyboard.h"
#include "R_Input_Mouse.h"
#include "R_Input_Joystick.h"
#include "R_Input_ForceFeedback.h"

namespace REDEEMER
{
	namespace INPUT
	{
		/*!
		**	\brief Interface for event handling for keyboards, mouses and joysticks
		*/
		class C_InputEventHandler : public C_KeyboardListener, public C_MouseListener, public C_JoyStickListener
		{
		public:
			//	Constructor
			C_InputEventHandler () { }

			//	Destructor
			~C_InputEventHandler () { }

			//	Keyboard events
			virtual bool KeyPressed (const C_KeyboardEvent &event) = 0;
			virtual bool KeyReleased (const C_KeyboardEvent &event) = 0;

			//	Mouse events
			virtual bool MouseMoved (const C_MouseEvent &event) = 0;
			virtual bool MousePressed (const C_MouseEvent &event, EMouseButtonID buttonID) = 0;
			virtual bool MouseReleased (const C_MouseEvent &event, EMouseButtonID buttonID) = 0;

			//	Joystick events
			virtual bool ButtonPressed (const C_JoyStickEvent &event, int button) = 0;
			virtual bool ButtonReleased (const C_JoyStickEvent &event, int button) = 0;
			virtual bool AxisMoved (const C_JoyStickEvent &event, int axis) = 0;
			virtual bool SliderMoved (const C_JoyStickEvent &event, int index) = 0;
			virtual bool PovMoved (const C_JoyStickEvent &event, int index) = 0;
			virtual bool Vector3Moved (const C_JoyStickEvent &event, int index) = 0;
		};

	}	//	namespace INPUT
}	//	namespace REDEEMER

#endif	//	_R_INPUT_INPUTEVENTHANDLER_H_
