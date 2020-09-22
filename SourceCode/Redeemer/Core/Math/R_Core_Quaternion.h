/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_Quaternion.h
**		Quaternions are used in 3D rotations
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_CORE_QUATERNION_H_
#define _R_CORE_QUATERNION_H_

#include "R_Core_Matrix.h"

namespace REDEEMER
{
	namespace CORE
	{
		/*!
		**	\brief  C_Quaternion class for representing rotations.
		**	It provides cheap combinations and avoids gimbal locks. Also useful for interpolations.
		*/
		class C_Quaternion
		{	
		public:	
			/*!	Default constructor
			*/
			C_Quaternion() :
			  X(0.0f), 
			  Y(0.0f), 
			  Z(0.0f), 
			  W(1.0f) 
			  {
				  //	EMPTY
			  }

			/*! Constructor
			*/
			C_Quaternion (float x, float y, float z, float w) : 
			  X (x),
			  Y (y), 
			  Z (z), 
			  W (w) 
			  {
				  //	EMPTY
			  }

			/*! Constructor which converts euler angles (radians) to a C_Quaternion
			*/
			C_Quaternion (float x, float y, float z)
			{
				Set (x, y, z);
			}

			/*! Constructor which converts euler angles (radians) to a C_Quaternion
			*/
			C_Quaternion (const C_Vector3D<float>& vec)
			{
				Set (vec.X, vec.Y, vec.Z);
			}

			/*! Constructor which converts a matrix to a C_Quaternion
			*/
			C_Quaternion (const C_Matrix<float>& mat)
			{
				(*this) = mat;
			}

			/*! Constructor which uses rotation vector and angle
			*/
			C_Quaternion(const C_Vector3D<float>& vec, float angle)
			{
				FromAngleAxis (angle, vec);
			}

			/*! Equalilty operator
			*/
			bool operator== (const C_Quaternion& other) const
			{
				return ((X == other.X) && (Y == other.Y) && (Z == other.Z) && (W == other.W));
			}

			/*! Inequality operator
			*/
			bool operator!= (const C_Quaternion& other) const
			{
				return !(*this == other);
			}

			/*!	Assignment operator
			*/
			C_Quaternion& operator= (const C_Quaternion& other)
			{
				X = other.X;
				Y = other.Y;
				Z = other.Z;
				W = other.W;

				return *this;
			}

			/*! Matrix assignment operator
			*/
			C_Quaternion& operator= (const C_Matrix<float>& m)
			{
				const float diag = m(0,0) + m(1,1) + m(2,2) + 1;

				if (diag > 0.0f)
				{
					const float scale = sqrtf(diag) * 2.0f; // get scale from diagonal

					// TODO: speed this up
					X = ( m(2,1) - m(1,2)) / scale;
					Y = ( m(0,2) - m(2,0)) / scale;
					Z = ( m(1,0) - m(0,1)) / scale;
					W = 0.25f * scale;
				}
				else
				{
					if ( m(0,0) > m(1,1) && m(0,0) > m(2,2))
					{
						// 1st element of diag is greatest value
						// find scale according to 1st element, and double it
						const float scale = sqrtf( 1.0f + m(0,0) - m(1,1) - m(2,2)) * 2.0f;

						// TODO: speed this up
						X = 0.25f * scale;
						Y = (m(0,1) + m(1,0)) / scale;
						Z = (m(2,0) + m(0,2)) / scale;
						W = (m(2,1) - m(1,2)) / scale;
					}
					else if ( m(1,1) > m(2,2))
					{
						// 2nd element of diag is greatest value
						// find scale according to 2nd element, and double it
						const float scale = sqrtf( 1.0f + m(1,1) - m(0,0) - m(2,2)) * 2.0f;

						// TODO: speed this up
						X = (m(0,1) + m(1,0) ) / scale;
						Y = 0.25f * scale;
						Z = (m(1,2) + m(2,1) ) / scale;
						W = (m(0,2) - m(2,0) ) / scale;
					}
					else
					{
						// 3rd element of diag is greatest value
						// find scale according to 3rd element, and double it
						const float scale = sqrtf( 1.0f + m(2,2) - m(0,0) - m(1,1)) * 2.0f;

						// TODO: speed this up
						X = (m(0,2) + m(2,0)) / scale;
						Y = (m(1,2) + m(2,1)) / scale;
						Z = 0.25f * scale;
						W = (m(1,0) - m(0,1)) / scale;
					}
				}

				return Normalize ();
			}

			/*! Add operator
			*/
			C_Quaternion operator+ (const C_Quaternion& other) const
			{
				return C_Quaternion(X + other.X, Y + other.Y, Z + other.Z, W + other.W);
			}

			/*! Multiplication operator
			*/
			C_Quaternion operator* (const C_Quaternion& other) const
			{
				C_Quaternion tmp;

				tmp.W = W * other.W - X * other.X - Y * other.Y - Z * other.Z;
				tmp.X = W * other.X + X * other.W + Y * other.Z - Z * other.Y;
				tmp.Y = W * other.Y + Y * other.W + Z * other.X - X * other.Z;
				tmp.Z = W * other.Z + Z * other.W + X * other.Y - Y * other.X;

				return tmp;
			}

			/*! Multiplication operator with scalar
			*/
			C_Quaternion operator* (float s) const
			{
				return C_Quaternion (s * X, s * Y, s * Z, s * W);
			}

			/*! Multiplication operator with scalar
			*/
			C_Quaternion& operator*= (float s)
			{
				X *= s;
				Y *= s;
				Z *= s;
				W *= s;

				return *this;
			}

			/*! Multiplication operator
			*/
			C_Vector3D<float> operator* (const C_Vector3D<float>& v) const
			{
				// nVidia SDK implementation

				C_Vector3D<float> uv, uuv;
				C_Vector3D<float> qvec(X, Y, Z);

				uv = qvec.GetCross(v);
				uuv = qvec.GetCross(uv);
				uv *= (2.0f * W);
				uuv *= 2.0f;

				return v + uv + uuv;
			}

			/*! Multiplication operator
			*/
			C_Quaternion& operator*= (const C_Quaternion& other)
			{
				return (*this = other * (*this));
			}

			/*! Calculates the dot product
			*/
			float DotProduct (const C_Quaternion& other) const
			{
				return (X * other.X) + (Y * other.Y) + (Z * other.Z) + (W * other.W);
			}

			/*! Sets new C_Quaternion
			*/
			C_Quaternion& Set (float x, float y, float z, float w)
			{
				X = x;
				Y = y;
				Z = z;
				W = w;

				return *this;
			}

			/*! Sets new C_Quaternion based on euler angles (radians)
			*/
			C_Quaternion& Set (float x, float y, float z)
			{
				double angle;

				angle = x * 0.5;
				const double sr = sin(angle);
				const double cr = cos(angle);

				angle = y * 0.5;
				const double sp = sin(angle);
				const double cp = cos(angle);

				angle = z * 0.5;
				const double sy = sin(angle);
				const double cy = cos(angle);

				const double cpcy = cp * cy;
				const double spcy = sp * cy;
				const double cpsy = cp * sy;
				const double spsy = sp * sy;

				X = (float)(sr * cpcy - cr * spsy);
				Y = (float)(cr * spcy + sr * cpsy);
				Z = (float)(cr * cpsy - sr * spcy);
				W = (float)(cr * cpcy + sr * spsy);

				return Normalize ();
			}

			/*! Sets new C_Quaternion based on euler angles (radians)
			*/
			C_Quaternion& Set (const C_Vector3D<float>& vec)
			{
				return Set (vec.X, vec.Y, vec.Z);
			}

			/*! Sets new C_Quaternion from other C_Quaternion
			*/
			C_Quaternion& Set (const C_Quaternion& quat)
			{
				return (*this = quat);
			}

			/*! Returns if this C_Quaternion equals the other one, taking floating point rounding errors into account
			*/
			bool Equals (const C_Quaternion& other,	const float tolerance = C_MathUtils::ROUNDING_ERROR_FLOAT) const
			{
				return C_MathUtils::Equals(X, other.X, tolerance) &&
					C_MathUtils::Equals(Y, other.Y, tolerance) &&
					C_MathUtils::Equals(Z, other.Z, tolerance) &&
					C_MathUtils::Equals(W, other.W, tolerance);
			}

			/*! Normalizes the C_Quaternion
			*/
			C_Quaternion& Normalize ()
			{
				const float n = X*X + Y*Y + Z*Z + W*W;

				if (n == 1)
					return *this;

				//n = 1.0f / sqrtf(n);
				return (*this *= C_MathUtils::ReciprocalSquareRoot(n));
			}

			/*! Creates a matrix from this C_Quaternion
			*/
			C_Matrix<float> GetMatrix () const
			{
				C_Matrix<float> m;

				GetMatrix_transposed (m);

				return m;

			}

			/*! Creates a matrix from this C_Quaternion
			*/
			void GetMatrix (C_Matrix<float> &dest, const C_Vector3D<float> &translation) const
			{
				float * m = dest.GetArray();

				m[0] = 1.0f - 2.0f*Y*Y - 2.0f*Z*Z;
				m[1] = 2.0f*X*Y + 2.0f*Z*W;
				m[2] = 2.0f*X*Z - 2.0f*Y*W;
				m[3] = 0.0f;

				m[4] = 2.0f*X*Y - 2.0f*Z*W;
				m[5] = 1.0f - 2.0f*X*X - 2.0f*Z*Z;
				m[6] = 2.0f*Z*Y + 2.0f*X*W;
				m[7] = 0.0f;

				m[8] = 2.0f*X*Z + 2.0f*Y*W;
				m[9] = 2.0f*Z*Y - 2.0f*X*W;
				m[10] = 1.0f - 2.0f*X*X - 2.0f*Y*Y;
				m[11] = 0.0f;

				m[12] = translation.X;
				m[13] = translation.Y;
				m[14] = translation.Z;
				m[15] = 1.f;

				dest.SetDefinitelyIdentityMatrix (false);
			}

			/*! Creates a matrix from this C_Quaternion
			*/
			void GetMatrix_transposed (C_Matrix<float> &dest) const
			{
				dest[0] = 1.0f - 2.0f*Y*Y - 2.0f*Z*Z;
				dest[4] = 2.0f*X*Y + 2.0f*Z*W;
				dest[8] = 2.0f*X*Z - 2.0f*Y*W;
				dest[12] = 0.0f;

				dest[1] = 2.0f*X*Y - 2.0f*Z*W;
				dest[5] = 1.0f - 2.0f*X*X - 2.0f*Z*Z;
				dest[9] = 2.0f*Z*Y + 2.0f*X*W;
				dest[13] = 0.0f;

				dest[2] = 2.0f*X*Z + 2.0f*Y*W;
				dest[6] = 2.0f*Z*Y - 2.0f*X*W;
				dest[10] = 1.0f - 2.0f*X*X - 2.0f*Y*Y;
				dest[14] = 0.0f;

				dest[3] = 0.f;
				dest[7] = 0.f;
				dest[11] = 0.f;
				dest[15] = 1.f;

				//dest.setDefinitelyIdentityMatrix ( matrix4::BIT_IS_NOT_IDENTITY );

				dest.SetDefinitelyIdentityMatrix  (false);
			}

			/*! Inverts this C_Quaternion
			*/
			C_Quaternion& MakeInverse ()
			{
				X = -X; 
				Y = -Y; 
				Z = -Z;

				return *this;
			}

			/*! Set this C_Quaternion to the result of the interpolation between two quaternions
			*/
			C_Quaternion& Slerp (C_Quaternion q1, C_Quaternion q2, float interpolate)
			{
				float angle = q1.DotProduct(q2);

				if (angle < 0.0f)
				{
					q1 *= -1.0f;
					angle *= -1.0f;
				}

				float scale;
				float invscale;

				if ((angle + 1.0f) > 0.05f)
				{
					if ((1.0f - angle) >= 0.05f) // spherical interpolation
					{
						const float theta = acosf(angle);
						const float invsintheta = C_MathUtils::Reciprocal(sinf(theta));
						scale = sinf(theta * (1.0f-interpolate)) * invsintheta;
						invscale = sinf(theta * interpolate) * invsintheta;
					}
					else // linear interploation
					{
						scale = 1.0f - interpolate;
						invscale = interpolate;
					}
				}
				else
				{
					q2.Set(-q1.Y, q1.X, -q1.W, q1.Z);
					scale = sinf(C_MathUtils::PI * (0.5f - interpolate));
					invscale = sinf(C_MathUtils::PI * interpolate);
				}

				return (*this = (q1*scale) + (q2*invscale));
			}

