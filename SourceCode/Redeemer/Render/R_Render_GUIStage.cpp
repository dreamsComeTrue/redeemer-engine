/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_GUIStage.cpp
**		Stage for processing GUI controls
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_GUIStage.h"
#include "R_Render_RenderDevice.h"
#include "..\\GUI\\R_GUI_GUISystem.h"
#include "..\\Gui\\R_GUI_GUIFontManager.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{			
		//------------------------------------------------------------------------------------------------------------------------
		C_GUIStage::C_GUIStage ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUIStage::~C_GUIStage ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_GUIStage::Initialize (int priority)
		{
			C_RenderStage::Initialize(priority);

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_GUIStage::Finalize ()
		{
			return C_RenderStage::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIStage::Update ()
		{
			C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->SetRenderMode(RENDER::RM_2D);
																					  
			C_RedeemerEngine::GetSingleton().GetGUIManager()->GetGUISystem()->Render();

			C_RedeemerEngine::GetSingleton().GetGUIManager()->GetGUISystem()->GetFontManager()->RenderFonts();

			//	Switch back to 3D
			C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->SetRenderMode(RENDER::RM_3D);	  
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER
