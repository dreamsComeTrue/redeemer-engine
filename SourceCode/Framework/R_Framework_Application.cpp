/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Framework_Application.cpp
**		Main application object.
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Framework_Application.h"

namespace REDEEMER
{
	namespace FRAMEWROK
	{
		//------------------------------------------------------------------------------------------------------------------------

		C_Application::C_Application () : 
			m_Engine (NULL)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Application::~C_Application ()
		{
			if (IsInitialized())
				Finalize ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_Application::Initialize ()
		{
			m_Engine = new C_RedeemerEngine ();

			return m_Engine->Initialize ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_Application::Finalize ()
		{
			return m_Engine->Finalize ();
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace FRAMEWROK
}	//	namespace REDEEMER