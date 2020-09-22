/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Physics_Actor.h
**		Actors are physical entities for which collisions are computed
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_PHYSICS_ACTOR_H_
#define _R_PHYSICS_ACTOR_H_

#include <string>
#include <vector>

class NxActor;

namespace REDEEMER
{
	namespace DSM
	{
		class C_DSMSceneNodeEntity;
	}

	namespace PHYSICS
	{
		class C_PhysicsSceneManager;
		class C_PhysicsShape;
		class C_PhysicsMaterial;

		/*!	Various types of actors
		*/
		enum EPhysicsActorType
		{
			PAT_Static,
			PAT_Kinematic,
			PAT_Dynamic
		};

		/*!	\brief	Actors are physical entities for which collisions are computed
		*/
		class C_PhysicsActor
		{
		public:
			/*!	Constructor
			*/
			C_PhysicsActor (C_PhysicsSceneManager* creator, EPhysicsActorType type, const std::vector <C_PhysicsShape*>& shapes, const std::wstring& name = L"");

			/*!	Destructor
			*/
			virtual ~C_PhysicsActor ();

			/*!	Sets material name
			*/
			void SetName (const std::wstring& name = L"");

			/*!	Retrieves name
			*/
			std::wstring GetName () const;	

			/*!	Adds new shape to actor's list
			*/
			void AddShape (C_PhysicsShape* shape);

			/*!	Sets actor's mass
			*/
			void SetMass (float mass);

			/*!	Returns actor's mass
			*/
			float GetMass () const;		

			/*!	Attaches visual node from DSM
			*/
			void AttachNode (DSM::C_DSMSceneNodeEntity* node);

			/*!	Process actor's collisions
			*/
			void Update ();

			/*!	Sets physics material for given shape
			*/
			void SetShapeMaterial (unsigned int index, C_PhysicsMaterial* material);

			/*!	Returns material for given shape
			*/
			C_PhysicsMaterial* GetShapeMaterial (unsigned int index);

		private:
			std::wstring						m_Name;					///<	Actor's name
			std::vector <C_PhysicsShape*>		m_Shapes;				///<	Geometrical shapes approximating the body's shape
			EPhysicsActorType					m_Type;					///<	Actor type
			C_PhysicsSceneManager*				m_SceneManager;			///<	Which scene created this actor
			DSM::C_DSMSceneNodeEntity*			m_Node;					///<	Visual node
			NxActor*							m_PhysXActor;			
		};

	}	//	namespace PHYSICS
}	//	namespace REDEEMER

#endif	//	_R_PHYSICS_ACTOR_H_
