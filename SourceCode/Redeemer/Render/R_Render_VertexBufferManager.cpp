/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_VertexBufferManager.cpp
**		VB Manager keeps tracks about all created Vertex Buffers
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_VertexBufferManager.h"
#include "R_Render_VertexFormat.h"
#include "R_Render_VertexBuffer.h"
#include "R_Render_IndexBuffer.h"
#include "R_Render_RenderDevice.h"
#include "R_Render_RenderDeviceFactory.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{	
		//------------------------------------------------------------------------------------------------------------------------
		C_VertexBufferManager::C_VertexBufferManager () :
			m_DeviceFactory (NULL)
		{
			m_VertexBuffers.clear();
			m_IndexBuffers.clear();
			m_VertexFormats.clear();

			m_VertexBufferIndex = m_IndexBufferIndex = 0;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_VertexBufferManager::~C_VertexBufferManager ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_VertexBufferManager::Initialize ()
		{
			C_BaseClass::Initialize();

			m_DeviceFactory = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetRenderDeviceFactory ();

			CreateVertexFormatDeclarations();

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_VertexBufferManager::Finalize ()
		{
			if(!m_VertexBuffers.empty())
			{
				for (std::map<unsigned long, C_VertexBuffer*>::iterator it = m_VertexBuffers.begin(); it != m_VertexBuffers.end(); ++ it)
					REDEEMER_SAFE_DELETE (it->second);

				m_VertexBuffers.clear();
			}

			if(!m_IndexBuffers.empty())
			{
				for (std::map<unsigned long, C_IndexBuffer*>::iterator it = m_IndexBuffers.begin(); it != m_IndexBuffers.end(); ++ it)
					REDEEMER_SAFE_DELETE ((*it).second);

				m_IndexBuffers.clear();
			}

			if(!m_VertexFormats.empty())
			{
				for (std::map<unsigned long, C_VertexFormat*>::iterator it = m_VertexFormats.begin(); it != m_VertexFormats.end(); ++ it)
					REDEEMER_SAFE_DELETE (it->second);

				m_VertexFormats.clear();
			}

			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_VertexBuffer* C_VertexBufferManager::CreateVertexBuffer (unsigned long verticesCount, ERendererFVFFormat vertexFormat, bool isDynamic)
		{
			C_VertexBuffer* vertexBuffer = m_DeviceFactory->CreateVertexBuffer (verticesCount, vertexFormat, isDynamic);

			m_VertexBuffers[m_VertexBufferIndex ++] = vertexBuffer;

			return vertexBuffer;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_VertexBufferManager::RemoveVertexBuffer (C_VertexBuffer* vertexBuffer)
		{
			for (std::map<unsigned long, C_VertexBuffer*>::iterator it = m_VertexBuffers.begin(); it != m_VertexBuffers.end(); ++ it)
			{
				if (vertexBuffer == (*it).second)
				{
					REDEEMER_SAFE_DELETE (it->second);
					m_VertexBuffers.erase(it);

					break;
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_IndexBuffer* C_VertexBufferManager::CreateIndexBuffer (unsigned long indicesCount, DWORD indexSize, bool isDynamic)
		{
			C_IndexBuffer* indexBuffer = m_DeviceFactory->CreateIndexBuffer (indicesCount, indexSize, isDynamic);

			m_IndexBuffers[m_IndexBufferIndex ++] = indexBuffer;

			return indexBuffer;

		}

		//------------------------------------------------------------------------------------------------------------------------

		C_VertexFormat* C_VertexBufferManager::CreateVertexFormat (unsigned int type, S_VertexFormatDescription* descriptions, unsigned int count)
		{				
			if (m_VertexFormats.find(type) != m_VertexFormats.end())
				return m_VertexFormats[type];

			C_VertexFormat* format = m_DeviceFactory->CreateVertexFormat(descriptions, count);

			m_VertexFormats[type] = format;

			return format;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_VertexBufferManager::CreateVertexFormatDeclarations ()
		{
			// Vertex2 + Texture coord + Color.
			S_VertexFormatDescription elementsVertex2TC[3] =
			{
				{0, 0,  0, VET_Posiition, 3},
				{0, 12, 0, VET_Color, 4},
				{0, 16, 0, VET_TexCoord, 2}
			};

			// Vertex3.
			S_VertexFormatDescription elementsVertex3[3] =
			{
				{0, 0,  0, VET_Posiition, 3},
			};

			// Vertex3 + Color.
			S_VertexFormatDescription elementsVertex3C[3] =
			{
				{0, 0,  0, VET_Posiition, 3},
				{0, 12, 0, VET_Color, 4},
			};

			// Vertex3 + Texture coord.
			S_VertexFormatDescription elementsVertex3T[3] =
			{
				{0, 0,  0, VET_Posiition, 3},
				{0, 12, 0, VET_TexCoord, 2}
			};

			// Vertex3 + Texture coord + Color.
			S_VertexFormatDescription elementsVertex3TC[3] =
			{
				{0, 0,  0, VET_Posiition, 3},
				{0, 12, 0, VET_TexCoord, 2},
				{0, 20, 0, VET_Color, 4}
			};

			// Vertex3 + Texture coord + Normal vector.
			S_VertexFormatDescription elementsVertex3TN[3] =
			{
				{0, 0,  0, VET_Posiition, 3},
				{0, 12, 0, VET_Normal, 3},
				{0, 24, 0, VET_TexCoord, 2}
			};

			// Vertex3 + Texture coord + Normal vector + Color.
			S_VertexFormatDescription elementsVertex3TNC[4] =
			{
				{0, 0,  0, VET_Posiition, 3},
				{0, 12, 0, VET_Normal, 3},
				{0, 24, 0, VET_TexCoord, 2},
				{0, 32, 0, VET_Color, 4}
			};

			CreateVertexFormat (FVF_VERTEX2TC, elementsVertex2TC, 3);
			CreateVertexFormat (FVF_VERTEX3, elementsVertex3, 1);
			CreateVertexFormat (FVF_VERTEX3C, elementsVertex3C, 2);
			CreateVertexFormat (FVF_VERTEX3T, elementsVertex3T, 2);
			CreateVertexFormat (FVF_VERTEX3TC, elementsVertex3TC, 3);
			CreateVertexFormat (FVF_VERTEX3TN, elementsVertex3TN, 3);
			CreateVertexFormat (FVF_VERTEX3TNC, elementsVertex3TNC, 4);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_VertexFormat* C_VertexBufferManager::GetVertexFormat (unsigned int type)
		{
			if (m_VertexFormats.find(type) == m_VertexFormats.end())
				return NULL;

			return m_VertexFormats[type];
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER
