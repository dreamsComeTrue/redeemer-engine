/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Physics_Material.cpp
**		Physical material describes what surfaces behaves when they are in contact with each other
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Physics_Material.h"
#include "R_Physics_SceneManager.h"

#pragma warning(push)
#pragma warning(disable:4512)
#include <NxMaterial.h>
#include <NxMaterialDesc.h>
#include <NxScene.h>
#pragma warning(pop)

namespace REDEEMER
{
	namespace PHYSICS
	{
		unsigned int C_PhysicsMaterial::s_MaterialCount = 1;

		//------------------------------------------------------------------------------------------------------------------------
		C_PhysicsMaterial::C_PhysicsMaterial (C_PhysicsSceneManager* creator) :
			m_MaterialIndex (s_MaterialCount++),
			m_Name (L""),
			m_Restitution (0.5f),
			m_StaticFriction (0.5f),
			m_DynamicFriction (0.5f),
			m_SceneManager (creator)
		{
			// Create material with default settings
			NxMaterialDesc materialDescriptor;
			materialDescriptor.restitution		= m_Restitution;
			materialDescriptor.staticFriction	= m_StaticFriction;
			materialDescriptor.dynamicFriction	= m_DynamicFriction;
			m_PhysXMaterial						= m_SceneManager->GetPhysXScene()->createMaterial (materialDescriptor);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PhysicsMaterial::~C_PhysicsMaterial ()
		{
			m_SceneManager->GetPhysXScene()->releaseMaterial(*m_PhysXMaterial);

			m_PhysXMaterial = NULL;
			m_SceneManager = NULL;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsMaterial::SetName (const std::wstring& name)
		{
			m_Name = name;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::wstring C_PhysicsMaterial::GetName () const
		{
			return m_Name;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsMaterial::SetRestitution (float restitution)
		{
			m_Restitution = restitution;

			m_PhysXMaterial->setRestitution(m_Restitution);
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_PhysicsMaterial::GetRestitution () const
		{
			return m_Restitution;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsMaterial::SetStaticFriction (float friction)
		{
			m_StaticFriction = friction;

			m_PhysXMaterial->setStaticFriction (m_StaticFriction);
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_PhysicsMaterial::GetStaticFriction() const
		{
			return m_StaticFriction;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsMaterial::SetDynamicFriction (float friction)
		{
			m_DynamicFriction = friction;

			m_PhysXMaterial->setDynamicFriction (m_DynamicFriction);
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_PhysicsMaterial::GetDynamicFriction() const
		{
			return m_DynamicFriction;
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned int C_PhysicsMaterial::GetMaterialIndex() const
		{
			return m_MaterialIndex;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace PHYSICS
}	//	namespace REDEEMER

