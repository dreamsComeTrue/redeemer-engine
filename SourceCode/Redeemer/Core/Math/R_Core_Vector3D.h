/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_Vector3D.h
**		Contains all the mathematical operations for a 3D vector
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_CORE_VECTOR3D_H_
#define _R_CORE_VECTOR3D_H_

namespace REDEEMER
{
	namespace CORE
	{
		/*!
		**	\brief Contains all the mathematical operations for a 3D vector
		*/
		template<typename T> class C_Vector3D
		{	
		public:	
			static const C_Vector3D<float> VECTOR_UNIT_X;
			static const C_Vector3D<float> VECTOR_UNIT_Y;
			static const C_Vector3D<float> VECTOR_UNIT_Z;
			static const C_Vector3D<float> VECTOR_NEGATIVE_UNIT_X;
			static const C_Vector3D<float> VECTOR_NEGATIVE_UNIT_Y;
			static const C_Vector3D<float> VECTOR_NEGATIVE_UNIT_Z;
			static const C_Vector3D<float> VECTOR_ZERO_VECTOR;
			static const C_Vector3D<float> VECTOR_ONE_VECTOR;

			/*!
			**	Default constructor of the C_Vector3D class
			*/
			C_Vector3D () :
			  X ((T)0),
			  Y ((T)0),
			  Z ((T)0)
			{
				//	EMPTY
			}

			/*!
			**	Default constructor of the C_Vector3D class
			**	\param x x coordinate value
			**	\param y y coordinate value
			**	\param z z coordinate value
			*/
			C_Vector3D (const T& x, const T& y, const T& z) :
				X (x),
				Y (y),
				Z (z)
			{
				//	EMPTY
			}

			/*!
			**	Copy constructor of the Vector2D class
			**	\param pVector vector to copy
			*/
			C_Vector3D(const C_Vector3D<T>* pVector) :
				X (pVector->X),
				Y (pVector->Y),
				Z (pVector->Z)
			{
				//	EMPTY
			}

			/*!
			**	Copy constructor of the Vector2D class
			**	\param pVector vector to copy
			*/
			C_Vector3D(const C_Vector3D<T>& pVector) :
				X (pVector.X),
				Y (pVector.Y),
				Z (pVector.Z)
			{
				//	EMPTY
			}
			
			/*!
			**	Magnitude of the vector
			**	\return magnitude of the vector
			*/
			const T GetMagnitude() const
			{
				return static_cast<T> (sqrt (X * X + Y * Y + Z * Z));
			}

			/*!
			**	Dot product of the two vectors
			**	\param vector second vector on which the dot product is applied
			**	\return dot product value
			*/
			const T GetDot(const C_Vector3D<T>& vector) const
			{
				return (X * vector.X + Y * vector.Y + Z * vector.Z);
			}

			/*!
			**	Angle between the two vectors
			**	\param vector second vector of the pair, where the angle is calculated
			**	\return angle between the two vectors
			*/
			const T GetAngle(const C_Vector3D<T>& vector) const
			{
				const T magnitudesProduct = GetMagnitude() * vector.GetMagnitude();

				if (magnitudesProduct != (T)0)
					return static_cast<float>(acos (GetDot (vector) / magnitudesProduct));
				else
					return 0.0f;
			}

			/*!
			**	Calculates the distance between two vectors
			**	\param vector vector to calculate the distance to
			**	\return distance between the two vectors
			*/
			const T GetDistance(C_Vector3D<T>& vector) const
			{
				C_Vector3D<T> tmp = *this;
				tmp -= vector;

				return tmp.GetMagnitude();
			}

			/*!
			**	Normalizes an INSTANCE of the vector
			**	\return reference to a new normalized Vector2D
			*/
			C_Vector3D<T> GetNormalized() const
			{
				const T magnitude = GetMagnitude();

				if (magnitude != (T)0)
					return C_Vector3D<T>(X / magnitude, Y / magnitude, Z / magnitude);
				else
					return (*this);
			}

			/*!
			**	Inverts an INSTANCE of the vector
			**	\return reference to a new inverted Vector2D
			*/
			C_Vector3D<T> GetInverted() const
			{
				return C_Vector3D<T> (-X, -Y, -Z);
			}

			/*!
			**	Cross product of the two Vector2D, outputs a Vector3D
			**	\param vector second vector of the pair
			**	\return new perpendicular Vector3D
			*/
			C_Vector3D<T> GetCross(const C_Vector3D<T>& vector) const
			{
				return C_Vector3D<T> (Y * vector.Z - Z * vector.Y, Z * vector.X - X * vector.Z, X * vector.Y - Y * vector.X);
			}

			/*!
			**	Projects an INSTANCE of the vector onto another
			**	\param vector vector on which the projection is done
			**	\return reference to a new normalized Vector2D
			*/
			C_Vector3D<T> GetProjected(C_Vector3D<T>& vector) const
			{
				const T selfDot = vector.GetDot (vector);

				if (selfDot != (T)0)
					return (vector * (GetDot(vector) / (selfDot)));
				else
					return (*this);
			}

			/*!
			**	Calculates the spherical coordinates of the vector (3D)
			**	\param angle1 first angle
			**	\param angle2 second angle
			**	\return magnitude of the vector
			*/
			const float GetSphericCoordinates (T& angle1, T& angle2) const 
			{
				const T magnitude = GetMagnitude ();
				const T squaredMagnitude = magnitude * magnitude;
				const T squaredY = Y * Y;
				const T ratio = static_cast<T>(squaredMagnitude - squaredY);

				if ((ratio != (T)0) && (magnitude != (T)0)) 
				{
					angle1 = static_cast<float>(acos (X / ratio));
					angle2 = static_cast<float>(acos (Y / magnitude));
				}

				return magnitude;
			}
					
			/*!
			**	Changes the magnitude of the vector
			**	\param magnitude new magnitude value
			**	\return new vector
			*/
			C_Vector3D<T>& SetMagnitude(const T& magnitude)
			{
				const T currentMagnitude = GetMagnitude ();

				if (currentMagnitude != (T)0) 
				{
					const T magnitudeRatio = magnitude / currentMagnitude;	//Faster, but not nice.

					X *= magnitudeRatio;
					Y *= magnitudeRatio;
					Z *= magnitudeRatio;
				}

				return (*this);
			}

			/*!
			**	Normalizes the vector
			**	\return reference to THIS normalized Vector2D
			*/
			C_Vector3D<T>& SetNormalized()
			{
				const T magnitude = GetMagnitude();

				if (magnitude != (T)0) 
				{
					X /= magnitude;
					Y /= magnitude;
					Z /= magnitude;
				}

				return (*this);
			}

			/*!
			**	Inverts the vector
			**	\return reference to THIS inverted Vector2D
			*/
			C_Vector3D<T>& SetInverted()
			{
				X = -X;
				Y = -Y;
				Z = -Z;

				return (*this);
			}

			/*!
			**	Cross product of the current vector and param, onto current vector
			**	\param vector vector with which the cross product is done
			**	\return reference to THIS crossed Vector3D
			*/
			C_Vector3D<T>& SetCross (const C_Vector3D<T>& vector)
			{
				*this = GetCross (vector);

				return (*this);
			}

			/*!
			**	Cross product of 2 param vectors, stored in current vector.
			**	\param vector1 first vector of the cross product
			**	\param vector2 second vector of the cross product
			**	\return reference to THIS crossed Vector3D
			*/
			C_Vector3D<T>& SetCross(const C_Vector3D<T>& vector1, const C_Vector3D<T>& vector2) 
			{
				*this = vector1.GetCross (vector2);

				return (*this);
			}

			/*!
			**	Sets the current vector as the projection on the param vector
			**	\param vector vector to project the first one on
			**	\return reference to THIS projected Vector2D
			*/
			C_Vector3D<T>& SetProjected(const C_Vector3D<T>& vector)
			{
				*this = GetProjected(vector);

				return (*this);
			}

			/*!
			**	Sets the current vector as the projection of the two vectors
			**	\param vector1 first vector to project onto, reference vector
			**	\param vector2 second vector, projected vector
			**	\return reference to THIS Vector2D
			*/
			C_Vector3D<T>& SetProjected(const C_Vector3D<T>& vector1, const C_Vector3D<T>& vector2)
			{
				*this = vector1.GetProjected(vector2);

				return (*this);
			}
							
			/*!
			**	ADD operator replacement
			**	\param vector vector to sum
			**	\return summation result
			*/
			C_Vector3D<T> GetAdd(const C_Vector3D<T>& vector) const
			{
				return C_Vector3D<T> (this->X + vector.X, this->Y + vector.Y, this->Z + vector.Z);
			}

			/*!
			**	ADD_EQUAL operator replacement
			**	\param vector vector to add to the current vector
			**	\return summation result
			*/
			C_Vector3D<T>& SetAdd(const C_Vector3D<T>& vector)
			{
				this->X += vector.X;
				this->Y += vector.Y;
				this->Z += vector.Z;

				return (*this);
			}

			/*!
			**	ADD_EQUAL operator replacement
			**	\param vector1 first vector to add 
			**	\param vector2 second vector to add
			**	\return summation result
			*/
			C_Vector3D<T>& SetAdd(const C_Vector3D<T>& vector1, const C_Vector3D<T>& vector2)
			{
				this->X = vector1.X + vector2.X;
				this->Y = vector1.Y + vector2.Y;
				this->Z = vector1.Z + vector2.Z;

				return (*this);
			}

			/*!
			**	SUB operator replacement
			**	\param vector vector to sub
			**	\return substraction result
			*/
			C_Vector3D<T> GetSub(const C_Vector3D<T>& vector) const
			{
				return C_Vector3D<T> (this->X - vector.X, this->Y - vector.Y, this->Z - vector.Z);
			}

			/*!
			**	SUB_EQUAL operator replacement
			**	\param vector vector to sub to the current vector
			**	\return substraction result
			*/
			C_Vector3D<T>& SetSub(const C_Vector3D<T>& vector)
			{
				this->X -= vector.X;
				this->Y -= vector.Y;
				this->Z -= vector.Z;

				return (*this);
			}

			/*!
			**	SUB_EQUAL operator replacement
			**	\param vector1 first vector to sub 
			**	\param vector2 second vector to sub
			**	\return substraction result
			*/
			C_Vector3D<T>& SetSub(const C_Vector3D<T>& vector1, const C_Vector3D<T>& vector2)
			{
				this->X = vector1.X - vector2.X;
				this->Y = vector1.Y - vector2.Y;
				this->Z = vector1.Z - vector2.Z;

				return (*this);
			}

			/*!
			**	MULTIPLY operator replacement
			**	\param scalar scalar value multiplied by the vector
			**	\return scaled vector
			*/
			C_Vector3D<T> GetMul(const float& scalar) const
			{
				return C_Vector3D<T> (this->X * scalar, this->Y * scalar, this->Z * scalar);
			}

			/*!
			**	MULTIPLY_EQUAL operator replacement
			**	\param scalar scalar multiplied by the current vector
			**	\return scaled vector
			*/
			C_Vector3D<T>& SetMul(const float& scalar)
			{
				this->X *= scalar;
				this->Y *= scalar;
				this->Z *= scalar;

				return (*this);
			}

			/*!
			**	MULTIPLY_EQUAL operator replacement
			**	\param vector first vector to multiply 
			**	\param scalar scalar to multiply to the first vector
			**	\return scaled vector
			*/
			C_Vector3D<T>& SetMul(const C_Vector3D<T>& vector, const float& scalar)
			{
				this->X = vector.X * scalar;
				this->Y = vector.Y * scalar;
				this->Z = vector.Z * scalar;

				return (*this);
			}

			/*!
			**	DIVIDE operator replacement
			**	\param scalar scalar value that divides the vector
			**	\return scaled vector
			*/
			C_Vector3D<T> GetDiv(const float& scalar) const
			{
				return C_Vector3D<T>(this->X / scalar, this->Y / scalar, this->Z / scalar);
			}

			/*!
			**	DIVIDE_EQUAL operator replacement
			**	\param scalar scalar multiplied by the current vector
			**	\return scaled vector
			*/
			C_Vector3D<T>& SetDiv(const float& scalar)
			{
				this->X /= scalar;
				this->Y /= scalar;
				this->Z /= scalar;

				return (*this);
			}

			/*!
			**	MULTIPLY_EQUAL operator replacement
			**	\param vector first vector to multiply 
			**	\param scalar scalar to multiply to the first vector
			**	\return scaled vector
			*/
			C_Vector3D<T>& SetDiv(const C_Vector3D<T>& vector, const float& scalar)
			{
				this->X = vector.X / scalar;
				this->Y = vector.Y / scalar;
				this->Z = vector.Z / scalar;

				return (*this);
			}

			/*!
			**	EQUAL_EQUAL operator
			**	\param vector vector to compare
			**	\return equal value
			*/
			const bool equals(const C_Vector3D<T>& vector) const
			{
				return ((this->X == vector.X) && (this->Y == vector.Y) && (this->Z == vector.Z));
			}

			/*!
			**	NOT_EQUAL operator
			**	\param vector vector to compare
			**	\return not equal value
			*/
			const bool notEquals(const C_Vector3D<T>& vector) const
			{
				return ((this->X != vector.X) && (this->Y != vector.Y) && (this->Z != vector.Z));
			}
						
			/*!
			**	ADD operator
			**	\param vector vector to sum
			**	\return summation result
			*/
			C_Vector3D<T> operator + (const C_Vector3D<T>& vector) const
			{
				return C_Vector3D<T> (X + vector.X, Y + vector.Y, Z + vector.Z);
			}

			/*!
			**	SUB operator
			**	\param vector vector to sub
			**	\return substraction result
			*/
			C_Vector3D<T> operator - (const C_Vector3D<T>& vector) const
			{
				return C_Vector3D<T> (X - vector.X, Y - vector.Y, Z - vector.Z);
			}

			/*!
			**	MULTIPLY operator
			**	\param scalar scalar value multiplied by the vector
			**	\return scaled vector
			*/
			C_Vector3D<T> operator * (const float& scalar) const
			{
				return C_Vector3D<T> (X * scalar, Y * scalar, Z * scalar);
			}

			/*!
			**	DIVIDE operator
			**	\param scalar scalar value divided by the vector
			**	\return scaled vector
			*/
			C_Vector3D<T> operator / (const float& scalar) const
			{
				if (scalar !=(T)0)
					return C_Vector3D<T>(X / scalar, Y / scalar, Z /scalar);
				else
					return (*this);
			}

			/*!
			**	ADD_EQUAL operator
			**	\param vector vector to add to the current vector
			**	\return summation result
			*/
			C_Vector3D<T>& operator += (const C_Vector3D<T>& vector)
			{
				X += vector.X;
				Y += vector.Y;
				Z += vector.Z;

				return (*this);
			}

			/*!
			**	SUB_EQUAL operator
			**	\param vector vector to sub to the current vector
			**	\return substraction result
			*/
			C_Vector3D<T>& operator -= (const C_Vector3D<T>& vector)
			{
				X -= vector.X;
				Y -= vector.Y;
				Z -= vector.Z;

				return *this;
			}

			/*!
			**	MULTIPLY_EQUAL operator
			**	\param scalar scalar multiplied by the current vector
			**	\return scaled result
			*/
			C_Vector3D<T>& operator *= (const float& scalar)
			{
				X *= scalar;
				Y *= scalar;
				Z *= scalar;

				return *this;
			}

			/*!
			**	DIVIDE_EQUAL operator
			**	\param scalar scalar divided by the current vector
			**	\return scaled result
			*/
			C_Vector3D<T>& operator /= (const float& scalar)
			{
				if (scalar != (T)0) 
				{
					X /= scalar;
					Y /= scalar;
					Z /= scalar;
				}

				return (*this);
			}

			/*!
			**	SIGN operator
			**	\return signed result
			*/
			C_Vector3D<T> operator - () const
			{
				return C_Vector3D<T>(-X, -Y, -Z);
			}

			/*!
			**	EQUAL_EQUAL operator
			**	\param vector vector to compare
			**	\return equal value
			*/
			const bool operator == (C_Vector3D<T>& vector) const
			{
				return ((X == vector.X) && (Y == vector.Y) && (Z == vector.Z)) ? true : false;
			}

			/*!
			**	NOT_EQUAL operator
			**	\param vector vector to compare
			**	\return not equal value
			*/
			const bool operator != (C_Vector3D<T>& vector) const
			{
				return ((X != vector.X) && (Y != vector.Y) && (Z != vector.Z)) ? true : false;
			}

		public:
			T X;	///<	X coordinate
			T Y;	///<	Y coordinate
			T Z;	///<	Z coordinate
		};

		/*!
		**	Global MULTIPLY operator
		**	\param scalar scalar value to multiply with the second parameter
		**	\param vector vector to multiply the scalar value with
		**	\return scaled vector instance
		*/
		template<typename T> inline C_Vector3D<T> operator * (float scalar, C_Vector3D<T>& vector) 
		{
			return C_Vector3D<T>(vector.X * scalar, vector.Y * scalar, vector.Z * scalar);
		}

		/*!
		**	Global DIVIDE operator
		**	\param scalar scalar value to divide with the second parameter
		**	\param vector vector to divide the scalar value with
		**	\return scaled vector instance
		*/
		template<typename T> inline C_Vector3D<T> operator / (float scalar, C_Vector3D<T>& vector) 
		{
			return C_Vector3D<T>(vector.X / scalar, vector.Y / scalar, vector.Z / scalar);
		}
	}	//	namespace CORE
}	//	namespace REDEEMER

#endif	//	_R_CORE_VECTOR3D_H_
