/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_PostProcessStage.cpp
**		This stage can add various effects on output rendered image
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_PostProcessStage.h"
#include "R_Render_Material.h"
#include "R_Render_Sprite.h"
#include "R_Render_RenderDevice.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_PostProcessStage::C_PostProcessStage () :
			m_ScreenSprite (NULL)
		{
			m_Materials.clear();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PostProcessStage::~C_PostProcessStage ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_PostProcessStage::Initialize (int priority)
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

		int C_PostProcessStage::Finalize ()
		{
			m_Materials.clear();

			REDEEMER_SAFE_DELETE (m_ScreenSprite);

			return C_RenderStage::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PostProcessStage::AddToRenderQueue (unsigned int priority, C_Material* material)
		{
			if (m_Materials.find(priority) != m_Materials.end())
			{
				REDEEMER_LOG << LOG_ERROR << L"RENDER: There is already material set with priority: '" << priority << "'!" << LOG_ENDMESSAGE;

				return;
			}

			m_Materials[priority] = material;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PostProcessStage::Update ()
		{
			C_RenderDevice* device = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice();
			
		//	device->SetSeparateAlphaBlendEnabled(true);

		//	device->SetSeparateAlphaSourceBlendingMode (BM_SrcAlpha);
		//	device->SetSeparateAlphaDestinationBlendingMode (BM_InvSrcAlpha);

			device->SetRenderMode(RENDER::RM_2D);
						
			//	Render all materials
			for (std::map<unsigned int, C_Material*>::iterator it = m_Materials.begin(); it != m_Materials.end (); ++it)
			{					
				m_ScreenSprite->SetMaterial((*it).second);
				m_ScreenSprite->Render();
			}	

			//	Switch back to 3D
			C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->SetRenderMode(RENDER::RM_3D);	

			device->SetSeparateAlphaBlendEnabled(false);
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER
