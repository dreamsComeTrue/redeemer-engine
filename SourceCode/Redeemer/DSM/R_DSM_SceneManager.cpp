/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_DSM_SceneManager.cpp
**		DSMSceneManger (scene graph) is responsible for controlling lifetime of scene objects
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_DSM_SceneManager.h"
#include "R_DSM_SceneNode.h"
#include "R_DSM_SceneNodeEntity.h"
#include "R_DSM_BoxSceneNode.h"
#include "R_DSM_PlaneSceneNode.h"
#include "R_DSM_SkyBoxSceneNode.h"
#include "R_DSM_CameraSceneNode.h"
#include "R_DSM_Camera3DSceneNode.h"
#include "R_DSM_LightSceneNode.h"
#include "R_DSM_WorldSceneManager.h"
#include "..\\Render\\R_Render_RenderDevice.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace DSM
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_DSMSceneManager::C_DSMSceneManager (const std::wstring& name) :
			m_IsEnabled (true),
			m_RootNode (NULL),
			m_Name (name),
			m_ActiveCamera (NULL),
			m_ActiveLight (NULL)
		{
			m_Lights.clear();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMSceneManager::~C_DSMSceneManager ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_DSMSceneManager::Initialize ()
		{
			C_BaseClass::Initialize();

			m_RootNode = new C_DSMSceneNode (L"RootSceneNode", this);

			m_RootNode->Initialize();

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_DSMSceneManager::Finalize ()
		{
			REDEEMER_SAFE_DELETE (m_RootNode);

			m_SceneNodes.clear();

			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMSceneManager::SetEnabled (bool enabled)
		{
			m_IsEnabled = enabled;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_DSMSceneManager::IsEnabled () const
		{
			return m_IsEnabled;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMSceneManager::SetName (const std::wstring& name)
		{
			m_Name = name;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::wstring& C_DSMSceneManager::GetName ()
		{
			return m_Name;
		}

		//------------------------------------------------------------------------------------------------------------------------
		void C_DSMSceneManager::Update ()
		{
			if (m_ActiveCamera)
			{
				C_RedeemerEngine::GetSingleton ().GetDSMWorldSceneManager ()->SetActiveCamera (m_ActiveCamera);
				m_ActiveCamera->UpdateCamera ();
			}

			//	Just update root node, and all nodes will be updated recursively
			if (m_RootNode->IsEnabled())
				m_RootNode->Update();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMSceneNode* C_DSMSceneManager::GetRootNode ()
		{
			return m_RootNode;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMSceneNode* C_DSMSceneManager::GetNode (const std::wstring& name)
		{
			if (m_SceneNodes.find(name) == m_SceneNodes.end())
			{
				REDEEMER_LOG << LOG_WARNING << "C_DSMSceneManager::GetNode -> Can not find node with name: '" << name << "'!" << LOG_ENDMESSAGE;

				return NULL;
			}

			return m_SceneNodes[name];
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::map <std::wstring, C_DSMSceneNode*>& C_DSMSceneManager::GetNodes()
		{
			return m_SceneNodes;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_DSMSceneManager::AddNode (const std::wstring& name, C_DSMSceneNode* node)
		{
			//	There is already node with this name :(
			if (m_SceneNodes.find (name) != m_SceneNodes.end())
				return false;

			m_SceneNodes[name] = node;
			m_SceneNodes[name]->SetName (name);

			C_LightSceneNode* lightNode = NULL;

			lightNode = dynamic_cast <C_LightSceneNode*> (node);

			if (lightNode != NULL)
				m_Lights.push_back(lightNode);

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMSceneManager::RemoveNode (const std::wstring& name)
		{
			C_DSMSceneNode* node = GetNode (name);

			if (node == NULL)
				return;
								
			m_SceneNodes.erase (name);

			C_LightSceneNode* lightNode = NULL;

			lightNode = dynamic_cast <C_LightSceneNode*> (node);

			if (lightNode != NULL)
			{
				for (std::vector<C_LightSceneNode*>::iterator it = m_Lights.begin(); it != m_Lights.end(); ++ it)
				{
					if (lightNode == *it)
					{
						m_Lights.erase(it);

						break;
					}
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMBoxSceneNode* C_DSMSceneManager::CreateBoxSceneNode (CORE::C_AABBox3D<float>& box)
		{
			C_DSMBoxSceneNode* boxNode = new C_DSMBoxSceneNode (box);

			boxNode->SetSceneManager(this);

			return boxNode;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMPlaneSceneNode* C_DSMSceneManager::CreatePlaneSceneNode (CORE::C_Plane3D<float> plane, float size, CORE::C_Vector2D<float> textureMultipier)
		{
			C_DSMPlaneSceneNode* planeNode = new C_DSMPlaneSceneNode (plane, size, textureMultipier);

			planeNode->SetSceneManager(this);

			return planeNode;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMSkyBoxSceneNode* C_DSMSceneManager::CreateSkyBoxSceneNode(float distance)
		{
			C_DSMSkyBoxSceneNode* skyBoxNode = new C_DSMSkyBoxSceneNode (distance);

			skyBoxNode->SetSceneManager(this);

			return skyBoxNode;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMCameraSceneNode* C_DSMSceneManager::CreateCameraSceneNode ()
		{
			C_DSMCameraSceneNode* cameraNode = new C_DSMCameraSceneNode ();
			
			cameraNode->SetSceneManager(this);

			return cameraNode;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMCamera3DSceneNode* C_DSMSceneManager::Create3DCameraSceneNode (float velocity)
		{
			C_DSMCamera3DSceneNode* cameraNode = new C_DSMCamera3DSceneNode ();

			cameraNode->SetSceneManager(this);

			return cameraNode;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_LightSceneNode* C_DSMSceneManager::CreateLightSceneNode ()
		{
			C_LightSceneNode* lightNode = new C_LightSceneNode ();

			lightNode->SetSceneManager(this);

			return lightNode;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMSceneManager::SetActiveCamera (C_DSMCameraSceneNode* camera)
		{
			m_ActiveCamera = camera;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMCameraSceneNode* C_DSMSceneManager::GetActiveCamera ()
		{
			return m_ActiveCamera;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMSceneManager::SetActiveLight(C_LightSceneNode* lightNode)
		{
			m_ActiveLight = lightNode;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_LightSceneNode* C_DSMSceneManager::GetActiveLight()
		{
			return m_ActiveLight;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::vector<C_LightSceneNode*>* C_DSMSceneManager::GetSceneLights ()
		{
			return &m_Lights;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace DSM
}	//	namespace REDEEMER
