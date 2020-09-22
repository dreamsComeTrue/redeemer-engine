/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_DSM_SkyBoxSceneNode.cpp
**		SkyBox is a cube with 6 textures rendered inside to get a feeling of watching at sky
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_DSM_SkyBoxSceneNode.h"
#include "..\\Render\\R_Render_Material.h"
#include "..\\Render\\R_Render_RenderEntity.h"
#include "..\\Render\\R_Render_VertexBuffer.h"
#include "..\\Render\\R_Render_ShaderEntity.h"
#include "..\\Render\\R_Render_RenderTechnique.h"
#include "..\\Render\\R_Render_RenderPass.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace DSM
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_DSMSkyBoxSceneNode::C_DSMSkyBoxSceneNode (float distance) :
			m_Distance (distance)
		{
			InitData();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMSkyBoxSceneNode::~C_DSMSkyBoxSceneNode ()
		{
			for (unsigned int i = 0; i < 6; i ++)
			{
				C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetVertexBufferManager()->RemoveVertexBuffer (m_VertexBuffers[i]);
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMSkyBoxSceneNode::SetSkyMaterial (RENDER::C_Material* skyBoxMaterial)
		{
			RENDER::C_RenderEntity* entity;

			entity = GetRenderEntity (L"Geometry_UP");

			entity->SetMaterial(skyBoxMaterial);

			entity = GetRenderEntity (L"Geometry_BOTTOM");

			entity->SetMaterial(skyBoxMaterial);

			entity = GetRenderEntity (L"Geometry_FRONT");

			entity->SetMaterial(skyBoxMaterial);

			entity = GetRenderEntity (L"Geometry_BACK");

			entity->SetMaterial(skyBoxMaterial);

			entity = GetRenderEntity (L"Geometry_RIGHT");

			entity->SetMaterial(skyBoxMaterial);

			entity = GetRenderEntity (L"Geometry_LEFT");

			entity->SetMaterial(skyBoxMaterial);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMSkyBoxSceneNode::InitData ()
		{	
			C_DSMSceneNodeEntity::Initialize();

			short indices[6] = {0, 1, 2, 3, 1, 2};
			std::wstring names[6] = {L"Geometry_UP", L"Geometry_BOTTOM", L"Geometry_FRONT", L"Geometry_BACK", L"Geometry_RIGHT", L"Geometry_LEFT"};

			CORE::C_Vector3D<float> positions[6][4] = { 
				//	UP
				{CORE::C_Vector3D<float>(-m_Distance, m_Distance, m_Distance), CORE::C_Vector3D<float>(m_Distance, m_Distance, m_Distance),
				CORE::C_Vector3D<float>(-m_Distance, m_Distance, -m_Distance), CORE::C_Vector3D<float>(m_Distance, m_Distance, -m_Distance)}, 

				//	BOTTOM
				{CORE::C_Vector3D<float>(-m_Distance, -m_Distance, -m_Distance), CORE::C_Vector3D<float>(m_Distance, -m_Distance, -m_Distance), 
				CORE::C_Vector3D<float>(-m_Distance, -m_Distance, m_Distance), CORE::C_Vector3D<float>(m_Distance, -m_Distance, m_Distance)},

				//	FRONT
				{CORE::C_Vector3D<float>(-m_Distance, -m_Distance, m_Distance), CORE::C_Vector3D<float>(m_Distance, -m_Distance, m_Distance), 
				CORE::C_Vector3D<float>(-m_Distance, m_Distance, m_Distance), CORE::C_Vector3D<float>(m_Distance, m_Distance, m_Distance)},

				//	BACK
				{CORE::C_Vector3D<float>(m_Distance, -m_Distance, -m_Distance), CORE::C_Vector3D<float>(-m_Distance, -m_Distance, -m_Distance),
				CORE::C_Vector3D<float>(m_Distance, m_Distance, -m_Distance), CORE::C_Vector3D<float>(-m_Distance, m_Distance, -m_Distance)}, 

				//	RIGHT
				{CORE::C_Vector3D<float>(-m_Distance, -m_Distance, -m_Distance), CORE::C_Vector3D<float>(-m_Distance, -m_Distance, m_Distance), 
				CORE::C_Vector3D<float>(-m_Distance, m_Distance, -m_Distance), CORE::C_Vector3D<float>(-m_Distance, m_Distance, m_Distance)},

				//	LEFT
				{CORE::C_Vector3D<float>(m_Distance, -m_Distance, m_Distance), CORE::C_Vector3D<float>(m_Distance, -m_Distance, -m_Distance), 
				CORE::C_Vector3D<float>(m_Distance, m_Distance, m_Distance), CORE::C_Vector3D<float>(m_Distance, m_Distance, -m_Distance)}
			};

			CORE::C_Vector3D<float> normals[6] = {
				CORE::C_Vector3D<float>::VECTOR_NEGATIVE_UNIT_Y,
				CORE::C_Vector3D<float>::VECTOR_UNIT_Y,
				CORE::C_Vector3D<float>::VECTOR_NEGATIVE_UNIT_Z,
				CORE::C_Vector3D<float>::VECTOR_UNIT_Z,
				CORE::C_Vector3D<float>::VECTOR_NEGATIVE_UNIT_X,
				CORE::C_Vector3D<float>::VECTOR_UNIT_X
			};

			for (unsigned int i = 0; i < 6; i ++)
			{
				m_VertexBuffers[i] = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetVertexBufferManager()->CreateVertexBuffer(4, RENDER::FVF_VERTEX3TNC, false);
				m_VertexBuffers[i]->SetPrimitiveType (RENDER::RPT_TriangleList);
				m_VertexBuffers[i]->SetPrimitivesCount (2);
				m_VertexBuffers[i]->AttachIndexBuffer(6);
				m_VertexBuffers[i]->GetIndexBuffer()->SetIndexData(indices, 6);

				RENDER::S_VertexNormalColorTexture *vertices;

				m_VertexBuffers[i]->Lock (4, (void**)&vertices);

				vertices[0].Position.X = positions[i][0].X;
				vertices[0].Position.Y = positions[i][0].Y;
				vertices[0].Position.Z = positions[i][0].Z;
				vertices[0].Color = 0xffffffff;
				vertices[0].Normal = normals[i];
				vertices[0].TexCoord = CORE::C_Vector2D<float>(0.0f,  1.0f);

				vertices[1].Position.X = positions[i][1].X;
				vertices[1].Position.Y = positions[i][1].Y;
				vertices[1].Position.Z = positions[i][1].Z;
				vertices[1].Color = 0xffffffff;
				vertices[1].Normal = normals[i];
				vertices[1].TexCoord = CORE::C_Vector2D<float>(1.0f, 1.0f);

				vertices[2].Position.X = positions[i][2].X;
				vertices[2].Position.Y = positions[i][2].Y;
				vertices[2].Position.Z = positions[i][2].Z;
				vertices[2].Color = 0xffffffff;
				vertices[2].Normal = normals[i];
				vertices[2].TexCoord = CORE::C_Vector2D<float>(0.0f, 0.0f);

				vertices[3].Position.X = positions[i][3].X;
				vertices[3].Position.Y = positions[i][3].Y;
				vertices[3].Position.Z = positions[i][3].Z;
				vertices[3].Color = 0xffffffff;
				vertices[3].Normal = normals[i];
				vertices[3].TexCoord = CORE::C_Vector2D<float>(1.0f, 0.0f);

				m_VertexBuffers[i]->Unlock ();

				RENDER::C_RenderEntity* entity = new RENDER::C_RenderEntity (names[i]);

				entity->SetVertexBuffer(m_VertexBuffers[i]);
				entity->OnBeforeRender->bind(this, &C_DSMSkyBoxSceneNode::ChangeSkyBoxTexture);

				AddRenderEntity (entity->GetName(), entity);
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMSkyBoxSceneNode::ChangeSkyBoxTexture (RENDER::C_RenderEntity* entity)
		{
			std::wstring names[6] = {L"Geometry_UP", L"Geometry_BOTTOM", L"Geometry_FRONT", L"Geometry_BACK", L"Geometry_RIGHT", L"Geometry_LEFT"};

			for (unsigned int i = 0; i < 6; i ++)
			{
				if (entity->GetName() == names[i])
				{
					entity->GetMaterial()->GetActiveRenderTechnique()->GetRenderPass(0)->GetPixelShader()->GetShader()->SetData("TextureIndex", (int)i);

					break;
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace DMS
}	//	namespace REDEEMER

