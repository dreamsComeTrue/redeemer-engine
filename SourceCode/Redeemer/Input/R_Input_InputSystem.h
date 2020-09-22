/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Input_InputSystem.h
**		Engine's input system communication layer
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_INPUT_INPUTSYSTEM_H_
#define _R_INPUT_INPUTSYSTEM_H_

#define DIRECTINPUT_VERSION 0x0800

#include "..\\Core\\R_Core_Singleton.h"
#include "..\\Core\\R_Core_BaseClass.h"

#include "R_Input_InputDevice.h"
#include "R_Input_JoyStick.h"

#include <dinput.h>
#include <string>
#include <map>
#include <vector>

namespace REDEEMER
{
	namespace INPUT
	{
		class C_DirectXMouseDevice;
		class C_DirectXKeyboardDevice;
		class C_DirectXJoyStickDevice;


		/*!	Each Input class has a General Type variable, a form of RTTI
		*/
		enum EInputDeviceParamater
		{
			IDP_DISCL_BACKGROUND	= 0,
			IDP_DISCL_EXCLUSIVE		= 1,
			IDP_DISCL_FOREGROUND	= 2,
			IDP_DISCL_NONEXCLUSIVE	= 3,
			IDP_DISCL_NOWINKEY		= 4
		};

		/*!	Map of device objects connected and their respective vendors
		*/
		typedef std::map<EInputDeviceType, std::string> DeviceVendors;

		/*!	Used for initialization purposes, contains type of device and it's parameter to be set when
		**	device object is constructing
		*/
		typedef std::multimap<EInputDeviceType, EInputDeviceParamater> DeviceParameter;

		/*!
		**	\brief C_InputManager provides communication layer between engine and DirectX's input system
		*/
		class C_InputSystem : public CORE::C_Singleton<C_InputSystem>, public CORE::C_BaseClass
		{
			friend class C_DirectXMouseDevice;
			friend class C_DirectXKeyboardDevice;
			friend class C_DirectXJoyStickDevice;

		public:
			/*!	Constructor
			*/
			C_InputSystem ();

			/*!	Destructor
			*/
			virtual ~C_InputSystem ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize (DeviceParameter& parameters);

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Creates appropriate input system.
			**	\param paramList ParamList contains specific info for devices
			**	\param hwnd Handle of the window
			**	\return A pointer to the created manager, or raises Exception
			*/
			static C_InputSystem* CreateInputSystem (DeviceParameter &paramList, HWND hwnd);

			/*!	Destroys the input system
			**	\param inputSystem System to destroy
			*/
			static void DestroyInputSystem (C_InputSystem* inputSystem);

			/*!	Returns the number of the specified InputDeviceType devices discovered by engine
			**	\param deviceType Type that you are interested in
			*/
			int GetNumberOfDevices (EInputDeviceType deviceType);

			/*!	Returns name of the input system
			*/
			const std::string& GetInputSystemName ();

			/*!	Tries to create an object with the specified vendor. If you have no preference of vendor, leave vendor as default (""). 
			**	Raises exception on failure
			*/
			C_InputDevice* CreateInputDevice (EInputDeviceType type, bool isBuffered, const std::string &vendor = "");

			/*!	Destroys Input device
			*/
			void DestroyInputDevice (C_InputDevice* device);

		private:	
			/*! Internal method, used for flagging keyboard as available/unavailable for creation
			*/
			void SetKeyboardUsed (bool used);

			/*! Internal method, used for flagging mouse as available/unavailable for creation
			*/
			void SetMouseUsed (bool used);

			/*! Internal method, return unused joystick to queue
			*/
			void ReturnJoyStick (const S_JoyStickInfo& joystick);

			/*! Returns HWND needed by DirectInput Device Object
			*/
			HWND GetWindowHandle ();

			/*! Used when enumerating devices
			*/
			static BOOL CALLBACK EnumerateDevicesCallback (LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);

			HWND						m_HWND;					///<	The window handle we are using			
			IDirectInput8*				m_DirectInput;			///<	Direct Input Interface			

			DWORD						m_KeyboardParameters;	///<	Keyboard device parameters
			DWORD						m_MouseParameters;		///<	Mouse device parameters
			DWORD						m_JoyStickParameters;	///<	JoyStick device parameters			

			bool						m_KeyboardUsed;			///<	Used to know if we used up keyboard
			bool						m_MouseUsed;			///<	Used to know if we used up mouse

			int							m_JoySticksCount;		///<	Number of JoySticks
			std::vector<S_JoyStickInfo>	m_UnusedJoysticks;		///<	List of unused joysticks
			const std::string			m_InputSystemName;		///<	Name of the input system

			std::vector<C_InputDevice*>	m_InputDevices;			///<	Created devices
		};

	}	//	namespace INPUT
}	//	namespace REDEEMER

#endif	//	_R_INPUT_INPUTSYSTEM_H_
