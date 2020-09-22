/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_RenderStage.cpp
**		Stages are independent units in rendering pipeline. This class is a base class for all stages, such as Geometry, Lights, etc.
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_RenderStage.h"

namespace REDEEMER
{
	namespace RENDER
	{
		//------------------------------------------------------------------------------------------------------------------------
		bool operator< (const C_RenderStage& left, const C_RenderStage& right)
		{
			return (left.m_Priority < right.m_Priority);
		}

		//------------------------------------------------------------------------------------------------------------------------
		C_RenderStage::C_RenderStage ()	:
			m_IsEnabled (true),
			m_Priority (-1)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_RenderStage::~C_RenderStage ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_RenderStage::Initialize (int priority)
		{
			C_BaseClass::Initialize();

			m_Priority = priority;

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_RenderStage::Finalize() 
		{
			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderStage::SetEnabled (bool enabled)
		{
			m_IsEnabled = enabled;
		}	

		//------------------------------------------------------------------------------------------------------------------------
			
		bool C_RenderStage::IsEnabled () const
		{
			return m_IsEnabled;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER
