/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Input_DirectXJoyStick.h
**		DirectX implementation of JoyStick class
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_INPUT_DIRECTXJOYSTICK_H_
#define _R_INPUT_DIRECTXJOYSTICK_H_

#include "R_Input_JoyStick.h"

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

namespace REDEEMER
{
	namespace INPUT
	{
		class C_DirectXForceFeedback;

		/*!
		**	\brief DirectX implementation of JoyStick class
		*/
		class C_DirectXJoyStickDevice : public C_JoyStickDevice
		{
		public:
			/*!	Constructor
			*/
			C_DirectXJoyStickDevice (C_InputSystem* inputSystem, IDirectInput8* inputDevice, bool isBuffered, DWORD cooperationSettings, const S_JoyStickInfo &info);

			/*!	Destructor
			*/
			virtual ~C_DirectXJoyStickDevice ();

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
			/*! Enumerate axis callback
			*/
			static BOOL CALLBACK DIEnumDeviceObjectsCallback (LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);

			/*! Enumerate Force Feedback callback
			*/
			static BOOL CALLBACK DIEnumEffectsCallback (LPCDIEFFECTINFO pdei, LPVOID pvRef);

			/*!	Simulate button click
			*/
			bool DoButtonClick (int button, DIDEVICEOBJECTDATA& di);

			/*!	Simulate changing POV
			*/
			bool ChangePOV (int pov, DIDEVICEOBJECTDATA& di);

			IDirectInput8*				m_DirectInput;			///<	DirectX input device
			IDirectInputDevice8*		m_JoyStickDevice;		///<	DirectX JoyStick device
			DWORD						m_CooperationSettings;	///<	Coops settings
			GUID						m_DeviceGuid;			///<	JoyStick GUID number

			C_DirectXForceFeedback*		m_ForceFeedbackDevice;	///<	A force feedback device
			int							m_AxisNumber;			///<	Mapping
		};

	}	//	namespace INPUT
}	//	namespace REDEEMER

#endif	//	_R_INPUT_DIRECTXJOYSTICK_H_
