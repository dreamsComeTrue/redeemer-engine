/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Input_JoyStick.cpp
**		JoyStick base class
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Input_JoyStick.h"

namespace REDEEMER
{
	namespace INPUT
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_JoyStickVector3::C_JoyStickVector3 ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_JoyStickVector3::C_JoyStickVector3 (float x, float y, float z) :
			X (x),
			Y (y),
			Z (z)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_JoyStickVector3::Clear ()
		{
			X = Y = Z = 0.0f;
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_JoyStickPOV::C_JoyStickPOV ()	:
			Direction (0)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_JoyStickSlider::C_JoyStickSlider () :
			AbX (0),
			AbY (0)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_JoyStickState::C_JoyStickState ()
		{
			Clear ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_JoyStickState::Clear ()
		{
			for (std::vector<bool>::iterator it = m_Buttons.begin(); it != m_Buttons.end(); it ++)
				(*it) = false;

			for (std::vector<C_Axis>::iterator it = m_Axes.begin (); it != m_Axes.end(); it ++)
			{
				it->AbsOnly = true; //Currently, joysticks only report Absolute values
				it->Clear();
			}

			for (std::vector<C_JoyStickVector3>::iterator it = m_Vectors.begin (); it != m_Vectors.end (); it ++)
				it->Clear ();

			for (int i = 0; i < 4; ++i)
			{
				m_POV[i].Direction = C_JoyStickPOV::Centered;
				m_Sliders[i].AbX = m_Sliders[i].AbY = 0;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_JoyStickEvent::C_JoyStickEvent (const C_JoyStickState &state) :
			State (state)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_JoyStickEvent::~C_JoyStickEvent ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_JoyStickListener::~C_JoyStickListener ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_JoyStickListener::SliderMoved (const C_JoyStickEvent &event, int index)
		{
			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_JoyStickListener::PovMoved (const C_JoyStickEvent &event, int index)
		{
			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_JoyStickListener::Vector3Moved (const C_JoyStickEvent &event, int index)
		{
			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_JoyStickDevice::C_JoyStickDevice (const std::string &vendorName, bool isBuffered, C_InputSystem* inputSystem) :
			C_InputDevice (IDT_JoyStick, vendorName, isBuffered, inputSystem),
			m_SlidersCount(0),
			m_POVCount(0),
			m_Vector3Sensitivity (INPUT_JOYSTICK_VECTOR3_SENSITIVITY)
		{
			m_Listeners.clear();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_JoyStickDevice::~C_JoyStickDevice ()
		{
			if (!m_Listeners.empty())
				m_Listeners.clear();
		}

		//------------------------------------------------------------------------------------------------------------------------

		const C_JoyStickState& C_JoyStickDevice::GetJoyStickState () const
		{
			return m_State;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_JoyStickDevice::AttachJoyStickListener (C_JoyStickListener *joystickListener)
		{
			for (unsigned int i = 0; i < m_Listeners.size(); i ++)
			{
				if (m_Listeners[i] == joystickListener)
					return;
			}	

			m_Listeners.push_back(joystickListener);
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::vector<C_JoyStickListener*>& C_JoyStickDevice::GetJoyStickListeners ()
		{
			return m_Listeners;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_JoyStickDevice::SetVector3Sensitivity (float degrees)
		{
			m_Vector3Sensitivity = degrees;
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_JoyStickDevice::GetVector3Sensitivity ()
		{
			return m_Vector3Sensitivity;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_JoyStickDevice::GetNumberOfButtons () const
		{
			return m_State.m_Buttons.size();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_JoyStickDevice::GetNumberOfAxes () const
		{
			return m_State.m_Axes.size();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_JoyStickDevice::GetNumberOfSliders () const
		{
			return m_SlidersCount;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_JoyStickDevice::GetNumberOfPOVs () const
		{
			return m_POVCount;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_JoyStickDevice::GetNumberOfVector3 () const
		{
			return m_State.m_Vectors.size();
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace INPUT
}	//	namespace REDEEMER

