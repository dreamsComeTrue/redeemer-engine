/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_RenderPass.cpp
**		Render pass represents settings for material (vertex + pixel shaders and textures)
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_RenderPass.h"
#include "R_Render_RenderDevice.h"
#include "R_Render_TextureSampler.h"
#include "R_Render_Shader.h"
#include "R_Render_ShaderEntity.h"
#include "DirectX9\\R_Render_DirectX9TextureSampler.h"
#include "..\\FileSystem\\R_FileSystem_PhysicalFileANSI.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_RenderPass::C_RenderPass ()  :
			m_Name (L""),
			m_VertexShader (NULL),
			m_PixelShader (NULL)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_RenderPass::C_RenderPass (const std::wstring& name) :
			m_Name (name),
			m_VertexShader (NULL),
			m_PixelShader (NULL)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_RenderPass::C_RenderPass (const C_RenderPass& pass)
		{
			C_RenderDevice* device = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice();

			m_Name = pass.m_Name;
			m_VertexShader = pass.m_VertexShader ? new C_ShaderEntity (*pass.m_VertexShader) : NULL;
			m_PixelShader = pass.m_PixelShader ? new C_ShaderEntity (*pass.m_PixelShader) : NULL;

			for (unsigned int i = 0; i < pass.m_TextureSamplers.size(); ++ i)
			{
				C_TextureSampler* sampler = device->GetTextureManager()->CreateTextureSampler();

				*sampler = *pass.m_TextureSamplers[i];

				m_TextureSamplers.push_back(sampler);		   
			}

			for (unsigned int i = 0; i < pass.m_RenderTargets.size(); ++ i)
			{
				C_TextureSampler* sampler = device->GetTextureManager()->CreateTextureSampler();

				*sampler = *pass.m_RenderTargets[i];

				m_RenderTargets.push_back(sampler);			   
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_RenderPass::~C_RenderPass ()
		{
			m_TextureSamplers.clear();
			m_RenderTargets.clear();

			REDEEMER_SAFE_DELETE (m_VertexShader);
			REDEEMER_SAFE_DELETE (m_PixelShader);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderPass::SetName (std::wstring& name)
		{
			m_Name = name;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::wstring C_RenderPass::GetName () const
		{
			return m_Name;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderPass::AddTextureSampler (C_TextureSampler* sampler)
		{
			m_TextureSamplers.push_back (sampler);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_TextureSampler& C_RenderPass::GetTextureSampler (unsigned int index)
		{
			if (index > m_TextureSamplers.size () - 1)
			{
				index = m_TextureSamplers.size () - 1;
			}

			return *m_TextureSamplers[index];
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderPass::AddRenderTarget (C_TextureSampler* sampler)
		{
			m_RenderTargets.push_back (sampler);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_TextureSampler& C_RenderPass::GetRenderTarget (unsigned int index)
		{
			if (index > m_RenderTargets.size () - 1)
			{
				index = m_RenderTargets.size () - 1;
			}

			return *m_RenderTargets[index];
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderPass::SetVertexShader (C_ShaderEntity* shader)
		{
			m_VertexShader = shader;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_ShaderEntity* C_RenderPass::GetVertexShader ()
		{
			return m_VertexShader;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderPass::SetPixelShader (C_ShaderEntity* shader)
		{
			m_PixelShader = shader;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_ShaderEntity* C_RenderPass::GetPixelShader ()
		{
			return m_PixelShader;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderPass::UpdateShaders ()
		{
			if (m_VertexShader)
				m_VertexShader->UpdateVariables ();

			if (m_PixelShader)
				m_PixelShader->UpdateVariables ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderPass::BindForRendering ()
		{
			if (m_VertexShader)
				m_VertexShader->BindForRendering();

			if (m_PixelShader)
				m_PixelShader->BindForRendering();

			//	Set all textures from pass as current textures stages
			for (unsigned int i = 0; i < m_TextureSamplers.size (); i ++)
				m_TextureSamplers[i]->BindForRendering ();

			for (unsigned int i = 0; i < m_RenderTargets.size (); i ++)
				m_RenderTargets[i]->BindForRendering();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderPass::UnbindFromRendering ()
		{
			if (m_VertexShader)
				m_VertexShader->UnbindFromRendering();

			if (m_PixelShader)
				m_PixelShader->UnbindFromRendering();

			//	Set all textures from material as current textures stages
			for (unsigned int i = 0; i < m_TextureSamplers.size (); i ++)
				m_TextureSamplers[i]->UnbindFromRendering ();

			for (unsigned int i = 0; i < m_RenderTargets.size (); i ++)
				m_RenderTargets[i]->UnbindFromRendering ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderPass::Parse (FILESYSTEM::C_XMLDocument::C_XMLNode* xmlFile)
		{
			C_RenderDevice* device = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice();

			// Parse child nodes
			FILESYSTEM::C_XMLDocument::C_XMLNodeEnumerator node = xmlFile->GetNodeEnumerator();

			while (node.Next ())
			{
				// Load vertex shader
				if (node->GetName() == "VertexShader")
				{
					std::string file = node->FindAttribute("FileName");
					std::wstring shadFile(file.begin(), file.end());
					std::string name = node->FindAttribute("Name");
					std::wstring shadName = shadFile;
					std::string function = node->FindAttribute("EntryPoint");

					if (name.compare("") != 0)
						shadName = std::wstring(name.begin(), name.end());

					C_Shader* shader = device->GetShaderManager()->GetShader (shadName);

					if (!shader)
					{
						FILESYSTEM::C_PhysicalFileANSI shaderFile (shadFile, FILESYSTEM::FOM_Read | FILESYSTEM::FOM_Binary);
						std::wstring tmp;

						C_Shader* pVertexShader = device->GetShaderManager()->CreateShader (shadName, &shaderFile, function.c_str(), RST_VertexShader);
						
						shaderFile.Close();
					}

					m_VertexShader = new C_ShaderEntity ();
					m_VertexShader->Parse(node.Current ());
					m_VertexShader->SetShader(device->GetShaderManager()->GetShader (shadName));
				}

				// Load pixel shader
				if (node->GetName() == "PixelShader")
				{
					std::string file = node->FindAttribute ("FileName");
					std::wstring shadFile(file.begin(), file.end());
					std::string name = node->FindAttribute ("Name");
					std::wstring shadName = shadFile;
					std::string function = node->FindAttribute ("EntryPoint");

					if (name.compare("") != 0)
						shadName = std::wstring(name.begin(), name.end());

					C_Shader* shader = device->GetShaderManager()->GetShader (shadName);

					if(!shader)
					{
						FILESYSTEM::C_PhysicalFileANSI shaderFile (shadFile, FILESYSTEM::FOM_Read | FILESYSTEM::FOM_Binary);
						std::wstring tmp;

						C_Shader* pPixelShader = device->GetShaderManager()->CreateShader (shadName, &shaderFile, function.c_str(), RST_PixelShader);

						shaderFile.Close();
					}

					m_PixelShader = new C_ShaderEntity ();
					m_PixelShader->Parse(node.Current ());
					m_PixelShader->SetShader(device->GetShaderManager()->GetShader (shadName));
				}

				// Load/create texture
				if (node->GetName() == "Texture")
				{					
					C_TextureSampler* sampler =  device->GetTextureManager()->CreateTextureSampler();

					sampler->Parse(node.Current ());

					if ((!sampler->IsForceSet()) && sampler->GetTexture()->GetUsage() == RTU_RenderTarget)
						m_RenderTargets.push_back(sampler);
					else
						m_TextureSamplers.push_back(sampler);
				}
			}

		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER

