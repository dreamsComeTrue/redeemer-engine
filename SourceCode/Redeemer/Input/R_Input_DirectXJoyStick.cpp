/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Input_DirectXJoyStick.cpp
**		DirectX implementation of JoyStick class
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Input_DirectXJoyStick.h"
#include "R_Input_DirectXForceFeedback.h"
#include "..\\RedeemerEngine.h"

//DX Only defines macros for the JOYSTICK not JOYSTICK2, so fix it
#undef DIJOFS_BUTTON
#undef DIJOFS_POV

#define DIJOFS_BUTTON(n)  (FIELD_OFFSET(DIJOYSTATE2, rgbButtons) + (n))
#define DIJOFS_POV(n)     (FIELD_OFFSET(DIJOYSTATE2, rgdwPOV)+(n)*sizeof(DWORD))
#define DIJOFS_SLIDER0(n) (FIELD_OFFSET(DIJOYSTATE2, rglSlider)+(n) * sizeof(LONG))
#define DIJOFS_SLIDER1(n) (FIELD_OFFSET(DIJOYSTATE2, rglVSlider)+(n) * sizeof(LONG))
#define DIJOFS_SLIDER2(n) (FIELD_OFFSET(DIJOYSTATE2, rglASlider)+(n) * sizeof(LONG))
#define DIJOFS_SLIDER3(n) (FIELD_OFFSET(DIJOYSTATE2, rglFSlider)+(n) * sizeof(LONG))

namespace REDEEMER
{
	namespace INPUT
	{
		#define INPUTDEVICE_JOYSTICK_DIRECTX_BUFFERSIZE 124

