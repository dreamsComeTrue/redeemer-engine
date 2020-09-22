/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_DSM_SceneNodeEntity.cpp
**		Scene nodes entities are derived from regular scene nodes and also contain RenderEntities data
*/

//------------------------------------------------------------------------------------------------------------------------

#include "R_DSM_SceneNodeEntity.h"
#include "..\\Render\\R_Render_RenderDevice.h"
#include "..\\Render\\R_Render_RenderEntity.h"
#include "..\\Render\\R_Render_RenderPipeline.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace DSM
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_DSMSceneNodeEntity::C_DSMSceneNodeEntity ()
		{
			m_RenderEntities.clear();

			m_RenderDevice = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMSceneNodeEntity::~C_DSMSceneNodeEntity ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_DSMSceneNodeEntity::Initialize ()
		{
			C_DSMSceneNode::Initialize();

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_DSMSceneNodeEntity::Finalize ()
		{
			//	Remove all entities
			for (std::map<std::wstring, RENDER::C_RenderEntity*>::iterator it = m_RenderEntities.begin(); it != m_RenderEntities.end(); ++ it)
				REDEEMER_SAFE_DELETE ((*it).second);

			m_RenderEntities.clear();

			return C_DSMSceneNode::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMSceneNodeEntity::AddRenderEntity (const std::wstring& name, RENDER::C_RenderEntity* entity)
		{
			// The material already exists, so return
			if (m_RenderEntities.find (name) != m_RenderEntities.end())
				return;

			m_RenderEntities [name] = entity;
			m_RenderEntities [name]->SetName(name);
			m_RenderEntities [name]->SetSceneNodeEntity(this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		RENDER::C_RenderEntity* C_DSMSceneNodeEntity::GetRenderEntity (const std::wstring& name)
		{
			if (m_RenderEntities.find(name) == m_RenderEntities.end())
				return NULL;

			return m_RenderEntities[name];
		}

		//------------------------------------------------------------------------------------------------------------------------

		RENDER::C_RenderEntity* C_DSMSceneNodeEntity::GetRenderEntity (unsigned int index)
		{
			unsigned int counter = 0;

			for (std::map<std::wstring, RENDER::C_RenderEntity*>::iterator it = m_RenderEntities.begin(); it != m_RenderEntities.end(); ++ it)
			{
				if (index == counter)
					return (*it).second;

				counter ++;
			}

			return NULL;
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned int C_DSMSceneNodeEntity::GetRenderEntityCount () const
		{
			return m_RenderEntities.size();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMSceneNodeEntity::BeforeUpdate ()
		{
			C_DSMSceneNode::BeforeUpdate ();

			for (std::map<std::wstring, RENDER::C_RenderEntity*>::iterator it = m_RenderEntities.begin(); it != m_RenderEntities.end(); ++ it)
			{
				RENDER::C_Material* material = (*it).second->GetMaterial();
				if (material)
					m_RenderDevice->GetRenderPipeline().AddToRenderQueue((*it).second, material);
			}				
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace DSM
}	//	namespace REDEEMER
