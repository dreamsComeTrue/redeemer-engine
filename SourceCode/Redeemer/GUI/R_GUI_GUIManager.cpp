/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_GUI_GUIManager.cpp
**		Graphical User Interface manager
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_GUI_GUIManager.h"
#include "R_GUI_GUISystem.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace GUI
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_GUIManager::C_GUIManager () :
			m_GUISystem (NULL)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUIManager::~C_GUIManager ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_GUIManager::Initialize ()
		{
			C_BaseClass::Initialize();

			m_GUISystem = new C_GUISystem ();

			m_GUISystem->Initialize();

			REDEEMER_LOG << LOG_INFO << L"GUI Manager initialized!" << LOG_ENDMESSAGE;

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_GUIManager::Finalize ()
		{
			m_GUISystem->Finalize();

			REDEEMER_SAFE_DELETE (m_GUISystem);

			REDEEMER_LOG << LOG_INFO << L"GUI Manager finalized!" << LOG_ENDMESSAGE;

			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUISystem* C_GUIManager::GetGUISystem ()
		{
			return m_GUISystem;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace GUI
}	//	namespace REDEEMER
