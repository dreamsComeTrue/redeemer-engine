/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Physics_SceneManager.cpp
**		Physical scene manager holds implementation of virtually colliding objects providing physical manipulation
*/

//------------------------------------------------------------------------------------------------------------------------
#define NOMINMAX
#include "R_Physics_SceneManager.h"
#include "R_Physics_WorldSceneManager.h"
#include "R_Physics_PhysXConvert.h"
#include "R_Physics_CharacterController.h"
#include "R_Physics_Material.h"	
#include "R_Physics_ShapeBox.h"
#include "..\\RedeemerEngine.h"

#pragma warning(push)
#pragma warning(disable:4245)
#pragma warning(disable:4512)
#include "NxScene.h"
#include "NxSceneDesc.h"
#pragma warning(pop)

namespace REDEEMER
{
	namespace PHYSICS
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_PhysicsSceneManager::C_PhysicsSceneManager (const std::wstring& name, CORE::C_Vector3D<float> gravityVector) :
			m_GravityVector (gravityVector),
			m_IsEnabled (true),
			m_Name (name),
			m_PhysXScene (NULL)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PhysicsSceneManager::~C_PhysicsSceneManager ()
		{
			if (IsInitialized ())
				Finalize ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_PhysicsSceneManager::Initialize ()
		{
			C_BaseClass::Initialize();

			NxSceneDesc sceneDescription;

			sceneDescription.gravity = C_PhysXConvert::ConvertVector3ToNxVec3 (m_GravityVector);
			sceneDescription.simType = NX_SIMULATION_HW;

			m_PhysXScene = C_RedeemerEngine::GetSingleton().GetPhysicsWorldSceneManager()->GetPhysXObject()->createScene(sceneDescription);

			if (!m_PhysXScene)
			{
				REDEEMER_LOG << LOG_WARNING << L"Physics Scene Manager can not be initialized in HARDWARE mode." << LOG_ENDMESSAGE;

				sceneDescription.simType = NX_SIMULATION_SW;

				m_PhysXScene = C_RedeemerEngine::GetSingleton().GetPhysicsWorldSceneManager()->GetPhysXObject()->createScene(sceneDescription);

				if (!m_PhysXScene)
				{
					REDEEMER_LOG << LOG_WARNING << L"Physics Scene Manager can not be initialized in SOFTWARE mode." << LOG_ENDMESSAGE;

					return RESULT_FAIL;
				}

				REDEEMER_LOG << LOG_INFO << L"Physics Scene Manager initialized in SOFTWARE mode." << LOG_ENDMESSAGE;
			}
			else
				REDEEMER_LOG << LOG_INFO << L"Physics Scene Manager initialized in HARDWARE mode." << LOG_ENDMESSAGE;
								 
			//	Try to simulate PhysX engine

			CORE::C_Timer* timer = C_RedeemerEngine::GetSingleton().GetTimer();

			m_PreviousTime = timer->GetMiliseconds();

			float elapsedTime = (timer->GetMiliseconds() - m_PreviousTime) * 0.001f;

			m_PhysXScene->simulate (elapsedTime);
			m_PhysXScene->flushStream();

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_PhysicsSceneManager::Finalize ()
		{	
			//	Release all actors
			for (std::map<std::wstring, C_PhysicsActor*>::iterator it = m_Actors.begin(); it != m_Actors.end(); ++it)
			{
				REDEEMER_SAFE_DELETE ((*it).second);
			}

			m_Actors.clear();

			//	Release all materials
			for (std::map<std::wstring, C_PhysicsMaterial*>::iterator it = m_Materials.begin(); it != m_Materials.end(); ++it)
			{
				REDEEMER_SAFE_DELETE ((*it).second);
			}

			m_Materials.clear();

			//	Release all shapes
			for (std::map<std::wstring, C_PhysicsShape*>::iterator it = m_Shapes.begin(); it != m_Shapes.end(); ++it)
			{
				REDEEMER_SAFE_DELETE ((*it).second);
			}

			m_Shapes.clear();

			//	Release all character controllers
			for (std::map<std::wstring, C_CharacterController*>::iterator it = m_CharacterControllers.begin(); it != m_CharacterControllers.end(); ++it)
			{
				REDEEMER_SAFE_DELETE ((*it).second);
			}

			m_CharacterControllers.clear();

			C_RedeemerEngine::GetSingleton().GetPhysicsWorldSceneManager()->GetPhysXObject()->releaseScene(*m_PhysXScene);
			m_PhysXScene = NULL;

			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsSceneManager::SetEnabled (bool enabled)
		{
			m_IsEnabled = enabled;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_PhysicsSceneManager::IsEnabled () const
		{
			return m_IsEnabled;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsSceneManager::SetName (const std::wstring& name)
		{
			m_Name = name;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::wstring& C_PhysicsSceneManager::GetName ()
		{
			return m_Name;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsSceneManager::Update ()
		{
			CORE::C_Timer* timer = C_RedeemerEngine::GetSingleton().GetTimer();	   

			while (!m_PhysXScene->fetchResults(NX_RIGID_BODY_FINISHED, false))
				;

			float elapsedTime = (timer->GetMiliseconds() - m_PreviousTime) * 0.001f;

			m_PhysXScene->simulate (elapsedTime);
			m_PhysXScene->flushStream();

			//	Do all updates here

			//	Actors
			for (std::map<std::wstring, C_PhysicsActor*>::iterator it = m_Actors.begin(); it != m_Actors.end(); ++it)
				(*it).second->Update ();

			//	Character controllers
			for (std::map<std::wstring, C_CharacterController*>::iterator it = m_CharacterControllers.begin(); it != m_CharacterControllers.end(); ++it)
				(*it).second->Update (elapsedTime);

			m_PreviousTime = timer->GetMiliseconds();
		}

		//------------------------------------------------------------------------------------------------------------------------

		NxScene* C_PhysicsSceneManager::GetPhysXScene ()
		{
			return m_PhysXScene;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PhysicsActor* C_PhysicsSceneManager::CreatePhysicsActor (const std::wstring& name, EPhysicsActorType type, const std::vector <C_PhysicsShape*>& shapes)
		{
			C_PhysicsActor* actor = new C_PhysicsActor (this, type, shapes, name);

			if (actor != NULL)
			{
				m_Actors[name] = actor;
			}

			return actor;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsSceneManager::AddPhysicsActor (const std::wstring& name, C_PhysicsActor* actor)
		{
			// The material already exists, so return
			if (m_Actors.find (name) != m_Actors.end())
				return;

			m_Actors [name] = actor;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PhysicsActor* C_PhysicsSceneManager::GetPhysicsActor (const std::wstring& name)
		{
			if (m_Actors.find (name) == m_Actors.end())
			{
				return NULL;
			}

			return m_Actors[name];
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsSceneManager::RemovePhysicsActor (const std::wstring& name)
		{
			// Material does not exist, so exit
			if (m_Actors.find (name) == m_Actors.end())
				return;

			REDEEMER_SAFE_DELETE (m_Actors [name]);
			m_Actors.erase (name);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PhysicsShape* C_PhysicsSceneManager::CreatePhysicsShape (const std::wstring& name, EPhysicsShapeType type)
		{
			C_PhysicsShape* shape = NULL;

			switch (type)
			{
			case PST_Box:
				shape = new C_PhysicsShapeBox ();

				break;

			case PST_Plane:
				break;
			}

			if (shape != NULL)
			{
				m_Shapes[name] = shape;
			}

			return shape;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsSceneManager::AddPhysicsShape (const std::wstring& name, C_PhysicsShape* shape)
		{
			// The material already exists, so return
			if (m_Shapes.find (name) != m_Shapes.end())
				return;

			m_Shapes [name] = shape;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PhysicsShape* C_PhysicsSceneManager::GetPhysicsShape (const std::wstring& name)
		{
			if (m_Shapes.find (name) == m_Shapes.end())
			{
				return NULL;
			}

			return m_Shapes[name];
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsSceneManager::RemovePhysicsShape (const std::wstring& name)
		{
			// Material does not exist, so exit
			if (m_Shapes.find (name) == m_Shapes.end())
				return;

			REDEEMER_SAFE_DELETE (m_Shapes [name]);
			m_Shapes.erase (name);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PhysicsMaterial* C_PhysicsSceneManager::CreatePhysicsMaterial (const std::wstring& name)
		{
			C_PhysicsMaterial* material = new C_PhysicsMaterial (this);

			if (material != NULL)
			{
				m_Materials[name] = material;
			}

			return material;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsSceneManager::AddPhysicsMaterial (const std::wstring& name, C_PhysicsMaterial* material)
		{
			// The material already exists, so return
			if (m_Materials.find (name) != m_Materials.end())
				return;

			m_Materials [name] = material;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PhysicsMaterial* C_PhysicsSceneManager::GetPhysicsMaterial (const std::wstring& name)
		{
			if (m_Materials.find (name) == m_Materials.end())
			{
				return NULL;
			}

			return m_Materials[name];
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsSceneManager::RemovePhysicsMaterial (const std::wstring& name)
		{
			// Material does not exist, so exit
			if (m_Materials.find (name) == m_Materials.end())
				return;

			REDEEMER_SAFE_DELETE (m_Materials [name]);
			m_Materials.erase (name);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_CharacterController* C_PhysicsSceneManager::CreateCharacterController (const std::wstring& name, float radius, float height, float slopeLimit, float obstacleLimit)
		{
			C_CharacterController* controller = new C_CharacterController (this, radius, height, slopeLimit, obstacleLimit);

			if (controller != NULL)
			{
				m_CharacterControllers[name] = controller;
			}

			return controller;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsSceneManager::AddCharacterController (const std::wstring& name, C_CharacterController* controller)
		{
			// The material already exists, so return
			if (m_CharacterControllers.find (name) != m_CharacterControllers.end())
				return;

			m_CharacterControllers [name] = controller;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_CharacterController* C_PhysicsSceneManager::GetCharacterController (const std::wstring& name)
		{
			if (m_CharacterControllers.find (name) == m_CharacterControllers.end())
			{
				return NULL;
			}

			return m_CharacterControllers[name];
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsSceneManager::RemoveCharacterController (const std::wstring& name)
		{
			// Material does not exist, so exit
			if (m_CharacterControllers.find (name) == m_CharacterControllers.end())
				return;

			REDEEMER_SAFE_DELETE (m_CharacterControllers [name]);
			m_CharacterControllers.erase (name);
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace PHYSICS
}	//	namespace REDEEMER
