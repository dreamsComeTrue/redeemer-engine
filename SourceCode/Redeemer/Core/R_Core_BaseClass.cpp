/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_BaseClass.cpp
**		Base class is the basic parent for almost all types in engine
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Core_BaseClass.h"

namespace REDEEMER
{
	namespace CORE
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_BaseClass::C_BaseClass ()
		{
			//By default the object ins't initialized
			m_IsInitialized = false;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_BaseClass::~C_BaseClass ()
		{
			//	If object still initialized
			if (m_IsInitialized)
			{
				//	Finalize the object
				Finalize ();
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_BaseClass::Initialize ()
		{
			//	If objects is already initialized
			if (m_IsInitialized)
				//	Return fail code
				return RESULT_FAIL;
			else
				//	Store object initialization
				m_IsInitialized = true;

			//	Return value
			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_BaseClass::Finalize ()
		{
			//	If the object isn't initialized
			if (!m_IsInitialized)
				//	Return fail code
				return RESULT_FAIL;
			else
				//	Store object finalization
				m_IsInitialized = false;

			//	Return value
			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_BaseClass::IsInitialized () const
		{
			return m_IsInitialized;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace CORE
}	//	namespace REDEEMER
