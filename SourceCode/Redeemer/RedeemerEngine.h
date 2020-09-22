/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	RedeemerEngine.h
**		Main class of the engine - links all submodules
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _REDEEMERENGINE_H_
#define _REDEEMERENGINE_H_

#include "Audio\\R_Audio_AudioManager.h"
#include "Core\\R_Core_Singleton.h"
#include "Core\\R_Core_BaseClass.h"
#include "Core\\R_Core_SystemInfo.h"
#include "Core\\R_Core_System.h"
#include "Core\\R_Core_Timer.h"
#include "FileSystem\\R_FileSystem_Logger.h"
#include "FileSystem\\R_FileSystem_FileSystemManager.h"
#include "DSM\\R_DSM_WorldSceneManager.h"
#include "GUI\\R_GUI_GUIManager.h"
#include "Input\\R_Input_InputSystem.h"
#include "Input\\R_Input_InputManager.h"
#include "Network\\R_Network_NetworkManager.h"
#include "Physics\\R_Physics_WorldSceneManager.h"
#include "Render\\R_Render_RenderManager.h"
#include "Script\\R_Script_ScriptManager.h"

///	Some useful macros for releasing and deleting dynamically allocated memory.
#define REDEEMER_SAFE_RELEASE(x)		{	if(x)	{	(x)->Release();	(x) = NULL;	}	}
#define REDEEMER_SAFE_DELETE(x)			{	if(x)	{	delete (x);		(x) = NULL;	}	}
#define REDEEMER_SAFE_DELETE_ARRAY(x)	{	if(x)	{	delete[] (x);   (x) = NULL;	}	}

//	handy macro for logging purposes
#define REDEEMER_LOG (*C_RedeemerEngine::GetSingleton ().GetLogger ())

namespace REDEEMER
{
	/*!
	**	\brief A C_RedeemerEngine class contains links to all modules of the engine
	*/
	class C_RedeemerEngine : public CORE::C_BaseClass, public CORE::C_Singleton<C_RedeemerEngine>
	{
		friend class CORE::C_Window;

	public:
		///	Constructor
		C_RedeemerEngine ();

		///	Destructor
		virtual ~C_RedeemerEngine ();

		/*!	Initialization code
		*/
		virtual int Initialize ();

		/*!	Finalization code
		*/
		virtual int Finalize ();

		/*!	Returns pointer to logger object
		*/
		FILESYSTEM::C_Logger* GetLogger ();

		/*!	Returns pointer to system object
		*/
		CORE::C_System* GetSystem ();

		/*!	Returns pointer to system info object
		*/
		CORE::C_SystemInfo* GetSystemInfo ();

		/*!	Returns pointer to system timer
		*/
		CORE::C_Timer* GetTimer ();

		/*!	Returns pointer to file system manager
		*/
		FILESYSTEM::C_FileSystemManager* GetFileSystemManager ();

		/*!	Returns pointer to audio manager
		*/
		AUDIO::C_AudioManager* GetAudioManager ();

		/*!	Returns pointer to input manager
		*/
		INPUT::C_InputManager* GetInputManager ();

		/*!	Returns pointer to script manager
		*/
		SCRIPT::C_ScriptManager* GetScriptManager ();

		/*!	Returns pointer to network manager
		*/
		NETWORK::C_NetworkManager* GetNetworkManager ();

		/*!	Returns pointer to render manager
		*/
		RENDER::C_RenderManager* GetRenderManager ();

		/*!	Returns pointer to DSM World Scene Manager
		*/
		DSM::C_DSMWorldSceneManager* GetDSMWorldSceneManager ();

		/*!	Returns pointer to GUI Manager
		*/
		GUI::C_GUIManager* GetGUIManager ();

		/*!	Returns reference to physics World Scene Manager
		*/
		PHYSICS::C_PhysicsWorldSceneManager* GetPhysicsWorldSceneManager ();

		/*!	Updates all engine's systems
		*/
		void UpdateSystems ();

	private:
		/*!	Post creates systems
		*/
		void CreateDependantSystems ();

		CORE::C_SmartPointer<FILESYSTEM::C_Logger>					m_EngineLogger;				///<	Engine needs some output debug information
		CORE::C_SmartPointer<CORE::C_System>						m_System;					///<	System object
		CORE::C_SmartPointer<CORE::C_SystemInfo>					m_SystemInfo;				///<	System info object
		CORE::C_SmartPointer<CORE::C_Timer>							m_Timer;					///<	System timer object
		CORE::C_SmartPointer<AUDIO::C_AudioManager>					m_AudioManager;				///<	Audio manager object
		CORE::C_SmartPointer<FILESYSTEM::C_FileSystemManager>		m_FileSystemManager;		///<	FileSystem manager object
		CORE::C_SmartPointer<INPUT::C_InputManager>					m_InputManager;				///<	Input manager object
		CORE::C_SmartPointer<SCRIPT::C_ScriptManager>				m_ScriptManager;			///<	Script manager object
		CORE::C_SmartPointer<NETWORK::C_NetworkManager>				m_NetworkManager;			///<	Network manager object
		CORE::C_SmartPointer<RENDER::C_RenderManager>				m_RenderManager;			///<	Render manager object
		CORE::C_SmartPointer<DSM::C_DSMWorldSceneManager>			m_DSMWorldSceneManager;		///<	DSM	World Scene Manager object
		CORE::C_SmartPointer<GUI::C_GUIManager>						m_GUIManager;				///<	GUI manager object
		CORE::C_SmartPointer<PHYSICS::C_PhysicsWorldSceneManager>	m_PhysicsWorldSceneManager;	///<	Physics	World Scene Manager object
	};
}	//	namespace REDEEMER

#endif	//	_REDEEMERENGINE_H_
