/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_RenderDeviceFactory.h
**		Factory is used as a abstraction layer for providing render resources, like vertex buffers, formats etc.
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_RENDER_RENDERDEVICEFACTORY_H_
#define _R_RENDER_RENDERDEVICEFACTORY_H_

#include "..\\Core\\R_Core_BaseClass.h"
#include "..\\Core\\R_Core_Singleton.h"

#include "R_Render_VertexBufferManager.h"
#include "R_Render_Shader.h"
#include "R_Render_Texture.h"

#include <Windows.h>

namespace REDEEMER
{
	namespace FILESYSTEM
	{
		class IDataFile;
	}

	namespace RENDER
	{
		class C_VertexBuffer;
		class C_IndexBuffer;
		class C_VertexFormat;
		class C_Texture;
		class C_TextureSampler;
		class C_Material;
		struct S_VertexFormatDescription;

		/*!
		**	\brief Factory is used as a abstraction layer for providing render resources, like vertex buffers, formats etc.
		*/
		class C_RenderDeviceFactory : public CORE::C_Singleton<C_RenderDeviceFactory>, public CORE::C_BaseClass
		{
		public:
			/*!	Constructor
			*/
			C_RenderDeviceFactory ();

			/*!	Destructor
			*/
			virtual ~C_RenderDeviceFactory ();
			
			/*!	\brief	Initialization code
			*/
			virtual int Initialize ();

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Creates new Index Buffer implementation
			*/
			virtual C_IndexBuffer* CreateIndexBuffer (unsigned long indicesCount, DWORD indexSize, bool isDynamic) = 0;

			/*!	Creates new Vertex Buffer implementation
			*/
			virtual C_VertexBuffer* CreateVertexBuffer (unsigned long verticesCount, ERendererFVFFormat vertexFormat, bool isDynamic) = 0;

			/*!	Creates new Vertex Format declaration
			*/
			virtual C_VertexFormat* CreateVertexFormat (S_VertexFormatDescription* descriptions, unsigned int count) = 0;

			/*!	Creates new texture
			*/
			virtual C_Texture* CreateTexture (std::wstring name = L"", ERendererTextureFormat format = RTF_A8R8G8B8, ERendererTextureType type = RTT_2D, 
				ERendererTextureUsage usage = RTU_Normal, unsigned int width = 512, unsigned int height = 512, unsigned int depth = 1) = 0;

			/*!	Creates new texture from external file
			*/
			virtual C_Texture* CreateTexture (FILESYSTEM::IDataFile* file, std::wstring name = L"", ERendererTextureFormat format = RTF_A8R8G8B8, 
				ERendererTextureType type = RTT_2D, ERendererTextureUsage usage = RTU_Normal) = 0;

			/*!	Method to create texture sampler
			*/
			virtual C_TextureSampler* CreateTextureSampler () = 0;

			/*!	Create shader from given file
			*/
			virtual C_Shader* CreateShaderFromFile (FILESYSTEM::IDataFile* file, const char* mainFunction, ERendererShaderType type) = 0;

			/*!	Create shader from data
			*/
			virtual C_Shader* CreateShaderFromData (char* shaderData, char* mainFunction, ERendererShaderType type) = 0;

			/*!	Method to create material
			*/
			virtual C_Material* CreateMaterial (const std::wstring name) = 0;
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_RENDERDEVICEFACTORY_H_
