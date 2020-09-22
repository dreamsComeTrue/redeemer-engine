/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Physics_Shape.h
**		Abstract class for various physics shapes
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_PHYSICS_SHAPE_H_
#define _R_PHYSICS_SHAPE_H_

#include "..\\Core\\Math\\R_Core_Vector3D.h"

class NxShapeDesc;

namespace REDEEMER
{
	namespace PHYSICS
	{
		class C_PhysicsMaterial;
			
		/*!	Various types of shapes
		*/
		enum EPhysicsShapeType
		{
			PST_Box,
			PST_Plane,
		};

		/*!	\brief	Abstract class for various physics shapes
		*/
		class C_PhysicsShape
		{
		public:
			/*!	Constructor
			*/
			C_PhysicsShape ();

			/*!	Destructor
			*/
			virtual ~C_PhysicsShape ();

			/*!	Sets new dimensions
			*/
			virtual void SetDimensions (const CORE::C_Vector3D<float>& dimensions);

			/*!	Returns dimensions of the shape.
			*/
			CORE::C_Vector3D<float>& GetDimensions();

			/*!	Sets local position of the shape.
			*/
			virtual void SetLocalPosition (const CORE::C_Vector3D<float>& position);

			/*!	Sets material for this shape.
			*/
			virtual void SetMaterial (C_PhysicsMaterial* material);

			/*!	Returns material of this shape.
			*/
			virtual C_PhysicsMaterial* GetMaterial() const;

			/*!	Returns PhysX shape description.
			*/
			virtual NxShapeDesc* GetPhysXDescription () = 0;

		protected:			
			CORE::C_Vector3D<float> m_DimensionsVector;			///<	Dimensions of the shape
			CORE::C_Vector3D<float> m_LocalPositionVector;		///<	Position of shape in Actor's space.
			C_PhysicsMaterial*		m_Material;					///<	Pointer to the PhysicsMaterial object.
		};

	}	//	namespace PHYSICS
}	//	namespace REDEEMER

#endif	//	_R_PHYSICS_SHAPE_H_
