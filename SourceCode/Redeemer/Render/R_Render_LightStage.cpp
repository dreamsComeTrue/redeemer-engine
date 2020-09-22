/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_LightStage.cpp
**		Light stage uses data collected from geometry stage to lit the surfaces
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_LightStage.h"
#include "R_Render_Material.h"
#include "R_Render_Sprite.h"
#include "R_Render_RenderDevice.h"
#include "..\\DSM\\R_DSM_WorldSceneManager.h"
#include "..\\DSM\\R_DSM_SceneManager.h"
#include "..\\DSM\\R_DSM_LightSceneNode.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_LightStage::C_LightStage () : 
			m_ScreenSprite (NULL)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_LightStage::~C_LightStage ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_LightStage::Initialize (int priority)
		{
			C_RenderStage::Initialize(priority);

			CORE::C_Rectangle<float> dimensions;

			C_RenderDevice* device = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice();

			dimensions.UpperLeftCorner.X = 0;
			dimensions.UpperLeftCorner.Y = 0;
			dimensions.LowerRightCorner.X = (float)device->GetRenderVideoMode().ScreenResolution.Width;
			dimensions.LowerRightCorner.Y = (float)device->GetRenderVideoMode().ScreenResolution.Height;

			m_ScreenSprite = new C_Sprite (dimensions);

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_LightStage::Finalize ()
		{
			REDEEMER_SAFE_DELETE (m_ScreenSprite);

			return C_RenderStage::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_LightStage::Update ()
		{
			C_RenderDevice* device = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice();

			device->SetSeparateAlphaBlendEnabled(true);

			device->SetSeparateAlphaSourceBlendingMode (BM_SrcAlpha);
			device->SetSeparateAlphaDestinationBlendingMode (BM_InvSrcAlpha);

			device->SetRenderMode(RENDER::RM_2D);

			DSM::C_DSMWorldSceneManager* worldSceneManager = C_RedeemerEngine::GetSingleton().GetDSMWorldSceneManager(); 

			for (unsigned int i = 0; i < worldSceneManager->GetSceneManagersCount(); ++i)
			{
				DSM::C_DSMSceneManager* sceneManager = worldSceneManager->GetSceneManager(i);

				if (sceneManager == NULL || !sceneManager->IsEnabled())
					continue;

				std::vector<DSM::C_LightSceneNode*>* lights = sceneManager->GetSceneLights();

				if (lights->empty())
					continue;

				//	Render all lights
				for (std::vector<DSM::C_LightSceneNode*>::iterator it = lights->begin(); it != lights->end(); ++ it)
				{					
					m_ScreenSprite->SetMaterial((*it)->GetMaterial());
					m_ScreenSprite->Render();
				}	
			}
  
			//	Switch back to 3D
			C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->SetRenderMode(RENDER::RM_3D);	

			device->SetSeparateAlphaBlendEnabled(false);
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER
