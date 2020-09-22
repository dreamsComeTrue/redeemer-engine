/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_ShaderManager.cpp
**		Shader Manager holds a collection of shaders
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_ShaderManager.h"
#include "R_Render_RenderDevice.h"
#include "R_Render_RenderDeviceFactory.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_ShaderManager::C_ShaderManager ()
		{
			m_Shaders.clear();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_ShaderManager::~C_ShaderManager ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_ShaderManager::Initialize ()
		{
			C_BaseClass::Initialize();

			m_Shaders.clear();

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_ShaderManager::Finalize ()
		{
			for (std::map<std::wstring, C_Shader*>::iterator it = m_Shaders.begin(); it != m_Shaders.end(); ++ it)
			{
				REDEEMER_SAFE_DELETE (it->second);
			}

			m_Shaders.clear();

			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Shader* C_ShaderManager::CreateShader (std::wstring name, FILESYSTEM::IDataFile* file, const char* mainFunction, ERendererShaderType type)
		{
			C_RenderDevice* device = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice();

			C_Shader* shader = device->GetRenderDeviceFactory()->CreateShaderFromFile(file, mainFunction, type);

			if (shader != NULL)
			{
				m_Shaders[name] = shader;
			}

			return shader;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_ShaderManager::AddShader (const std::wstring& name, C_Shader* shader)
		{
			// The material already exists, so return
			if (m_Shaders.find (name) != m_Shaders.end())
				return;

			m_Shaders [name] = shader;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Shader* C_ShaderManager::GetShader (const std::wstring& name)
		{
			if (m_Shaders.find (name) == m_Shaders.end())
			{
				return NULL;
			}

			return m_Shaders[name];
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_ShaderManager::RemoveShader (const std::wstring& name)
		{
			// Material does not exist, so exit
			if (m_Shaders.find (name) == m_Shaders.end())
				return;

			REDEEMER_SAFE_DELETE (m_Shaders [name]);
			m_Shaders.erase (name);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_ShaderManager::RemoveShader (C_Shader* shader)
		{
			for (std::map<std::wstring, C_Shader*>::iterator it = m_Shaders.begin(); it != m_Shaders.end(); ++ it)
			{
				if (it->second == shader)
				{
					m_Shaders.erase (it);
					REDEEMER_SAFE_DELETE (it->second);

					break;
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned int C_ShaderManager::GetShadersCount () const
		{
			  return m_Shaders.size();
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER

