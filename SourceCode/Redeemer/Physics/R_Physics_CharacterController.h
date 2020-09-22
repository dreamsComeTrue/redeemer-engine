/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Physics_CharacterController.h
**		Controller provides user an interface to move in 3D environment
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_PHYSICS_CHARACTERCONTROLLER_H_
#define _R_PHYSICS_CHARACTERCONTROLLER_H_

#include "..\\Core\\Math\\R_Core_Vector3D.h"

class NxController;
class NxActor;

namespace REDEEMER
{
	namespace DSM
	{
		class C_DSMSceneNode;
	}

	namespace PHYSICS
	{
		class C_PhysicsSceneManager;

		/*!	\brief	Controller provides user an interface to move in 3D environment
		*/
		class C_CharacterController
		{
		public:
			/*!	Constructor
			*/
			C_CharacterController (C_PhysicsSceneManager* scene, float radius = 0.5f, float height = 0.5f, float slopeLimit = 0.7f, float obstacleLimit = 0.5f);

			/*!	Destructor
			*/
			virtual ~C_CharacterController ();

			/*!	Connects DSM node to this controller
			*/
			void AttachNode (DSM::C_DSMSceneNode* node);

			/*!	Moves character controller in given direction
			*/
			void Move (CORE::C_Vector3D<float> direction);

			/*!	Updates the character controller.
			*/
			void Update (float delta);

			/*!	Sets controller velocity
			*/
			void SetVelocity(float velocity);

			/*!	Retrieves velocity
			*/
			float GetVelocity() const;

		private:
			NxController*			m_Controller;					///<	Pointer to PhysX character controller
			NxActor*				m_Actor;						///<	Linked Actor object
			C_PhysicsSceneManager*	m_SceneManager;					///<	Which scene created this material
			DSM::C_DSMSceneNode*	m_SceneNode;					///<	Connected scene node

			float					m_Velocity;						///<	Character controller velocity
			CORE::C_Vector3D<float> m_MovementDirection;			///<	Direction of movement
			bool					m_IsMoving;						///<	Is character moving?

			float					m_Radius;						///<	Radius of the capsule
			float					m_Height;						///<	Height of the capsule
			float					m_SlopeLimit;					///<	Maximum slope (cosine of the angle) character can walk against
			float					m_ObstacleLimit;				///<	Maximum obstacle height character can climb onto
		};

	}	//	namespace PHYSICS
}	//	namespace REDEEMER

#endif	//	_R_PHYSICS_CHARACTERCONTROLLER_H_
