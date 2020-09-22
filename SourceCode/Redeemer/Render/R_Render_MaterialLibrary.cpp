/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_MaterialLibrary.cpp
**		Material library holds a collection of material managers
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_MaterialLibrary.h"
#include "R_Render_MaterialManager.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_MaterialLibrary::C_MaterialLibrary ()
		{
			 m_MaterialManagers.clear();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_MaterialLibrary::~C_MaterialLibrary ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_MaterialLibrary::Initialize ()
		{
			C_BaseClass::Initialize();

			m_MaterialManagers.clear();

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_MaterialLibrary::Finalize ()
		{
			for (std::map<std::wstring, C_MaterialManager*>::iterator it = m_MaterialManagers.begin(); it != m_MaterialManagers.end(); ++ it)
			{
				it->second->Finalize();

				REDEEMER_SAFE_DELETE (it->second);
			}

			m_MaterialManagers.clear();

			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_MaterialManager* C_MaterialLibrary::CreateMaterialManager (std::wstring name)
		{
			C_MaterialManager* materialManager = new C_MaterialManager ();	 
			
			if (materialManager != NULL)
			{
				materialManager->Initialize();

				m_MaterialManagers[name] = materialManager;
			}

			return materialManager;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_MaterialLibrary::AddMaterialManager (const std::wstring& name, C_MaterialManager* materialManager)
		{
			// The material already exists, so return
			if (m_MaterialManagers.find (name) != m_MaterialManagers.end())
				return;

			m_MaterialManagers [name] = materialManager;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_MaterialManager* C_MaterialLibrary::GetMaterialManager (const std::wstring& name)
		{
			if (m_MaterialManagers.find(name) == m_MaterialManagers.end())
				return NULL;

			return m_MaterialManagers[name];
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_MaterialLibrary::RemoveMaterialManager (const std::wstring& name)
		{
			for (std::map<std::wstring, C_MaterialManager*>::iterator it = m_MaterialManagers.begin(); it != m_MaterialManagers.end(); ++ it)
			{
				if (it->first == name)
				{
					it->second->Finalize();

					REDEEMER_SAFE_DELETE (it->second);
					m_MaterialManagers.erase (name);

					break;
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_MaterialLibrary::RemoveMaterialManager (C_MaterialManager* materialManager)
		{
			for (std::map<std::wstring, C_MaterialManager*>::iterator it = m_MaterialManagers.begin(); it != m_MaterialManagers.end(); ++ it)
			{
				if (it->second == materialManager)
				{
					it->second->Finalize();
					REDEEMER_SAFE_DELETE (it->second);
					m_MaterialManagers.erase (it);

					break;
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned int C_MaterialLibrary::GetMaterialManagersCount () const
		{
			return m_MaterialManagers.size();
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER

