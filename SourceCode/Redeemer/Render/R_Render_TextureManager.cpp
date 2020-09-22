/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_TextureManager.cpp
**		Texture Manager holds a collection of textures
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_TextureManager.h"
#include "R_Render_RenderDevice.h"
#include "R_Render_RenderDeviceFactory.h"
#include "R_Render_TextureSampler.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_TextureManager::C_TextureManager ()
		{
			m_Textures.clear();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_TextureManager::~C_TextureManager ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_TextureManager::Initialize ()
		{
			C_BaseClass::Initialize();

			m_Textures.clear();

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_TextureManager::Finalize ()
		{
			for (std::map<std::wstring, C_Texture*>::iterator it = m_Textures.begin(); it != m_Textures.end(); ++ it)
			{
				REDEEMER_SAFE_DELETE (it->second);
			}

			for (std::vector<C_TextureSampler*>::iterator it = m_TextureSamplers.begin(); it != m_TextureSamplers.end(); ++ it)
			{
				REDEEMER_SAFE_DELETE (*it);
			}

			m_Textures.clear();
			m_TextureSamplers.clear();

			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Texture* C_TextureManager::CreateTexture (std::wstring name, ERendererTextureFormat format, ERendererTextureType type, 
			ERendererTextureUsage usage, unsigned int width, unsigned int height, unsigned int depth)
		{
			if (GetTexture(name) != NULL)
				return GetTexture(name);

			C_RenderDevice* device = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice();

			C_Texture* texture = device->GetRenderDeviceFactory()->CreateTexture(name, format, type, usage, width, height, depth);

			if (texture != NULL)
			{
				m_Textures[name] = texture;
			}

			return texture;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Texture* C_TextureManager::CreateTexture (FILESYSTEM::IDataFile* file, std::wstring name, ERendererTextureFormat format, 
			ERendererTextureType type, ERendererTextureUsage usage)
		{
			if (GetTexture(name) != NULL)
				return GetTexture(name);

			C_RenderDevice* device = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice();

			C_Texture* texture = device->GetRenderDeviceFactory()->CreateTexture(file, name, format, type, usage);

			if (texture != NULL)
			{
				m_Textures[name] = texture;
			}

			return texture;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_TextureManager::AddTexture (const std::wstring& name, C_Texture* texture)
		{
			// The material already exists, so return
			if (m_Textures.find (name) != m_Textures.end())
				return;

			m_Textures [name] = texture;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Texture* C_TextureManager::GetTexture (const std::wstring& name)
		{
			if (m_Textures.find(name) == m_Textures.end())
				return NULL;

			return m_Textures[name];
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_TextureManager::RemoveTexture (const std::wstring& name)
		{
			for (std::map<std::wstring, C_Texture*>::iterator it = m_Textures.begin(); it != m_Textures.end(); ++ it)
			{
				if (it->second->GetName() == name)
				{
					REDEEMER_SAFE_DELETE (it->second);
					m_Textures.erase (name);

					break;
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_TextureManager::RemoveTexture (C_Texture* texture)
		{
			for (std::map<std::wstring, C_Texture*>::iterator it = m_Textures.begin(); it != m_Textures.end(); ++ it)
			{
				if (it->second == texture)
				{
					std::wstring name = texture->GetName();

					REDEEMER_SAFE_DELETE (it->second);
					m_Textures.erase (name);

					break;
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned int C_TextureManager::GetTextureCount () const
		{
			return m_Textures.size();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_TextureSampler* C_TextureManager::CreateTextureSampler ()
		{						 
			C_RenderDevice* device = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice();
			
			C_TextureSampler* sampler = device->GetRenderDeviceFactory()->CreateTextureSampler();

			if (sampler != NULL)
			{
				m_TextureSamplers.push_back (sampler);
			}

			return sampler;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_TextureManager::AddTextureSampler (C_TextureSampler* sampler)
		{
			// The material already exists, so return
			for (unsigned int i = 0; i < m_TextureSamplers.size(); ++ i)
				if (m_TextureSamplers[i] == sampler)
					return;

			m_TextureSamplers.push_back(sampler);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_TextureManager::RemoveTextureSampler (int index)
		{
			int i = 0;

			// Material does not exist, so exit
			for (std::vector<C_TextureSampler*>::iterator it = m_TextureSamplers.begin(); it != m_TextureSamplers.end(); ++ it)
			{
				if (i == index)
				{
					REDEEMER_SAFE_DELETE (m_TextureSamplers [i]);
					m_TextureSamplers.erase (it);

					break;
				}

				i ++;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_TextureSampler* C_TextureManager::GetTextureSampler (int index)
		{
			return m_TextureSamplers[index];
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned int C_TextureManager::GetTextureSamplersCount () const
		{
			return m_TextureSamplers.size();
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER
