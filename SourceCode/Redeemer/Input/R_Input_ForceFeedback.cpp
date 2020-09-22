/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Input_ForceFeedback.cpp
**		Interface class for dealing with Force Feedback devices
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Input_ForceFeedback.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace INPUT
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_ForceFeedback::C_ForceFeedback ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_ForceFeedback::~C_ForceFeedback ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		const C_ForceFeedback::SupportedEffectList& C_ForceFeedback::GetSupportedEffects () const
		{
			return m_SupportedEffects;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_ForceFeedback::AddEffectTypes (C_ForceFeedbackEffect::EForceType force, C_ForceFeedbackEffect::EEffectType type)
		{
			if (force == C_ForceFeedbackEffect::UnknownForce || type == C_ForceFeedbackEffect::Unknown)
				REDEEMER_LOG << LOG_WARNING << "Unknown Force was added to Force Feedback Device's effect list" << LOG_ENDMESSAGE;

			m_SupportedEffects[force] = type;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace INPUT
}	//	namespace REDEEMER

