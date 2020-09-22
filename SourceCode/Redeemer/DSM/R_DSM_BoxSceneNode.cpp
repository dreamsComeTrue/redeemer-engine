/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_DSM_BoxSceneNode.cpp
**		Represents standard cube
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_DSM_BoxSceneNode.h"
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
		C_DSMBoxSceneNode::C_DSMBoxSceneNode (CORE::C_AABBox3D<float> box) :
			m_Box (box),
			m_VertexBuffer (NULL)
		{
			CreateData();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMBoxSceneNode::~C_DSMBoxSceneNode ()
		{
			C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetVertexBufferManager()->RemoveVertexBuffer(m_VertexBuffer);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMBoxSceneNode::CreateData ()
		{
			C_DSMSceneNodeEntity::Initialize();

			RENDER::C_RenderDevice* device = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice();

			if (!device)
				return;

			m_VertexBuffer = device->GetVertexBufferManager()->CreateVertexBuffer(24, RENDER::FVF_VERTEX3TNC, false);

			m_VertexBuffer->SetPrimitivesCount(12);
			m_VertexBuffer->SetPrimitiveType(RENDER::RPT_TriangleList);

			m_VertexBuffer->AttachIndexBuffer(36);

			RENDER::S_VertexNormalColorTexture *vertices;

			//	Set vertices
			m_VertexBuffer->Lock(24, (void**)&vertices);

			unsigned int b = 0;

			for (unsigned int i = 0; i < 3; ++i)
			{
				vertices[b].Position = m_Box.MinEdge;
				vertices[b].Color = 0xffffffff;

				++b;
				vertices[b].Position.X = m_Box.MaxEdge.X;
				vertices[b].Position.Y = m_Box.MinEdge.Y;
				vertices[b].Position.Z = m_Box.MinEdge.Z;
				vertices[b].Color = 0xffffffff;

				++b;
				vertices[b].Position.X = m_Box.MinEdge.X;
				vertices[b].Position.Y = m_Box.MinEdge.Y;
				vertices[b].Position.Z = m_Box.MaxEdge.Z;
				vertices[b].Color = 0xffffffff;

				++b;
				vertices[b].Position.X = m_Box.MaxEdge.X;
				vertices[b].Position.Y = m_Box.MinEdge.Y;
				vertices[b].Position.Z = m_Box.MaxEdge.Z;
				vertices[b].Color = 0xffffffff;

				++b;
				vertices[b].Position.X = m_Box.MinEdge.X;
				vertices[b].Position.Y = m_Box.MaxEdge.Y;
				vertices[b].Position.Z = m_Box.MinEdge.Z;
				vertices[b].Color = 0xffffffff;

				++b;
				vertices[b].Position.X = m_Box.MaxEdge.X;
				vertices[b].Position.Y = m_Box.MaxEdge.Y;
				vertices[b].Position.Z = m_Box.MinEdge.Z;
				vertices[b].Color = 0xffffffff;

				++b;
				vertices[b].Position.X = m_Box.MinEdge.X;
				vertices[b].Position.Y = m_Box.MaxEdge.Y;
				vertices[b].Position.Z = m_Box.MaxEdge.Z;
				vertices[b].Color = 0xffffffff;

				++b;
				vertices[b].Position = m_Box.MaxEdge;
				vertices[b].Color = 0xffffffff;

				++b;
			}

			vertices[0].Normal =
				vertices[1].Normal =
				vertices[2].Normal =
				vertices[3].Normal = CORE::C_Vector3D<float>::VECTOR_NEGATIVE_UNIT_Y;

			vertices[4].Normal =
				vertices[5].Normal =
				vertices[6].Normal =
				vertices[7].Normal = CORE::C_Vector3D<float>::VECTOR_UNIT_Y;

			vertices[9].Normal  =
				vertices[11].Normal =
				vertices[13].Normal =
				vertices[15].Normal = CORE::C_Vector3D<float>::VECTOR_UNIT_X;

			vertices[8].Normal  =
				vertices[10].Normal =
				vertices[12].Normal =
				vertices[14].Normal = CORE::C_Vector3D<float>::VECTOR_NEGATIVE_UNIT_X;

			vertices[16].Normal =
				vertices[17].Normal =
				vertices[20].Normal =
				vertices[21].Normal = CORE::C_Vector3D<float>::VECTOR_NEGATIVE_UNIT_Z;

			vertices[18].Normal =
				vertices[19].Normal =
				vertices[22].Normal =
				vertices[23].Normal = CORE::C_Vector3D<float>::VECTOR_UNIT_Z;

			vertices[0].TexCoord =
				vertices[4].TexCoord =
				vertices[9].TexCoord =
				vertices[8].TexCoord =
				vertices[16].TexCoord=
				vertices[18].TexCoord= CORE::C_Vector2D<float>(1.0f, 1.0f);

			vertices[1].TexCoord =
				vertices[5].TexCoord =
				vertices[11].TexCoord=
				vertices[10].TexCoord=
				vertices[17].TexCoord=
				vertices[19].TexCoord= CORE::C_Vector2D<float>(0.0f, 1.0f);

			vertices[2].TexCoord =
				vertices[6].TexCoord =
				vertices[13].TexCoord=
				vertices[12].TexCoord=
				vertices[20].TexCoord=
				vertices[22].TexCoord= CORE::C_Vector2D<float>(1.0f, 0.0f);

			vertices[3].TexCoord =
				vertices[7].TexCoord =
				vertices[15].TexCoord=
				vertices[14].TexCoord=
				vertices[21].TexCoord=
				vertices[23].TexCoord= CORE::C_Vector2D<float>(0.0f, 0.0f);

			m_VertexBuffer->Unlock ();

			// Set indices
			short indices[36] = {3, 1, 0, 0, 2, 3,
								 4, 5, 7, 7, 6, 4,
								 9, 11, 15, 15, 13, 9,
								 14, 10, 8, 8, 12, 14,
								 16, 17, 21, 21, 20, 16,
								 23, 19, 18, 18, 22, 23};

			m_VertexBuffer->GetIndexBuffer()->SetIndexData(indices, 36);

			//	Finally, create and add valid entity

			RENDER::C_RenderEntity* entity = new RENDER::C_RenderEntity (L"GeometryData");

			entity->SetVertexBuffer(m_VertexBuffer);

			AddRenderEntity(entity->GetName(), entity);
		}

	}	//	namespace DSM
}	//	namespace REDEEMER
