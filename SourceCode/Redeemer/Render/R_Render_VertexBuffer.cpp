/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_VertexBuffer.cpp
**		Generic implementation of Vertex Buffer - a source of vertex data that is sent to the GPU.
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_VertexBuffer.h"
#include "R_Render_RenderDeviceFactory.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{	
		//------------------------------------------------------------------------------------------------------------------------
		C_VertexBuffer::C_VertexBuffer () :
			m_IndexBuffer (NULL),
			m_PrimitiveType (RPT_TriangleStrip),
			m_PrimitivesCount (0)
		{
			//	EMPTY
		}			

		//------------------------------------------------------------------------------------------------------------------------

		C_VertexBuffer::~C_VertexBuffer ()
		{
			//	We don't need to remove index buffer, as it is a VertexBufferManager's job 
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_VertexBuffer::SetPrimitivesCount (unsigned int count)
		{
			m_PrimitivesCount = count;
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned int C_VertexBuffer::GetPrimitivesCount () const
		{
			return m_PrimitivesCount;
		}

		//------------------------------------------------------------------------------------------------------------------------

		long C_VertexBuffer::GetVerticesCount () const
		{
			return m_VerticesCount;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_VertexBuffer::SetPrimitiveType (ERendererPrimitiveType type)
		{
			m_PrimitiveType = type;
		}

		//------------------------------------------------------------------------------------------------------------------------

		ERendererPrimitiveType C_VertexBuffer::GetPrimitiveType () const
		{
			return m_PrimitiveType;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_IndexBuffer* C_VertexBuffer::GetIndexBuffer ()
		{
			return m_IndexBuffer;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_VertexBuffer::AttachIndexBuffer (unsigned long indicesCount, DWORD indexSize, bool isDynamic)
		{
			C_VertexBufferManager* manager = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetVertexBufferManager ();

			if (!manager)
				return;

			if (!m_IndexBuffer)
			{
				m_IndexBuffer = manager->CreateIndexBuffer(indicesCount, indexSize, isDynamic);
			}
			else
			{
				if (indicesCount != m_IndexBuffer->GetIndicesCount())
				{
					REDEEMER_SAFE_DELETE (m_IndexBuffer);

					m_IndexBuffer = manager->CreateIndexBuffer(indicesCount, indexSize, isDynamic);
				}
			}	
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER
