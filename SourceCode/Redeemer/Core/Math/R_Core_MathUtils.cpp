/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_MathUtils.cpp
**		Various mathematical functions
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Core_MathUtils.h"
#include "R_Core_Vector2D.h"
#include "R_Core_Vector3D.h"

namespace REDEEMER
{
	namespace CORE
	{
		//------------------------------------------------------------------------------------------------------------------------
		// Static members initialization
		const C_Vector2D<float> C_Vector2D<float>::VECTOR_UNIT_X(1.0f, 0.0f);
		const C_Vector2D<float> C_Vector2D<float>::VECTOR_UNIT_Y(0.0f, 1.0f);
		const C_Vector2D<float> C_Vector2D<float>::VECTOR_NEGATIVE_UNIT_X(-1.0f, 0.0f);
		const C_Vector2D<float> C_Vector2D<float>::VECTOR_NEGATIVE_UNIT_Y(0.0f, -1.0f);

		const C_Vector2D<float> C_Vector2D<float>::VECTOR_ZERO_VECTOR(0.0f, 0.0f);
		const C_Vector2D<float> C_Vector2D<float>::VECTOR_ONE_VECTOR(1.0f, 1.0f);

		const C_Vector3D<float> C_Vector3D<float>::VECTOR_UNIT_X(1.0f, 0.0f, 0.0f);
		const C_Vector3D<float> C_Vector3D<float>::VECTOR_UNIT_Y(0.0f, 1.0f, 0.0f);
		const C_Vector3D<float> C_Vector3D<float>::VECTOR_UNIT_Z(0.0f, 0.0f, 1.0f);
		const C_Vector3D<float> C_Vector3D<float>::VECTOR_NEGATIVE_UNIT_X(-1.0f, 0.0f, 0.0f);
		const C_Vector3D<float> C_Vector3D<float>::VECTOR_NEGATIVE_UNIT_Y(0.0f, -1.0f, 0.0f);
		const C_Vector3D<float> C_Vector3D<float>::VECTOR_NEGATIVE_UNIT_Z(0.0f, 0.0f, -1.0f);

		const C_Vector3D<float> C_Vector3D<float>::VECTOR_ZERO_VECTOR(0.0f, 0.0f, 0.0f);
		const C_Vector3D<float> C_Vector3D<float>::VECTOR_ONE_VECTOR(1.0f, 1.0f, 1.0f);		  

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		float C_MathUtils::Cos (const float& value)
		{
			return static_cast<float>(cos(value));
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_MathUtils::Sin (const float& value)
		{
			return static_cast<float>(sin(value));
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_MathUtils::Tan (const float& value)
		{
			return static_cast<float>(tan(value));
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_MathUtils::ArcCos (const float& value)
		{
			return static_cast<float>(acos(value));	
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_MathUtils::ArcSin (const float& value)
		{
			return static_cast<float>(asin(value));
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_MathUtils::ArcTan (const float& value)
		{
			return static_cast<float>(atan(value));
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_MathUtils::DegToRad (const float& value)
		{
			return (value * C_MathUtils::RAD_PER_DEG);
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_MathUtils::RadToDeg (const float& value)
		{
			return (value * C_MathUtils::DEG_PER_RAD);
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_MathUtils::Abs (const float& value)
		{
			return (((value) < 0) ? -(value) : (value));
		}

		//------------------------------------------------------------------------------------------------------------------------
											  
		int C_MathUtils::Floor (const float& value)
		{
			return static_cast<int>(floor(value));
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_MathUtils::Ceiling (const float& value)
		{	
			return static_cast<int>(ceilf(value));
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_MathUtils::Round (const float& value)
		{
			return ((value) > 0 ? static_cast<int>(value + 0.5f) : -static_cast<int>(0.5f - value));
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_MathUtils::FloatRound1000 (const float& value)
		{
			return static_cast<float>(Round (value * 10000) / 10000.0);
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_MathUtils::Sign (const float& value)
		{
			return (((value) < 0) ? -1 : 1);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_MathUtils::Swap (int& value1, int& value2)
		{
			value1 ^= value2; 
			value2 ^= value1; 
			value1 ^= value2;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_MathUtils::Odd (const int& value)
		{
			return (((value) & 1) ? true : false);
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_MathUtils::FloatToInt (const float& value)
		{
			/// Represents either float or integer
			typedef union
			{
				int i;
				float f;
			} INT_OR_FLOAT;

			INT_OR_FLOAT n;
			INT_OR_FLOAT bias;

			if (value > 0.0f)
				bias.i = (23 + 127) << 23;
			else
				bias.i = ((23 + 127) << 23) + (1 << 22);

			n.f = value;

			n.f += bias.f;
			n.i -= bias.i;

			return n.i;
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		const float C_MathUtils::PI				= 3.1415926535897932f;
		const double C_MathUtils::PI64			= 3.1415926535897932384626433832795028841971693993751;
		const float C_MathUtils::PI_DOUBLE		= 6.2831853071795865f;
		const float C_MathUtils::PI_HALF		= 1.5707963267948966f;
		const float C_MathUtils::PI_SQUARE		= 9.8696044010893586f;
		const float C_MathUtils::E				= 2.7182818284590452f;
		const float C_MathUtils::RAD_PER_DEG	= 0.0174532925199433f;
		const float C_MathUtils::DEG_PER_RAD	= 57.2957795130823069f;

		const float C_MathUtils::DEGTORAD = C_MathUtils::PI / 180.0f;
		const float C_MathUtils::RADTODEG   = 180.0f / C_MathUtils::PI;
		const double C_MathUtils::DEGTORAD64 = C_MathUtils::PI64 / 180.0;
		const double C_MathUtils::RADTODEG64 = 180.0 / C_MathUtils::PI64;   

		const int C_MathUtils::ROUNDING_ERROR_INT = 0;
		const float C_MathUtils::ROUNDING_ERROR_FLOAT = 0.000001f;
		const double C_MathUtils::ROUNDING_ERROR_DOUBLE = 0.00000001;

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace CORE
}	//	namespace REDEEMER

