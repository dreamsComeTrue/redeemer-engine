/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Physics_WorldSceneManager.cpp
**		Physics world scene manager holds a collection of physics scene managers
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Physics_WorldSceneManager.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace PHYSICS
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_PhysicsWorldSceneManager::C_PhysicsWorldSceneManager () 
		{
			m_SceneManagers.clear();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PhysicsWorldSceneManager::~C_PhysicsWorldSceneManager ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_PhysicsWorldSceneManager::Initialize ()
		{
			C_BaseClass::Initialize();

			m_SceneManagers.clear();

			REDEEMER_LOG << LOG_INFO << L"Physics World Scene Manager initialized!" << LOG_ENDMESSAGE;

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_PhysicsWorldSceneManager::Finalize ()
		{
			for (std::map<std::wstring, C_PhysicsSceneManager*>::iterator it = m_SceneManagers.begin(); it != m_SceneManagers.end(); ++ it)
			{
//				it->second->Finalize();

				REDEEMER_SAFE_DELETE (it->second);
			}

			m_SceneManagers.clear();

			REDEEMER_LOG << LOG_INFO << L"Physics World Scene Manager finalized!" << LOG_ENDMESSAGE;

			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PhysicsSceneManager* C_PhysicsWorldSceneManager::CreateSceneManager (std::wstring name, CORE::C_Vector3D<float> gravityVector)
		{
			C_PhysicsSceneManager* sceneManager = NULL;//new C_PhysicsSceneManager (name, gravityVector);

//			sceneManager->Initialize();

			if (sceneManager != NULL)
			{
				m_SceneManagers[name] = sceneManager;
			}

			return sceneManager;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsWorldSceneManager::AddSceneManager (const std::wstring& name, C_PhysicsSceneManager* sceneManager)
		{
			// The material already exists, so return
			if (m_SceneManagers.find (name) != m_SceneManagers.end())
				return;

			m_SceneManagers [name] = sceneManager;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsWorldSceneManager::RemoveSceneManager (const std::wstring& name)
		{
			// Material does not exist, so exit
			if (m_SceneManagers.find (name) == m_SceneManagers.end())
				return;

			REDEEMER_SAFE_DELETE (m_SceneManagers [name]);
			m_SceneManagers.erase (name);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PhysicsSceneManager* C_PhysicsWorldSceneManager::GetSceneManager (const std::wstring& name)
		{
			if (m_SceneManagers.find (name) == m_SceneManagers.end())
			{
				return NULL;
			}

			return m_SceneManagers[name];
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned int C_PhysicsWorldSceneManager::GetSceneManagersCount () const
		{
			return m_SceneManagers.size();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsWorldSceneManager::UpdateSceneManagers ()
		{
			for (std::map<std::wstring, C_PhysicsSceneManager*>::iterator it = m_SceneManagers.begin(); it != m_SceneManagers.end(); ++ it)
			{
//				if (it->second->IsEnabled ())
	//				it->second->Update ();
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace PHYSICS
}	//	namespace REDEEMER
