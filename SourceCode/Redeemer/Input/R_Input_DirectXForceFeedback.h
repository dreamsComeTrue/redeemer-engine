/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Input_DirectXForceFeedback.h
**		DirectX implementation of Force Feedback device
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_INPUT_DIRECTXFORCEFEEDBACK_H_
#define _R_INPUT_DIRECTXFORCEFEEDBACK_H_

#include "R_Input_ForceFeedback.h"

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

namespace REDEEMER
{
	namespace INPUT
	{
		/*!
		**	\brief DirectX implementation of Force Feedback device
		*/
		class C_DirectXForceFeedback : public C_ForceFeedback
		{
		public:
			/*!	Constructor
			*/
			C_DirectXForceFeedback (IDirectInputDevice8* joyStick);

			/*!	Destructor
			*/
			virtual ~C_DirectXForceFeedback ();

			/*!	This is like setting the master volume of an audio device.	Individual effects have gain levels; however, this affects all
			**	effects at once.
			**	\param	level A value between 0.0 and 1.0 represent the percentage of gain. 1.0	being the highest possible force level (means no scaling).
			*/
			virtual void SetMasterGain (float level);
			
			/*!	If using Force Feedback effects, this should be turned off before uploading any effects. Auto centering is the motor moving
			**	the joystick back to center. 
			**	\param autoOn true to turn auto centering on, false to turn off.
			*/
			virtual void SetAutoCenterMode (bool autoOn);

			/*!	Creates and Plays the effect immediately. If the device is full	of effects, it will fail to be uploaded. You will know this by
			**	an invalid Effect Handle
			*/
			virtual void Upload (const C_ForceFeedbackEffect* effect);

			/*!	Modifies an effect that is currently playing
			*/
			virtual void Modify (const C_ForceFeedbackEffect* effect);

			/*!	Remove the effect from the device
			*/
			virtual void Remove (const C_ForceFeedbackEffect* effect);

			/*! Get the number of supported Axes for FF usage
			*/
			virtual short GetForceFeedbackAxesCount();

			/*!	Internal use.. Used during enumeration to build a list of a devices	support effects.
			*/
			void AddEffectSupport (LPCDIEFFECTINFO pdei);

		protected:
			//	Specific Effect Settings

			/*!	Uploads constant effect to the device
			*/
			void UpdateConstantEffect (const C_ForceFeedbackEffect* effect);

			/*!	Uploads ramp effect to the device
			*/
			void UpdateRampEffect (const C_ForceFeedbackEffect* effect);

			/*!	Uploads periodic effect to the device
			*/
			void UpdatePeriodicEffect (const C_ForceFeedbackEffect* effect);

			/*!	Uploads conditional effect to the device
			*/
			void UpdateConditionalEffect (const C_ForceFeedbackEffect* effect);

			/*!	Uploads custom effect to the device
			*/
			void UpdateCustomEffect (const C_ForceFeedbackEffect* effect);

			/*!	Sets the common properties to all effects
			*/
			void SetCommonProperties (DIEFFECT* diEffect, DWORD* rgdwAxes, LONG* rglDirection, DWORD struct_size, LPVOID struct_type, const C_ForceFeedbackEffect* effect);

			/*!	Actually do the upload
			*/
			void Upload (GUID, DIEFFECT*, const C_ForceFeedbackEffect*);

			IDirectInputDevice8*						m_JoyStickDevice;	///<	JoyStick device this effect is attached to

			/*!Effect list type
			*/
			typedef std::map<int, LPDIRECTINPUTEFFECT>	EffectList;
			EffectList									m_EffectList;		///<	List of valid effects

			/*!	Simple unique handle creation - allows for upto 2+ million effects during the lifetime of application. Hopefully, that is enough.
			*/
			int											m_Handles;			
		};

	}	//	namespace INPUT
}	//	namespace REDEEMER

#endif	//	_R_INPUT_DIRECTXFORCEFEEDBACK_H_
