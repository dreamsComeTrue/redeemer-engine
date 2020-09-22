/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Physics_SceneManager.h
**		Physical scene manager holds implementation of virtually colliding objects providing physical manipulation
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_PHYSICS_SCENEMANAGER_H_
#define _R_PHYSICS_SCENEMANAGER_H_

#include "R_Physics_Actor.h"
#include "R_Physics_Shape.h"
#include "..\\Core\\R_Core_BaseClass.h"
#include "..\\Core\\Math\\R_Core_Vector3D.h"

#include <string>
#include <map>

class NxScene;

namespace REDEEMER
{
	namespace PHYSICS
	{
		class C_CharacterController;
		class C_PhysicsMaterial;

		/*!	\brief Physical scene manager holds implementation of virtually colliding objects providing physical manipulation
		*/
		class C_PhysicsSceneManager : public CORE::C_BaseClass
		{
		public:
			/*!	Constructor
			*/
			C_PhysicsSceneManager (const std::wstring& name, CORE::C_Vector3D<float> gravityVector = CORE::C_Vector3D<float> (0.0f, -9.8f, 0.0f));

			/*!	Destructor
			*/
			virtual ~C_PhysicsSceneManager ();

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

			/*!	Returns PhysX scene object
			*/
			NxScene* GetPhysXScene ();

			//===================================	Actors

			/*!	Creates new physics actor and adds it to the processing collection
			*/
			C_PhysicsActor* CreatePhysicsActor (const std::wstring& name, EPhysicsActorType type, const std::vector <C_PhysicsShape*>& shapes);

			/*!	Adds physics actor to the collection
			*/
			void AddPhysicsActor (const std::wstring& name, C_PhysicsActor* actor);

			/*!	Gets physics actor with given name
			*/
			C_PhysicsActor* GetPhysicsActor (const std::wstring& name);

			/*!	Destroys given physics actor
			*/
			void RemovePhysicsActor (const std::wstring& name);

			//===================================	Shapes

			/*!	Creates new physics shape and adds it to the processing collection
			*/
			C_PhysicsShape* CreatePhysicsShape (const std::wstring& name, EPhysicsShapeType type);

			/*!	Adds physics shape to the collection
			*/
			void AddPhysicsShape (const std::wstring& name, C_PhysicsShape* shape);

			/*!	Gets physics shape with given name
			*/
			C_PhysicsShape* GetPhysicsShape (const std::wstring& name);

			/*!	Destroys given physics shape
			*/
			void RemovePhysicsShape (const std::wstring& name);

			//===================================	Materials

			/*!	Creates new material and adds it to the processing collection
			*/
			C_PhysicsMaterial* CreatePhysicsMaterial (const std::wstring& name);

			/*!	Adds material to the collection
			*/
			void AddPhysicsMaterial (const std::wstring& name, C_PhysicsMaterial* material);

			/*!	Gets material with given name
			*/
			C_PhysicsMaterial* GetPhysicsMaterial (const std::wstring& name);

			/*!	Destroys given material
			*/
			void RemovePhysicsMaterial (const std::wstring& name);

			//===================================	Character Controller

			/*!	Creates new character controller and adds it to the processing collection
			*/
			C_CharacterController* CreateCharacterController (const std::wstring& name, float radius = 0.5f, float height = 0.5f, float slopeLimit = 0.7f, float obstacleLimit = 0.5f);

			/*!	Adds character controller to the collection
			*/
			void AddCharacterController (const std::wstring& name, C_CharacterController* controller);

			/*!	Gets controller with given name
			*/
			C_CharacterController* GetCharacterController (const std::wstring& name);

			/*!	Destroys given character controller
			*/
			void RemoveCharacterController (const std::wstring& name);

		private:
			bool											m_IsEnabled;			///<	Tells, if world updates are enabled
			std::wstring									m_Name;					///<	Scene manager name
			CORE::C_Vector3D<float>							m_GravityVector;		///<	Are there any gravitational forces?
			NxScene*										m_PhysXScene;			///<	PhysX scene object
			unsigned long									m_PreviousTime;			///<	Time counter for advancing simulation

			std::map<std::wstring, C_PhysicsActor*>			m_Actors;				///<	Actors
			std::map<std::wstring, C_PhysicsShape*>			m_Shapes;				///<	Shapes
			std::map<std::wstring, C_PhysicsMaterial*>		m_Materials;			///<	Materials
			std::map<std::wstring, C_CharacterController*>	m_CharacterControllers;	///<	Character controllers
		};

	}	//	namespace PHYSICS
}	//	namespace REDEEMER

#endif	//	_R_PHYSICS_SCENEMANAGER_H_
