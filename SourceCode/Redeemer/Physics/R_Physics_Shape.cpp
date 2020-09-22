/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Physics_Shape.cpp
**		Abstract class for various physics shapes
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Physics_Shape.h"

namespace REDEEMER
{
	namespace PHYSICS
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_PhysicsShape::C_PhysicsShape () :
			m_Material (0)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PhysicsShape::~C_PhysicsShape ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsShape::SetDimensions (const CORE::C_Vector3D<float>& dimensions)
		{
			m_DimensionsVector = dimensions * 0.5f;
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Vector3D<float>& C_PhysicsShape::GetDimensions()
		{
			return m_DimensionsVector;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsShape::SetLocalPosition (const CORE::C_Vector3D<float>& position)
		{
			m_LocalPositionVector = position;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsShape::SetMaterial (C_PhysicsMaterial* material)
		{
			m_Material = material;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PhysicsMaterial* C_PhysicsShape::GetMaterial() const
		{
			return m_Material;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace PHYSICS
}	//	namespace REDEEMER
