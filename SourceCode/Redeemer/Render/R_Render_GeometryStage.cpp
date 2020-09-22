/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_GeometryStage.cpp
**		Stage for processing geometry data
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_GeometryStage.h"
#include "R_Render_RenderEntity.h"
#include "R_Render_Material.h"
#include "R_Render_RenderTechnique.h"
#include "R_Render_RenderPass.h"

namespace REDEEMER
{
	namespace RENDER
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_GeometryStage::C_GeometryStage () 
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GeometryStage::~C_GeometryStage ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_GeometryStage::Initialize (int priority)
		{
			C_RenderStage::Initialize(priority);

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_GeometryStage::Finalize ()
		{

			return C_RenderStage::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GeometryStage::AddToRenderQueue (C_RenderEntity* entity, C_Material* material)
		{
			std::wstring name = material->GetName();

			if (name == L"DeferredMaterial")
				m_PreRenderQueue[name].AddRenderEntity(entity);
			else
				m_RenderQueue[name].AddRenderEntity (entity);

			m_Materials[name] = material;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GeometryStage::Update ()
		{
			C_Material* preMaterial = NULL;

			for (std::map<std::wstring, C_RenderQueue>::iterator it = m_PreRenderQueue.begin(); it != m_PreRenderQueue.end(); ++it)
			{
				preMaterial = m_Materials [it->first];

				preMaterial->BindForRendering ();

				C_RenderTechnique* preTechnique = preMaterial->GetActiveRenderTechnique();

				for (unsigned int i = 0; i < preTechnique->GetRenderPassesCount(); ++i)
				{
					C_RenderPass* pass = preTechnique->GetRenderPass (i);

					pass->BindForRendering ();

					// G-Buffer rendering
					C_Material* renderMaterial = NULL;
					for (std::map<std::wstring, C_RenderQueue>::iterator it2 = m_RenderQueue.begin(); it2 != m_RenderQueue.end(); ++it2)
					{
						renderMaterial = m_Materials[it2->first];

						renderMaterial->BindForRendering ();

						it2->second.ProcessQueue(renderMaterial);

						renderMaterial->UnbindFromRendering();
					}

					pass->UnbindFromRendering ();
				}

				preMaterial->UnbindFromRendering ();
			}

			m_RenderQueue.clear();
			m_PreRenderQueue.clear();
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER
