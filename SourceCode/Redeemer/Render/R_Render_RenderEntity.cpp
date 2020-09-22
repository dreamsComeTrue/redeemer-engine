/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_RenderEntity.cpp
**		Render entities holds material used for rendering process as well as vertex buffer containing geometrical data
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_RenderEntity.h"
#include "R_Render_RenderDevice.h"
#include "..\\DSM\\R_DSM_SceneNodeEntity.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{	
		//------------------------------------------------------------------------------------------------------------------------
		C_RenderEntity::C_RenderEntity (std::wstring name) :
			m_Name (name),
			m_VertexBuffer (NULL),
			m_Material (NULL),
			m_IsEnabled (true),
			m_SceneNodeEntity (NULL),
			OnBeforeRender(NULL),
			OnAfterRender(NULL)
		{
			m_RenderDevice = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice();

			OnBeforeRender = new RENDER_EVENT ();
			OnAfterRender = new RENDER_EVENT ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_RenderEntity::~C_RenderEntity ()
		{
			REDEEMER_SAFE_DELETE (OnBeforeRender);
			REDEEMER_SAFE_DELETE (OnAfterRender);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderEntity::SetName (const std::wstring& name)
		{
			m_Name = name;
		}

		//------------------------------------------------------------------------------------------------------------------------

		const std::wstring& C_RenderEntity::GetName ()
		{
			return m_Name;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderEntity::SetMaterial (C_Material* material)
		{
			m_Material = material;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Material* C_RenderEntity::GetMaterial ()
		{
			return m_Material;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderEntity::SetVertexBuffer (C_VertexBuffer* vertexBuffer)
		{
			m_VertexBuffer = vertexBuffer;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_VertexBuffer* C_RenderEntity::GetVertexBuffer ()
		{
			return m_VertexBuffer;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderEntity::SetEnabled (bool enable)
		{
			m_IsEnabled = enable;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_RenderEntity::IsEnabled() const
		{
			return m_IsEnabled;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderEntity::Render ()
		{
			if (!m_IsEnabled || !m_Material || !m_SceneNodeEntity || !m_SceneNodeEntity->IsEnabled())
				return;

			if (*OnBeforeRender)
				OnBeforeRender->operator () (this);

			m_RenderDevice->SetRenderMatrix(RENDER::RMM_World, m_SceneNodeEntity->GetTransformationMatrix());
			m_RenderDevice->DrawPrimitives (m_VertexBuffer);

			if (*OnAfterRender)
				OnAfterRender->operator () (this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderEntity::SetSceneNodeEntity (DSM::C_DSMSceneNodeEntity* entity)
		{
			m_SceneNodeEntity = entity;
		}

		//------------------------------------------------------------------------------------------------------------------------

		DSM::C_DSMSceneNodeEntity* C_RenderEntity::GetSceneNodeEntity ()
		{
			return m_SceneNodeEntity;
		}


	}	//	namespace RENDER
}	//	namespace REDEEMER
