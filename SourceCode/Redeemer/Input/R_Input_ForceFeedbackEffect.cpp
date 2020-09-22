/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Input_ForceFeedbackEffect.cpp
**		Creating various effects on input devices supporting FF
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Input_ForceFeedbackEffect.h"
#include "..\\RedeemerEngine.h"

// disable:  forcing value to bool 'true' or 'false' (performance warning)
#pragma warning (disable: 4800)

namespace REDEEMER
{
	namespace INPUT
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_ForceFeedbackEffect::C_ForceFeedbackEffect ()	:
			m_Force (UnknownForce),
			m_Type (Unknown),
			m_Effect (NULL),
			m_AxesCount (1)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_ForceFeedbackEffect::C_ForceFeedbackEffect (EForceType ef, EEffectType et)   :
			m_Force (ef), 
			m_Type (et),
			m_Direction (North), 
			m_TriggerButton (-1),
			m_TriggerInterval (0),
			m_ReplayLength (C_ForceFeedbackEffect::FORCEFEEDBACK_INFINITE),
			m_ReplayDelay (0),
			m_Handle (-1),
			m_AxesCount (1)
		{
			m_Effect = 0;

			switch( ef )
			{
			case ConstantForce:    
				m_Effect = new C_ForceFeedback_ConstantEffect();
				break;

			case RampForce:	       
				m_Effect = new C_ForceFeedback_RampEffect();
				break;

			case PeriodicForce:    
				m_Effect = new C_ForceFeedback_PeriodicEffect(); 
				break;

			case ConditionalForce: 
				m_Effect = new C_ForceFeedback_ConditionalEffect(); 
				break;

			default: 
				break;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_ForceFeedbackEffect::~C_ForceFeedbackEffect()
		{
			REDEEMER_SAFE_DELETE (m_Effect);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_ForceFeedback_ForceEffect* C_ForceFeedbackEffect::GetForceEffect() const
		{
			//	If no effect was created in constructor, then we raise an error here
			if (m_Effect == 0)
				REDEEMER_LOG << LOG_FATAL << L"Can not return requested ForceFeedback Effect" << LOG_ENDMESSAGE;

			return m_Effect;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_ForceFeedbackEffect::SetAxesCount (short nAxes)
		{
			//	Can only be set before a handle was assigned (effect created)
			if (m_Handle != -1)
				m_AxesCount = nAxes;
		}

		//------------------------------------------------------------------------------------------------------------------------

		short C_ForceFeedbackEffect::GetAxesCount () const
		{
			return m_AxesCount;
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_ForceFeedback_ForceEffect::~C_ForceFeedback_ForceEffect ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_ForceFeedback_Envelope::C_ForceFeedback_Envelope ()  :
			m_AttackLength (0),
			m_AttackLevel (0),
			m_FadeLength (0),
			m_FadeLevel (0)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_ForceFeedback_Envelope::IsUsed ()
		{
			return (bool)(m_AttackLength | m_AttackLevel | m_FadeLength | m_FadeLevel);
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_ForceFeedback_ConstantEffect::C_ForceFeedback_ConstantEffect () :
			m_Level (5000)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_ForceFeedback_RampEffect::C_ForceFeedback_RampEffect ()  :
			m_StartLevel (0),
			m_EndLevel (0)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_ForceFeedback_PeriodicEffect::C_ForceFeedback_PeriodicEffect ()  :
			m_Magnitude (0),
			m_Offset (0),
			m_Phase (0),
			m_Period (0)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_ForceFeedback_ConditionalEffect::C_ForceFeedback_ConditionalEffect () :
			m_RightCoeff (0),
			m_LeftCoeff (0),
			m_RightSaturation (0),
			m_LeftSaturation (0),
			m_DeadBand (0),
			m_Center (0)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace INPUT
}	//	namespace REDEEMER

