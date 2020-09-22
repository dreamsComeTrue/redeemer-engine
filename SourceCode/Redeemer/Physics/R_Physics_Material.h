/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Physics_Material.h
**		Physical material describes what surfaces behaves when they are in contact with each other
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_PHYSICS_MATERIAL_H_
#define _R_PHYSICS_MATERIAL_H_

#include <string>

class NxMaterial;

namespace REDEEMER
{
	namespace PHYSICS
	{
		class C_PhysicsSceneManager;

		/*!	\brief	Physical material describes what surfaces behaves when they are in contact with each other
		*/
		class C_PhysicsMaterial
		{
		public:
			/*!	Constructor
			*/
			C_PhysicsMaterial (C_PhysicsSceneManager* creator);

			/*!	Destructor
			*/
			virtual ~C_PhysicsMaterial ();

			/*!	Sets material name
			*/
			void SetName (const std::wstring& name = L"");

			/*!	Retrieves name
			*/
			std::wstring GetName () const;			

			/*!	Sets dynamic friction coefficient.
			**	\param restitution coefficient value. Valid values are in the range <0.0f, 1.0f>.
			*/
			void SetRestitution (float restitution);

			/*!	Returns restitution coefficient.
			*/
			float GetRestitution () const;

			/*!	Sets static friction coefficient.
			**	\param friction coefficient value. Valid values are in the range <0.0f, INFINITY).
			*/
			void SetStaticFriction (float friction);

			/*!	Returns static friction coefficient.
			*/
			float GetStaticFriction() const;

			/*!	Sets dynamic friction coefficient.
			**	\param friction coefficient value. Valid values are in the range <0.0f, INFINITY).
			*/
			void SetDynamicFriction (float friction);

			/*!	Returns dynamic friction coefficient.
			*/
			float GetDynamicFriction() const;

			/*!	Gets next indexed material
			*/
			unsigned int GetMaterialIndex() const;

		private:
			std::wstring			m_Name;							///<	Material's name
			float					m_Restitution;					///<	Restitution
			float					m_StaticFriction;				///<	Static friction
			float					m_DynamicFriction;				///<	Dynamic friction
			NxMaterial*				m_PhysXMaterial;				///<	Pointer to the PhysX material
			C_PhysicsSceneManager*	m_SceneManager;					///<	Which scene created this material

			unsigned int			m_MaterialIndex;
			static unsigned int		s_MaterialCount;
		};

	}	//	namespace PHYSICS
}	//	namespace REDEEMER

#endif	//	_R_PHYSICS_MATERIAL_H_
