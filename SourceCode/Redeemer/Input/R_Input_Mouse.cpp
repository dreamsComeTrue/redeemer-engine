/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Input_Mouse.cpp
**		Base class for mouse device
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Input_Mouse.h"

namespace REDEEMER
{
	namespace INPUT
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_MouseState::C_MouseState () :
			DisplayAreaWidth (50),
			DisplayAreaHeight (50),
			Buttons (0)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_MouseState::IsButtonDown (EMouseButtonID button) const
		{
			return ((Buttons & ( 1L << button )) == 0) ? false : true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_MouseState::Clear ()
		{
			X.Clear ();
			Y.Clear ();
			Z.Clear ();
			
			Buttons = 0;
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_MouseEvent::C_MouseEvent (const C_MouseState& state) :
			State (state)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_MouseEvent::~C_MouseEvent()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_MouseListener::~C_MouseListener ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_MouseDevice::C_MouseDevice (const std::string &vendorName, bool isBuffered, C_InputSystem* inputSystem) :
			C_InputDevice (IDT_Mouse, vendorName, isBuffered, inputSystem)
		{
			 m_Listeners.clear();
		}

		//------------------------------------------------------------------------------------------------------------------------
		
		C_MouseDevice::~C_MouseDevice ()
		{
			if (!m_Listeners.empty())
				m_Listeners.clear();
		}

		//------------------------------------------------------------------------------------------------------------------------

		const C_MouseState& C_MouseDevice::GetMouseState () const
		{
			return m_State;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_MouseDevice::AttachMouseListener (C_MouseListener *mouseListener)
		{
			for (unsigned int i = 0; i < m_Listeners.size(); i ++)
			{
				if (m_Listeners[i] == mouseListener)
					return;
			}	

			m_Listeners.push_back(mouseListener);
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::vector<C_MouseListener*>& C_MouseDevice::GetMouseListeners ()
		{
			return m_Listeners;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace INPUT
}	//	namespace REDEEMER

