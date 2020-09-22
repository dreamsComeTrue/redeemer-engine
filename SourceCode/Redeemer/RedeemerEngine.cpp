/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	RedeemerEngine.cpp
**		Main class of the engine - links all submodules
*/

//------------------------------------------------------------------------------------------------------------------------
#include "RedeemerEngine.h"
#include "RedeemerEngineDefines.h"
#include "Render\\R_Render_RenderDevice.h"

namespace REDEEMER
{
	//------------------------------------------------------------------------------------------------------------------------
	C_RedeemerEngine::C_RedeemerEngine () :
		m_EngineLogger (new FILESYSTEM::C_Logger ()),
		m_System (new CORE::C_System ()),
		m_SystemInfo (new CORE::C_SystemInfo ()),
		m_Timer (new CORE::C_Timer ()),
		m_AudioManager (new AUDIO::C_AudioManager ()),
		m_FileSystemManager (new FILESYSTEM::C_FileSystemManager ()),
		m_InputManager (new INPUT::C_InputManager ()),
		m_ScriptManager (new SCRIPT::C_ScriptManager ()),
		m_NetworkManager (new NETWORK::C_NetworkManager ()),
		m_RenderManager (new RENDER::C_RenderManager ()),
		m_DSMWorldSceneManager (new DSM::C_DSMWorldSceneManager ()),
		m_GUIManager (new GUI::C_GUIManager ()),
		m_PhysicsWorldSceneManager (new PHYSICS::C_PhysicsWorldSceneManager ())
	{
		//	EMPTY
	}

	//------------------------------------------------------------------------------------------------------------------------

	C_RedeemerEngine::~C_RedeemerEngine ()
	{
		if (IsInitialized())
			Finalize();
	}

	//------------------------------------------------------------------------------------------------------------------------

	int C_RedeemerEngine::Initialize ()
	{
		m_Timer->StartTimer();

		if (m_EngineLogger)
			m_EngineLogger->CreateLogFile(L"EngineLog.html");

		if (m_SystemInfo)
			m_SystemInfo->LogSystemInfo ();

		if (m_AudioManager)
			m_AudioManager->Initialize();

		if (m_FileSystemManager)
			m_FileSystemManager->Initialize ();

		if (m_ScriptManager)
			m_ScriptManager->Initialize();

		if (m_NetworkManager)
			m_NetworkManager->Initialize();

		if (m_DSMWorldSceneManager)
			m_DSMWorldSceneManager->Initialize();

		if (m_PhysicsWorldSceneManager)
			m_PhysicsWorldSceneManager->Initialize();

		return 0;
	}

	//------------------------------------------------------------------------------------------------------------------------

	void C_RedeemerEngine::CreateDependantSystems ()
	{
		if (m_InputManager && (!m_InputManager->IsInitialized()))
		{
			INPUT::DeviceParameter parameterList;

			parameterList.insert(std::make_pair(INPUT::IDT_Mouse, INPUT::IDP_DISCL_FOREGROUND));
			parameterList.insert(std::make_pair(INPUT::IDT_Mouse, INPUT::IDP_DISCL_NONEXCLUSIVE));

			m_InputManager->Initialize (parameterList, m_System->GetMainWindow()->GetHWND());
		}

		if (m_RenderManager && (!m_RenderManager->IsInitialized()))
			m_RenderManager->Initialize();

		if (m_GUIManager && (!m_GUIManager->IsInitialized()))
			m_GUIManager->Initialize();
	}

	//------------------------------------------------------------------------------------------------------------------------

	int C_RedeemerEngine::Finalize ()
	{
		if (m_PhysicsWorldSceneManager)
			m_PhysicsWorldSceneManager->Finalize();

		if (m_DSMWorldSceneManager)
			m_DSMWorldSceneManager->Finalize();

		if (m_GUIManager)
			m_GUIManager->Finalize();

		if (m_RenderManager)
			m_RenderManager->Finalize ();

		if (m_InputManager)
			m_InputManager->Finalize();

		if (m_NetworkManager)
			m_NetworkManager->Finalize();

		if (m_ScriptManager)
			m_ScriptManager->Finalize();

		if (m_FileSystemManager)
			m_FileSystemManager->Finalize ();

		if (m_AudioManager)
			m_AudioManager->Finalize();

		return 0;
	}

	//------------------------------------------------------------------------------------------------------------------------

	FILESYSTEM::C_Logger* C_RedeemerEngine::GetLogger ()
	{
		return m_EngineLogger;
	}

	//------------------------------------------------------------------------------------------------------------------------

	CORE::C_System* C_RedeemerEngine::GetSystem ()
	{
		return m_System;
	}

	//------------------------------------------------------------------------------------------------------------------------

	CORE::C_SystemInfo* C_RedeemerEngine::GetSystemInfo ()
	{
		return m_SystemInfo;
	}

	//------------------------------------------------------------------------------------------------------------------------

	CORE::C_Timer* C_RedeemerEngine::GetTimer ()
	{
		return m_Timer;
	}				   

	//------------------------------------------------------------------------------------------------------------------------

	FILESYSTEM::C_FileSystemManager* C_RedeemerEngine::GetFileSystemManager ()
	{
		return m_FileSystemManager;
	}

	//------------------------------------------------------------------------------------------------------------------------

	AUDIO::C_AudioManager* C_RedeemerEngine::GetAudioManager ()
	{
		return m_AudioManager;
	}

	//------------------------------------------------------------------------------------------------------------------------

	INPUT::C_InputManager* C_RedeemerEngine::GetInputManager ()
	{
		return m_InputManager;
	}

	//------------------------------------------------------------------------------------------------------------------------

	SCRIPT::C_ScriptManager* C_RedeemerEngine::GetScriptManager()
	{
		return m_ScriptManager;
	}

	//------------------------------------------------------------------------------------------------------------------------
	
	NETWORK::C_NetworkManager* C_RedeemerEngine::GetNetworkManager ()
	{
		return m_NetworkManager;
	}

	//------------------------------------------------------------------------------------------------------------------------

	RENDER::C_RenderManager* C_RedeemerEngine::GetRenderManager ()
	{
		return m_RenderManager;
	}

	//------------------------------------------------------------------------------------------------------------------------

	DSM::C_DSMWorldSceneManager* C_RedeemerEngine::GetDSMWorldSceneManager ()
	{
		return m_DSMWorldSceneManager;
	}

	//------------------------------------------------------------------------------------------------------------------------

	GUI::C_GUIManager* C_RedeemerEngine::GetGUIManager ()
	{
		return m_GUIManager;
	}

	//------------------------------------------------------------------------------------------------------------------------

	PHYSICS::C_PhysicsWorldSceneManager* C_RedeemerEngine::GetPhysicsWorldSceneManager ()
	{
		return m_PhysicsWorldSceneManager;
	}

	//------------------------------------------------------------------------------------------------------------------------

	void C_RedeemerEngine::UpdateSystems()
	{
		//	Update system time
		m_Timer->DoTimer();

		//	Get input data
		m_InputManager->RetrieveData();

		//	Update all physical calculations
		m_PhysicsWorldSceneManager->UpdateSceneManagers();

		//	Update all scenes
		m_DSMWorldSceneManager->UpdateSceneManagers();

		//	Update GUI controls
		m_GUIManager->GetGUISystem()->UpdateControls ();

		//	Finally - render data queues
		m_RenderManager->GetRenderDevice()->Render();
	}

	//------------------------------------------------------------------------------------------------------------------------

}	//	namespace REDEEMER
