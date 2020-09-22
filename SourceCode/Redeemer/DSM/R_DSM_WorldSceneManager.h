/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_DSM_WorldSceneManager.h
**		World scene manager holds a collection of scene managers
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_DSM_WORLDSCENEMANAGER_H_
#define _R_DSM_WORLDSCENEMANAGER_H_

#include "..\\Core\\R_Core_BaseClass.h"

#include <string>
#include <map>

namespace REDEEMER
{
	namespace DSM
	{
		class C_DSMSceneManager;
		class C_DSMCameraSceneNode;

		/*!	\brief World scene manager holds a collection of scene managers
		*/
		class C_DSMWorldSceneManager : public CORE::C_BaseClass
		{
		public:
			/*!	Constructor
			*/
			C_DSMWorldSceneManager ();

			/*!	Destructor
			*/
			virtual ~C_DSMWorldSceneManager ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize ();

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Creates new scene manager
			*/
			C_DSMSceneManager* CreateSceneManager (std::wstring name = L"");

			/*!	Adds newly created scene manager to the collection
			*/
			void AddSceneManager (const std::wstring& name, C_DSMSceneManager* sceneManager);

			/*!	Removes scene manager from library
			*/
			void RemoveSceneManager (const std::wstring& name);

			/*!	Searches for scene manager with given name
			*/
			C_DSMSceneManager* GetSceneManager (const std::wstring& name);

			/*!	Searches for scene manager with given index
			*/
			C_DSMSceneManager* GetSceneManager (int index);

			/*!	Returns number of scene managers
			*/
			unsigned int GetSceneManagersCount () const;

			/*!	Updates enabled scene managers
			*/
			void UpdateSceneManagers ();

			/*!	Sets active camera node
			*/
			void SetActiveCamera (C_DSMCameraSceneNode* camera);

			/*!	Gets active camera node
			*/
			C_DSMCameraSceneNode* GetActiveCamera ();

		private:
			std::map <std::wstring, C_DSMSceneManager*>	m_SceneManagers;

			C_DSMCameraSceneNode*						m_ActiveCamera;
		};

	}	//	namespace DSM
}	//	namespace REDEEMER

#endif	//	_R_DSM_WORLDSCENEMANAGER_H_
