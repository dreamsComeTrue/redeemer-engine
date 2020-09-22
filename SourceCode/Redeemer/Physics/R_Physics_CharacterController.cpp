/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Physics_CharacterController.cpp
**		Controller provides user an interface to move in 3D environment
*/

//------------------------------------------------------------------------------------------------------------------------
#define NOMINMAX
#include "R_Physics_CharacterController.h"
#include "R_Physics_SceneManager.h"
#include "R_Physics_WorldSceneManager.h"
#include "R_Physics_PhysXConvert.h"
#include "..\\DSM\\R_DSM_SceneNode.h"
#include "..\\RedeemerEngine.h"

#pragma warning(push)
#pragma warning(disable:4245)
#pragma warning(disable:4512)
#include <NxActor.h>
#include <NxController.h>
#include <NxControllerManager.h>
#include <NxPhysics.h>
#include <NxCapsuleController.h>
#pragma warning(pop)

namespace REDEEMER
{
	namespace PHYSICS
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_CharacterController::C_CharacterController (C_PhysicsSceneManager* scene, float radius, float height, float slopeLimit, float obstacleLimit) :
			m_SceneManager (scene),
			m_SceneNode (NULL),
			m_Actor (NULL),
			m_Controller (NULL),
			m_Velocity (2.0f),
			m_IsMoving (false),
			m_Radius (radius),
			m_Height (height),
			m_SlopeLimit (slopeLimit),
			m_ObstacleLimit (obstacleLimit)
		{
			NxCapsuleControllerDesc capsuleDescriptor;

			capsuleDescriptor.radius = radius;
			capsuleDescriptor.height = height;
			capsuleDescriptor.slopeLimit = slopeLimit;
			capsuleDescriptor.stepOffset = obstacleLimit;
			capsuleDescriptor.skinWidth = 0.1f;
			capsuleDescriptor.upDirection	= NX_Y;
			capsuleDescriptor.position.x = 0.0f;
			capsuleDescriptor.position.y = 0.0f;
			capsuleDescriptor.position.z = 0.0f;

			m_Controller = C_RedeemerEngine::GetSingleton().GetPhysicsWorldSceneManager()->GetPhysXControllerManager()->createController(scene->GetPhysXScene(), capsuleDescriptor);

			if (!m_Controller)
			{
				REDEEMER_LOG << LOG_ERROR << L"Physics: Can not create valid Character Controller!" << LOG_ENDMESSAGE;

				return;
			}		  

			m_Actor = m_Controller->getActor();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_CharacterController::~C_CharacterController ()
		{
			m_SceneManager->GetPhysXScene()->releaseActor(*m_Actor);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_CharacterController::AttachNode (DSM::C_DSMSceneNode* node)
		{
			m_SceneNode = node;

			//	Gets node's position and place character controller there
			if (node)
				m_Controller->setPosition(C_PhysXConvert::ConvertVector3ToNxExtendedVec3(node->GetPositionLocal()));
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_CharacterController::Move(CORE::C_Vector3D<float> direction)
		{
			m_MovementDirection = direction;
			m_IsMoving = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_CharacterController::Update(float deltaTime)
		{
			if (!m_SceneNode)
				return;

			NxVec3 movement;

			m_SceneManager->GetPhysXScene()->getGravity(movement);

			if (m_IsMoving)
			{
				m_MovementDirection.Y = 0.0f;
				m_MovementDirection.SetNormalized();

				movement.x += m_MovementDirection.X * m_Velocity;
				movement.z += m_MovementDirection.Z * m_Velocity;
				m_IsMoving = false;
			}

			NxU32 collisionFlags;
			NxVec3 disp = movement * deltaTime;

			m_Controller->move(disp, (1 << 1) | (1 << 2), 0.000001f, collisionFlags);

			CORE::C_Vector3D<float> vecPos = C_PhysXConvert::ConvertNxExtendedVec3ToVector3 (m_Controller->getPosition());

			m_SceneNode->SetPosition (vecPos);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_CharacterController::SetVelocity(float velocity)
		{
			m_Velocity = velocity;
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_CharacterController::GetVelocity() const
		{
			return m_Velocity;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace PHYSICS
}	//	namespace REDEEMER
