/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Input_InputDevice.h
**		Base class for all input devices like keyboards, mouses, etc.
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_INPUT_INPUTDEVICE_H_
#define _R_INPUT_INPUTDEVICE_H_

#include "..\\Core\\R_Core_BaseClass.h"

#include <string>

namespace REDEEMER
{
	namespace INPUT
	{
		class C_InputSystem;


		/*!	Each Input class has a General Type variable, a form of RTTI
		*/
		enum EInputDeviceType
		{
			IDT_Unknown   = 0,
			IDT_Keyboard  = 1,
			IDT_Mouse     = 2,
			IDT_JoyStick  = 3,
		};

		/*!	Axis component - used with mouses, joysticks..
		*/
		class C_Axis
		{
		public:
			/*!	Constructor
			*/
			C_Axis();

			/*!	Clears value components
			*/
			void Clear ();

			//	Public data fields

			int		Abs;		///<	Absolute value component
			int		Rel;		///<	Relative value component
			bool	AbsOnly;	///<	Indicates if this Axis only supports Absolute (i.e JoyStick)
		};

		/*! Input device's interface is a way to gain write access to devices which support	it. For example, force feedback.
		*/
		class C_InputDeviceInterface
		{
		public:
			/*!	Destructor
			*/
			virtual ~C_InputDeviceInterface ();

			/*!	Type of Interface
			*/
			enum EInputDeviceInterfaceType
			{
				IDIT_ForceFeedback,
				IDIT_Reserved
			};
		};

		/*!
		**	\brief Base class for all input devices like keyboards, mouses, etc.
		*/
		class C_InputDevice : public CORE::C_BaseClass
		{
		public:
			/*!	Destructor
			*/
			virtual ~C_InputDevice ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize () = 0;

			/*!	\brief	Finalization code
			*/
			virtual int Finalize () = 0;

			/*!	Returns the type of device
			*/
			EInputDeviceType GetType () const;

			/*!	Returns the vendor name
			*/
			const std::string& GetVendorName () const;

			/*!	Sets device's buffered mode	
			*/
			virtual void SetBuffered (bool buffered) = 0;

			/*!	Checks if device is in buffered mode
			*/
			virtual bool IsBuffered () const;

			/*!	Returns parent object who created this device
			*/
			C_InputSystem* GetInputSystem ();

			/*!	If available, get an interface to write to some devices.
			**	\param type The type of interface you are looking for
			*/
			virtual C_InputDeviceInterface* QueryInterface (C_InputDeviceInterface::EInputDeviceInterfaceType type) = 0;

			/*!	Used for retrieving device's data
			*/
			virtual void RetrieveData () = 0;

		protected:	
			/*!	Private constructor
			*/
			C_InputDevice (EInputDeviceType deviceType, const std::string &vendorName, bool isBuffered, C_InputSystem* inputSystem);

			EInputDeviceType	m_Type;				///<	Type of device			
			std::string			m_VendorName;		///<	Device's vendor name			
			bool				m_IsBuffered;		///<	Is device using buffering
			C_InputSystem*		m_InputSystem;		///<	Parent object who created this device
		};

	}	//	namespace INPUT
}	//	namespace REDEEMER

#endif	//	_R_INPUT_INPUTDEVICE_H_
