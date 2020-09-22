/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Input_JoyStick.h
**		JoyStick base class
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_INPUT_JOYSTICK_H_
#define _R_INPUT_JOYSTICK_H_

#include "R_Input_InputDevice.h"

#include <windows.h>
#include <vector>

namespace REDEEMER
{
	namespace INPUT
	{
		/*!	Default sensitivity for vector3 component of joystick 
		*/
#define INPUT_JOYSTICK_VECTOR3_SENSITIVITY 2.28f

		/*!	Contains data describing joysticks
		*/
		struct S_JoyStickInfo
		{
		public:
			int				DeviceID;		///<	JoyStick ID
			GUID			DeviceGUID;		///<	GUID number
			std::string		VendorName;		///<	Vendor name
		};	 

		/*!	A 3D Vector component 
		*/
		class C_JoyStickVector3
		{
		public:
			/*!	Constructor
			*/
			C_JoyStickVector3 ();

			/*!	Constructor
			*/
			C_JoyStickVector3 (float x, float y, float z);

			/*!	Clears all components
			*/
			void Clear();
						
		public:
			float X;		///<	X component of vector			
			float Y;		///<	Y component of vector
			float Z;		///<	Z component of vector
		};

		/*!	POV / HAT Joystick component
		*/
		class C_JoyStickPOV
		{
		public:
			/*!	Constructor
			*/
			C_JoyStickPOV ();


			//	Member fields describing orientation of the POV
			static const int	Centered  = 0x00000000;		///<	Centered
			static const int	North     = 0x00000001;		///<	North
			static const int	South     = 0x00000010;		///<	South
			static const int	East      = 0x00000100;		///<	East
			static const int	West      = 0x00001000;		///<	West
			static const int	NorthEast = 0x00000101;		///<	North east
			static const int	SouthEast = 0x00000110;		///<	South east
			static const int	NorthWest = 0x00001001;		///<	North west
			static const int	SouthWest = 0x00001010;		///<	South west

			int					Direction;					///<	Actual direction
		};

		/*!	A sliding axis
		*/
		class C_JoyStickSlider
		{
		public:
			/*!	Constructor
			*/
			C_JoyStickSlider ();
						
			int AbX;	///<	True if pushed, false otherwise
			int	AbY;	///<	True if pushed, false otherwise
		};

		/*!	Holds the state of the joystick
		*/
		class C_JoyStickState
		{
		public:
			/*!	Constructor
			*/
			C_JoyStickState ();

			/*!	Resets all variables to initial values
			*/
			void Clear ();	 

			//	Public member data
			
			std::vector<bool>				m_Buttons;		///<	Represents all the buttons (uses a bitset)
			std::vector<C_Axis>				m_Axes;			///<	Represents all the single axes on the device
			C_JoyStickPOV					m_POV[4];		///<	Represents the value of a POV. Maximum of 4
			C_JoyStickSlider				m_Sliders[4];	///<	Represent the max sliders
			std::vector<C_JoyStickVector3>	m_Vectors;		///<	Represents all Vector type controls the device exports
		};

		/*!	Joysticks events are used in conjunction with joystick listeners
		*/
		class C_JoyStickEvent
		{
		public:
			/*!	Constructor
			*/
			C_JoyStickEvent (const C_JoyStickState &state);

			/*!	Destructor
			*/
			virtual ~C_JoyStickEvent();

			const C_JoyStickState &State;	///<	Represents state of the joystick
		};

		/*!	JoyStick listeners are used to retrieve buffer input from device. They should be attached to joystick device using
		**	AttachJoyStickListener method
		*/
		class C_JoyStickListener
		{
		public:
			/*!	Destructor
			*/
			virtual ~C_JoyStickListener ();

			/*!	Occurs on joystick button down event
			*/
			virtual bool ButtonPressed (const C_JoyStickEvent &event, int button) = 0;
			
			/*!	Occurs on joystick button up event 
			*/
			virtual bool ButtonReleased (const C_JoyStickEvent &event, int button) = 0;

			/*!	Occurs on joystick axis moved event 
			*/
			virtual bool AxisMoved (const C_JoyStickEvent &event, int axis) = 0;

			/*!	Occurs when slider moved
			*/
			virtual bool SliderMoved (const C_JoyStickEvent &event, int index);
			
			/*!	Occurs when POV moved
			*/
			virtual bool PovMoved (const C_JoyStickEvent &event, int index);

			/*!	Occurs when Vector3 moved
			*/
			virtual bool Vector3Moved (const C_JoyStickEvent &event, int index);
		};

					  
		/*!
		**	\brief JoyStick base class
		*/
		class C_JoyStickDevice : public C_InputDevice
		{
		public:
			/*!	Destructor
			*/
			virtual ~C_JoyStickDevice ();

			/*!	Returns the state of the joystick
			*/
			const C_JoyStickState& GetJoyStickState () const;

			/*!	Attaches new joystick listener
			**	\param	joystickListener Pointer to a class derived from C_JoyStickListener
			*/
			virtual void AttachJoyStickListener (C_JoyStickListener *joystickListener);

			/*!	Returns list of attached listeners
			*/
			std::vector<C_JoyStickListener*>& GetJoyStickListeners ();

			/*!	Sets the degree under which Vector3 events should be discarded
			*/
			void SetVector3Sensitivity (float degrees = INPUT_JOYSTICK_VECTOR3_SENSITIVITY);

			/*!	Returns the sensitivity cutoff for Vector3 Component
			*/
			float GetVector3Sensitivity ();

			/*!	Returns number of buttons
			*/
			int GetNumberOfButtons () const;

			/*!	Returns number of axes
			*/
			int GetNumberOfAxes () const;

			/*!	Returns number of sliders
			*/
			int GetNumberOfSliders () const;

			/*!	Returns number of POVs
			*/
			int GetNumberOfPOVs () const;

			/*!	Returns number of Vector3's	
			*/
			int GetNumberOfVector3 () const;
						
			static const int JOYSTICK_MIN_AXIS = -32768;		///<	The minimal axis value
			static const int JOYSTICK_MAX_AXIS = 32767;			///<	The maximum axis value

		protected:	
			/*!	Constructor
			*/
			C_JoyStickDevice (const std::string &vendorName, bool isBuffered, C_InputSystem* inputSystem);
						
			int									m_SlidersCount;			///<	Number of sliders			
			int									m_POVCount;				///<	Number of POVs 
			C_JoyStickState						m_State;				///<	C_JoyStickState structure (contains all component values)
			std::vector<C_JoyStickListener*>	m_Listeners;			///<	Attached listeners			
			float								m_Vector3Sensitivity;	///<	Adjustment factor for orientation vector accuracy
		};

	}	//	namespace INPUT
}	//	namespace REDEEMER

#endif	//	_R_INPUT_JOYSTICK_H_
