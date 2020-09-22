/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Physics_WorldSceneManager.h
**		Physics world scene manager holds a collection of physics scene managers
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_PHYSICS_WORLDSCENEMANAGER_H_
#define _R_PHYSICS_WORLDSCENEMANAGER_H_

#include "..\\Core\\R_Core_BaseClass.h"
#include "..\\Core\\Math\\R_Core_Vector3D.h"

#include <string>
#include <map>

class NxPhysicsSDK;
class NxControllerManager;

namespace REDEEMER
{
	namespace PHYSICS
	{
		class C_PhysicsSceneManager;

		/*!	\brief Physics world scene manager holds a collection of physics scene managers
		*/
		class C_PhysicsWorldSceneManager : public CORE::C_BaseClass
		{
		public:
			/*!	Constructor
			*/
			C_PhysicsWorldSceneManager ();

			/*!	Destructor
			*/
			virtual ~C_PhysicsWorldSceneManager ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize ();

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Creates new scene manager
			*/
			C_PhysicsSceneManager* CreateSceneManager (std::wstring name = L"", CORE::C_Vector3D<float> gravityVector = CORE::C_Vector3D<float> (0.0f, -9.8f, 0.0f));

			/*!	Adds newly created scene manager to the collection
			*/
			void AddSceneManager (const std::wstring& name, C_PhysicsSceneManager* sceneManager);

			/*!	Removes scene manager from library
			*/
			void RemoveSceneManager (const std::wstring& name);

			/*!	Searches for scene manager with given name
			*/
			C_PhysicsSceneManager* GetSceneManager (const std::wstring& name);

			/*!	Returns number of scene managers
			*/
			unsigned int GetSceneManagersCount () const;

			/*!	Updates enabled scene managers
			*/
			void UpdateSceneManagers ();

		private:
			std::map <std::wstring, C_PhysicsSceneManager*>		m_SceneManagers;
		};

	}	//	namespace PHYSICS
}	//	namespace REDEEMER

#endif	//	_R_PHYSICS_WORLDSCENEMANAGER_H_
