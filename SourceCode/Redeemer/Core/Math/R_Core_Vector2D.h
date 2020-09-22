/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_Vector2D.h
**		Contains all the mathematical operations for a 2D vector
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_CORE_VECTOR2D_H_
#define _R_CORE_VECTOR2D_H_

#include "R_Core_Vector3D.h"

namespace REDEEMER
{
	namespace CORE
	{
		/*!
		**	\brief Contains all the mathematical operations for a 2D vector
		*/
		template<typename T> class C_Vector2D
		{	
		public:	
			static const C_Vector2D<float> VECTOR_UNIT_X;
			static const C_Vector2D<float> VECTOR_UNIT_Y;
			static const C_Vector2D<float> VECTOR_UNIT_Z;
			static const C_Vector2D<float> VECTOR_NEGATIVE_UNIT_X;
			static const C_Vector2D<float> VECTOR_NEGATIVE_UNIT_Y;
			static const C_Vector2D<float> VECTOR_NEGATIVE_UNIT_Z;
			static const C_Vector2D<float> VECTOR_ZERO_VECTOR;
			static const C_Vector2D<float> VECTOR_ONE_VECTOR;

			/*!
			**	Default constructor of the C_Vector2D class
			*/
			C_Vector2D () :
			  X ((T)0),
			  Y ((T)0)
			{
				//	EMPTY
			}

			/*!
			**	Default constructor of the C_Vector2D class
			**	\param x x coordinate value
			**	\param y y coordinate value
			*/
			C_Vector2D (const T& x, const T& y) :
				X (x),
				Y (y)
			{
				//	EMPTY
			}

			/*!
			**	Copy constructor of the Vector2D class
			**	\param pVector vector to copy
			*/
			C_Vector2D(const C_Vector2D<T>* pVector) :
				X (pVector->X),
				Y (pVector->Y)
			{
				//	EMPTY
			}

			/*!
			**	Copy constructor of the Vector2D class
			**	\param pVector vector to copy
			*/
			C_Vector2D(const C_Vector2D<T>& pVector) :
				X (pVector.X),
				Y (pVector.Y)
			{
				//	EMPTY
			}
			
			/*!
			**	Magnitude of the vector
			**	\return magnitude of the vector
			*/
			const T GetMagnitude() const
			{
				return static_cast<T> (sqrt (X * X + Y * Y));
			}

			/*!
			**	Dot product of the two vectors
			**	\param vector second vector on which the dot product is applied
			**	\return dot product value
			*/
			const T GetDot(const C_Vector2D<T>& vector) const
			{
				return (X * vector.X + Y * vector.Y);
			}

			/*!
			**	Angle between the two vectors
			**	\param vector second vector of the pair, where the angle is calculated
			**	\return angle between the two vectors
			*/
			const T GetAngle(const C_Vector2D<T>& vector) const
			{
				const T magnitudesProduct = GetMagnitude() * vector.GetMagnitude();

				if (magnitudesProduct != (T)0)
					return static_cast<float>(C_Math::ArcCos (GetDot (vector) / magnitudesProduct));
				else
					return 0.0f;
			}

			/*!
			**	Calculates the polar angle of the vector
			**	\return angle value
			*/
			const T GetPolarAngle() const
			{
				const C_Vector2D<T> reference (1, 0);

				return static_cast<T>(GetAngle (reference));
			}

			/*!
			**	Calculates the distance between two vectors
			**	\param vector vector to calculate the distance to
			**	\return distance between the two vectors
			*/
			const T GetDistance(C_Vector2D<T>& vector) const
			{
				C_Vector2D<T> tmp = *this;
				tmp -= vector;

				return tmp.GetMagnitude();
			}

			/*!
			**	Normalizes an INSTANCE of the vector
			**	\return reference to a new normalized Vector2D
			*/
			C_Vector2D<T> GetNormalized() const
			{
				const T magnitude = GetMagnitude();

				if (magnitude != (T)0)
					return C_Vector2D<T>(X / magnitude, Y / magnitude);
				else
					return (*this);
			}

			/*!
			**	Inverts an INSTANCE of the vector
			**	\return reference to a new inverted Vector2D
			*/
			C_Vector2D<T> GetInverted() const
			{
				return C_Vector2D<T> (-X, -Y);
			}

			/*!
			**	Cross product of the two Vector2D, outputs a Vector3D
			**	\param vector second vector of the pair
			**	\return new perpendicular Vector3D
			*/
			C_Vector3D<T> GetCross(const C_Vector2D<T>& vector) const
			{
				return C_Vector3D<T> (0, 0, X * vector.Y - Y * vector.X);
			}

			/*!
			**	Projects an INSTANCE of the vector onto another
			**	\param vector vector on which the projection is done
			**	\return reference to a new normalized Vector2D
			*/
			C_Vector2D<T> GetProjected(C_Vector2D<T>& vector) const
			{
				const T selfDot = vector.GetDot (vector);

				if (selfDot != (T)0)
					return ((GetDot (vector) / (selfDot)) * vector);
				else
					return (*this);
			}
					
			/*!
			**	Changes the magnitude of the vector
			**	\param magnitude new magnitude value
			**	\return new vector
			*/
			C_Vector2D<T>& SetMagnitude(const T& magnitude)
			{
				const T currentMagnitude = GetMagnitude ();

				if (currentMagnitude != (T)0) 
				{
					const T magnitudeRatio = magnitude / currentMagnitude;	//Faster, but not nice.

					X *= magnitudeRatio;
					Y *= magnitudeRatio;
				}

				return (*this);
			}

			/*!
			**	Normalizes the vector
			**	\return reference to THIS normalized Vector2D
			*/
			C_Vector2D<T>& SetNormalized()
			{
				const T magnitude = GetMagnitude();

				if (magnitude != (T)0) 
				{
					X /= magnitude;
					Y /= magnitude;
				}

				return (*this);
			}

			/*!
			**	Inverts the vector
			**	\return reference to THIS inverted Vector2D
			*/
			C_Vector2D<T>& SetInverted()
			{
				X = -X;
				Y = -Y;

				return (*this);
			}

			/*!
			**	Sets the current vector as the projection on the param vector
			**	\param vector vector to project the first one on
			**	\return reference to THIS projected Vector2D
			*/
			C_Vector2D<T>& SetProjected(const C_Vector2D<T>& vector)
			{
				return SetProjected (*this, vector);
			}

			/*!
			**	Sets the current vector as the projection of the two vectors
			**	\param vector1 first vector to project onto, reference vector
			**	\param vector2 second vector, projected vector
			**	\return reference to THIS Vector2D
			*/
			C_Vector2D<T>& SetProjected(const C_Vector2D<T>& vector1, const C_Vector2D<T>& vector2)
			{
				const T selfDot = vector2.GetDot(vector2);

				if (selfDot != (T)0)
					*this = (((vector1.GetDot (vector2)) / (selfDot)) * vector2);

				return (*this);
			}
							
			/*!
			**	ADD operator replacement
			**	\param vector vector to sum
			**	\return summation result
			*/
			C_Vector2D<T> GetAdd(const C_Vector2D<T>& vector) const
			{
				return C_Vector2D<T> (this->X + vector.X, this->Y + vector.Y);
			}

			/*!
			**	ADD_EQUAL operator replacement
			**	\param vector vector to add to the current vector
			**	\return summation result
			*/
			C_Vector2D<T>& SetAdd(const C_Vector2D<T>& vector)
			{
				this->X += vector.X;
				this->Y += vector.Y;

				return (*this);
			}

			/*!
			**	ADD_EQUAL operator replacement
			**	\param vector1 first vector to add 
			**	\param vector2 second vector to add
			**	\return summation result
			*/
			C_Vector2D<T>& SetAdd(const C_Vector2D<T>& vector1, const C_Vector2D<T>& vector2)
			{
				this->X = vector1.X + vector2.X;
				this->Y = vector1.Y + vector2.Y;

				return (*this);
			}

			/*!
			**	SUB operator replacement
			**	\param vector vector to sub
			**	\return substraction result
			*/
			C_Vector2D<T> GetSub(const C_Vector2D<T>& vector) const
			{
				return C_Vector2D<T> (this->X - vector.X, this->Y - vector.Y);
			}

			/*!
			**	SUB_EQUAL operator replacement
			**	\param vector vector to sub to the current vector
			**	\return substraction result
			*/
			C_Vector2D<T>& SetSub(const C_Vector2D<T>& vector)
			{
				this->X -= vector.X;
				this->Y -= vector.Y;

				return (*this);
			}

			/*!
			**	SUB_EQUAL operator replacement
			**	\param vector1 first vector to sub 
			**	\param vector2 second vector to sub
			**	\return substraction result
			*/
			C_Vector2D<T>& SetSub(const C_Vector2D<T>& vector1, const C_Vector2D<T>& vector2)
			{
				this->X = vector1.X - vector2.X;
				this->Y = vector1.Y - vector2.Y;

				return (*this);
			}

			/*!
			**	MULTIPLY operator replacement
			**	\param scalar scalar value multiplied by the vector
			**	\return scaled vector
			*/
			C_Vector2D<T> GetMul(const float& scalar) const
			{
				return C_Vector2D<T> (this->X * scalar, this->Y * scalar);
			}

			/*!
			**	MULTIPLY_EQUAL operator replacement
			**	\param scalar scalar multiplied by the current vector
			**	\return scaled vector
			*/
			C_Vector2D<T>& SetMul(const float& scalar)
			{
				this->X *= scalar;
				this->Y *= scalar;

				return (*this);
			}

			/*!
			**	MULTIPLY_EQUAL operator replacement
			**	\param vector first vector to multiply 
			**	\param scalar scalar to multiply to the first vector
			**	\return scaled vector
			*/
			C_Vector2D<T>& SetMul(const C_Vector2D<T>& vector, const float& scalar)
			{
				this->X = vector.X * scalar;
				this->Y = vector.Y * scalar;

				return (*this);
			}

			/*!
			**	DIVIDE operator replacement
			**	\param scalar scalar value that divides the vector
			**	\return scaled vector
			*/
			C_Vector2D<T> GetDiv(const float& scalar) const
			{
				return C_Vector2D<T>(this->X / scalar, this->Y / scalar);
			}

			/*!
			**	DIVIDE_EQUAL operator replacement
			**	\param scalar scalar multiplied by the current vector
			**	\return scaled vector
			*/
			C_Vector2D<T>& SetDiv(const float& scalar)
			{
				this->X /= scalar;
				this->Y /= scalar;

				return (*this);
			}

			/*!
			**	MULTIPLY_EQUAL operator replacement
			**	\param vector first vector to multiply 
			**	\param scalar scalar to multiply to the first vector
			**	\return scaled vector
			*/
			C_Vector2D<T>& SetDiv(const C_Vector2D<T>& vector, const float& scalar)
			{
				this->X = vector.X / scalar;
				this->Y = vector.Y / scalar;

				return (*this);
			}

			/*!
			**	EQUAL_EQUAL operator
			**	\param vector vector to compare
			**	\return equal value
			*/
			const bool equals(const C_Vector2D<T>& vector) const
			{
				return ((this->X == vector.X) && (this->Y == vector.Y));
			}

			/*!
			**	NOT_EQUAL operator
			**	\param vector vector to compare
			**	\return not equal value
			*/
			const bool notEquals(const C_Vector2D<T>& vector) const
			{
				return ((this->X != vector.X) && (this->Y != vector.Y));
			}
						
			/*!
			**	ADD operator
			**	\param vector vector to sum
			**	\return summation result
			*/
			C_Vector2D<T> operator + (const C_Vector2D<T>& vector) const
			{
				return C_Vector2D<T> (X + vector.X, Y + vector.Y);
			}

			/*!
			**	SUB operator
			**	\param vector vector to sub
			**	\return substraction result
			*/
			C_Vector2D<T> operator - (const C_Vector2D<T>& vector) const
			{
				return C_Vector2D<T> (X - vector.X, Y - vector.Y);
			}

			/*!
			**	MULTIPLY operator
			**	\param scalar scalar value multiplied by the vector
			**	\return scaled vector
			*/
			C_Vector2D<T> operator * (const float& scalar) const
			{
				return C_Vector2D<T> (X * scalar, Y * scalar);
			}

			/*!
			**	DIVIDE operator
			**	\param scalar scalar value divided by the vector
			**	\return scaled vector
			*/
			C_Vector2D<T> operator / (const float& scalar) const
			{
				if (scalar !=(T)0)
					return C_Vector2D<T>(X / scalar, Y / scalar);
				else
					return (*this);
			}

			/*!
			**	ADD_EQUAL operator
			**	\param vector vector to add to the current vector
			**	\return summation result
			*/
			C_Vector2D<T>& operator += (const C_Vector2D<T>& vector)
			{
				X += vector.X;
				Y += vector.Y;

				return (*this);
			}

			/*!
			**	SUB_EQUAL operator
			**	\param vector vector to sub to the current vector
			**	\return substraction result
			*/
			C_Vector2D<T>& operator -= (const C_Vector2D<T>& vector)
			{
				X -= vector.X;
				Y -= vector.Y;

				return *this;
			}

			/*!
			**	MULTIPLY_EQUAL operator
			**	\param scalar scalar multiplied by the current vector
			**	\return scaled result
			*/
			C_Vector2D<T>& operator *= (const float& scalar)
			{
				X *= scalar;
				Y *= scalar;

				return *this;
			}

			/*!
			**	DIVIDE_EQUAL operator
			**	\param scalar scalar divided by the current vector
			**	\return scaled result
			*/
			C_Vector2D<T>& operator /= (const float& scalar)
			{
				if (scalar != (T)0) 
				{
					X /= scalar;
					Y /= scalar;
				}

				return (*this);
			}

			/*!
			**	SIGN operator
			**	\return signed result
			*/
			C_Vector2D<T> operator - () const
			{
				return C_Vector2D<T>(-X, -Y);
			}

			/*!
			**	EQUAL_EQUAL operator
			**	\param vector vector to compare
			**	\return equal value
			*/
			const bool operator == (C_Vector2D<T>& vector) const
			{
				return ((X == vector.X) && (Y == vector.Y)) ? true : false;
			}

			/*!
			**	NOT_EQUAL operator
			**	\param vector vector to compare
			**	\return not equal value
			*/
			const bool operator != (C_Vector2D<T>& vector) const
			{
				return ((X != vector.X) && (Y != vector.Y)) ? true : false;
			}

		public:
			union
			{
				struct 
				{
					T X;	///<	X coordinate
					T Y;	///<	Y coordinate
				};

				struct  
				{
					T Width;	///<	X coordinate
					T Height;	///<	Y coordinate
				};
			};
		};

