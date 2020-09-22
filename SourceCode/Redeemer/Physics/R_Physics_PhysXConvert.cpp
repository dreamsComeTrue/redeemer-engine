/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Physics_PhysXConvert.cpp
**		Utility for converting between PhysX types and engine's internals
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Physics_PhysXConvert.h"

#pragma warning(push)
#pragma warning(disable:4512)
#include "NxVec3.h"
#include <NxExtended.h>
#pragma warning(pop)

namespace REDEEMER
{
	namespace PHYSICS
	{
		//------------------------------------------------------------------------------------------------------------------------
		NxVec3 C_PhysXConvert::ConvertVector3ToNxVec3 (const CORE::C_Vector3D<float>& vector)
		{
			return NxVec3 (vector.X, vector.Y, vector.Z);
		}

		//------------------------------------------------------------------------------------------------------------------------

		NxExtendedVec3 C_PhysXConvert::ConvertVector3ToNxExtendedVec3(const CORE::C_Vector3D<float>& vector)
		{
			return NxExtendedVec3 (vector.X, vector.Y, vector.Z);
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Vector3D<float> C_PhysXConvert::ConvertNxVec3ToVector3 (const NxVec3& vector)
		{
			return CORE::C_Vector3D<float> (vector.x, vector.y, vector.z);
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Vector3D<float> C_PhysXConvert::ConvertNxExtendedVec3ToVector3 (const NxExtendedVec3& vector)
		{
			return CORE::C_Vector3D<float> (static_cast<float>(vector.x), static_cast<float>(vector.y), static_cast<float>(vector.z));
		}

		//------------------------------------------------------------------------------------------------------------------------

		NxMat34 C_PhysXConvert::ConvertMatrix4x4ToNxMat34(const CORE::C_Matrix<float>& matrix)
		{
			NxMat33 matRot (NxVec3(matrix._m11, matrix._m12, matrix._m13),
							NxVec3(matrix._m21, matrix._m22, matrix._m23),
							NxVec3(matrix._m31, matrix._m32, matrix._m33));

			NxVec3 vecTrans (matrix._m41, matrix._m42, matrix._m43);

			NxMat34 mat (matRot, vecTrans);

			return mat;
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Quaternion C_PhysXConvert::ConvertNxQuatToQuaternion (const NxQuat& quat)
		{
			return CORE::C_Quaternion (quat.x, quat.y, quat.z, quat.w);
		}

		//------------------------------------------------------------------------------------------------------------------------

		NxQuat C_PhysXConvert::ConvertQuaternionToNxQuat(const CORE::C_Quaternion& quat)
		{
			NxQuat result;
			result.setXYZW (quat.X, quat.Y, quat.Z, quat.W);

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace PHYSICS
}	//	namespace REDEEMER
