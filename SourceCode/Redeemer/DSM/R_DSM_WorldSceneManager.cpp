/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_DSM_WorldSceneManager.cpp
**		World scene manager holds a collection of scene managers
*/

//------------------------------------------------------------------------------------------------------------------------

#include "R_DSM_WorldSceneManager.h"
#include "R_DSM_SceneManager.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace DSM
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_DSMWorldSceneManager::C_DSMWorldSceneManager () :
			m_ActiveCamera (NULL)
		{
			m_SceneManagers.clear();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMWorldSceneManager::~C_DSMWorldSceneManager ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_DSMWorldSceneManager::Initialize ()
		{
			C_BaseClass::Initialize();

			m_SceneManagers.clear();

			REDEEMER_LOG << LOG_INFO << L"DSM World Scene Manager initialized!" << LOG_ENDMESSAGE;

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_DSMWorldSceneManager::Finalize ()
		{
			for (std::map<std::wstring, C_DSMSceneManager*>::iterator it = m_SceneManagers.begin(); it != m_SceneManagers.end(); ++ it)
			{
				it->second->Finalize();

				REDEEMER_SAFE_DELETE (it->second);
			}

			m_SceneManagers.clear();

			REDEEMER_LOG << LOG_INFO << L"DSM World Scene Manager finalized!" << LOG_ENDMESSAGE;

			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMSceneManager* C_DSMWorldSceneManager::CreateSceneManager (std::wstring name)
		{
			C_DSMSceneManager* sceneManager = new C_DSMSceneManager (name);

			sceneManager->Initialize();

			if (sceneManager != NULL)
			{
				m_SceneManagers[name] = sceneManager;
			}

			return sceneManager;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMWorldSceneManager::AddSceneManager (const std::wstring& name, C_DSMSceneManager* sceneManager)
		{
			// The material already exists, so return
			if (m_SceneManagers.find (name) != m_SceneManagers.end())
				return;

			m_SceneManagers [name] = sceneManager;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMWorldSceneManager::RemoveSceneManager (const std::wstring& name)
		{
			// Material does not exist, so exit
			if (m_SceneManagers.find (name) == m_SceneManagers.end())
				return;

			REDEEMER_SAFE_DELETE (m_SceneManagers [name]);
			m_SceneManagers.erase (name);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMSceneManager* C_DSMWorldSceneManager::GetSceneManager (const std::wstring& name)
		{
			if (m_SceneManagers.find (name) == m_SceneManagers.end())
			{
				return NULL;
			}

			return m_SceneManagers[name];
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMSceneManager* C_DSMWorldSceneManager::GetSceneManager (int index)
		{
			int counter = 0;

			for (std::map<std::wstring, C_DSMSceneManager*>::iterator it = m_SceneManagers.begin(); it != m_SceneManagers.end(); ++ it)
			{
				if (index == counter)
					return it->second;

				counter ++;
			}

			return NULL;
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned int C_DSMWorldSceneManager::GetSceneManagersCount () const
		{
			return m_SceneManagers.size();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMWorldSceneManager::UpdateSceneManagers ()
		{
			for (std::map<std::wstring, C_DSMSceneManager*>::iterator it = m_SceneManagers.begin(); it != m_SceneManagers.end(); ++ it)
			{
				if (it->second->IsEnabled ())
					it->second->Update ();
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMWorldSceneManager::SetActiveCamera (C_DSMCameraSceneNode* camera)
		{
			m_ActiveCamera = camera;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMCameraSceneNode* C_DSMWorldSceneManager::GetActiveCamera ()
		{
			return m_ActiveCamera;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace DSM
}	//	namespace REDEEMER
