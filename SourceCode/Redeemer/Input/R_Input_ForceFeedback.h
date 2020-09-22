/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Input_ForceFeedback.h
**		Interface class for dealing with Force Feedback devices
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_INPUT_FORCEFEEDBACK_H_
#define _R_INPUT_FORCEFEEDBACK_H_

#include "R_Input_InputDevice.h"
#include "R_Input_ForceFeedbackEffect.h"

#include <map>

namespace REDEEMER
{
	namespace INPUT
	{
		/*!
		**	\brief Interface class for dealing with Force Feedback devices
		*/
		class C_ForceFeedback : public C_InputDeviceInterface
		{
		public:
			/*!	Constructor
			*/
			C_ForceFeedback();

			/*!	Destructor
			*/
			virtual ~C_ForceFeedback ();

			/*!	This is like setting the master volume of an audio device.	Individual effects have gain levels; however, this affects all
			**	effects at once.
			**	\param	level A value between 0.0 and 1.0 represent the percentage of gain. 1.0	being the highest possible force level (means no scaling).
			*/
			virtual void SetMasterGain (float level) = 0;
			
			/*!	If using Force Feedback effects, this should be turned off before uploading any effects. Auto centering is the motor moving
			**	the joystick back to center. 
			**	\param autoOn true to turn auto centering on, false to turn off.
			*/
			virtual void SetAutoCenterMode (bool autoOn) = 0;

			/*!	Creates and Plays the effect immediately. If the device is full	of effects, it will fail to be uploaded. You will know this by
			**	an invalid Effect Handle
			*/
			virtual void Upload (const C_ForceFeedbackEffect* effect) = 0;

			/*!	Modifies an effect that is currently playing
			*/
			virtual void Modify (const C_ForceFeedbackEffect* effect) = 0;

			/*!	Remove the effect from the device
			*/
			virtual void Remove (const C_ForceFeedbackEffect* effect) = 0;

			/*! Get the number of supported Axes for FF usage
			*/
			virtual short GetForceFeedbackAxesCount() = 0;

			/*!	Type for supported effect list
			*/
			typedef std::map<C_ForceFeedbackEffect::EForceType, C_ForceFeedbackEffect::EEffectType> SupportedEffectList;

			/*!	Get a list of all supported effects
			*/
			const SupportedEffectList& GetSupportedEffects () const;

			/*!	Adds new effect
			*/
			void AddEffectTypes (C_ForceFeedbackEffect::EForceType force, C_ForceFeedbackEffect::EEffectType type);

		protected:			
			SupportedEffectList m_SupportedEffects;		///<	List of supported effects
		};

	}	//	namespace INPUT
}	//	namespace REDEEMER

#endif	//	_R_INPUT_FORCEFEEDBACK_H_
