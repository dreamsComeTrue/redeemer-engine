/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_DirectX9RenderDeviceFactory.h
**		DirectX9 Render Device Factory implementation
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_RENDER_DIRECTX9RENDERDEVICEFACTORY_H_
#define _R_RENDER_DIRECTX9RENDERDEVICEFACTORY_H_

#include "..\\R_Render_RenderDeviceFactory.h"

namespace REDEEMER
{											 
	namespace RENDER
	{
		/*!
		**	\brief DirectX9 Render Device Factory implementation
		*/
		class C_DirectX9RenderDeviceFactory : public C_RenderDeviceFactory
		{
		public:
			/*!	Constructor
			*/
			C_DirectX9RenderDeviceFactory ();

			/*!	Destructor
			*/
			virtual ~C_DirectX9RenderDeviceFactory ();
			
			/*!	\brief	Initialization code
			*/
			virtual int Initialize ();

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Creates new Vertex Buffer implementation
			*/
			virtual C_VertexBuffer* CreateVertexBuffer (unsigned long verticesCount, ERendererFVFFormat vertexFormat, bool isDynamic);

			/*!	Creates new Index Buffer implementation
			*/
			virtual C_IndexBuffer* CreateIndexBuffer (unsigned long indicesCount, DWORD indexSize, bool isDynamic);

			/*!	Creates new Vertex Format declaration
			*/
			virtual C_VertexFormat* CreateVertexFormat (S_VertexFormatDescription* descriptions, unsigned int count);

			/*!	Creates new texture
			*/
			virtual C_Texture* CreateTexture (std::wstring name = L"", ERendererTextureFormat format = RTF_A8R8G8B8, ERendererTextureType type = RTT_2D, 
				ERendererTextureUsage usage = RTU_Normal, unsigned int width = 512, unsigned int height = 512, unsigned int depth = 1);

			/*!	Creates new texture from external file
			*/
			virtual C_Texture* CreateTexture (FILESYSTEM::IDataFile* file, std::wstring name = L"", ERendererTextureFormat format = RTF_A8R8G8B8, 
				ERendererTextureType type = RTT_2D, ERendererTextureUsage usage = RTU_Normal);

			/*!	Method to create texture sampler
			*/
			virtual C_TextureSampler* CreateTextureSampler ();

			/*!	Create shader from given file
			*/
			virtual C_Shader* CreateShaderFromFile (FILESYSTEM::IDataFile* file, const char* mainFunction, ERendererShaderType type);

			/*!	Create shader from data
			*/
			virtual C_Shader* CreateShaderFromData (char* shaderData, char* mainFunction, ERendererShaderType type);

			/*!	Method to create material
			*/
			virtual C_Material* CreateMaterial (const std::wstring name);
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_DIRECTX9RENDERDEVICEFACTORY_H_
