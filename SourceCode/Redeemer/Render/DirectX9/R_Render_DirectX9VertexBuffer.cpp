/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_DirectX9VertexBuffer.cpp
**		DirectX 9 implementation of Vertex Buffer - a source of vertex data that is sent to the GPU.
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_DirectX9VertexBuffer.h"
#include "R_Render_DirectX9RenderDevice.h"
#include "..\\R_Render_VertexFormat.h"
#include "..\\..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{	
#define VERTEX_SIZE_POSITION	sizeof(float) * 3
#define VERTEX_SIZE_NORMAL		sizeof(float) * 3
#define VERTEX_SIZE_COLOR		sizeof (DWORD)
#define VERTEX_SIZE_TEXTURE		sizeof (float) * 2

		//------------------------------------------------------------------------------------------------------------------------
		C_DirectX9VertexBuffer::C_DirectX9VertexBuffer (unsigned long verticesCount, ERendererFVFFormat vertexFormat, bool isDynamic)
		{
			C_DirectX9RenderDevice* device = static_cast<C_DirectX9RenderDevice*>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice());

			m_D3DVertexBuffer = NULL;
			m_FVFSize = 0;
			m_VerticesCount = verticesCount;
			m_VertexFormat = vertexFormat;	 

			switch (vertexFormat)
			{
			case FVF_VERTEX2TC:// Vertex2 + Texture coord + Color.
				{
					m_FVFSize += VERTEX_SIZE_POSITION + VERTEX_SIZE_TEXTURE + VERTEX_SIZE_COLOR;
				}

				break;

			case FVF_VERTEX3:// Vertex3.
				{
					m_FVFSize += VERTEX_SIZE_POSITION;
				}

				break;

			case FVF_VERTEX3C:// Vertex3 + Color.
				{
					m_FVFSize += VERTEX_SIZE_POSITION + VERTEX_SIZE_COLOR;
				}

				break;		  

			case FVF_VERTEX3T:// Vertex3 + Texture coord.
				{
					m_FVFSize += VERTEX_SIZE_POSITION + VERTEX_SIZE_TEXTURE;
				}

				break;

			case FVF_VERTEX3TC:// Vertex3 + Texture coord + Color.
				{
					m_FVFSize += VERTEX_SIZE_POSITION + VERTEX_SIZE_TEXTURE + VERTEX_SIZE_COLOR;
				}

				break;

			case FVF_VERTEX3TN:// Vertex3 + Texture coord + Normal vector.
				{
					m_FVFSize += VERTEX_SIZE_POSITION + VERTEX_SIZE_TEXTURE + VERTEX_SIZE_NORMAL;
				}

				break;

			case FVF_VERTEX3TNC:// Vertex3 + Texture coord + Normal vector + Color.
				{
					m_FVFSize += VERTEX_SIZE_POSITION + VERTEX_SIZE_TEXTURE + VERTEX_SIZE_NORMAL + VERTEX_SIZE_COLOR;
				}

				break;
			}

			m_IsDynamic = isDynamic;
			m_IsLocked = false;
			m_Data = NULL;

			HRESULT result;

			if (m_IsDynamic)
			{
				result = device->GetDirectX9Device()->CreateVertexBuffer (m_FVFSize * verticesCount, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, 
					&m_D3DVertexBuffer, NULL);
			}
			else
			{
				result = device->GetDirectX9Device()->CreateVertexBuffer (m_FVFSize * verticesCount, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &m_D3DVertexBuffer, NULL);
			}

			if (FAILED (result))
				throw "C_DirectXVertexBuffer::C_DirectXVertexBuffer -> creation of Vertex Buffer failed!";
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DirectX9VertexBuffer::~C_DirectX9VertexBuffer ()
		{
			REDEEMER_SAFE_RELEASE (m_D3DVertexBuffer);
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_DirectX9VertexBuffer::Lock (unsigned int count, void** vertices)
		{
			// Lock DirectX 9.0 Vertex Buffer
			DWORD dwLock = 0;

			if (m_IsDynamic)
				dwLock = D3DLOCK_DISCARD;

			HRESULT result = m_D3DVertexBuffer->Lock (0, count * m_FVFSize, vertices, dwLock);

			if (FAILED(result))
			{
				REDEEMER_LOG << LOG_ERROR << "C_DirectXVertexBuffer::Lock -> Can not lock D3D Vertex Buffer!" << LOG_ENDMESSAGE;

				return false;
			}

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_DirectX9VertexBuffer::Unlock ()
		{
			HRESULT result = m_D3DVertexBuffer->Unlock();

			if (FAILED(result))
				REDEEMER_LOG << LOG_ERROR << "C_DirectXVertexBuffer::Unlock -> Can not unlock D3D Vertex Buffer!" << LOG_ENDMESSAGE;

			m_IsLocked = false;

			return (!FAILED(result));
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_DirectX9VertexBuffer::IsLocked ()
		{
			return m_IsLocked;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_DirectX9VertexBuffer::BindForRendering ()
		{
			C_DirectX9RenderDevice* device = static_cast<C_DirectX9RenderDevice*>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice());

			HRESULT result = device->GetDirectX9Device()->SetStreamSource (0, m_D3DVertexBuffer, 0, m_FVFSize);

			if (FAILED(result))
				return false;

			if (m_IndexBuffer)
				m_IndexBuffer->BindForRendering();

			C_VertexFormat* format = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice ()->GetVertexBufferManager()->GetVertexFormat(m_VertexFormat);
			
			if (format)
				format->BindForRendering();

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9VertexBuffer::SetVertexData (void* data, unsigned int count)
		{
			void* vertices = NULL;

			Lock (count, &vertices);

			if (data)
				memcpy(vertices, data, count * m_FVFSize);

			Unlock();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9VertexBuffer::GetVertexData (void* data, unsigned int count)
		{
			void* vertices;
			DWORD dwLock = 0;

			if (m_IsDynamic)
				dwLock = D3DLOCK_DISCARD;

			m_D3DVertexBuffer->Lock (0, count * m_FVFSize, &vertices, 0);

			if (vertices)
				memcpy (data, vertices, count * m_FVFSize);

			m_D3DVertexBuffer->Unlock();
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER
