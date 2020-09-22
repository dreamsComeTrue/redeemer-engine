/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Physics_PhysXConvert.h
**		Utility for converting between PhysX types and engine's internals
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_PHYSICS_PHYSXCONVERT_H_
#define _R_PHYSICS_PHYSXCONVERT_H_

#include "..\\Core\\Math\\R_Core_Vector3D.h"
#include "..\\Core\\Math\\R_Core_Matrix.h"
#include "..\\Core\\Math\\R_Core_Quaternion.h"

class NxVec3;
struct NxExtendedVec3;
class NxMat34;
class NxQuat;

namespace REDEEMER
{
	namespace PHYSICS
	{
		class C_PhysXConvert
		{
		public:
			/*!	Converts engine's CORE::C_Vector3D<float> to NxVec3 of PhysX.
			*/
			static NxVec3 ConvertVector3ToNxVec3 (const CORE::C_Vector3D<float>& vector);

			/*!	Converts engine's CORE::C_Vector3D<float> to NxExtendedVec3 of PhysX.
			*/
			static NxExtendedVec3 ConvertVector3ToNxExtendedVec3 (const CORE::C_Vector3D<float>& vector);

			/*!	Converts PhysX NxVec3 into engine's CORE::C_Vector3D<float>.
			*/
			static CORE::C_Vector3D<float> ConvertNxVec3ToVector3 (const NxVec3& vector);

			/*!	Converts PhysX NxExtendedVec3 into engine's CORE::C_Vector3D<float>.
			*/
			static CORE::C_Vector3D<float> ConvertNxExtendedVec3ToVector3 (const NxExtendedVec3& vector);

			/*!	Converts engine's CORE::C_Matrix<float> to NxMat34 of PhysX.
			*/
			static NxMat34 ConvertMatrix4x4ToNxMat34(const CORE::C_Matrix<float>& matrix);

			/*!	Converts PhysX NxQuat into engine's Quaternion.
			*/
			static CORE::C_Quaternion ConvertNxQuatToQuaternion (const NxQuat& quat);

			/*!	Converts engine's Quaternion to NxQuat of PhysX.
			*/
			static NxQuat ConvertQuaternionToNxQuat(const CORE::C_Quaternion& quat);
		};

	}	//	namespace PHYSICS
}	//	namespace REDEEMER

#endif	//	_R_PHYSICS_PHYSXCONVERT_H_