		/*!
		**	Global MULTIPLY operator
		**	\param scalar scalar value to multiply with the second parameter
		**	\param vector vector to multiply the scalar value with
		**	\return scaled vector instance
		*/
		template<typename T> inline C_Vector2D<T> operator * (float scalar, C_Vector2D<T>& vector) 
		{
			return C_Vector2D<T>(vector.X * scalar, vector.Y * scalar);
		}

		/*!
		**	Global DIVIDE operator
		**	\param scalar scalar value to divide with the second parameter
		**	\param vector vector to divide the scalar value with
		**	\return scaled vector instance
		*/
		template<typename T> inline C_Vector2D<T> operator / (float scalar, C_Vector2D<T>& vector) 
		{
			return C_Vector2D<T>(vector.X / scalar, vector.Y / scalar);
		}

		/**
		**	Global MULTIPLY operator
		**	\param scalar scalar value to multiply with the second parameter
		**	\param vector const vector to multiply the scalar value with
		**	\return scaled vector instance
		*/
		template<typename T> inline C_Vector2D<T> operator * (float scalar, const C_Vector2D<T>& vector) 
		{
			return C_Vector2D<T>(vector.X * scalar, vector.Y * scalar);
		}
	}	//	namespace CORE
}	//	namespace REDEEMER

#endif	//	_R_CORE_VECTOR2D_H_
