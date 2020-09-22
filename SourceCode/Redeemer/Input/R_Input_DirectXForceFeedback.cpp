/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Input_DirectXForceFeedback.cpp
**		DirectX implementation of Force Feedback device
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Input_DirectXForceFeedback.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace INPUT
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_DirectXForceFeedback::C_DirectXForceFeedback (IDirectInputDevice8* joyStick) :
			m_JoyStickDevice (joyStick),
			m_Handles (0)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DirectXForceFeedback::~C_DirectXForceFeedback ()
		{
			//	Get the effect - if it exists
			for(EffectList::iterator it = m_EffectList.begin(); it != m_EffectList.end(); it ++)
			{
				LPDIRECTINPUTEFFECT dxEffect = it->second;

				if (dxEffect)
					dxEffect->Unload();
			}

			m_EffectList.clear();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectXForceFeedback::SetMasterGain (float level)
		{
			//Between 0 - 10,000
			int gain_level = (int)(10000.0f * level);

			if (gain_level > 10000)
				gain_level = 10000;
			else 
				if (gain_level < 0)
					gain_level = 0;

			DIPROPDWORD DIPropGain;
			DIPropGain.diph.dwSize       = sizeof(DIPropGain);
			DIPropGain.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			DIPropGain.diph.dwObj        = 0;
			DIPropGain.diph.dwHow        = DIPH_DEVICE;
			DIPropGain.dwData            = gain_level;

			m_JoyStickDevice->SetProperty (DIPROP_FFGAIN, &DIPropGain.diph);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectXForceFeedback::SetAutoCenterMode (bool autoOn)
		{
			//DI Property DIPROPAUTOCENTER_OFF = 0, 1 is on
			DIPROPDWORD DIPropAutoCenter;
			DIPropAutoCenter.diph.dwSize       = sizeof(DIPropAutoCenter);
			DIPropAutoCenter.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			DIPropAutoCenter.diph.dwObj        = 0;
			DIPropAutoCenter.diph.dwHow        = DIPH_DEVICE;
			DIPropAutoCenter.dwData            = autoOn;

			//hr =
			m_JoyStickDevice->SetProperty (DIPROP_AUTOCENTER, &DIPropAutoCenter.diph);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectXForceFeedback::Upload (const C_ForceFeedbackEffect* effect)
		{
			switch (effect->m_Force)
			{
			case C_ForceFeedbackEffect::ConstantForce: 
				UpdateConstantEffect (effect);	
				break;

			case C_ForceFeedbackEffect::RampForce: 
				UpdateRampEffect (effect);	
				break;

			case C_ForceFeedbackEffect::PeriodicForce: 
				UpdatePeriodicEffect (effect);
				break;

			case C_ForceFeedbackEffect::ConditionalForce:	
				UpdateConditionalEffect (effect);
				break;

			default: 
				REDEEMER_LOG << LOG_ERROR << "Requested force for Force Feedback device not implemented" << LOG_ENDMESSAGE;
				break;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectXForceFeedback::Modify (const C_ForceFeedbackEffect* effect)
		{
			//	Modifying is essentially the same as an upload, so, just reuse that function
			Upload (effect);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectXForceFeedback::Remove (const C_ForceFeedbackEffect* effect)
		{
			//Get the effect - if it exists
			EffectList::iterator it = m_EffectList.find (effect->m_Handle);

			if (it != m_EffectList.end())
			{
				LPDIRECTINPUTEFFECT dxEffect = it->second;

				if (dxEffect)
				{
					dxEffect->Stop ();

					//	We care about the return value - as the effect might not have been unlaoded
					if (SUCCEEDED (dxEffect->Unload()))
						m_EffectList.erase(it);
				}
				else
					m_EffectList.erase(it);
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		short C_DirectXForceFeedback::GetForceFeedbackAxesCount()
		{
			return 1;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectXForceFeedback::AddEffectSupport (LPCDIEFFECTINFO pdei)
		{
			//	Determine what the effect is and how it corresponds to our Enums
			//	We could save the GUIDs too, however, we will just use the predefined ones later
			if (pdei->guid == GUID_ConstantForce)
				AddEffectTypes((C_ForceFeedbackEffect::EForceType)DIEFT_GETTYPE(pdei->dwEffType), C_ForceFeedbackEffect::Constant);
			else if( pdei->guid == GUID_Triangle )
				AddEffectTypes((C_ForceFeedbackEffect::EForceType)DIEFT_GETTYPE(pdei->dwEffType), C_ForceFeedbackEffect::Triangle);
			else if( pdei->guid == GUID_Spring )
				AddEffectTypes((C_ForceFeedbackEffect::EForceType)DIEFT_GETTYPE(pdei->dwEffType), C_ForceFeedbackEffect::Spring);
			else if( pdei->guid == GUID_Friction )
				AddEffectTypes((C_ForceFeedbackEffect::EForceType)DIEFT_GETTYPE(pdei->dwEffType), C_ForceFeedbackEffect::Friction);
			else if( pdei->guid == GUID_Square )
				AddEffectTypes((C_ForceFeedbackEffect::EForceType)DIEFT_GETTYPE(pdei->dwEffType), C_ForceFeedbackEffect::Square);
			else if( pdei->guid == GUID_Sine )
				AddEffectTypes((C_ForceFeedbackEffect::EForceType)DIEFT_GETTYPE(pdei->dwEffType), C_ForceFeedbackEffect::Sine);
			else if( pdei->guid == GUID_SawtoothUp )
				AddEffectTypes((C_ForceFeedbackEffect::EForceType)DIEFT_GETTYPE(pdei->dwEffType), C_ForceFeedbackEffect::SawToothUp);
			else if( pdei->guid == GUID_SawtoothDown )
				AddEffectTypes((C_ForceFeedbackEffect::EForceType)DIEFT_GETTYPE(pdei->dwEffType), C_ForceFeedbackEffect::SawToothDown);
			else if( pdei->guid == GUID_Damper )
				AddEffectTypes((C_ForceFeedbackEffect::EForceType)DIEFT_GETTYPE(pdei->dwEffType), C_ForceFeedbackEffect::Damper);
			else if( pdei->guid == GUID_Inertia )
				AddEffectTypes((C_ForceFeedbackEffect::EForceType)DIEFT_GETTYPE(pdei->dwEffType), C_ForceFeedbackEffect::Inertia);
			else if( pdei->guid == GUID_CustomForce )
				AddEffectTypes((C_ForceFeedbackEffect::EForceType)DIEFT_GETTYPE(pdei->dwEffType), C_ForceFeedbackEffect::Custom);
			else if( pdei->guid == GUID_RampForce )
				AddEffectTypes((C_ForceFeedbackEffect::EForceType)DIEFT_GETTYPE(pdei->dwEffType), C_ForceFeedbackEffect::Ramp);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectXForceFeedback::UpdateConstantEffect (const C_ForceFeedbackEffect* effect)
		{
			DWORD           rgdwAxes[2]     = { DIJOFS_X, DIJOFS_Y };
			LONG            rglDirection[2] = { 0, 0 };
			DICONSTANTFORCE cf;
			DIEFFECT        diEffect;

			//Currently only support 1 axis
			//if( effect->getNumAxes() == 1 )
			cf.lMagnitude = static_cast<C_ForceFeedback_ConstantEffect*>(effect->GetForceEffect())->m_Level;

			SetCommonProperties (&diEffect, rgdwAxes, rglDirection, sizeof(DICONSTANTFORCE), &cf, effect);
			Upload (GUID_ConstantForce, &diEffect, effect);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectXForceFeedback::UpdateRampEffect (const C_ForceFeedbackEffect* effect)
		{
			DWORD           rgdwAxes[2]     = { DIJOFS_X, DIJOFS_Y };
			LONG            rglDirection[2] = { 0, 0 };
			DIRAMPFORCE     rf;
			DIEFFECT        diEffect;

			//Currently only support 1 axis
			rf.lStart = static_cast<C_ForceFeedback_RampEffect*>(effect->GetForceEffect())->m_StartLevel;
			rf.lEnd = static_cast<C_ForceFeedback_RampEffect*>(effect->GetForceEffect())->m_EndLevel;

			SetCommonProperties (&diEffect, rgdwAxes, rglDirection, sizeof(DIRAMPFORCE), &rf, effect);
			Upload (GUID_RampForce, &diEffect, effect);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectXForceFeedback::UpdatePeriodicEffect (const C_ForceFeedbackEffect* effect)
		{
			DWORD           rgdwAxes[2]     = { DIJOFS_X, DIJOFS_Y };
			LONG            rglDirection[2] = { 0, 0 };
			DIPERIODIC      pf;
			DIEFFECT        diEffect;

			//Currently only support 1 axis
			pf.dwMagnitude = static_cast<C_ForceFeedback_PeriodicEffect*>(effect->GetForceEffect())->m_Magnitude;
			pf.lOffset = static_cast<C_ForceFeedback_PeriodicEffect*>(effect->GetForceEffect())->m_Offset;
			pf.dwPhase = static_cast<C_ForceFeedback_PeriodicEffect*>(effect->GetForceEffect())->m_Phase;
			pf.dwPeriod = static_cast<C_ForceFeedback_PeriodicEffect*>(effect->GetForceEffect())->m_Period;

			SetCommonProperties(&diEffect, rgdwAxes, rglDirection, sizeof(DIPERIODIC), &pf, effect);

			switch (effect->m_Type)
			{
			case C_ForceFeedbackEffect::Square: 
				Upload(GUID_Square, &diEffect, effect);
				break;

			case C_ForceFeedbackEffect::Triangle: 
				Upload(GUID_Triangle, &diEffect, effect);
				break;

			case C_ForceFeedbackEffect::Sine:
				Upload(GUID_Sine, &diEffect, effect);
				break;

			case C_ForceFeedbackEffect::SawToothUp:
				Upload(GUID_SawtoothUp, &diEffect, effect);
				break;

			case C_ForceFeedbackEffect::SawToothDown:	
				Upload(GUID_SawtoothDown, &diEffect, effect);
				break;

			default: break;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectXForceFeedback::UpdateConditionalEffect (const C_ForceFeedbackEffect* effect)
		{
			DWORD           rgdwAxes[2]     = { DIJOFS_X, DIJOFS_Y };
			LONG            rglDirection[2] = { 0, 0 };
			DICONDITION     cf;
			DIEFFECT        diEffect;

			cf.lOffset = static_cast<C_ForceFeedback_ConditionalEffect*>(effect->GetForceEffect())->m_DeadBand;
			cf.lPositiveCoefficient = static_cast<C_ForceFeedback_ConditionalEffect*>(effect->GetForceEffect())->m_RightCoeff;
			cf.lNegativeCoefficient = static_cast<C_ForceFeedback_ConditionalEffect*>(effect->GetForceEffect())->m_LeftCoeff;
			cf.dwPositiveSaturation = static_cast<C_ForceFeedback_ConditionalEffect*>(effect->GetForceEffect())->m_RightSaturation;
			cf.dwNegativeSaturation = static_cast<C_ForceFeedback_ConditionalEffect*>(effect->GetForceEffect())->m_LeftSaturation;
			cf.lDeadBand = static_cast<C_ForceFeedback_ConditionalEffect*>(effect->GetForceEffect())->m_DeadBand;

			SetCommonProperties (&diEffect, rgdwAxes, rglDirection, sizeof(DICONDITION), &cf, effect);

			switch (effect->m_Type)
			{
			case C_ForceFeedbackEffect::Friction:	
				Upload(GUID_Friction, &diEffect, effect); 
				break;

			case C_ForceFeedbackEffect::Damper: 
				Upload(GUID_Damper, &diEffect, effect);
				break;

			case C_ForceFeedbackEffect::Inertia:
				Upload(GUID_Inertia, &diEffect, effect);
				break;

			case C_ForceFeedbackEffect::Spring: 
				Upload(GUID_Spring, &diEffect, effect); 
				break;

			default: 
				break;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectXForceFeedback::UpdateCustomEffect (const C_ForceFeedbackEffect* effect)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectXForceFeedback::SetCommonProperties (DIEFFECT* diEffect, DWORD* rgdwAxes, LONG* rglDirection, DWORD struct_size, LPVOID struct_type, const C_ForceFeedbackEffect* effect)
		{
			ZeroMemory (diEffect, sizeof(DIEFFECT));

			diEffect->dwSize                  = sizeof(DIEFFECT);
			diEffect->dwFlags                 = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
			diEffect->dwDuration              = effect->m_ReplayLength;
			diEffect->dwSamplePeriod          = 0;
			diEffect->dwGain                  = DI_FFNOMINALMAX;
			diEffect->dwTriggerButton         = DIEB_NOTRIGGER;
			diEffect->dwTriggerRepeatInterval = 0;
			diEffect->cAxes                   = effect->GetAxesCount();
			diEffect->rgdwAxes                = rgdwAxes;
			diEffect->rglDirection            = rglDirection;
			diEffect->lpEnvelope              = 0;
			diEffect->cbTypeSpecificParams    = struct_size;
			diEffect->lpvTypeSpecificParams   = struct_type;
			diEffect->dwStartDelay            = effect->m_ReplayDelay;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectXForceFeedback::Upload (GUID guid, DIEFFECT* diEffect, const C_ForceFeedbackEffect* effect)
		{
			LPDIRECTINPUTEFFECT dxEffect = 0;

			//	Get the effect - if it exists
			EffectList::iterator it = m_EffectList.find (effect->m_Handle);

			//	It has been created already
			if (it != m_EffectList.end())
				dxEffect = it->second;
			else //	This effect has not yet been created - generate a handle
				effect->m_Handle = m_Handles++;

			if (dxEffect == 0)
			{
				//This effect has not yet been created, so create it
				HRESULT hr = m_JoyStickDevice->CreateEffect (guid, diEffect, &dxEffect, NULL);
				
				if (SUCCEEDED (hr))
				{
					m_EffectList[effect->m_Handle] = dxEffect;

					dxEffect->Start (INFINITE, 0);
				}
				else 
					if	(hr == DIERR_DEVICEFULL)
						REDEEMER_LOG << LOG_FATAL << "Remove an effect before adding more for Force Feedback Device!" << LOG_ENDMESSAGE;
					else
						REDEEMER_LOG << LOG_FATAL << "Unknown error creating Force Feedback effect" <<LOG_ENDMESSAGE;
			}
			else
			{
				//ToDo -- Update the Effect
				HRESULT hr = dxEffect->SetParameters (diEffect, DIEP_DIRECTION |
					DIEP_DURATION | DIEP_ENVELOPE | DIEP_STARTDELAY | DIEP_TRIGGERBUTTON |
					DIEP_TRIGGERREPEATINTERVAL | DIEP_TYPESPECIFICPARAMS | DIEP_START );

				if (FAILED (hr)) 
					REDEEMER_LOG << LOG_FATAL << "Error updating Force Feedback device!" << LOG_ENDMESSAGE;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------
																																  
	}	//	namespace INPUT
}	//	namespace REDEEMER

