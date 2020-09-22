/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	S_GameApplication.cpp
**		Main application object.
*/

//------------------------------------------------------------------------------------------------------------------------
#include "S_GameApplication.h"

namespace REDEEMER
{
	namespace SHAKE
	{
		//------------------------------------------------------------------------------------------------------------------------
		S_GameApplication::S_GameApplication ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		S_GameApplication::~S_GameApplication ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int S_GameApplication::Initialize ()
		{
			C_Application::Initialize ();		

			REDEEMER_LOG << LOG_INFO << "Creating main window..." << LOG_ENDLINE;

			m_GameWindow = new S_GameWindow ();	

			m_GameWindow->Initialize ();

			return 0;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int S_GameApplication::Finalize ()
		{	
			m_GameWindow->Finalize();

			return C_Application::Finalize ();
		}

		//------------------------------------------------------------------------------------------------------------------------
	}	//	namespace SHAKE
}	//	namespace REDEEMER