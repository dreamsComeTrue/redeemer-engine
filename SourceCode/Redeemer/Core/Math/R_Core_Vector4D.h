/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_Vector4D.h
**		Contains all the mathematical operations for a 4D vector
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_CORE_VECTOR4D_H_
#define _R_CORE_VECTOR4D_H_

namespace REDEEMER
{
	namespace CORE
	{
		/*!
		**	\brief Contains all the mathematical operations for a 4D vector
		*/
		template<typename T> class C_Vector4D
		{	
		public:	
			/*!
			**	Default constructor of the C_Vector4D class
			*/
			C_Vector4D () :
			  X ((T)0),
			  Y ((T)0),
			  Z ((T)0),
			  W ((T)0)
			{
				//	EMPTY
			}

			/*!
			**	Default constructor of the C_Vector4D class
			**	\param x x coordinate value
			**	\param y y coordinate value
			**	\param z z coordinate value
			**	\param w w coordinate value
			*/
			C_Vector4D (const T& x, const T& y, const T& z, const T& w) :
				X (x),
				Y (y),
				Z (z),
				W (w)
			{
				//	EMPTY
			}

			/*!
			**	Copy constructor of the Vector2D class
			**	\param pVector vector to copy
			*/
			C_Vector4D(const C_Vector4D<T>* pVector) :
				X (pVector->X),
				Y (pVector->Y),
				Z (pVector->Z),
				W (pVector->W)
			{
				//	EMPTY
			}

			/*!
			**	Copy constructor of the Vector2D class
			**	\param pVector vector to copy
			*/
			C_Vector4D(const C_Vector4D<T>& pVector) :
				X (pVector.X),
				Y (pVector.Y),
				Z (pVector.Z),
				W (pVector.W)
			{
				//	EMPTY
			}
			
			/*!
			**	Magnitude of the vector
			**	\return magnitude of the vector
			*/
			const T GetMagnitude() const
			{
				return static_cast<T> (sqrt (X * X + Y * Y + Z * Z + W * W));
			}

			/*!
			**	Dot product of the two vectors
			**	\param vector second vector on which the dot product is applied
			**	\return dot product value
			*/
			const T GetDot(const C_Vector4D<T>& vector) const
			{
				return (X * vector.X + Y * vector.Y + Z * vector.Z + W * vector.W);
			}

			/*!
			**	Angle between the two vectors
			**	\param vector second vector of the pair, where the angle is calculated
			**	\return angle between the two vectors
			*/
			const T GetAngle(const C_Vector4D<T>& vector) const
			{
				const T magnitudesProduct = GetMagnitude() * vector.GetMagnitude();

				if (magnitudesProduct != (T)0)
					return static_cast<float>(acos (GetDot (vector) / magnitudesProduct));
				else
					return (T)0;
			}

			/*!
			**	Calculates the distance between two vectors
			**	\param vector vector to calculate the distance to
			**	\return distance between the two vectors
			*/
			const T GetDistance(C_Vector4D<T>& vector) const
			{
				C_Vector4D<T> tmp = *this;
				tmp -= vector;

				return tmp.GetMagnitude();
			}

			/*!
			**	Normalizes an INSTANCE of the vector
			**	\return reference to a new normalized Vector2D
			*/
			C_Vector4D<T> GetNormalized() const
			{
				const T magnitude = GetMagnitude();

				if (magnitude != (T)0)
					return C_Vector4D<T>(X / magnitude, Y / magnitude, Z / magnitude, W / magnitude);
				else
					return (*this);
			}

			/*!
			**	Inverts an INSTANCE of the vector
			**	\return reference to a new inverted Vector2D
			*/
			C_Vector4D<T> GetInverted() const
			{
				return C_Vector4D<T> (-X, -Y, -Z, -W);
			}

			/*!
			**	Homogenizes the vector (4D)
			**	\return reference to THIS homogenized Vector4D
			*/
			C_Vector4D<T> GetHomogenized() const 
			{
				if (W != (T)0)
					return (C_Vector4D<T> (X / W, Y / W, Z / W, 1.0f));
				else 
					return (*this);
			}

			/*!
			**	Cross product of the two Vector2D, outputs a Vector4D
			**	\param vector second vector of the pair
			**	\return new perpendicular Vector4D
			*/
			C_Vector4D<T> GetCross(const C_Vector4D<T>& vector) const
			{
				return C_Vector4D<T> (Y * vector.Z - Z * vector.Y, X * vector.Z - Z * vector.X, X * vector.Y - Y * vector.X);
			}

			/*!
			**	Projects an INSTANCE of the vector onto another
			**	\param vector vector on which the projection is done
			**	\return reference to a new normalized Vector2D
			*/
			C_Vector4D<T> GetProjected(C_Vector4D<T>& vector) const
			{
				const T selfDot = vector.GetDot (vector);

				if (selfDot != (T)0)
					return (vector * (GetDot(vector) / (selfDot)));
				else
					return (*this);
			}

			/*!
			**	Calculates the spherical coordinates of the vector (4D)
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
			C_Vector4D<T>& SetMagnitude(const T& magnitude)
			{
				const T currentMagnitude = GetMagnitude ();

				if (currentMagnitude != (T)0) 
				{
					const T magnitudeRatio = magnitude / currentMagnitude;	//Faster, but not nice.

					X *= magnitudeRatio;
					Y *= magnitudeRatio;
					Z *= magnitudeRatio;
					W *= magnitudeRatio;
				}

				return (*this);
			}

			/*!
			**	Normalizes the vector
			**	\return reference to THIS normalized Vector2D
			*/
			C_Vector4D<T>& SetNormalized()
			{
				const T magnitude = GetMagnitude();

				if (magnitude != (T)0) 
				{
					X /= magnitude;
					Y /= magnitude;
					Z /= magnitude;
					W /= magnitude;
				}

				return (*this);
			}

			/*!
			**	Homogenizes the vector (4D)
			**	\return reference to THIS homogenized Vector4D
			*/
			C_Vector4D<T>& SetHomogenized () 
			{
				if (W != (T)0) 
				{
					X /= W;
					Y /= W;
					Z /= W;
					W /= (T)1;
				}

				return (*this);
			}

			/*!
			**	Inverts the vector
			**	\return reference to THIS inverted Vector2D
			*/
			C_Vector4D<T>& SetInverted()
			{
				X = -X;
				Y = -Y;
				Z = -Z;

				return (*this);
			}

			/*!
			**	Cross product of the current vector and param, onto current vector
			**	\param vector vector with which the cross product is done
			**	\return reference to THIS crossed Vector4D
			*/
			C_Vector4D<T>& SetCross (const C_Vector4D<T>& vector)
			{
				*this = GetCross (vector);

				return (*this);
			}

			/*!
			**	Cross product of 2 param vectors, stored in current vector.
			**	\param vector1 first vector of the cross product
			**	\param vector2 second vector of the cross product
			**	\return reference to THIS crossed Vector4D
			*/
			C_Vector4D<T>& SetCross(const C_Vector4D<T>& vector1, const C_Vector4D<T>& vector2) 
			{
				*this = vector1.GetCross (vector2);

				return (*this);
			}

			/*!
			**	Sets the current vector as the projection on the param vector
			**	\param vector vector to project the first one on
			**	\return reference to THIS projected Vector2D
			*/
			C_Vector4D<T>& SetProjected(const C_Vector4D<T>& vector)
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
			C_Vector4D<T>& SetProjected(const C_Vector4D<T>& vector1, const C_Vector4D<T>& vector2)
			{
				*this = vector1.GetProjected(vector2);

				return (*this);
			}
							
			/*!
			**	ADD operator replacement
			**	\param vector vector to sum
			**	\return summation result
			*/
			C_Vector4D<T> GetAdd(const C_Vector4D<T>& vector) const
			{
				return C_Vector4D<T> (this->X + vector.X, this->Y + vector.Y, this->Z + vector.Z, this->W + vector.W);
			}

			/*!
			**	ADD_EQUAL operator replacement
			**	\param vector vector to add to the current vector
			**	\return summation result
			*/
			C_Vector4D<T>& SetAdd(const C_Vector4D<T>& vector)
			{
				this->X += vector.X;
				this->Y += vector.Y;
				this->Z += vector.Z;
				this->W += vector.W;

				return (*this);
			}

			/*!
			**	ADD_EQUAL operator replacement
			**	\param vector1 first vector to add 
			**	\param vector2 second vector to add
			**	\return summation result
			*/
			C_Vector4D<T>& SetAdd(const C_Vector4D<T>& vector1, const C_Vector4D<T>& vector2)
			{
				this->X = vector1.X + vector2.X;
				this->Y = vector1.Y + vector2.Y;
				this->Z = vector1.Z + vector2.Z;
				this->W = vector1.W + vector2.W;

				return (*this);
			}

			/*!
			**	SUB operator replacement
			**	\param vector vector to sub
			**	\return substraction result
			*/
			C_Vector4D<T> GetSub(const C_Vector4D<T>& vector) const
			{
				return C_Vector4D<T> (this->X - vector.X, this->Y - vector.Y, this->Z - vector.Z, this->W - vector.W);
			}

			/*!
			**	SUB_EQUAL operator replacement
			**	\param vector vector to sub to the current vector
			**	\return substraction result
			*/
			C_Vector4D<T>& SetSub(const C_Vector4D<T>& vector)
			{
				this->X -= vector.X;
				this->Y -= vector.Y;
				this->Z -= vector.Z;
				this->W -= vector.W;

				return (*this);
			}

			/*!
			**	SUB_EQUAL operator replacement
			**	\param vector1 first vector to sub 
			**	\param vector2 second vector to sub
			**	\return substraction result
			*/
			C_Vector4D<T>& SetSub(const C_Vector4D<T>& vector1, const C_Vector4D<T>& vector2)
			{
				this->X = vector1.X - vector2.X;
				this->Y = vector1.Y - vector2.Y;
				this->Z = vector1.Z - vector2.Z;
				this->W = vector1.W - vector2.W;

				return (*this);
			}

			/*!
			**	MULTIPLY operator replacement
			**	\param scalar scalar value multiplied by the vector
			**	\return scaled vector
			*/
			C_Vector4D<T> GetMul(const float& scalar) const
			{
				return C_Vector4D<T> (this->X * scalar, this->Y * scalar, this->Z * scalar, this->W * scalar);
			}

			/*!
			**	MULTIPLY_EQUAL operator replacement
			**	\param scalar scalar multiplied by the current vector
			**	\return scaled vector
			*/
			C_Vector4D<T>& SetMul(const float& scalar)
			{
				this->X *= scalar;
				this->Y *= scalar;
				this->Z *= scalar;
				this->W *= scalar;

				return (*this);
			}

			/*!
			**	MULTIPLY_EQUAL operator replacement
			**	\param vector first vector to multiply 
			**	\param scalar scalar to multiply to the first vector
			**	\return scaled vector
			*/
			C_Vector4D<T>& SetMul(const C_Vector4D<T>& vector, const float& scalar)
			{
				this->X = vector.X * scalar;
				this->Y = vector.Y * scalar;
				this->Z = vector.Z * scalar;
				this->W = vector.W * scalar;

				return (*this);
			}

			/*!
			**	DIVIDE operator replacement
			**	\param scalar scalar value that divides the vector
			**	\return scaled vector
			*/
			C_Vector4D<T> GetDiv(const float& scalar) const
			{
				return C_Vector4D<T>(this->X / scalar, this->Y / scalar, this->Z / scalar, this->W / scalar);
			}

			/*!
			**	DIVIDE_EQUAL operator replacement
			**	\param scalar scalar multiplied by the current vector
			**	\return scaled vector
			*/
			C_Vector4D<T>& SetDiv(const float& scalar)
			{
				this->X /= scalar;
				this->Y /= scalar;
				this->Z /= scalar;
				this->W /= scalar;

				return (*this);
			}

			/*!
			**	MULTIPLY_EQUAL operator replacement
			**	\param vector first vector to multiply 
			**	\param scalar scalar to multiply to the first vector
			**	\return scaled vector
			*/
			C_Vector4D<T>& SetDiv(const C_Vector4D<T>& vector, const float& scalar)
			{
				this->X = vector.X / scalar;
				this->Y = vector.Y / scalar;
				this->Z = vector.Z / scalar;
				this->W = vector.W / scalar;

				return (*this);
			}

			/*!
			**	EQUAL_EQUAL operator
			**	\param vector vector to compare
			**	\return equal value
			*/
			const bool equals(const C_Vector4D<T>& vector) const
			{
				return ((this->X == vector.X) && (this->Y == vector.Y) && (this->Z == vector.Z) && (this->W == vector.W));
			}

			/*!
			**	NOT_EQUAL operator
			**	\param vector vector to compare
			**	\return not equal value
			*/
			const bool notEquals(const C_Vector4D<T>& vector) const
			{
				return ((this->X != vector.X) && (this->Y != vector.Y) && (this->Z != vector.Z) && (this->W != vector.W));
			}
						
			/*!
			**	ADD operator
			**	\param vector vector to sum
			**	\return summation result
			*/
			C_Vector4D<T> operator + (const C_Vector4D<T>& vector) const
			{
				return C_Vector4D<T> (X + vector.X, Y + vector.Y, Z + vector.Z, W + vector.W);
			}

			/*!
			**	SUB operator
			**	\param vector vector to sub
			**	\return substraction result
			*/
			C_Vector4D<T> operator - (const C_Vector4D<T>& vector) const
			{
				return C_Vector4D<T> (X - vector.X, Y - vector.Y, Z - vector.Z, W - vector.W);
			}

			/*!
			**	MULTIPLY operator
			**	\param scalar scalar value multiplied by the vector
			**	\return scaled vector
			*/
			C_Vector4D<T> operator * (const float& scalar) const
			{
				return C_Vector4D<T> (X * scalar, Y * scalar, Z * scalar, W * scalar);
			}

			/*!
			**	DIVIDE operator
			**	\param scalar scalar value divided by the vector
			**	\return scaled vector
			*/
			C_Vector4D<T> operator / (const float& scalar) const
			{
				if (scalar !=(T)0)
					return C_Vector4D<T>(X / scalar, Y / scalar, Z /scalar, W / scalar);
				else
					return (*this);
			}

			/*!
			**	ADD_EQUAL operator
			**	\param vector vector to add to the current vector
			**	\return summation result
			*/
			C_Vector4D<T>& operator += (const C_Vector4D<T>& vector)
			{
				X += vector.X;
				Y += vector.Y;
				Z += vector.Z;
				W += vector.W;

				return (*this);
			}

			/*!
			**	SUB_EQUAL operator
			**	\param vector vector to sub to the current vector
			**	\return substraction result
			*/
			C_Vector4D<T>& operator -= (const C_Vector4D<T>& vector)
			{
				X -= vector.X;
				Y -= vector.Y;
				Z -= vector.Z;
				W -= vector.W;

				return *this;
			}

			/*!
			**	MULTIPLY_EQUAL operator
			**	\param scalar scalar multiplied by the current vector
			**	\return scaled result
			*/
			C_Vector4D<T>& operator *= (const float& scalar)
			{
				X *= scalar;
				Y *= scalar;
				Z *= scalar;
				W *= scalar;

				return *this;
			}

			/*!
			**	DIVIDE_EQUAL operator
			**	\param scalar scalar divided by the current vector
			**	\return scaled result
			*/
			C_Vector4D<T>& operator /= (const float& scalar)
			{
				if (scalar != (T)0) 
				{
					X /= scalar;
					Y /= scalar;
					Z /= scalar;
					W /= scalar;
				}

				return (*this);
			}

			/*!
			**	SIGN operator
			**	\return signed result
			*/
			C_Vector4D<T> operator - () const
			{
				return C_Vector4D<T>(-X, -Y, -Z, -W);
			}

			/*!
			**	EQUAL_EQUAL operator
			**	\param vector vector to compare
			**	\return equal value
			*/
			const bool operator == (C_Vector4D<T>& vector) const
			{
				return ((X == vector.X) && (Y == vector.Y) && (Z == vector.Z) && (W == vector.W)) ? true : false;
			}

			/*!
			**	NOT_EQUAL operator
			**	\param vector vector to compare
			**	\return not equal value
			*/
			const bool operator != (C_Vector4D<T>& vector) const
			{
				return ((X != vector.X) && (Y != vector.Y) && (Z != vector.Z) && (W != vector.W)) ? true : false;
			}

		public:
			T X;	///<	X coordinate
			T Y;	///<	Y coordinate
			T Z;	///<	Z coordinate
			T W;	///<	W coordinate
		};

		/*!
		**	Global MULTIPLY operator
		**	\param scalar scalar value to multiply with the second parameter
		**	\param vector vector to multiply the scalar value with
		**	\return scaled vector instance
		*/
		template<typename T> inline C_Vector4D<T> operator * (float scalar, C_Vector4D<T>& vector) 
		{
			return C_Vector4D<T>(vector.X * scalar, vector.Y * scalar, vector.Z * scalar, vector.W * scalar);
		}

		/*!
		**	Global DIVIDE operator
		**	\param scalar scalar value to divide with the second parameter
		**	\param vector vector to divide the scalar value with
		**	\return scaled vector instance
		*/
		template<typename T> inline C_Vector4D<T> operator / (float scalar, C_Vector4D<T>& vector) 
		{
			return C_Vector4D<T>(vector.X / scalar, vector.Y / scalar, vector.Z / scalar, vector.W / scalar);
		}
	}	//	namespace CORE
}	//	namespace REDEEMER

#endif	//	_R_CORE_VECTOR4D_H_
