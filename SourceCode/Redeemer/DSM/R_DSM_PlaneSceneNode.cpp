/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_DSM_PlaneSceneNode.cpp
**		Represents textured plane
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_DSM_PlaneSceneNode.h"
#include "..\\Render\\R_Render_RenderDevice.h"
#include "..\\Render\\R_Render_VertexBufferManager.h"
#include "..\\Render\\R_Render_VertexBuffer.h"
#include "..\\Render\\R_Render_RenderEntity.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace DSM
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_DSMPlaneSceneNode::C_DSMPlaneSceneNode (CORE::C_Plane3D<float> plane, float size, CORE::C_Vector2D<float> textureMultipier) :
			m_Plane (plane),
			m_Size (size),
			m_TextureMultipier (textureMultipier),
			m_VertexBuffer (NULL)
		{
			CreateData();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMPlaneSceneNode::~C_DSMPlaneSceneNode ()
		{
			C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetVertexBufferManager()->RemoveVertexBuffer(m_VertexBuffer);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMPlaneSceneNode::CreateData ()
		{
			C_DSMSceneNodeEntity::Initialize();

			RENDER::C_RenderDevice* device = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice();

			if (!device)
				return;

			m_VertexBuffer = device->GetVertexBufferManager()->CreateVertexBuffer(4, RENDER::FVF_VERTEX3TNC, false);

			m_VertexBuffer->SetPrimitivesCount(2);
			m_VertexBuffer->SetPrimitiveType(RENDER::RPT_TriangleList);

			m_VertexBuffer->AttachIndexBuffer(6);

			RENDER::S_VertexNormalColorTexture *vertices;

			//	Set vertices
			m_VertexBuffer->Lock(4, (void**)&vertices);

			CORE::C_Vector3D<float> p0 = m_Plane.GetMemberPoint ();
			CORE::C_Vector3D<float> Normal = m_Plane.Normal;

			vertices[0].Position.X = p0.X - (m_Size * 0.5f);
			vertices[0].Position.Y = p0.Y;
			vertices[0].Position.Z = p0.Z - (m_Size * 0.5f);
			vertices[0].Color = 0xffffffff;
			vertices[0].Normal = Normal;
			vertices[0].TexCoord = CORE::C_Vector2D<float>(0.0f, m_TextureMultipier.Height * 1.0f);

			vertices[1].Position.X = p0.X + (m_Size * 0.5f);
			vertices[1].Position.Y = p0.Y;
			vertices[1].Position.Z = p0.Z - (m_Size * 0.5f);
			vertices[1].Color = 0xffffffff;
			vertices[1].Normal = Normal;
			vertices[1].TexCoord = CORE::C_Vector2D<float>(m_TextureMultipier.Width * 1.0f, m_TextureMultipier.Height * 1.0f);

			vertices[2].Position.X = p0.X - (m_Size * 0.5f);
			vertices[2].Position.Y = p0.Y;
			vertices[2].Position.Z = p0.Z + (m_Size * 0.5f);
			vertices[2].Color = 0xffffffff;
			vertices[2].Normal = Normal;
			vertices[2].TexCoord = CORE::C_Vector2D<float>(0.0f, 0.0f);

			vertices[3].Position.X = p0.X + (m_Size * 0.5f);
			vertices[3].Position.Y = p0.Y;
			vertices[3].Position.Z = p0.Z + (m_Size * 0.5f);
			vertices[3].Color = 0xffffffff;
			vertices[3].Normal = Normal;
			vertices[3].TexCoord = CORE::C_Vector2D<float>(m_TextureMultipier.Width * 1.0f, 0.0f);			   

			m_VertexBuffer->Unlock ();
						
			//	Set indices
			short indices[6] = {0, 1, 2, 3, 2, 1};

			m_VertexBuffer->GetIndexBuffer()->SetIndexData(indices, 6);
						
			//	Finally, create and add valid entity

			RENDER::C_RenderEntity* entity = new RENDER::C_RenderEntity (L"GeometryData");

			entity->SetVertexBuffer(m_VertexBuffer);

			AddRenderEntity(entity->GetName(), entity);
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace DSM
}	//	namespace REDEEMER
