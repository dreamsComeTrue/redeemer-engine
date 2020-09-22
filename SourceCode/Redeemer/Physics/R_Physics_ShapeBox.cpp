/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Physics_ShapeBox.cpp
**		Box physics primitive
*/

//------------------------------------------------------------------------------------------------------------------------
#define NOMINMAX
#include "R_Physics_ShapeBox.h"
#include "R_Physics_PhysXConvert.h"
#include "..\\RedeemerEngine.h"

#pragma warning(push)
#pragma warning(disable:4512)
#include <NxBoxShapeDesc.h>
#pragma warning(pop)

namespace REDEEMER
{
	namespace PHYSICS
	{					   
		//------------------------------------------------------------------------------------------------------------------------
		C_PhysicsShapeBox::C_PhysicsShapeBox () :
			m_BoxDescription (new NxBoxShapeDesc ())
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PhysicsShapeBox::~C_PhysicsShapeBox ()
		{
			REDEEMER_SAFE_DELETE (m_BoxDescription);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsShapeBox::SetDimensions (const CORE::C_Vector3D<float>& dimensions)
		{
			C_PhysicsShape::SetDimensions(dimensions);

			m_BoxDescription->dimensions = C_PhysXConvert::ConvertVector3ToNxVec3 (dimensions * 0.5f);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicsShapeBox::SetLocalPosition (const CORE::C_Vector3D<float>& position)
		{
			C_PhysicsShape::SetLocalPosition (position);

			m_BoxDescription->localPose.t.x = position.X;
			m_BoxDescription->localPose.t.y = position.Y;
			m_BoxDescription->localPose.t.z = position.Z;
		}

		//------------------------------------------------------------------------------------------------------------------------

		NxShapeDesc* C_PhysicsShapeBox::GetPhysXDescription ()
		{
			return (static_cast<NxShapeDesc*>(m_BoxDescription));
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace PHYSICS
}	//	namespace REDEEMER