		//------------------------------------------------------------------------------------------------------------------------
		C_DirectXJoyStickDevice::C_DirectXJoyStickDevice (C_InputSystem* inputSystem, IDirectInput8* inputDevice, bool isBuffered, DWORD cooperationSettings, const S_JoyStickInfo &info) :
			C_JoyStickDevice (info.VendorName, isBuffered, inputSystem),
			m_DirectInput (inputDevice),
			m_CooperationSettings (cooperationSettings),
			m_JoyStickDevice (NULL),
			m_DeviceGuid (info.DeviceGUID),
			m_ForceFeedbackDevice (NULL)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DirectXJoyStickDevice::~C_DirectXJoyStickDevice ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_DirectXJoyStickDevice::Initialize ()
		{
			C_BaseClass::Initialize();

			//	Clear old state
			m_State.m_Axes.clear();

			REDEEMER_SAFE_DELETE (m_ForceFeedbackDevice);

			DIPROPDWORD dipdw;

			dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
			dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			dipdw.diph.dwObj        = 0;
			dipdw.diph.dwHow        = DIPH_DEVICE;
			dipdw.dwData            = INPUTDEVICE_JOYSTICK_DIRECTX_BUFFERSIZE;

			if (FAILED (m_DirectInput->CreateDevice (m_DeviceGuid, &m_JoyStickDevice, NULL)))
			{
				REDEEMER_LOG << LOG_ERROR << L"Could not initialize DirectX JoyStick device" << LOG_ENDMESSAGE;

				return RESULT_FAIL;
			}

			if (FAILED (m_JoyStickDevice->SetDataFormat (&c_dfDIJoystick2)))
			{
				REDEEMER_LOG << LOG_ERROR << L"Could not set proper data format for DirectX JoyStick device" << LOG_ENDMESSAGE;

				return RESULT_FAIL;
			}

			HWND hwin = m_InputSystem->GetWindowHandle ();

			if (FAILED (m_JoyStickDevice->SetCooperativeLevel (hwin, m_CooperationSettings)))
			{
				REDEEMER_LOG << LOG_ERROR << L"Failed to set cooperation level for DirectX JoyStick device" << LOG_ENDMESSAGE;

				return RESULT_FAIL;
			}

			if (FAILED (m_JoyStickDevice->SetProperty (DIPROP_BUFFERSIZE, &dipdw.diph)))
			{
				REDEEMER_LOG << LOG_ERROR << L"Failed to set buffer size for DirectX JoyStick device" << LOG_ENDMESSAGE;

				return RESULT_FAIL;
			}

			//	Enumerate all axes/buttons/sliders/etc before acquiring
			//	We can check force feedback here too
			DIDEVCAPS  DIJoyCaps;

			DIJoyCaps.dwSize = sizeof(DIDEVCAPS);

			m_JoyStickDevice->GetCapabilities (&DIJoyCaps);

			m_POVCount = (short)DIJoyCaps.dwPOVs;

			m_State.m_Buttons.resize (DIJoyCaps.dwButtons);
			m_State.m_Axes.resize (DIJoyCaps.dwAxes);

			//Reset the axis mapping enumeration value
			m_AxisNumber = 0;

			//Enumerate Force Feedback (if any)
			m_JoyStickDevice->EnumEffects (DIEnumEffectsCallback, this, DIEFT_ALL);

			//Enumerate and set axis constraints (and check FF Axes)
			m_JoyStickDevice->EnumObjects (DIEnumDeviceObjectsCallback, this, DIDFT_AXIS);

			m_State.Clear();

			RetrieveData ();

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_DirectXJoyStickDevice::Finalize ()
		{
			REDEEMER_SAFE_DELETE (m_ForceFeedbackDevice);

			if (m_JoyStickDevice)
			{
				m_JoyStickDevice->Unacquire();

				REDEEMER_SAFE_RELEASE (m_JoyStickDevice);
			}

			//Return joystick to pool
			S_JoyStickInfo js;
			js.DeviceID = -1;
			js.DeviceGUID = m_DeviceGuid;
			js.VendorName = m_VendorName;

			m_InputSystem->ReturnJoyStick (js);

			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectXJoyStickDevice::SetBuffered (bool buffered)
		{
			m_IsBuffered = buffered;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_InputDeviceInterface* C_DirectXJoyStickDevice::QueryInterface (C_InputDeviceInterface::EInputDeviceInterfaceType type)
		{
			//Thought about using covariant return type here.. however,
			//some devices may allow LED light changing, or other interface stuff

			if (m_ForceFeedbackDevice && type == C_InputDeviceInterface::IDIT_ForceFeedback)
				return m_ForceFeedbackDevice;
			else
				return 0;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectXJoyStickDevice::RetrieveData ()
		{
			DIDEVICEOBJECTDATA diBuff[INPUTDEVICE_JOYSTICK_DIRECTX_BUFFERSIZE];
			DWORD entries = INPUTDEVICE_JOYSTICK_DIRECTX_BUFFERSIZE;

			//	Poll the device to read the current state
			HRESULT hr = m_JoyStickDevice->Poll ();

			if (hr == DI_OK)
				hr = m_JoyStickDevice->GetDeviceData (sizeof(DIDEVICEOBJECTDATA), diBuff, &entries, 0);

			if (hr != DI_OK)
			{
				hr = m_JoyStickDevice->Acquire();

				while (hr == DIERR_INPUTLOST)
					hr = m_JoyStickDevice->Acquire();

				// Poll the device to read the current state
				m_JoyStickDevice->Poll ();

				hr = m_JoyStickDevice->GetDeviceData (sizeof(DIDEVICEOBJECTDATA), diBuff, &entries, 0);

				//	Perhaps the user just tabbed away
				if (FAILED (hr))
					return;
			}

			bool axisMoved[24] = {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
				false,false,false,false,false,false,false,false};
			bool sliderMoved[4] = {false,false,false,false};

			//	Loop through all the events
			for (unsigned int i = 0; i < entries; ++i)
			{
				//	First check to see if event entry is a Axis we enumerated earlier
				if (diBuff[i].uAppData != 0xFFFFFFFF && diBuff[i].uAppData > 0)
				{
					int axis = (int)(0x7FFFFFFF & diBuff[i].uAppData); //Mask out the high bit

					assert (axis >= 0 && axis < (int)m_State.m_Axes.size() && "Axis out of range!");

					m_State.m_Axes[axis].Abs = diBuff[i].dwData;
					axisMoved[axis] = true;
				}
				else
				{
					//	This may seem out of order, but is in order of the way these variables
					//	are declared in the JoyStick State 2 structure.
					switch (diBuff[i].dwOfs)
					{
						//------ slider -//
					case DIJOFS_SLIDER0(0):
						sliderMoved[0] = true;
						m_State.m_Sliders[0].AbX = diBuff[i].dwData;
						break;

					case DIJOFS_SLIDER0(1):
						sliderMoved[0] = true;
						m_State.m_Sliders[0].AbY = diBuff[i].dwData;
						break;

						//----- Max 4 POVs Next ---------------//
					case DIJOFS_POV(0):
						if (!ChangePOV (0, diBuff[i]))
							return;

						break;

					case DIJOFS_POV(1):
						if (!ChangePOV(1, diBuff[i]))
							return;

						break;

					case DIJOFS_POV(2):
						if (!ChangePOV(2, diBuff[i]))
							return;

						break;

					case DIJOFS_POV(3):
						if (!ChangePOV(3, diBuff[i]))
							return;

						break;

					case DIJOFS_SLIDER1(0):
						sliderMoved[1] = true;
						m_State.m_Sliders[1].AbX = diBuff[i].dwData;
						break;

					case DIJOFS_SLIDER1(1):
						sliderMoved[1] = true;
						m_State.m_Sliders[1].AbY = diBuff[i].dwData;
						break;

					case DIJOFS_SLIDER2(0):
						sliderMoved[2] = true;
						m_State.m_Sliders[2].AbX = diBuff[i].dwData;
						break;

					case DIJOFS_SLIDER2(1):
						sliderMoved[2] = true;
						m_State.m_Sliders[2].AbY = diBuff[i].dwData;
						break;

					case DIJOFS_SLIDER3(0):
						sliderMoved[3] = true;
						m_State.m_Sliders[3].AbX = diBuff[i].dwData;
						break;

					case DIJOFS_SLIDER3(1):
						sliderMoved[3] = true;
						m_State.m_Sliders[3].AbY = diBuff[i].dwData;
						break;

						//-----------------------------------------//
					default:
						//Handle Button Events Easily using the DX Offset Macros
						if (diBuff[i].dwOfs >= DIJOFS_BUTTON(0) && diBuff[i].dwOfs < DIJOFS_BUTTON(128))
						{
							if (!DoButtonClick ((diBuff[i].dwOfs - DIJOFS_BUTTON(0)), diBuff[i]))
								return;
						}
						break;
					}
				}
			}

			//Check to see if any of the axes values have changed.. if so send events
			if (m_IsBuffered && !m_Listeners.empty () && entries > 0)
			{
				C_JoyStickEvent temp(m_State);

				//	Update axes
				for (int i = 0; i < 24; ++i)
					if (axisMoved[i])
					{
						for (unsigned j = 0; j < m_Listeners.size(); j ++)
							if (m_Listeners[j]->AxisMoved (temp, i) == false)
								return;
					}

				//	Now update sliders
				for (int i = 0; i < 4; ++i)
					if (sliderMoved[i])
					{
						for (unsigned j = 0; j < m_Listeners.size(); j ++)
							if (m_Listeners[j]->SliderMoved (temp, i) == false)
								return;
					}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		BOOL CALLBACK C_DirectXJoyStickDevice::DIEnumDeviceObjectsCallback (LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
		{
			C_DirectXJoyStickDevice* _this = (C_DirectXJoyStickDevice*)pvRef;

			//Setup mappings
			DIPROPPOINTER diptr;
			diptr.diph.dwSize       = sizeof(DIPROPPOINTER);
			diptr.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			diptr.diph.dwHow        = DIPH_BYID;
			diptr.diph.dwObj        = lpddoi->dwType;

			//Add the high bit in so that an axis value of zero does not mean a null userdata
			diptr.uData             = 0x80000000 | _this->m_AxisNumber;

			//Check if axis is slider, if so, do not treat as regular axis
			if (GUID_Slider == lpddoi->guidType)
			{
				++_this->m_SlidersCount;

				//Decrease Axes, since this slider shows up in a different place
				_this->m_State.m_Axes.pop_back();
			}
			else 
				if (FAILED (_this->m_JoyStickDevice->SetProperty (DIPROP_APPDATA, &diptr.diph)))
				{	
					//	If for some reason we could not set needed user data, just ignore this axis
					return DIENUM_CONTINUE;
				}

			//Increase for next time through
			_this->m_AxisNumber += 1;

			//Set range
			DIPROPRANGE diprg;
			diprg.diph.dwSize       = sizeof(DIPROPRANGE);
			diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			diprg.diph.dwHow        = DIPH_BYID;
			diprg.diph.dwObj        = lpddoi->dwType;
			diprg.lMin              = JOYSTICK_MIN_AXIS;
			diprg.lMax              = JOYSTICK_MAX_AXIS;

			if (FAILED (_this->m_JoyStickDevice->SetProperty(DIPROP_RANGE, &diprg.diph)))
			{
				REDEEMER_LOG << LOG_ERROR << L"Failed to set min/max range property for DirectX JoyStick device" << LOG_ENDMESSAGE;
			}

			//Check if FF Axes
			if ((lpddoi->dwFlags & DIDOI_FFACTUATOR) != 0)
			{
				if (_this->m_ForceFeedbackDevice)
				{
					//todo - increment force feedback axis count
				}
			}

			return DIENUM_CONTINUE;
		}

		//------------------------------------------------------------------------------------------------------------------------

		BOOL CALLBACK C_DirectXJoyStickDevice::DIEnumEffectsCallback (LPCDIEFFECTINFO pdei, LPVOID pvRef)
		{
			C_DirectXJoyStickDevice* _this = (C_DirectXJoyStickDevice*)pvRef;

			//Create the FF class after we know there is at least one effect type
			if (_this->m_ForceFeedbackDevice == 0)
				_this->m_ForceFeedbackDevice = new C_DirectXForceFeedback (_this->m_JoyStickDevice);

			_this->m_ForceFeedbackDevice->AddEffectSupport (pdei);

			return DIENUM_CONTINUE;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_DirectXJoyStickDevice::DoButtonClick (int button, DIDEVICEOBJECTDATA& di)
		{
			bool result = true;

			if (di.dwData & 0x80)
			{
				m_State.m_Buttons[button] = true;

				if (m_IsBuffered && !m_Listeners.empty())
					for (unsigned int i = 0; i < m_Listeners.size(); i ++)
					{
						result = m_Listeners[i]->ButtonPressed (C_JoyStickEvent (m_State), button);
					}
			}
			else
			{
				m_State.m_Buttons[button] = false;

				if (m_IsBuffered && !m_Listeners.empty())
					for (unsigned int i = 0; i < m_Listeners.size(); i ++)
					{
						result = m_Listeners[i]->ButtonReleased (C_JoyStickEvent (m_State), button);
					}
			}

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_DirectXJoyStickDevice::ChangePOV (int pov, DIDEVICEOBJECTDATA& di)
		{
			//Some drivers report a value of 65,535, instead of —1, for the center position
			if (LOWORD (di.dwData) == 0xFFFF)
			{
				m_State.m_POV[pov].Direction = C_JoyStickPOV::Centered;
			}
			else
			{
				switch(di.dwData)
				{
				case 0: m_State.m_POV[pov].Direction = C_JoyStickPOV::North; break;
				case 4500: m_State.m_POV[pov].Direction = C_JoyStickPOV::NorthEast; break;
				case 9000: m_State.m_POV[pov].Direction = C_JoyStickPOV::East; break;
				case 13500: m_State.m_POV[pov].Direction = C_JoyStickPOV::SouthEast; break;
				case 18000: m_State.m_POV[pov].Direction = C_JoyStickPOV::South; break;
				case 22500: m_State.m_POV[pov].Direction = C_JoyStickPOV::SouthWest; break;
				case 27000: m_State.m_POV[pov].Direction = C_JoyStickPOV::West; break;
				case 31500: m_State.m_POV[pov].Direction = C_JoyStickPOV::NorthWest; break;
				}
			}

			bool result = true;

			if (m_IsBuffered && !m_Listeners.empty ())
			{
				for (unsigned int i = 0; i < m_Listeners.size(); i ++)
				{
					result = m_Listeners[i]->PovMoved (C_JoyStickEvent (m_State), pov);
				}
			}

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace INPUT
}	//	namespace REDEEMER

