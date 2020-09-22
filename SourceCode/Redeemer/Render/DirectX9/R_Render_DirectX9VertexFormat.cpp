/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_DirectX9VertexFormat.cpp
**		DirectX 9 FVF implementation
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_DirectX9VertexFormat.h"
#include "R_Render_DirectX9RenderDevice.h"
#include "..\\..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{	
		//------------------------------------------------------------------------------------------------------------------------
		C_DirectX9VertexFormat::C_DirectX9VertexFormat ()  :
			m_VertexDeclaration (NULL)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DirectX9VertexFormat::~C_DirectX9VertexFormat ()
		{
			REDEEMER_SAFE_RELEASE (m_VertexDeclaration);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9VertexFormat::AddVertexFormatDescription (S_VertexFormatDescription& description)
		{
			C_VertexFormat::AddVertexFormatDescription(description);
			CreateFormatDeclaration();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9VertexFormat::AddVertexFormatDescriptions (S_VertexFormatDescription* descriptions, unsigned int count)
		{
			C_VertexFormat::AddVertexFormatDescriptions(descriptions, count);
			CreateFormatDeclaration();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9VertexFormat::BindForRendering ()
		{
			C_DirectX9RenderDevice* device = static_cast<C_DirectX9RenderDevice*>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice());

			HRESULT result = device->GetDirectX9Device()->SetVertexDeclaration(m_VertexDeclaration);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9VertexFormat::CreateFormatDeclaration ()
		{
			//	Remove previously created format
			if (m_VertexDeclaration)
				REDEEMER_SAFE_RELEASE (m_VertexDeclaration);

			C_DirectX9RenderDevice* device = static_cast<C_DirectX9RenderDevice*>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice());

			unsigned int vertexElementsCount = m_Description.size();
			D3DVERTEXELEMENT9* vertexElements = new D3DVERTEXELEMENT9 [vertexElementsCount + 1];

			for (unsigned int i = 0; i < vertexElementsCount; ++i)
			{
				vertexElements[i].Stream = m_Description[i].Stream;
				vertexElements[i].Method = D3DDECLMETHOD_DEFAULT;
				vertexElements[i].Offset = m_Description[i].Offset;
				vertexElements[i].UsageIndex = m_Description[i].Index;

				CORE::C_MathUtils::Clamp(m_Description[i].Size, 1, 4);

				if (m_Description[i].Size == 1)
					vertexElements[i].Type = D3DDECLTYPE_FLOAT1;

				if (m_Description[i].Size == 2)
					vertexElements[i].Type = D3DDECLTYPE_FLOAT2;

				if (m_Description[i].Size == 3)
					vertexElements[i].Type = D3DDECLTYPE_FLOAT3;

				if (m_Description[i].Size == 4)
				{
					if(m_Description[i].Type == VET_Posiition)
						vertexElements[i].Type =  D3DDECLTYPE_FLOAT4;
					else if(m_Description[i].Type == VET_Color)
						vertexElements[i].Type =  D3DDECLTYPE_D3DCOLOR;
					else
						vertexElements[i].Type =  D3DDECLTYPE_FLOAT4;
				}

				if (m_Description[i].Type == VET_Posiition)
					vertexElements[i].Usage = D3DDECLUSAGE_POSITION;
				if (m_Description[i].Type == VET_Normal)
					vertexElements[i].Usage = D3DDECLUSAGE_NORMAL;
				if (m_Description[i].Type == VET_TexCoord)
					vertexElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
				if (m_Description[i].Type == VET_Color)
					vertexElements[i].Usage = D3DDECLUSAGE_COLOR;
				if (m_Description[i].Type == VET_Binormal)
					vertexElements[i].Usage = D3DDECLUSAGE_BINORMAL;
				if (m_Description[i].Type == VET_Tangent)
					vertexElements[i].Usage = D3DDECLUSAGE_TANGENT;
				if (m_Description[i].Type == VET_Fog)
					vertexElements[i].Usage = D3DDECLUSAGE_FOG;
				if (m_Description[i].Type == VET_Depth)
					vertexElements[i].Usage = D3DDECLUSAGE_DEPTH;
			}

			//	We need to add an empty element
			vertexElements[vertexElementsCount].Stream= 0xFF;
			vertexElements[vertexElementsCount].Offset= 0;
			vertexElements[vertexElementsCount].Type = D3DDECLTYPE_UNUSED;
			vertexElements[vertexElementsCount].Method= 0;
			vertexElements[vertexElementsCount].Usage = 0;
			vertexElements[vertexElementsCount].UsageIndex = 0;

			// Create vertex declaration
			device->GetDirectX9Device()->CreateVertexDeclaration (vertexElements, &m_VertexDeclaration);


			REDEEMER_SAFE_DELETE_ARRAY (vertexElements);
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER
