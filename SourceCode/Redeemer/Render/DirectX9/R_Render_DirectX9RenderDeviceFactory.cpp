/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_DirectX9RenderDeviceFactory.cpp
**		DirectX9 Render Device Factory implementation
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_DirectX9RenderDeviceFactory.h"
#include "R_Render_DirectX9VertexFormat.h"
#include "R_Render_DirectX9VertexBuffer.h"
#include "R_Render_DirectX9IndexBuffer.h"
#include "R_Render_DirectX9Texture.h"
#include "R_Render_DirectX9Shader.h"
#include "R_Render_DirectX9Material.h"
#include "R_Render_DirectX9TextureSampler.h"
#include "..\\..\\FileSystem\\R_FileSystem_DataFile.h"
#include "..\\..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{													
		//------------------------------------------------------------------------------------------------------------------------
		C_DirectX9RenderDeviceFactory::C_DirectX9RenderDeviceFactory ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DirectX9RenderDeviceFactory::~C_DirectX9RenderDeviceFactory ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_DirectX9RenderDeviceFactory::Initialize ()
		{
			C_RenderDeviceFactory::Initialize();

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_DirectX9RenderDeviceFactory::Finalize ()
		{
			return C_RenderDeviceFactory::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_VertexBuffer* C_DirectX9RenderDeviceFactory::CreateVertexBuffer (unsigned long verticesCount, ERendererFVFFormat vertexFormat, bool isDynamic)
		{
			C_VertexBuffer* vertexBuffer = new C_DirectX9VertexBuffer (verticesCount, vertexFormat, isDynamic);

			return vertexBuffer;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_IndexBuffer* C_DirectX9RenderDeviceFactory::CreateIndexBuffer (unsigned long indicesCount, DWORD indexSize, bool isDynamic)
		{
			C_IndexBuffer* indexBuffer = new C_DirectX9IndexBuffer (indicesCount, indexSize, isDynamic);

			return indexBuffer;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_VertexFormat* C_DirectX9RenderDeviceFactory::CreateVertexFormat (S_VertexFormatDescription* descriptions, unsigned int count)
		{
			C_VertexFormat* format = new C_DirectX9VertexFormat ();

			format->AddVertexFormatDescriptions(descriptions, count);

			return format;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Texture* C_DirectX9RenderDeviceFactory::CreateTexture (std::wstring name, ERendererTextureFormat format, ERendererTextureType type, 
			ERendererTextureUsage usage, unsigned int width, unsigned int height, unsigned int depth)
		{
			C_Texture* texture = new C_DirectX9Texture (name, format, type, usage, width, height, depth);

			return texture;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Texture* C_DirectX9RenderDeviceFactory::CreateTexture (FILESYSTEM::IDataFile* file, std::wstring name, ERendererTextureFormat format, 
			ERendererTextureType type, ERendererTextureUsage usage)
		{
			C_Texture* texture = new C_DirectX9Texture (file, name, format, type, usage);

			return texture;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_TextureSampler* C_DirectX9RenderDeviceFactory::CreateTextureSampler ()
		{
			C_DirectX9TextureSampler* sampler = new C_DirectX9TextureSampler ();

			return sampler;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Shader* C_DirectX9RenderDeviceFactory::CreateShaderFromFile (FILESYSTEM::IDataFile* file, const char* mainFunction, ERendererShaderType type)
		{
			C_DirectX9Shader* shader = new C_DirectX9Shader (type);

			unsigned int size = file->GetSize();
			void* buffer = new char[size + 1];
			file->Read((char*)buffer, size);

			shader->CreateFromData ((const char*)buffer, size, mainFunction);

			REDEEMER_SAFE_DELETE_ARRAY (buffer);

			return shader;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Shader* C_DirectX9RenderDeviceFactory::CreateShaderFromData (char* shaderData, char* mainFunction, ERendererShaderType type)
		{
			C_DirectX9Shader* shader = new C_DirectX9Shader (type);

			shader->CreateFromData (shaderData, strlen (shaderData), mainFunction);

			return shader;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Material* C_DirectX9RenderDeviceFactory::CreateMaterial (const std::wstring name)
		{
			C_DirectX9Material* material = new C_DirectX9Material (name);

			return material;
		}

		//------------------------------------------------------------------------------------------------------------------------


	}	//	namespace RENDER
}	//	namespace REDEEMER
