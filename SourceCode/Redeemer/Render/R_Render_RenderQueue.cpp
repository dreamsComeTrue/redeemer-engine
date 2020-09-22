/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_RenderQueue.cpp
**		Queue holds a collection of RenderEntities ready to be processed
*/

//------------------------------------------------------------------------------------------------------------------------

#include "R_Render_RenderQueue.h"
#include "R_Render_RenderEntity.h"
#include "R_Render_Material.h"
#include "R_Render_RenderTechnique.h"
#include "R_Render_RenderPass.h"
#include "R_Render_RenderDevice.h"
#include "..\\DSM\\R_DSM_SceneNodeEntity.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_RenderQueue::C_RenderQueue ()
		{
			m_RenderDevice = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice();

			m_Entities.clear();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_RenderQueue::~C_RenderQueue ()
		{
			m_Entities.clear();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderQueue::AddRenderEntity (C_RenderEntity* renderEntity)
		{
			m_Entities.push_back(renderEntity);
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::vector<C_RenderEntity*>* C_RenderQueue::GetRenderEntities ()
		{
			return &m_Entities;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderQueue::ProcessQueue (C_Material* material)
		{
			C_RenderTechnique* technique = material->GetActiveRenderTechnique();

			if (technique)
			{
				//	Render all passes
				for (unsigned int i = 0; i < technique->GetRenderPassesCount (); ++ i)
				{
					C_RenderPass* pass = technique->GetRenderPass(i);
						
					pass->BindForRendering();

					for (unsigned int i = 0; i < m_Entities.size(); ++ i)
					{
						m_RenderDevice->SetRenderMatrix(RENDER::RMM_World, (*m_Entities[i]).GetSceneNodeEntity()->GetTransformationMatrix());

						pass->UpdateShaders();

						m_Entities[i]->Render();
					}	

					pass->UnbindFromRendering();
				}	
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER
