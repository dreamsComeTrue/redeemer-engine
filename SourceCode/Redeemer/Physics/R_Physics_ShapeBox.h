/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Physics_ShapeBox.h
**		Box physics primitive
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_PHYSICS_SHAPEBOX_H_
#define _R_PHYSICS_SHAPEBOX_H_

#include "R_Physics_Shape.h"

class NxBoxShapeDesc;

namespace REDEEMER
{
	namespace PHYSICS
	{					   
		/*!	\brief	Box physics primitive
		*/
		class C_PhysicsShapeBox : public C_PhysicsShape
		{
		public:
			/*!	Constructor
			*/
			C_PhysicsShapeBox ();

			/*!	Destructor
			*/
			virtual ~C_PhysicsShapeBox ();

			/*!	Sets new dimensions
			*/
			virtual void SetDimensions (const CORE::C_Vector3D<float>& dimensions);

			/*!	Sets local position of the shape.
			*/
			virtual void SetLocalPosition (const CORE::C_Vector3D<float>& position);

			/*!	Returns PhysX shape description.
			*/
			virtual NxShapeDesc* GetPhysXDescription ();

		private:			
			NxBoxShapeDesc*		m_BoxDescription;	///<	PhysX description
		};

	}	//	namespace PHYSICS
}	//	namespace REDEEMER

#endif	//	_R_PHYSICS_SHAPEBOX_H_
