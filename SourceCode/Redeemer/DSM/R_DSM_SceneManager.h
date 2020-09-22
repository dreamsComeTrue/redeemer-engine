/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_DSM_SceneManager.h
**		DSMSceneManger (scene graph) is responsible for controlling lifetime of scene objects
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_DSM_SCENEMANAGER_H_
#define _R_DSM_SCENEMANAGER_H_

#include "..\\Core\\R_Core_BaseClass.h"
#include "..\\Core\\Math\\R_Core_AABBox3D.h"
#include "..\\Core\\Math\\R_Core_Plane3D.h"
#include "..\\Core\\Math\\R_Core_Vector2D.h"

#include <string>
#include <vector>
#include <map>

namespace REDEEMER
{
	namespace DSM
	{
		class C_DSMSceneNode;
		class C_DSMBoxSceneNode;
		class C_DSMPlaneSceneNode;
		class C_DSMSkyBoxSceneNode;
		class C_DSMCameraSceneNode;
		class C_DSMCamera3DSceneNode;
		class C_LightSceneNode;

		/*!	DSMSceneManger (scene graph) is responsible for controlling lifetime of scene objects
		*/
		class C_DSMSceneManager : public CORE::C_BaseClass
		{
		public:
			/*!	Constructor
			*/
			C_DSMSceneManager (const std::wstring& name);

			/*!	Destructor
			*/
			virtual ~C_DSMSceneManager ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize ();

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Sets enabled updating world
			*/
			void SetEnabled (bool enabled = true);

			/*!	Checks, if world is enabled
			*/
			bool IsEnabled () const;

			/*!	Sets scene manager's name
			*/
			void SetName (const std::wstring& name);

			/*!	Gets scene manager's name
			*/
			std::wstring& GetName ();

			/*!	Updates world data
			*/
			void Update ();

			/*!	Returns scene's root node
			*/
			C_DSMSceneNode* GetRootNode ();

			/*!	Returns node with given name
			*/
			C_DSMSceneNode* GetNode (const std::wstring& name);

			/*!	Returns a collection of nodes
			*/
			std::map <std::wstring, C_DSMSceneNode*>& GetNodes();

			/*!	Adds node to the Scene Graph.
			*/
			bool AddNode (const std::wstring& name, C_DSMSceneNode* node);

			/*!	Removes node from Scene Graph.
			*/
			void RemoveNode (const std::wstring& name);

			/*!	Sets active camera node
			*/
			void SetActiveCamera (C_DSMCameraSceneNode* camera);

			/*!	Returns active camera
			*/
			C_DSMCameraSceneNode* GetActiveCamera ();

			//	Creating nodes

			/*!	Create standard cube node
			*/
			C_DSMBoxSceneNode* CreateBoxSceneNode (CORE::C_AABBox3D<float>& box);
			
			/*!	Create standard plane node
			*/
			C_DSMPlaneSceneNode* CreatePlaneSceneNode (CORE::C_Plane3D<float> plane, float size, CORE::C_Vector2D<float> textureMultipier = CORE::C_Vector2D<float>::VECTOR_ONE_VECTOR);
			
			/*!	Create sky box node
			*/
			C_DSMSkyBoxSceneNode* CreateSkyBoxSceneNode (float distance = 10.0f);

			/*!	Create generic camera
			*/
			C_DSMCameraSceneNode* CreateCameraSceneNode ();

			/*!	Create 3D camera type (like in FPP's)
			*/
			C_DSMCamera3DSceneNode* Create3DCameraSceneNode (float velocity = 20.0f);

			/*!	Create light
			*/
			C_LightSceneNode* CreateLightSceneNode ();

			/*!	Sets light as active, for lighting stage
			*/
			void SetActiveLight (C_LightSceneNode* lightNode);

			/*!	Gets active light
			*/
			C_LightSceneNode* GetActiveLight ();

			/*!	Returns all lights belonging to this scene
			*/
			std::vector<C_LightSceneNode*>* GetSceneLights ();

		private:
			bool										m_IsEnabled;	///<	Tells, if world updates are enabled
			std::wstring								m_Name;			///<	Scene manager name
			C_DSMSceneNode*								m_RootNode;		///<	Root scene node			
			C_DSMCameraSceneNode*						m_ActiveCamera;	///<	Active camera node
			std::map <std::wstring, C_DSMSceneNode*>	m_SceneNodes;	///<	Map containing all nodes in the scene belonging	to this SceneManager.
			std::vector<C_LightSceneNode*>				m_Lights;		///<	All lights in scene
			C_LightSceneNode*							m_ActiveLight;	///<	Active light node
		};

	}	//	namespace DSM
}	//	namespace REDEEMER

#endif	//	_R_DSM_SCENEMANAGER_H_
