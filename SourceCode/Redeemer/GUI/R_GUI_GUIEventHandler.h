/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_GUI_GUIEventHandler.h
**		Interface for handling GUI events
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_GUI_GUIEVENTHANDLER_H_
#define _R_GUI_GUIEVENTHANDLER_H_

#include "..\\Input\\R_Input_InputEventHandler.h"

namespace REDEEMER
{
	namespace GUI
	{
		class C_GUIControl;

		/*!	Types of GUI events
		*/
		enum EGUIEventType
		{
			/*! A control has lost its focus. S_GUIEvent.Caller is losing the focus to S_GUIEvent.Control.
			**	If the event is absorbed then the focus will not be changed. 
			*/
			GUIET_ControlLostFocus,

			/*! A control has gained a focus. If the event is absorbed then the focus will not be changed. 
			*/
			GUIET_ControlGainedFocus,

			/*! The mouse cursor is over a control.
			*/
			GUIET_ControlHovered,

			/*!	The mouse cursor left hovered control
			*/
			GUIET_ControlLostHovered,

			/*!	Unknown event
			*/
			GUIET_Unknown
		};


		/*!	Events can hold data, such as key codes mouse positions, etc
		*/
		enum EGUIEventSource
		{
			GUIES_GUI,
			GUIES_Keyboard,
			GUIES_Mouse			
		};

		/*!	GUI event type
		*/
		struct S_GUIEvent
		{		
			//	Constructor
			S_GUIEvent () :
				Source (GUIES_GUI),
				KeyboardData (NULL),
				KeyboardPressed (false),
				MouseData (NULL),
				Caller (NULL),
				Control (NULL),
				EventType (GUIET_Unknown)
			{
				//	EMPTY
			}

		public:
			EGUIEventSource			Source;				///<	Source of event data (keyboard, mouse or GUI)

			INPUT::C_KeyboardEvent*	KeyboardData;		///<	Data associated with keyboard when event was generated
			bool					KeyboardPressed;	///<	Associated with KeyboardData

			INPUT::C_MouseEvent*	MouseData;			///<	Data associated with mouse when event was generated
			C_GUIControl*			Caller;				///<	Control, which called this event			
			C_GUIControl*			Control;			///<	Used, when event processing uses data from another control
			EGUIEventType			EventType;			///<	Type of the event
		};

		/*!
		**	\brief Interface for handling GUI events
		*/
		class C_GUIEventHandler
		{
		public:
			//	Constructor
			C_GUIEventHandler () { }

			//	Destructor
			~C_GUIEventHandler () { }

			/*! This method is called when the event happened. If you want to stop processing events further, control, which derives from this
			**	class should return "true"- this means that event processing is done.
			**
			**	\return True if the event was processed.
			*/
			virtual bool OnEvent(const S_GUIEvent& event) = 0;
		};


		/*!	\brief Specialized class used to connecting mouse/keyboards events with GUI system
		*/
		class C_GUIInputEventHandler:  public INPUT::C_InputEventHandler
		{
		public:
			C_GUIInputEventHandler ();

			//	Destructor
			~C_GUIInputEventHandler ();

			//	Keyboard events
			virtual bool KeyPressed (const INPUT::C_KeyboardEvent &event);
			virtual bool KeyReleased (const INPUT::C_KeyboardEvent &event);

			//	Mouse events
			virtual bool MouseMoved (const INPUT::C_MouseEvent &event);
			virtual bool MousePressed (const INPUT::C_MouseEvent &event, INPUT::EMouseButtonID buttonID);
			virtual bool MouseReleased (const INPUT::C_MouseEvent &event, INPUT::EMouseButtonID buttonID);

			//	Joystick events
			virtual bool ButtonPressed (const INPUT::C_JoyStickEvent &event, int button);
			virtual bool ButtonReleased (const INPUT::C_JoyStickEvent &event, int button);
			virtual bool AxisMoved (const INPUT::C_JoyStickEvent &event, int axis);
			virtual bool SliderMoved (const INPUT::C_JoyStickEvent &event, int index);
			virtual bool PovMoved (const INPUT::C_JoyStickEvent &event, int index);
			virtual bool Vector3Moved (const INPUT::C_JoyStickEvent &event, int index);
		};

	}	//	namespace GUI
}	//	namespace REDEEMER

#endif	//	_R_GUI_GUIEVENTHANDLER_H_
