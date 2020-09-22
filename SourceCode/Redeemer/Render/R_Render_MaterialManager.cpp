/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_MaterialManager.cpp
**		Material Manager holds a collection of materials
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_MaterialManager.h"
#include "R_Render_RenderDevice.h"
#include "R_Render_RenderDeviceFactory.h"
#include "R_Render_Material.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_MaterialManager::C_MaterialManager ()
		{
			m_Materials.clear();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_MaterialManager::~C_MaterialManager ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_MaterialManager::Initialize ()
		{
			C_BaseClass::Initialize();

			m_Materials.clear();

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_MaterialManager::Finalize ()
		{
			for (std::map<std::wstring, C_Material*>::iterator it = m_Materials.begin(); it != m_Materials.end(); ++ it)
			{
				REDEEMER_SAFE_DELETE (it->second);
			}

			m_Materials.clear();

			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Material* C_MaterialManager::CreateMaterial (std::wstring name)
		{
			C_RenderDevice* device = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice();

			C_Material* material = device->GetRenderDeviceFactory()->CreateMaterial(name);

			if (material != NULL)
			{
				m_Materials[name] = material;
			}

			return material;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_MaterialManager::AddMaterial (const std::wstring& name, C_Material* material)
		{
			// The material already exists, so return
			if (m_Materials.find (name) != m_Materials.end())
				return;

			m_Materials [name] = material;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Material* C_MaterialManager::GetMaterial (const std::wstring& name)
		{
			if (m_Materials.find(name) == m_Materials.end())
				return NULL;

			return m_Materials[name];
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_MaterialManager::RemoveMaterial (const std::wstring& name)
		{
			for (std::map<std::wstring, C_Material*>::iterator it = m_Materials.begin(); it != m_Materials.end(); ++ it)
			{
				if (it->second->GetName() == name)
				{
					REDEEMER_SAFE_DELETE (it->second);
					m_Materials.erase (name);

					break;
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_MaterialManager::RemoveMaterial (C_Material* material)
		{
			for (std::map<std::wstring, C_Material*>::iterator it = m_Materials.begin(); it != m_Materials.end(); ++ it)
			{
				if (it->second == material)
				{
					std::wstring name = material->GetName();

					REDEEMER_SAFE_DELETE (it->second);
					m_Materials.erase (name);

					break;
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned int C_MaterialManager::GetMaterialsCount () const
		{
			return m_Materials.size();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_MaterialManager::Parse (FILESYSTEM::C_XMLDocument* xmlFile)
		{
			// Parse child nodes
			FILESYSTEM::C_XMLDocument::C_XMLNodeEnumerator node = xmlFile->GetRootNode()->GetNodeEnumerator();
	
			while (node.Next ())
			{
				// Load material
				if (node->GetName() == "Material")
				{
					std::string matName = node->FindAttribute("Name");
					std::wstring tmp (matName.begin(), matName.end());

					C_Material* tempMat = CreateMaterial (tmp);

					tempMat->Parse(node.Current ());

					AddMaterial (tempMat->GetName(), tempMat);
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER
