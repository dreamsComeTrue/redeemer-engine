/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_RenderManager.cpp
**		Engine's render system communication layer
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_RenderManager.h"
#include "R_Render_RenderDevice.h"
#include "DirectX9\\R_Render_DirectX9RenderDevice.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_RenderManager::C_RenderManager () :
			m_RenderDevice (NULL)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_RenderManager::~C_RenderManager ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------
		int C_RenderManager::Initialize ()
		{
			C_BaseClass::Initialize();

			//	Currently, there is only DirectX9 device
			m_RenderDevice = new C_DirectX9RenderDevice ();

			m_RenderDevice->Initialize();

			REDEEMER_LOG << LOG_INFO << L"Render Manager initialized!" << LOG_ENDMESSAGE;

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_RenderManager::Finalize ()
		{
			m_RenderDevice->Finalize();

			REDEEMER_SAFE_DELETE (m_RenderDevice);

			REDEEMER_LOG << LOG_INFO << L"Render Manager finalized!" << LOG_ENDMESSAGE;

			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_RenderDevice* C_RenderManager::GetRenderDevice() 
		{
			return m_RenderDevice;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER
