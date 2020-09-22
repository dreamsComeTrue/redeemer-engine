/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Physics_Actor.cpp
**		Actors are physical entities for which collisions are computed
*/

//------------------------------------------------------------------------------------------------------------------------
#define NOMINMAX
#include "R_Physics_Actor.h"
#include "R_Physics_PhysXConvert.h"
#include "R_Physics_SceneManager.h"
#include "R_Physics_Material.h"
#include "R_Physics_Shape.h"
#include "..\\DSM\\R_DSM_SceneNodeEntity.h"

#pragma warning(push)
#pragma warning(disable:4512)
#include <NxActor.h>
#include <NxScene.h>
#pragma warning(pop)

namespace REDEEMER
{
	namespace PHYSICS
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_PhysicsActor::C_PhysicsActor (C_PhysicsSceneManager* creator, EPhysicsActorType type, const std::vector <C_PhysicsShape*>& shapes, const std::wstring& name) :
			m_Name (name),
			m_Type (type),
			m_SceneManager (creator),
			m_PhysXActor (NULL),
			m_Node (NULL)
		{
			// Create PhysX actor
			NxActorDesc actorDescriptor;

			for (unsigned int i = 0; i < shapes.size(); ++ i)
			{
				NxShapeDesc* shapeDesc = shapes[i]->GetPhysXDescription();
				actorDescriptor.shapes.pushBack(shapeDesc);

				m_Shapes.push_back(shapes[i]);
			}

			NxBodyDesc bodyDesc;

			if (m_Type == PAT_Kinematic)
				bodyDesc.flags |= NX_BF_KINEMATIC;

			if (m_Type == PAT_Dynamic || m_Type == PAT_Kinematic)
			{
				actorDescriptor.body = &bodyDesc;
				actorDescriptor.density = 10.0f;
			}

			if (actorDescriptor.isValid())
			{
				m_PhysXActor = m_SceneManager->GetPhysXScene()->createActor(actorDescriptor);
				m_PhysXActor->userData = this;
			}

			NxShape* const* shapes2 = m_PhysXActor->getShapes();
			unsigned int i = m_PhysXActor->getNbShapes();
			while(i--)
			{
				shapes2[i]->setGroup(1);
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PhysicsActor::~C_PhysicsActor ()
		{
			m_SceneManager->GetPhysXScene()->releaseActor (*m_PhysXActor);

			if(!m_Shapes.empty())
				m_Shapes.clear();

			m_PhysXActor = NULL;
			m_SceneManager = NULL; 
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsActor::SetName (const std::wstring& name)
		{
			m_Name = name;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::wstring C_PhysicsActor::GetName () const
		{
			return m_Name;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsActor::AddShape (C_PhysicsShape* shape)
		{
			m_Shapes.push_back (shape);

			NxShapeDesc* shapeDesc = shape->GetPhysXDescription();

			m_PhysXActor->createShape (*shapeDesc);
			m_PhysXActor->updateMassFromShapes (1.0f, m_PhysXActor->getMass());
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsActor::SetMass (float mass)
		{
			m_PhysXActor->setMass(mass);
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_PhysicsActor::GetMass () const
		{
			return m_PhysXActor->getMass();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsActor::AttachNode(DSM::C_DSMSceneNodeEntity* node)
		{
			m_Node = node;

			if (node)
			{
				CORE::C_Vector3D<float> vecPos = m_Node->GetPositionWorld();

				m_PhysXActor->setGlobalPosition(C_PhysXConvert::ConvertVector3ToNxVec3 (vecPos));
				m_PhysXActor->setGlobalOrientation(C_PhysXConvert::ConvertQuaternionToNxQuat (m_Node->GetRotationLocal()));

			}	
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsActor::Update()
		{
			// No node attached, so return
			if (!m_Node)
				return;

			// Re-enable collisions if necessary
			if (m_Node->IsEnabled() && m_PhysXActor->readActorFlag(NX_AF_DISABLE_COLLISION))
				m_PhysXActor->clearActorFlag(NX_AF_DISABLE_COLLISION);

			// We cannot update other parameters, so we are done
			if (m_Type != PAT_Kinematic && m_Type != PAT_Dynamic)
				return;

			if (m_Type != PAT_Kinematic)
			{
				CORE::C_Vector3D<float> vecPos = C_PhysXConvert::ConvertNxVec3ToVector3 (m_PhysXActor->getGlobalPosition());

				m_Node->SetPosition(vecPos);
				m_Node->SetRotation(C_PhysXConvert::ConvertNxQuatToQuaternion(m_PhysXActor->getGlobalOrientationQuat()));
			}
			else
			{
				CORE::C_Vector3D<float> vecPos = m_Node->GetPositionWorld();
				CORE::C_Quaternion quatRot = m_Node->GetRotationWorld();

				m_PhysXActor->setGlobalPosition(C_PhysXConvert::ConvertVector3ToNxVec3(vecPos));
				m_PhysXActor->setGlobalOrientation(C_PhysXConvert::ConvertQuaternionToNxQuat(quatRot));
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsActor::SetShapeMaterial (unsigned int index, C_PhysicsMaterial* material)
		{
			m_PhysXActor->getShapes()[index]->setMaterial(material->GetMaterialIndex());
			m_Shapes[index]->SetMaterial (material);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PhysicsMaterial* C_PhysicsActor::GetShapeMaterial (unsigned int index)
		{
			return m_Shapes[index]->GetMaterial();
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace PHYSICS
}	//	namespace REDEEMER
