/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Input_Mouse.h
**		Base class for mouse device
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_INPUT_MOUSE_H_
#define _R_INPUT_MOUSE_H_

#include "R_Input_InputDevice.h"

#include <vector>

namespace REDEEMER
{
	namespace INPUT
	{
		/*! ID numbers for mouse buttons
		*/
		enum EMouseButtonID
		{
			MB_Left = 0, 
			MB_Right, 
			MB_Middle,

			MB_Button3,
			MB_Button4,
			MB_Button5,
			MB_Button6,	
			MB_Button7
		};

		/*!	Contains all data describing mouse state
		*/
		class C_MouseState
		{
		public:
			/*!	Constructor
			*/
			C_MouseState ();

			/*!	Checks if given button is down
			*/
			bool IsButtonDown (EMouseButtonID button) const;

			/*!	Clear all the values
			*/
			void Clear ();

		public:
			mutable int	DisplayAreaWidth;			///<	Represents the width of your display area.
			mutable int DisplayAreaHeight;			///<	Represents the height of your display area.
			C_Axis		X;							///<	X Axis component
			C_Axis		Y;							///<	Y Axis Component
			C_Axis		Z;							///<	Z Axis Component
			int			Buttons;					///<	Buttons - bit position indicates button down

		};

		/*!	Mouse events are used in conjunction with mouse listeners
		*/
		class C_MouseEvent
		{
		public:
			/*!	Constructor
			*/
			C_MouseEvent (const C_MouseState& state);

			/*!	Destructor
			*/
			virtual ~C_MouseEvent();

		public:			
			const C_MouseState&	State;	///<	The state of the mouse
		};

		/*!	Mouse listeners are used to retrieve buffer input from device. They should be attached to mouse device using
		**	AttachMouseListener method
		*/
		class C_MouseListener
		{
		public:
			/*!	Destructor
			*/
			virtual ~C_MouseListener ();

			/*!	Occurs, when mouse is moved
			*/
			virtual bool MouseMoved (const C_MouseEvent &event) = 0;

			/*!	Occurs, when mouse button is pressed
			*/
			virtual bool MousePressed (const C_MouseEvent &event, EMouseButtonID buttonID) = 0;

			/*!	Occurs, when mouse button is released
			*/
			virtual bool MouseReleased (const C_MouseEvent &event, EMouseButtonID buttonID) = 0;	
		};


		/*!
		**	\brief Base class for mouse device
		*/
		class C_MouseDevice : public C_InputDevice
		{
		public:
			/*!	Destructor
			*/
			virtual ~C_MouseDevice ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize () = 0;

			/*!	\brief	Finalization code
			*/
			virtual int Finalize () = 0;

			/*!	Returns the state of the mouse
			*/
			const C_MouseState& GetMouseState () const;

			/*!	Attaches new mouse listener
			**	\param	mouseListener Pointer to a class derived from C_MouseListener
			*/
			virtual void AttachMouseListener (C_MouseListener *mouseListener);

			/*!	Returns list of attached listeners
			*/
			std::vector<C_MouseListener*>& GetMouseListeners ();

			/*!	Shows or hides mouse cursor
			*/
			virtual void ShowCursor (bool show = true) = 0;

			/*!	Sets mouse position on the screen
			*/
			virtual void SetPosition (long x, long y) = 0;

		protected:	
			/*!	Private constructor
			*/
			C_MouseDevice (const std::string &vendorName, bool isBuffered, C_InputSystem* inputSystem);

			C_MouseState					m_State;		///<	State of the mouse
			std::vector<C_MouseListener*>	m_Listeners;	///<	Attached listeners
		};

	}	//	namespace INPUT
}	//	namespace REDEEMER

#endif	//	_R_INPUT_MOUSE_H_