			/*! Create C_Quaternion from rotation angle and rotation axis.
			**	Axis must be unit length.
			**	The C_Quaternion representing the rotation is: q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k).
			**	\param angle Rotation Angle in radians.
			**	\param axis Rotation axis. 
			*/
			C_Quaternion& FromAngleAxis (float angle, const C_Vector3D<float>& axis)
			{
				const float fHalfAngle = 0.5f*angle;
				const float fSin = sinf(fHalfAngle);

				W = cosf(fHalfAngle);
				X = fSin*axis.X;
				Y = fSin*axis.Y;
				Z = fSin*axis.Z;

				return *this;
			}

			/*!	Builds quaternion from matrix
			*/
			void FromMatrix (const C_Matrix<float>& matrix)
			{
				float trace = matrix._m11 + matrix._m22 + matrix._m33;
				float root;

				if(trace > 0.0f)
				{
					root = sqrtf (trace + 1.0f);

					this->W = 0.5f * root;
					root = 0.5f / root;
					this->X = -(matrix._m32 - matrix._m23) * root;
					this->Y = -(matrix._m13 - matrix._m31) * root;
					this->Z = -(matrix._m21 - matrix._m12) * root;
				}
				else
				{
					unsigned int next[3] = {1, 2, 0};
					unsigned int i = 0;

					if(matrix._M[1][1] > matrix._M[0][0])
						i = 1;

					if(matrix._M[2][2] > matrix._M[i][i])
						i = 2;

					unsigned int j = next[i];
					unsigned int k = next[j];

					root = sqrtf(matrix._M[i][i] - matrix._M[j][j] - matrix._M[k][k] + 1.0f);

					float* pQuat[3] = {&this->X, &this->Y, &this->Z};

					*pQuat[i] = -0.5f * root;
					root = 0.5f / root;
					this->W = (matrix._M[k][j] - matrix._M[j][k]) * root;
					*pQuat[j] = -(matrix._M[j][i] + matrix._M[i][j]) * root;
					*pQuat[k] = -(matrix._M[k][i] + matrix._M[i][k]) * root;
				}
			}

			/*!	Fills an angle (radians) around an axis (unit vector)
			*/
			void ToAngleAxis (float &angle, C_Vector3D<float>& axis) const
			{
				const float scale = sqrtf (X*X + Y*Y + Z*Z);

				if (C_MathUtils::IsZero(scale) || W > 1.0f || W < -1.0f)
				{
					angle = 0.0f;
					axis.X = 0.0f;
					axis.Y = 1.0f;
					axis.Z = 0.0f;
				}
				else
				{
					const float invscale = C_MathUtils::Reciprocal (scale);
					angle = 2.0f * acosf(W);
					axis.X = X * invscale;
					axis.Y = Y * invscale;
					axis.Z = Z * invscale;
				}

			}

			/*! Output this C_Quaternion to an euler angle (radians)
			*/
			void ToEuler (C_Vector3D<float>& euler) const
			{
				const double sqw = W*W;
				const double sqx = X*X;
				const double sqy = Y*Y;
				const double sqz = Z*Z;

				// heading = rotation about z-axis
				euler.Z = (float) (atan2(2.0 * (X*Y +Z*W),(sqx - sqy - sqz + sqw)));

				// bank = rotation about x-axis
				euler.X = (float) (atan2(2.0 * (Y*Z +X*W),(-sqx - sqy + sqz + sqw)));

				// attitude = rotation about y-axis
				euler.Y = asinf(C_MathUtils::Clamp(-2.0f * (X*Z - Y*W), -1.0f, 1.0f));
			}

			/*! Set C_Quaternion to identity
			*/
			C_Quaternion& MakeIdentity ()
			{
				W = 1.f;
				X = 0.f;
				Y = 0.f;
				Z = 0.f;

				return *this;
			}

		public:
			float X;	///<	vectorial (imaginary) part
			float Y;	///<	Y part
			float Z;	///<	Z part
			float W;	///<	real part
		};
	}	//	namespace CORE
}	//	namespace REDEEMER

#endif	//	_R_CORE_QUATERNION_H_
