/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_DirectX9IndexBuffer.cpp
**		DirectX 9 implementation of Index Buffer - an ordered collection of vertices' indexes
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_DirectX9IndexBuffer.h"
#include "R_Render_DirectX9RenderDevice.h"
#include "..\\..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{	
		//------------------------------------------------------------------------------------------------------------------------
		C_DirectX9IndexBuffer::C_DirectX9IndexBuffer (unsigned long indicesCount, DWORD indexSize, bool isDynamic)
		{
			C_DirectX9RenderDevice* device = static_cast<C_DirectX9RenderDevice*>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice());
			
			m_IndicesCount = indicesCount;
			m_Size = indexSize;
			m_IsDynamic = isDynamic;
			m_IsLocked = false;
			m_D3DIndexBuffer = NULL;

			// Find out which format has to be used (16- or 32-bit)
			D3DFORMAT indexFmt;

			if (indicesCount > 65535)
			{
				indexFmt = D3DFMT_INDEX32;
			}
			else
			{
				indexFmt = D3DFMT_INDEX16;
				m_Size = sizeof (unsigned short);
			}

			DWORD usage = 0;
			D3DPOOL pool = D3DPOOL_MANAGED;

			if (m_IsDynamic)
			{
				usage = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;
				pool = D3DPOOL_DEFAULT;
			}			

			// Create index buffer
			device->GetDirectX9Device()->CreateIndexBuffer (m_IndicesCount * m_Size, usage, indexFmt, pool, &m_D3DIndexBuffer, NULL);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DirectX9IndexBuffer::~C_DirectX9IndexBuffer ()
		{
			REDEEMER_SAFE_RELEASE (m_D3DIndexBuffer);
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_DirectX9IndexBuffer::Lock (unsigned int count, void** indices)
		{
			DWORD dwLock = 0;

			if (m_IsDynamic)
				dwLock = D3DLOCK_DISCARD;

			m_D3DIndexBuffer->Lock (0, count * m_Size, reinterpret_cast<void**>(indices), dwLock);

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_DirectX9IndexBuffer::Unlock ()
		{
			m_D3DIndexBuffer->Unlock ();

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_DirectX9IndexBuffer::IsLocked ()
		{
			return m_IsLocked;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_DirectX9IndexBuffer::BindForRendering ()
		{
			C_DirectX9RenderDevice* device = static_cast<C_DirectX9RenderDevice*>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice());
	
			device->GetDirectX9Device()->SetIndices (m_D3DIndexBuffer);

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9IndexBuffer::SetIndexData (void* data, unsigned int count)
		{
			void* indices = NULL;

			Lock (count, &indices);

			if (data)
				memcpy (indices, data, count * m_Size);

			Unlock();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9IndexBuffer::GetIndexData (void* data, unsigned int count)
		{
			void* indices;
			DWORD dwLock = 0;

			if (m_IsDynamic)
				dwLock = D3DLOCK_DISCARD;

			m_D3DIndexBuffer->Lock (0, count * m_Size, reinterpret_cast<void**>(&indices), dwLock);

			if (indices)
				memcpy (data, indices, count * m_Size);

			m_D3DIndexBuffer->Unlock();	
		}

		//------------------------------------------------------------------------------------------------------------------------

		long C_DirectX9IndexBuffer::GetIndicesCount () const
		{
			return m_IndicesCount;
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned int C_DirectX9IndexBuffer::GetIndexSize () const
		{
			return m_Size;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER
