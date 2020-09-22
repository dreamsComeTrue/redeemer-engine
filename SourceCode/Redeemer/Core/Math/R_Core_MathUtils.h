/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_MathUtils.h
**		Various mathematical functions
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_CORE_MATHUTILS_H_
#define _R_CORE_MATHUTILS_H_

#include <cmath>

namespace REDEEMER
{
	namespace CORE
	{
		/*!
		**	\brief This class provides various mathematical routines
		*/
		class C_MathUtils
		{
		public:
			/*!
			**	Calculates the cos of a value
			**	\param value value to calculate from
			**	\return calculated cos value
			*/
			static float Cos (const float& value);

			/*!
			**	Calculates the sin of a value
			**	\param value value to calculate from
			**\return calculated sin value
			*/
			static float Sin (const float& value);

			/*!
			**	Calculates the tan of a value
			**	\param value value to calculate from
			**	\return calculated tan value
			*/	
			static float Tan (const float& value);

			/*!
			**	Calculates the arccos of a value
			**	\param value value to calculate from
			**	\return calculated arccos value
			*/
			static float ArcCos (const float& value);

			/*!
			**	Calculates the arcsin of a value
			**	\param value value to calculate from
			**	\return calculated arcsin value
			*/
			static float ArcSin (const float& value);

			/*!
			**	Calculates the arctan of a value
			**	\param value value to calculate from
			**	\return calculated arctan value
			*/
			static float ArcTan (const float& value);

			/*!
			**	Converts a degree value to a rad value
			**	\param value value to convert
			**	\return converted rad value
			*/
			static float DegToRad (const float& value);

			/*!
			**	Converts a rad value to a degree value
			**	\param value value to convert
			**	\return converted deg value
			*/
			static float RadToDeg (const float& value);

			/*!
			**	Calculates the absolute value
			**	\param value value to calculate from
			**	\return absolute value
			*/
			static float Abs (const float& value);
												  
			/*!
			**	Calculates the floor value
			**	\param value value to calculate from
			**	\return floor value
			*/
			static int Floor (const float& value);

			/*!
			**	Calculates the ceiling value
			**	\param value value to calculate from
			**	\return ceiling value
			*/
			static int Ceiling (const float& value);

			/*!
			**	Calculates the round value
			**	\param value value to calculate from
			**	\return round value
			*/
			static int Round (const float& value);

			/*!
			**	Calculates the round value
			**	\param value value to calculate from
			**	\return round value
			*/
			static float FloatRound1000 (const float& value);

			/*!
			**	Calculates the signed value
			**	\param value value to calculate from
			**	\return signed value (-1 = signed, 0 = not signed)
			*/	
			static int Sign (const float& value);

			/*! Returns minimum of two values. 
			**	\param value1 first value to calculate from 
			**	\param value2 second value to calculate from 
			**	\return minimum value
			*/
			template<class T> static const T& Min (const T& value1, const T& value2)
			{
				return value1 < value2 ? value1 : value2;
			}

			/*! Returns minimum of two values. 
			**	\param value1 first value to calculate from 
			**	\param value2 second value to calculate from 
			**	\param value3 third value to calculate from 
			**	\return minimum value
			*/
			template<class T> static const T& Min (const T& value1, const T& value2, const T& value3)
			{
				return value1 < value2 ? Min(value1, value3) : Min(value2, value3);
			}

			/*!
			**	Calculates the maximum value between 2 values
			**	\param value1 first value to calculate from 
			**	\param value2 second value to calculate from 
			**	\return maximum value
			*/				  
			template<class T> static const T& Max (const T& value1, const T& value2)
			{
				return value1 < value2 ? value2 : value1;
			}

			/*!
			**	Calculates the maximum value between 2 values
			**	\param value1 first value to calculate from 
			**	\param value2 second value to calculate from 
			**	\param value3 third value to calculate from 
			**	\return maximum value
			*/		
			template<class T> static const T& Max (const T& value1, const T& value2, const T& value3)
			{
				return value1 < value2 ? Max(value2, value3) : Max(value1, value3);
			}

			/*!
			**	Swaps two int values rapidly using masks
			**	\param value1 first value to switch
			**	\param value2 second value to switch
			*/
			static void Swap (int& value1, int& value2);

			/*!
			**	Identifies if a value is odd
			**	\param value value to check if odd or not
			**	\return odd (true) or not (false)
			*/
			static bool Odd (const int& value);

			/*! Returns linear interpolation of a and b with ratio t
			**	\return: a if t==0, b if t==1, and the linear interpolation else
			*/
			template<class T> static T Lerp (const T& a, const T& b, const float t)
			{
				return (T)(a*(1.f-t)) + (b*t);
			}

			/*! Clamps a value between low and high
			*/
			template <class T> static const T Clamp (const T& value, const T& low, const T& high)
			{
				return Min (Max (value, low), high);
			}

			/*!	Returns if a equals b, taking possible rounding errors into account
			*/
			static bool Equals (const double a, const double b, const double tolerance = ROUNDING_ERROR_DOUBLE)
			{
				return (a + tolerance >= b) && (a - tolerance <= b);
			}

			/*!	Returns if a equals b, taking possible rounding errors into account
			*/
			static bool Equals (const float a, const float b, const float tolerance = ROUNDING_ERROR_FLOAT)
			{
				return (a + tolerance >= b) && (a - tolerance <= b);
			}

			/*!	Returns if a equals zero, taking rounding errors into account
			*/
			static bool IsZero (const double a, const double tolerance = ROUNDING_ERROR_DOUBLE)
			{
				return fabs (a) <= tolerance;
			}

			/*!	Returns if a equals zero, taking rounding errors into account
			*/
			static bool IsZero (const float a, const float tolerance = ROUNDING_ERROR_FLOAT)
			{
				return fabsf (a) <= tolerance;
			}

			/*!	Returns if a equals NOT zero, taking rounding errors into account
			*/
			static bool IsNotZero (const float a, const float tolerance = ROUNDING_ERROR_FLOAT)
			{
				return fabsf (a) > tolerance;
			}

			/*! Calculate: 1 / x
			*/
			static float Reciprocal (const float f)
			{
				return 1.f / f;
			}

			/*! Calculate: 1 / x
			*/
			static  double Reciprocal (const double f)
			{
				return 1.0 / f;
			}

			/*!	Calculate: 1 / sqrt ( x )
			*/
			static double ReciprocalSquareRoot(const double x)
			{
				return 1.0 / sqrt(x);
			}

			/*! Calculate: 1 / sqrtf ( x )
			*/
			static float ReciprocalSquareRoot(const float f)
			{	 
				return 1.f / sqrtf(f);
			}

			/*! Calculate: 1 / sqrtf( x )
			*/
			static int ReciprocalSquareRoot(const int x)
			{
				return static_cast<int>(ReciprocalSquareRoot(static_cast<float>(x)));
			}

			/*!	Converts float to int
			*/
			static int FloatToInt (const float& value);

		public:
			static const float PI;				///<	PI number
			static const double PI64;			///<	PI double number
			static const float PI_DOUBLE;		///<	2 * PI number
			static const float PI_HALF;			///<	PI / 2 number
			static const float PI_SQUARE;		///<	PI * PI number
			static const float E;				///<	E number
			static const float RAD_PER_DEG;		///<	RAD_PER_DEG number
			static const float DEG_PER_RAD;		///<	DEG_PER_RAD number

			static const float DEGTORAD;		///<	C_MathUtils::PI / 180.0f
			static const float RADTODEG;		///<	180.0f / C_MathUtils::PI
			static const double DEGTORAD64;		///<	C_MathUtils::PI64 / 180.0
			static const double RADTODEG64;		///<	180.0 / C_MathUtils::PI64

			/*! Rounding error constant often used when comparing float values.
			*/
			static const int ROUNDING_ERROR_INT;		///<	0
			static const float ROUNDING_ERROR_FLOAT;	///<	0.000001f
			static const double ROUNDING_ERROR_DOUBLE;	///<	0.00000001
		};

	}	//	namespace CORE
}	//	namespace REDEEMER

#endif	//	_R_CORE_MATHUTILS_H_
