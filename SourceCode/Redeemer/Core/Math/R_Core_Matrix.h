/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_Matrix.h
**		A generic matrix implementation
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_CORE_MATRIX_H_
#define _R_CORE_MATRIX_H_

#include "R_Core_MathUtils.h"
#include "R_Core_Vector3D.h"

#include <cmath>
#include <cassert>
#include <memory>

namespace REDEEMER
{
	namespace CORE
	{
		/*!
		**	\brief  Generic 2-dimensional 4x4 matrix/vector mathematics class specialized for 3D usage
		**  Matrix is 4 columns x 4 rows. The matrix is row-major.
		*/
		template <class T = float>	class C_Matrix
		{
		public:
			//! Constructor Flags
			enum EConstructorFlag
			{
				CF_EMPTY = 0,
				CF_COPY,
				CF_IDENTITY,
				CF_TRANSPOSED,
				CF_INVERSE,
				CF_INVERSE_TRANSPOSED
			};

			/*!	Default constructor
			**	\param constructor Choose the initialization style
			*/
			C_Matrix (EConstructorFlag constructor = CF_IDENTITY) :
				m_DefinitelyIdentityMatrix (false)
			{
				switch (constructor)
				{
				case CF_EMPTY:
				case CF_COPY:
					break;

				case CF_IDENTITY:
				case CF_INVERSE:
				default:
					MakeIdentity ();
					break;
				}
			}

			/*! Copy constructor
			**	\param other Other matrix to copy from
			**	\param constructor Choose the initialization style 
			*/
			C_Matrix (const C_Matrix<T>& other, EConstructorFlag constructor = CF_COPY) : 
				m_DefinitelyIdentityMatrix (false)
			{
				switch (constructor)
				{
				case CF_IDENTITY:
					MakeIdentity ();
					break;

				case CF_EMPTY:
					break;

				case CF_COPY:
					*this = other;
					break;

				case CF_TRANSPOSED:
					other.GetTransposed (*this);
					break;

				case CF_INVERSE:
					if (!other.GetInverse (*this))
						memset(M, 0, 16*sizeof(T));
					break;

				case CF_INVERSE_TRANSPOSED:
					if (!other.GetInverse(*this))
						memset(M, 0, 16*sizeof(T));
					else
						*this = GetTransposed();
					break;
				}
			}

			/*! Simple operator for directly accessing every element of the matrix.
			*/
			T& operator () (const unsigned int row, const unsigned int col)
			{ 
				m_DefinitelyIdentityMatrix = false;

				return M [row * 4 + col];
			}

			/*! Simple operator for directly accessing every element of the matrix.
			*/
			const T& operator () (const unsigned int row, const unsigned int col) const 
			{
				return M[row * 4 + col]; 
			}

			/*! Simple operator for linearly accessing every element of the matrix.
			*/
			T& operator[] (unsigned int index)
			{ 
				m_DefinitelyIdentityMatrix = false; 

				return M[index];
			}

			/*!	Simple operator for linearly accessing every element of the matrix.
			*/
			const T& operator[] (unsigned int index) const 
			{ 
				return M[index]; 
			}

			/*! Sets this matrix equal to the other matrix.
			*/
			inline C_Matrix<T>& operator= (const C_Matrix<T> &other)
			{
				if (this == &other)
					return *this;

				memcpy (M, other.M, 16*sizeof(T));

				m_DefinitelyIdentityMatrix = other.m_DefinitelyIdentityMatrix;

				return *this;
			}

			/*! Sets all elements of this matrix to the value.
			*/
			inline C_Matrix<T>& operator= (const T& scalar)
			{
				for (int i = 0; i < 16; ++i)
					M[i] = scalar;

				m_DefinitelyIdentityMatrix = false;

				return *this;
			}

			/*! Returns pointer to internal array
			*/
			const T* GetArray () const 
			{ 
				return M; 
			}

			/*! Returns pointer to internal array
			*/
			T* GetArray () 
			{ 
				m_DefinitelyIdentityMatrix = false;

				return M;
			}

			/*! Returns true if other matrix is equal to this matrix.
			*/
			bool operator== (const C_Matrix<T> &other) const
			{
				if (m_DefinitelyIdentityMatrix && other.m_DefinitelyIdentityMatrix)
					return true;

				for (int i = 0; i < 16; ++i)
					if (M[i] != other.M[i])
						return false;

				return true;
			}

			/*! Returns true if other matrix is not equal to this matrix.
			*/
			bool operator!= (const C_Matrix<T> &other) const
			{
				return !(*this == other);
			}

			/*! Add another matrix.
			*/
			C_Matrix<T> operator+ (const C_Matrix<T>& other) const
			{
				C_Matrix<T> temp (CF_EMPTY);

				temp[0] = M[0] + other[0];
				temp[1] = M[1] + other[1];
				temp[2] = M[2] + other[2];
				temp[3] = M[3] + other[3];
				temp[4] = M[4] + other[4];
				temp[5] = M[5] + other[5];
				temp[6] = M[6] + other[6];
				temp[7] = M[7] + other[7];
				temp[8] = M[8] + other[8];
				temp[9] = M[9] + other[9];
				temp[10] = M[10] + other[10];
				temp[11] = M[11] + other[11];
				temp[12] = M[12] + other[12];
				temp[13] = M[13] + other[13];
				temp[14] = M[14] + other[14];
				temp[15] = M[15] + other[15];

				return temp;
			}

			/*! Add another matrix.
			*/
			C_Matrix<T>& operator+= (const C_Matrix<T>& other)
			{
				M[0] += other[0];
				M[1] += other[1];
				M[2] += other[2];
				M[3] += other[3];
				M[4] += other[4];
				M[5] += other[5];
				M[6] += other[6];
				M[7] += other[7];
				M[8] += other[8];
				M[9] += other[9];
				M[10] += other[10];
				M[11] += other[11];
				M[12] += other[12];
				M[13] += other[13];
				M[14] += other[14];
				M[15] += other[15];

				return *this;
			}

			/*! Subtract another matrix.
			*/
			C_Matrix<T> operator- (const C_Matrix<T>& other) const
			{
				C_Matrix<T> temp (CF_EMPTY);

				temp[0] = M[0] - other[0];
				temp[1] = M[1] - other[1];
				temp[2] = M[2] - other[2];
				temp[3] = M[3] - other[3];
				temp[4] = M[4] - other[4];
				temp[5] = M[5] - other[5];
				temp[6] = M[6] - other[6];
				temp[7] = M[7] - other[7];
				temp[8] = M[8] - other[8];
				temp[9] = M[9]-  other[9];
				temp[10] = M[10] - other[10];
				temp[11] = M[11] - other[11];
				temp[12] = M[12] - other[12];
				temp[13] = M[13] - other[13];
				temp[14] = M[14] - other[14];
				temp[15] = M[15] - other[15];

				return temp;
			}

			/*! Subtract another matrix.
			*/
			C_Matrix<T>& operator-= (const C_Matrix<T>& other)
			{
				M[0] -= other[0];
				M[1] -= other[1];
				M[2] -= other[2];
				M[3] -= other[3];
				M[4] -= other[4];
				M[5] -= other[5];
				M[6] -= other[6];
				M[7] -= other[7];
				M[8] -= other[8];
				M[9] -= other[9];
				M[10] -= other[10];
				M[11] -= other[11];
				M[12] -= other[12];
				M[13] -= other[13];
				M[14] -= other[14];
				M[15] -= other[15];

				return *this;
			}

			//! set this matrix to the product of two matrices
			C_Matrix<T>& SetByProduct (const C_Matrix<T>& other_a,const C_Matrix<T>& other_b )
			{
				if (other_a.IsIdentity ())
					return (*this = other_b);
				else
					if (other_b.IsIdentity ())
						return (*this = other_a);
					else
						return SetByProduct_nocheck (other_a, other_b);
			}

			/*!	Multiply by another matrix
			**	Set this matrix to the product of two other matrices. Goal is to reduce stack use and copy
			*/
			C_Matrix<T>& C_Matrix<T>::SetByProduct_nocheck (const C_Matrix<T>& other_a, const C_Matrix<T>& other_b)
			{
				const T *m1 = other_a.M;
				const T *other = other_b.M;

				this->M[0] = m1[0] * other[0] + m1[1] * other[4] + m1[2] * other[8] + m1[3] * other[12];
				this->M[1] = m1[0] * other[1] + m1[1] * other[5] + m1[2] * other[9] + m1[3] * other[13];
				this->M[2] = m1[0] * other[2] + m1[1] * other[6] + m1[2] * other[10] + m1[3] * other[14];
				this->M[3] = m1[0] * other[3] + m1[1] * other[7] + m1[2] * other[11] + m1[3] * other[15];

				this->M[4] = m1[4] * other[0] + m1[5] * other[4] + m1[6] * other[8] + m1[7] * other[12];
				this->M[5] = m1[4] * other[1] + m1[5] * other[5] + m1[6] * other[9] + m1[7] * other[13];
				this->M[6] = m1[4] * other[2] + m1[5] * other[6] + m1[6] * other[10] + m1[7] * other[14];		 
				this->M[7] = m1[4] * other[3] + m1[5] * other[7] + m1[6] * other[11] + m1[7] * other[15];

				this->M[8] = m1[8] * other[0] + m1[9] * other[4] + m1[10] * other[8] + m1[11] * other[12];
				this->M[9] = m1[8] * other[1] + m1[9] * other[5] + m1[10] * other[9] + m1[11] * other[13];
				this->M[10] = m1[8] * other[2] + m1[9] * other[6] + m1[10] * other[10] + m1[11] * other[14];
				this->M[11] = m1[8] * other[3] + m1[9] * other[7] + m1[10] * other[11] + m1[11] * other[15];

				this->M[12] = m1[12] * other[0] + m1[13] * other[4] + m1[14] * other[8] + m1[15] * other[12];
				this->M[13] = m1[12] * other[1] + m1[13] * other[5] + m1[14] * other[9] + m1[15] * other[13];
				this->M[14] = m1[12] * other[2] + m1[13] * other[6] + m1[14] * other[10] + m1[15] * other[14];
				this->M[15] = m1[12] * other[3] + m1[13] * other[7] + m1[14] * other[11] + m1[15] * other[15];

				m_DefinitelyIdentityMatrix = false;

				return *this;
			}

			/*! Multiply by another matrix.
			*/
			C_Matrix<T> operator* (const C_Matrix<T>& other) const
			{
				if (this->IsIdentity ())
					return other;

				if (other.IsIdentity ())
					return *this;

				C_Matrix<T> m3 (CF_EMPTY);
				
// 				m3[0] = m1[0]*other[0] + m1[4]*other[1] + m1[8]*other[2] + m1[12]*other[3];
// 				m3[1] = m1[1]*other[0] + m1[5]*other[1] + m1[9]*other[2] + m1[13]*other[3];
// 				m3[2] = m1[2]*other[0] + m1[6]*other[1] + m1[10]*other[2] + m1[14]*other[3];
// 				m3[3] = m1[3]*other[0] + m1[7]*other[1] + m1[11]*other[2] + m1[15]*other[3];
// 
// 				m3[4] = m1[0]*other[4] + m1[4]*other[5] + m1[8]*other[6] + m1[12]*other[7];
// 				m3[5] = m1[1]*other[4] + m1[5]*other[5] + m1[9]*other[6] + m1[13]*other[7];
// 				m3[6] = m1[2]*other[4] + m1[6]*other[5] + m1[10]*other[6] + m1[14]*other[7];
// 				m3[7] = m1[3]*other[4] + m1[7]*other[5] + m1[11]*other[6] + m1[15]*other[7];
// 
// 				m3[8] = m1[0]*other[8] + m1[4]*other[9] + m1[8]*other[10] + m1[12]*other[11];
// 				m3[9] = m1[1]*other[8] + m1[5]*other[9] + m1[9]*other[10] + m1[13]*other[11];
// 				m3[10] = m1[2]*other[8] + m1[6]*other[9] + m1[10]*other[10] + m1[14]*other[11];
// 				m3[11] = m1[3]*other[8] + m1[7]*other[9] + m1[11]*other[10] + m1[15]*other[11];
// 
// 				m3[12] = m1[0]*other[12] + m1[4]*other[13] + m1[8]*other[14] + m1[12]*other[15];
// 				m3[13] = m1[1]*other[12] + m1[5]*other[13] + m1[9]*other[14] + m1[13]*other[15];
// 				m3[14] = m1[2]*other[12] + m1[6]*other[13] + m1[10]*other[14] + m1[14]*other[15];
// 				m3[15] = m1[3]*other[12] + m1[7]*other[13] + m1[11]*other[14] + m1[15]*other[15];

				m3._M[0][0] = _M[0][0] * other._M[0][0] + _M[0][1] * other._M[1][0] +
					_M[0][2] * other._M[2][0] + _M[0][3] * other._M[3][0];
				m3._M[0][1] = _M[0][0] * other._M[0][1] + _M[0][1] * other._M[1][1] +
					_M[0][2] * other._M[2][1] + _M[0][3] * other._M[3][1];
				m3._M[0][2] = _M[0][0] * other._M[0][2] + _M[0][1] * other._M[1][2] +
					_M[0][2] * other._M[2][2] + _M[0][3] * other._M[3][2];
				m3._M[0][3] = _M[0][0] * other._M[0][3] + _M[0][1] * other._M[1][3] +
					_M[0][2] * other._M[2][3] + _M[0][3] * other._M[3][3];

				m3._M[1][0] = _M[1][0] * other._M[0][0] + _M[1][1] * other._M[1][0] +
					_M[1][2] * other._M[2][0] + _M[1][3] * other._M[3][0];
				m3._M[1][1] = _M[1][0] * other._M[0][1] + _M[1][1] * other._M[1][1] +
					_M[1][2] * other._M[2][1] + _M[1][3] * other._M[3][1];
				m3._M[1][2] = _M[1][0] * other._M[0][2] + _M[1][1] * other._M[1][2] +
					_M[1][2] * other._M[2][2] + _M[1][3] * other._M[3][2];
				m3._M[1][3] = _M[1][0] * other._M[0][3] + _M[1][1] * other._M[1][3] +
					_M[1][2] * other._M[2][3] + _M[1][3] * other._M[3][3];

				m3._M[2][0] = _M[2][0] * other._M[0][0] + _M[2][1] * other._M[1][0] +
					_M[2][2] * other._M[2][0] + _M[2][3] * other._M[3][0];
				m3._M[2][1] = _M[2][0] * other._M[0][1] + _M[2][1] * other._M[1][1] +
					_M[2][2] * other._M[2][1] + _M[2][3] * other._M[3][1];
				m3._M[2][2] = _M[2][0] * other._M[0][2] + _M[2][1] * other._M[1][2] +
					_M[2][2] * other._M[2][2] + _M[2][3] * other._M[3][2];
				m3._M[2][3] = _M[2][0] * other._M[0][3] + _M[2][1] * other._M[1][3] +
					_M[2][2] * other._M[2][3] + _M[2][3] * other._M[3][3];

				m3._M[3][0] = _M[3][0] * other._M[0][0] + _M[3][1] * other._M[1][0] +
					_M[3][2] * other._M[2][0] + _M[3][3] * other._M[3][0];
				m3._M[3][1] = _M[3][0] * other._M[0][1] + _M[3][1] * other._M[1][1] +
					_M[3][2] * other._M[2][1] + _M[3][3] * other._M[3][1];
				m3._M[3][2] = _M[3][0] * other._M[0][2] + _M[3][1] * other._M[1][2] +
					_M[3][2] * other._M[2][2] + _M[3][3] * other._M[3][2];
				m3._M[3][3] = _M[3][0] * other._M[0][3] + _M[3][1] * other._M[1][3] +
					_M[3][2] * other._M[2][3] + _M[3][3] * other._M[3][3];

				return m3;
			}

			/*! Multiply by another matrix.
			*/
			C_Matrix<T>& operator*= (const C_Matrix<T>& other)
			{
				// do checks on your own in order to avoid copy creation
				if (!other.IsIdentity())
				{
					if (this->IsIdentity ())
					{
						return (*this = other);
					}
					else
					{
						C_Matrix<T> temp (*this);

						return SetByProduct_nocheck (temp, other);
					}
				}
				return *this;
			}

			/*! Multiply by scalar.
			*/
			C_Matrix<T> operator* (const T& scalar) const
			{
				C_Matrix<T> temp (CF_EMPTY);

				temp[0] = M[0] * scalar;
				temp[1] = M[1] * scalar;
				temp[2] = M[2] * scalar;
				temp[3] = M[3] * scalar;
				temp[4] = M[4] * scalar;
				temp[5] = M[5] * scalar;
				temp[6] = M[6] * scalar;
				temp[7] = M[7] * scalar;
				temp[8] = M[8] * scalar;
				temp[9] = M[9] * scalar;
				temp[10] = M[10] * scalar;
				temp[11] = M[11] * scalar;
				temp[12] = M[12] * scalar;
				temp[13] = M[13] * scalar;
				temp[14] = M[14] * scalar;
				temp[15] = M[15] * scalar;

				return temp;
			}

			/*! Multiply by scalar.
			*/
			C_Matrix<T>& operator*= (const T& scalar)
			{
				M[0] *= scalar;
				M[1] *= scalar;
				M[2] *= scalar;
				M[3] *= scalar;
				M[4] *= scalar;
				M[5] *= scalar;
				M[6] *= scalar;
				M[7] *= scalar;
				M[8] *= scalar;
				M[9] *= scalar;
				M[10] *= scalar;
				M[11] *= scalar;
				M[12] *= scalar;
				M[13] *= scalar;
				M[14] *= scalar;
				M[15] *= scalar;

				return *this;
			}

			/*! Set matrix to identity.
			*/
			C_Matrix<T>& MakeIdentity ()
			{
				memset (M, 0, 16*sizeof(T));

				M[0] = M[5] = M[10] = M[15] = (T)1;

				m_DefinitelyIdentityMatrix = true;

				return *this;
			}

			/*! Returns true if the matrix is the identity matrix
			*/
			bool IsIdentity () const
			{
				if (!C_MathUtils::Equals( M[ 0], (T)1 ) ||
					!C_MathUtils::Equals( M[ 5], (T)1 ) ||
					!C_MathUtils::Equals( M[10], (T)1 ) ||
					!C_MathUtils::Equals( M[15], (T)1 ))
					return false;

				for (int i = 0; i < 4; ++i)
					for (int j = 0; j < 4; ++j)
						if ((j != i) && (!C_MathUtils::IsZero ((*this)(i,j))))
							return false;

				m_DefinitelyIdentityMatrix = true;

				return true;
			}

			/*! Returns true if the matrix is orthogonal
			*/
			bool IsOrthogonal() const
			{
				T dp=M[0] * M[4 ] + M[1] * M[5 ] + M[2 ] * M[6 ] + M[3 ] * M[7 ];

				if (!C_MathUtils::IsZero(dp))
					return false;

				dp = M[0] * M[8 ] + M[1] * M[9 ] + M[2 ] * M[10] + M[3 ] * M[11];

				if (!C_MathUtils::IsZero(dp))
					return false;

				dp = M[0] * M[12] + M[1] * M[13] + M[2 ] * M[14] + M[3 ] * M[15];

				if (!C_MathUtils::IsZero(dp))
					return false;

				dp = M[4] * M[8 ] + M[5] * M[9 ] + M[6 ] * M[10] + M[7 ] * M[11];

				if (!C_MathUtils::IsZero(dp))
					return false;

				dp = M[4] * M[12] + M[5] * M[13] + M[6 ] * M[14] + M[7 ] * M[15];

				if (!C_MathUtils::IsZero(dp))
					return false;

				dp = M[8] * M[12] + M[9] * M[13] + M[10] * M[14] + M[11] * M[15];

				return (C_MathUtils::IsZero(dp));
			}

			/*! 
			**	Set the translation of the current matrix. Will erase any previous values.
			*/
			C_Matrix<T>& SetTranslation (const CORE::C_Vector3D<T>& translation)
			{
				M[12] = translation.X;
				M[13] = translation.Y;
				M[14] = translation.Z;

				m_DefinitelyIdentityMatrix = false;

				return *this;
			}

			/*!	Gets the current translation
			*/
			C_Vector3D<T> GetTranslation () const
			{
				return C_Vector3D<T> (M[12], M[13], M[14]);

			}

			/*!	Set the inverse translation of the current matrix. Will erase any previous values.
			*/
			C_Matrix<T>& SetInverseTranslation (const CORE::C_Vector3D<T>& translation)
			{
				M[12] = -translation.X;
				M[13] = -translation.Y;
				M[14] = -translation.Z;

				m_DefinitelyIdentityMatrix = false;

				return *this;
			}

			/*!	Make a rotation matrix from Euler angles. The 4th row and column are unmodified.
			*/
			C_Matrix<T>& SetRotationRadians (const CORE::C_Vector3D<T>& rotation)
			{
				const double cr = cos( rotation.X );
				const double sr = sin( rotation.X );
				const double cp = cos( rotation.Y );
				const double sp = sin( rotation.Y );
				const double cy = cos( rotation.Z );
				const double sy = sin( rotation.Z );

				M[0] = (T)( cp*cy );
				M[1] = (T)( cp*sy );
				M[2] = (T)( -sp );

				const double srsp = sr*sp;
				const double crsp = cr*sp;

				M[4] = (T)( srsp*cy-cr*sy );
				M[5] = (T)( srsp*sy+cr*cy );
				M[6] = (T)( sr*cp );

				M[8] = (T)( crsp*cy+sr*sy );
				M[9] = (T)( crsp*sy-sr*cy );
				M[10] = (T)( cr*cp );

				m_DefinitelyIdentityMatrix = false;

				return *this;
			}

			/*!	Make a rotation matrix from Euler angles. The 4th row and column are unmodified.
			*/
			C_Matrix<T>& SetRotationDegrees (const CORE::C_Vector3D<T>& rotation)
			{
				return SetRotationRadians (rotation * C_MathUtils::DEGTORAD);

			}

			/*! Returns the rotation, as set by setRotation().
			*/
			CORE::C_Vector3D<T> GetRotationDegrees () const
			{
				const C_Matrix<T> &mat = *this;
				const C_Vector3D<T> scale = GetScale ();
				const C_Vector3D<double> invScale(C_MathUtils::Reciprocal(scale.X),C_MathUtils::Reciprocal(scale.Y),C_MathUtils::Reciprocal(scale.Z));

				double Y = -asin(mat[2]*invScale.X);
				const double C = cos(Y);
				Y *= C_MathUtils::RADTODEG64;

				double rotx, roty, X, Z;

				if (!C_MathUtils::IsZero (C))
				{
					const double invC = C_MathUtils::Reciprocal (C);

					rotx = mat[10] * invC * invScale.Z;
					roty = mat[6] * invC * invScale.Y;
					X = atan2( roty, rotx ) * C_MathUtils::RADTODEG64;
					rotx = mat[0] * invC * invScale.X;
					roty = mat[1] * invC * invScale.X;
					Z = atan2( roty, rotx ) * C_MathUtils::RADTODEG64;
				}
				else
				{
					X = 0.0;
					rotx = mat[5] * invScale.Y;
					roty = -mat[4] * invScale.Y;
					Z = atan2( roty, rotx ) * C_MathUtils::RADTODEG64;
				}

				// fix values that get below zero
				// before it would set (!) values to 360
				// that were above 360:
				if (X < 0.0) X += 360.0;
				if (Y < 0.0) Y += 360.0;
				if (Z < 0.0) Z += 360.0;

				return C_Vector3D<T><T> ((T)X,(T)Y,(T)Z);
			}

			/*!	Make an inverted rotation matrix from Euler angles.
			** The 4th row and column are unmodified. 
			*/
			inline C_Matrix<T>& SetInverseRotationRadians (const CORE::C_Vector3D<T>& rotation)
			{
				double cr = cos( rotation.X );
				double sr = sin( rotation.X );
				double cp = cos( rotation.Y );
				double sp = sin( rotation.Y );
				double cy = cos( rotation.Z );
				double sy = sin( rotation.Z );

				M[0] = (T)( cp*cy );
				M[4] = (T)( cp*sy );
				M[8] = (T)( -sp );

				double srsp = sr*sp;
				double crsp = cr*sp;

				M[1] = (T)( srsp*cy-cr*sy );
				M[5] = (T)( srsp*sy+cr*cy );
				M[9] = (T)( sr*cp );

				M[2] = (T)( crsp*cy+sr*sy );
				M[6] = (T)( crsp*sy-sr*cy );
				M[10] = (T)( cr*cp );

				m_DefinitelyIdentityMatrix = false;

				return *this;
			}

			/*!	Make an inverted rotation matrix from Euler angles.
			** The 4th row and column are unmodified. 
			*/
			C_Matrix<T>& SetInverseRotationDegrees (const CORE::C_Vector3D<T>& rotation)
			{
				return SetInverseRotationRadians (rotation * C_MathUtils::DEGTORAD);
			}

			void RotateYawPitchRoll( const CORE::C_Vector3D<T>& _euler)
			{
				float sy = sinf(_euler.Z), cy = cosf(_euler.Z);
				float sp = sinf(_euler.X), cp = cosf(_euler.X);
				float sr = sinf(_euler.Y), cr = cosf(_euler.Y);

				_m11 = cy*cr + sy*sp*sr;  _m12 = sr*cp; _m13 = -sy*cr + cy*sp*sr; _m14 = 0.0f;
				_m21 = -cy*sr + sy*sp*cr; _m22 = cr*cp; _m23 = sr*sy + cy*sp*cr;  _m24 = 0.0f;
				_m31 = sy*cp;             _m32 = -sp;   _m33 = cy*cp;             _m34 = 0.0f;
				_m41 = 0.0f;              _m42 = 0.0f;  _m43 = 0.0f;              _m44 = 1.0f;
			}

			void RotateYawPitchRoll (float _yaw, float _pitch, float _roll)
			{
				float sy = sinf(_yaw), cy = cosf(_yaw);
				float sp = sinf(_pitch), cp = cosf(_pitch);
				float sr = sinf(_roll), cr = cosf(_roll);

				_m11 = cy*cr + sy*sp*sr;  _m12 = sr*cp; _m13 = -sy*cr + cy*sp*sr; _m14 = 0.0f;
				_m21 = -cy*sr + sy*sp*cr; _m22 = cr*cp; _m23 = sr*sy + cy*sp*cr;  _m24 = 0.0f;
				_m31 = sy*cp;             _m32 = -sp;   _m33 = cy*cp;             _m34 = 0.0f;
				_m41 = 0.0f;              _m42 = 0.0f;  _m43 = 0.0f;              _m44 = 1.0f;
			}

			/*!	Set Scale
			*/
			C_Matrix<T>& SetScale (const CORE::C_Vector3D<T>& scale)
			{
				M[0] = scale.X;
				M[5] = scale.Y;
				M[10] = scale.Z;

				m_DefinitelyIdentityMatrix = false;

				return *this;
			}

			/*!	Set Scale
			*/
			C_Matrix<T>& SetScale (const T scale) 
			{ 
				return SetScale (CORE::C_Vector3D<T> (scale, scale, scale)); 
			}

			/*!	Get Scale
			*/
			CORE::C_Vector3D<T> GetScale() const
			{
				// Deal with the 0 rotation case first
				if (C_MathUtils::IsZero (M[1]) && C_MathUtils::IsZero(M[2]) &&
					C_MathUtils::IsZero(M[4]) && C_MathUtils::IsZero(M[6]) &&
					C_MathUtils::IsZero(M[8]) && C_MathUtils::IsZero(M[9]))
					return C_Vector3D<T>(M[0], M[5], M[10]);

				//	We have to do the full calculation.
				return C_Vector3D<T>(sqrtf(M[0] * M[0] + M[1] * M[1] + M[2] * M[2]),
					sqrtf(M[4] * M[4] + M[5] * M[5] + M[6] * M[6]),
					sqrtf(M[8] * M[8] + M[9] * M[9] + M[10] * M[10]));
			}

			/*!	Translate a vector by the inverse of the translation part of this matrix.
			*/
			void InverseTranslateVector (C_Vector3D<float>& vector) const
			{
				vector.X = vector.X - M[12];
				vector.Y = vector.Y - M[13];
				vector.Z = vector.Z - M[14];
			}

			/*! Rotate a vector by the inverse of the rotation part of this matrix.
			*/
			void InverseRotateVector (C_Vector3D<float>& vect) const
			{
				C_Vector3D<float> tmp = vect;

				vect.X = tmp.X*M[0] + tmp.Y*M[1] + tmp.Z*M[2];
				vect.Y = tmp.X*M[4] + tmp.Y*M[5] + tmp.Z*M[6];
				vect.Z = tmp.X*M[8] + tmp.Y*M[9] + tmp.Z*M[10];
			}

			/*! Rotate a vector by the rotation part of this matrix.
			*/
			void RotateVector (C_Vector3D<float>& vect) const
			{
				C_Vector3D<float> tmp = vect;

				vect.X = tmp.X*M[0] + tmp.Y*M[4] + tmp.Z*M[8];
				vect.Y = tmp.X*M[1] + tmp.Y*M[5] + tmp.Z*M[9];
				vect.Z = tmp.X*M[2] + tmp.Y*M[6] + tmp.Z*M[10];
			}

			/*! An alternate transform vector method, writing into a second vector
			*/
			void RotateVector (C_Vector3D<float>& out, const C_Vector3D<float>& in) const
			{
				out.X = in.X*M[0] + in.Y*M[4] + in.Z*M[8];
				out.Y = in.X*M[1] + in.Y*M[5] + in.Z*M[9];
				out.Z = in.X*M[2] + in.Y*M[6] + in.Z*M[10];
			}

			/*! An alternate transform vector method, writing into an array of 3 floats
			*/
			void RotateVector (T *out, const C_Vector3D<float> &in) const
			{
				out[0] = in.X*M[0] + in.Y*M[4] + in.Z*M[8];
				out[1] = in.X*M[1] + in.Y*M[5] + in.Z*M[9];
				out[2] = in.X*M[2] + in.Y*M[6] + in.Z*M[10];
			}

			/*! Transforms the vector by this matrix
			*/
			void TransformVector (C_Vector3D<float>& vect) const
			{
				float vector[3];

				vector[0] = vect.X*M[0] + vect.Y*M[4] + vect.Z*M[8] + M[12];
				vector[1] = vect.X*M[1] + vect.Y*M[5] + vect.Z*M[9] + M[13];
				vector[2] = vect.X*M[2] + vect.Y*M[6] + vect.Z*M[10] + M[14];

				vect.X = vector[0];
				vect.Y = vector[1];
				vect.Z = vector[2];
			}

			/*! Transforms input vector by this matrix and stores result in output vector
			*/
			void TransformVector (C_Vector3D<float>& out, const C_Vector3D<float>& in) const
			{
				out.X = in.X*M[0] + in.Y*M[4] + in.Z*M[8] + M[12];
				out.Y = in.X*M[1] + in.Y*M[5] + in.Z*M[9] + M[13];
				out.Z = in.X*M[2] + in.Y*M[6] + in.Z*M[10] + M[14];
			}	

			/*! An alternate transform vector method, writing into an array of 4 floats
			*/
			void TransformVector (T *out,const C_Vector3D<float> &in) const
			{
				out[0] = in.X*M[0] + in.Y*M[4] + in.Z*M[8] + M[12];
				out[1] = in.X*M[1] + in.Y*M[5] + in.Z*M[9] + M[13];
				out[2] = in.X*M[2] + in.Y*M[6] + in.Z*M[10] + M[14];
				out[3] = in.X*M[3] + in.Y*M[7] + in.Z*M[11] + M[15];
			}

			/*! Translate a vector by the translation part of this matrix.
			*/
			void TranslateVector (C_Vector3D<float>& vect) const
			{
				vect.X = vect.X + M[12];
				vect.Y = vect.Y + M[13];
				vect.Z = vect.Z + M[14];
			}

			/*! Transforms a plane by this matrix
			*/
//			void TransformPlane ( core::plane3d<float> &plane) const
//			{

//			}

			/*! Transforms a plane by this matrix
			*/
//			void TransformPlane (const core::plane3d<float> &in, core::plane3d<float> &out) const
//			{

//			}

			/*! Transforms a axis aligned bounding box
			**	The result box of this operation may not be accurate at all. For correct results, use TransformBoxEx()
			*/
//			void TransformBox (aabbox3d<float>& box) const
//			{

//			}

			/*! Transforms a axis aligned bounding box
			**	The result box of this operation should by accurate, but this operation is slower than TransformBox(). 
			*/
//			void TransformBoxEx (core::aabbox3d<float>& box) const
//			{

//			}

			/*!	Calculates inverse of matrix. Slow.
			**	\return Returns false if there is no inverse matrix.
			*/
			bool MakeInverse () 
			{
				if (m_DefinitelyIdentityMatrix)
					return true;

				C_Matrix<T> temp (CF_EMPTY);

				if (GetInverse (temp))
				{
					*this = temp;
					return true;
				}

				return false;
			}

			/*!	Inverts a primitive matrix which only contains a translation and a rotation
			**	\param out: where result matrix is written to. 
			*/
			bool GetInversePrimitive (C_Matrix<T>& out) const
			{
				out.M[0 ] = M[0];
				out.M[1 ] = M[4];
				out.M[2 ] = M[8];
				out.M[3 ] = 0;

				out.M[4 ] = M[1];
				out.M[5 ] = M[5];
				out.M[6 ] = M[9];
				out.M[7 ] = 0;

				out.M[8 ] = M[2];
				out.M[9 ] = M[6];
				out.M[10] = M[10];
				out.M[11] = 0;

				out.M[12] = (T)-(M[12]*M[0] + M[13]*M[1] + M[14]*M[2]);
				out.M[13] = (T)-(M[12]*M[4] + M[13]*M[5] + M[14]*M[6]);
				out.M[14] = (T)-(M[12]*M[8] + M[13]*M[9] + M[14]*M[10]);
				out.M[15] = 1;

				out.m_DefinitelyIdentityMatrix = m_DefinitelyIdentityMatrix;

				return true;
			}

			/*!	Gets the inversed matrix of this one
			**	\param out: where result matrix is written to.
			**	\return Returns false if there is no inverse matrix. 
			*/
			bool GetInverse (C_Matrix<T>& out) const
			{
				/// Calculates the inverse of this Matrix
				/// The inverse is calculated using Cramers rule.
				/// If no inverse exists then 'false' is returned.

				if (this->IsIdentity ())
				{
					out = *this;

					return true;
				}

				const C_Matrix<T> &m = *this;

				float d = (m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)) * (m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2)) -
					(m(0, 0) * m(1, 2) - m(0, 2) * m(1, 0)) * (m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1)) +
					(m(0, 0) * m(1, 3) - m(0, 3) * m(1, 0)) * (m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1)) +
					(m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)) * (m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0)) -
					(m(0, 1) * m(1, 3) - m(0, 3) * m(1, 1)) * (m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0)) +
					(m(0, 2) * m(1, 3) - m(0, 3) * m(1, 2)) * (m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0));

				if (C_MathUtils::IsZero(d))
					return false;

				d = C_MathUtils::Reciprocal(d);

				out(0, 0) = d * (m(1, 1) * (m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2)) +
					m(1, 2) * (m(2, 3) * m(3, 1) - m(2, 1) * m(3, 3)) +
					m(1, 3) * (m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1)));
				out(0, 1) = d * (m(2, 1) * (m(0, 2) * m(3, 3) - m(0, 3) * m(3, 2)) +
					m(2, 2) * (m(0, 3) * m(3, 1) - m(0, 1) * m(3, 3)) +
					m(2, 3) * (m(0, 1) * m(3, 2) - m(0, 2) * m(3, 1)));
				out(0, 2) = d * (m(3, 1) * (m(0, 2) * m(1, 3) - m(0, 3) * m(1, 2)) +
					m(3, 2) * (m(0, 3) * m(1, 1) - m(0, 1) * m(1, 3)) +
					m(3, 3) * (m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)));
				out(0, 3) = d * (m(0, 1) * (m(1, 3) * m(2, 2) - m(1, 2) * m(2, 3)) +
					m(0, 2) * (m(1, 1) * m(2, 3) - m(1, 3) * m(2, 1)) +
					m(0, 3) * (m(1, 2) * m(2, 1) - m(1, 1) * m(2, 2)));
				out(1, 0) = d * (m(1, 2) * (m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0)) +
					m(1, 3) * (m(2, 2) * m(3, 0) - m(2, 0) * m(3, 2)) +
					m(1, 0) * (m(2, 3) * m(3, 2) - m(2, 2) * m(3, 3)));
				out(1, 1) = d * (m(2, 2) * (m(0, 0) * m(3, 3) - m(0, 3) * m(3, 0)) +
					m(2, 3) * (m(0, 2) * m(3, 0) - m(0, 0) * m(3, 2)) +
					m(2, 0) * (m(0, 3) * m(3, 2) - m(0, 2) * m(3, 3)));
				out(1, 2) = d * (m(3, 2) * (m(0, 0) * m(1, 3) - m(0, 3) * m(1, 0)) +
					m(3, 3) * (m(0, 2) * m(1, 0) - m(0, 0) * m(1, 2)) +
					m(3, 0) * (m(0, 3) * m(1, 2) - m(0, 2) * m(1, 3)));
				out(1, 3) = d * (m(0, 2) * (m(1, 3) * m(2, 0) - m(1, 0) * m(2, 3)) +
					m(0, 3) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) +
					m(0, 0) * (m(1, 2) * m(2, 3) - m(1, 3) * m(2, 2)));
				out(2, 0) = d * (m(1, 3) * (m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0)) +
					m(1, 0) * (m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1)) +
					m(1, 1) * (m(2, 3) * m(3, 0) - m(2, 0) * m(3, 3)));
				out(2, 1) = d * (m(2, 3) * (m(0, 0) * m(3, 1) - m(0, 1) * m(3, 0)) +
					m(2, 0) * (m(0, 1) * m(3, 3) - m(0, 3) * m(3, 1)) +
					m(2, 1) * (m(0, 3) * m(3, 0) - m(0, 0) * m(3, 3)));
				out(2, 2) = d * (m(3, 3) * (m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)) +
					m(3, 0) * (m(0, 1) * m(1, 3) - m(0, 3) * m(1, 1)) +
					m(3, 1) * (m(0, 3) * m(1, 0) - m(0, 0) * m(1, 3)));
				out(2, 3) = d * (m(0, 3) * (m(1, 1) * m(2, 0) - m(1, 0) * m(2, 1)) +
					m(0, 0) * (m(1, 3) * m(2, 1) - m(1, 1) * m(2, 3)) +
					m(0, 1) * (m(1, 0) * m(2, 3) - m(1, 3) * m(2, 0)));
				out(3, 0) = d * (m(1, 0) * (m(2, 2) * m(3, 1) - m(2, 1) * m(3, 2)) +
					m(1, 1) * (m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0)) +
					m(1, 2) * (m(2, 1) * m(3, 0) - m(2, 0) * m(3, 1)));
				out(3, 1) = d * (m(2, 0) * (m(0, 2) * m(3, 1) - m(0, 1) * m(3, 2)) +
					m(2, 1) * (m(0, 0) * m(3, 2) - m(0, 2) * m(3, 0)) +
					m(2, 2) * (m(0, 1) * m(3, 0) - m(0, 0) * m(3, 1)));
				out(3, 2) = d * (m(3, 0) * (m(0, 2) * m(1, 1) - m(0, 1) * m(1, 2)) +
					m(3, 1) * (m(0, 0) * m(1, 2) - m(0, 2) * m(1, 0)) +
					m(3, 2) * (m(0, 1) * m(1, 0) - m(0, 0) * m(1, 1)));
				out(3, 3) = d * (m(0, 0) * (m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1)) +
					m(0, 1) * (m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2)) +
					m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0)));

				out.m_DefinitelyIdentityMatrix = m_DefinitelyIdentityMatrix;

				return true;
			}

			/*! Builds a right-handed perspective projection matrix based on a field of view
			*/
			C_Matrix<T>& BuildProjectionMatrixPerspectiveFovRH (float fieldOfViewRadians, float aspectRatio, float zNear, float zFar)
			{
				const double h = C_MathUtils::Reciprocal(tan (fieldOfViewRadians * 0.5));

				assert (aspectRatio != 0.f); //divide by zero

				const T w = h / aspectRatio;

				assert (zNear != zFar); //divide by zero

				M[0] = w;
				M[1] = 0;
				M[2] = 0;
				M[3] = 0;

				M[4] = 0;
				M[5] = (T)h;
				M[6] = 0;
				M[7] = 0;

				M[8] = 0;
				M[9] = 0;
				M[10] = (T)(zFar/(zNear-zFar)); // DirectX version
				//		M[10] = (T)(zFar+zNear/(zNear-zFar)); // OpenGL version
				M[11] = -1;

				M[12] = 0;
				M[13] = 0;
				M[14] = (T)(zNear*zFar/(zNear-zFar)); // DirectX version
				//		M[14] = (T)(2.0f*zNear*zFar/(zNear-zFar)); // OpenGL version
				M[15] = 0;

				m_DefinitelyIdentityMatrix = false;

				return *this;
			}

			/*! Builds a left-handed perspective projection matrix based on a field of view
			*/
			C_Matrix<T>& BuildProjectionMatrixPerspectiveFovLH (float fieldOfViewRadians, float aspectRatio, float zNear, float zFar)
			{
				const double h = C_MathUtils::Reciprocal (tan(fieldOfViewRadians * 0.5));

				assert (aspectRatio != 0.f); //divide by zero

				const T w = (T)(h / aspectRatio);

				assert (zNear != zFar); //divide by zero

				M[0] = w;
				M[1] = 0;
				M[2] = 0;
				M[3] = 0;

				M[4] = 0;
				M[5] = (T)h;
				M[6] = 0;
				M[7] = 0;

				M[8] = 0;
				M[9] = 0;
				M[10] = (T)(zFar/(zFar-zNear));
				M[11] = 1;

				M[12] = 0;
				M[13] = 0;
				M[14] = (T)(-zNear*zFar/(zFar-zNear));
				M[15] = 0;

				m_DefinitelyIdentityMatrix = false;

				return *this;
			}

			/*! Builds a right-handed perspective projection matrix.
			*/
			C_Matrix<T>& BuildProjectionMatrixPerspectiveRH (float widthOfViewVolume, float heightOfViewVolume, float zNear, float zFar)
			{
				_IRR_DEBUG_BREAK_IF(widthOfViewVolume==0.f); //divide by zero
				_IRR_DEBUG_BREAK_IF(heightOfViewVolume==0.f); //divide by zero
				_IRR_DEBUG_BREAK_IF(zNear==zFar); //divide by zero

				M[0] = (T)(2*zNear/widthOfViewVolume);
				M[1] = 0;
				M[2] = 0;
				M[3] = 0;

				M[4] = 0;
				M[5] = (T)(2*zNear/heightOfViewVolume);
				M[6] = 0;
				M[7] = 0;

				M[8] = 0;
				M[9] = 0;
				M[10] = (T)(zFar/(zNear-zFar));
				M[11] = -1;

				M[12] = 0;
				M[13] = 0;
				M[14] = (T)(zNear*zFar/(zNear-zFar));
				M[15] = 0;

				m_DefinitelyIdentityMatrix = false;

				return *this;
			}

			/*! Builds a left-handed perspective projection matrix.
			*/
			C_Matrix<T>& BuildProjectionMatrixPerspectiveLH (float widthOfViewVolume, float heightOfViewVolume, float zNear, float zFar)
			{
				_IRR_DEBUG_BREAK_IF(widthOfViewVolume==0.f); //divide by zero
				_IRR_DEBUG_BREAK_IF(heightOfViewVolume==0.f); //divide by zero
				_IRR_DEBUG_BREAK_IF(zNear==zFar); //divide by zero

				M[0] = (T)(2*zNear/widthOfViewVolume);
				M[1] = 0;
				M[2] = 0;
				M[3] = 0;

				M[4] = 0;
				M[5] = (T)(2*zNear/heightOfViewVolume);
				M[6] = 0;
				M[7] = 0;

				M[8] = 0;
				M[9] = 0;
				M[10] = (T)(zFar/(zFar-zNear));
				M[11] = 1;

				M[12] = 0;
				M[13] = 0;
				M[14] = (T)(zNear*zFar/(zNear-zFar));
				M[15] = 0;

				m_DefinitelyIdentityMatrix = false;

				return *this;
			}

			/*! Builds a left-handed orthogonal projection matrix.
			*/
			C_Matrix<T>& BuildProjectionMatrixOrthoLH (float widthOfViewVolume, float heightOfViewVolume, float zNear, float zFar)
			{
				assert (widthOfViewVolume != 0.f); //divide by zero
				assert (heightOfViewVolume != 0.f); //divide by zero
				assert (zNear != zFar); //divide by zero

				M[0] = (T)(2/widthOfViewVolume);
				M[1] = 0;
				M[2] = 0;
				M[3] = 0;

				M[4] = 0;
				M[5] = (T)(2/heightOfViewVolume);
				M[6] = 0;
				M[7] = 0;

				M[8] = 0;
				M[9] = 0;
				M[10] = (T)(1/(zFar-zNear));
				M[11] = 0;

				M[12] = 0;
				M[13] = 0;
				M[14] = (T)(zNear/(zNear-zFar));
				M[15] = 1;

				m_DefinitelyIdentityMatrix = false;

				return *this;
			}

			/*! Builds a right-handed orthogonal projection matrix.
			*/
			C_Matrix<T>& BuildProjectionMatrixOrthoRH (float widthOfViewVolume, float heightOfViewVolume, float zNear, float zFar)
			{
				_IRR_DEBUG_BREAK_IF(widthOfViewVolume==0.f); //divide by zero
				_IRR_DEBUG_BREAK_IF(heightOfViewVolume==0.f); //divide by zero
				_IRR_DEBUG_BREAK_IF(zNear==zFar); //divide by zero

				M[0] = (T)(2/widthOfViewVolume);
				M[1] = 0;
				M[2] = 0;
				M[3] = 0;

				M[4] = 0;
				M[5] = (T)(2/heightOfViewVolume);
				M[6] = 0;
				M[7] = 0;

				M[8] = 0;
				M[9] = 0;
				M[10] = (T)(1/(zNear-zFar));
				M[11] = 0;

				M[12] = 0;
				M[13] = 0;
				M[14] = (T)(zNear/(zNear-zFar));
				M[15] = -1;

				m_DefinitelyIdentityMatrix = false;

				return *this;
			}

			/*! Builds a left-handed look-at matrix.
			*/
			C_Matrix<T>& BuildCameraLookAtMatrixLH (const C_Vector3D<float>& position, const C_Vector3D<float>& target,	const C_Vector3D<float>& upVector)
			{
				C_Vector3D<float> zaxis = target - position;
				zaxis.SetNormalized();

				C_Vector3D<float> xaxis = upVector.GetCross(zaxis);
				xaxis.SetNormalized();

				C_Vector3D<float> yaxis = zaxis.GetCross(xaxis);

				M[0] = (T)xaxis.X;
				M[1] = (T)yaxis.X;
				M[2] = (T)zaxis.X;
				M[3] = 0;

				M[4] = (T)xaxis.Y;
				M[5] = (T)yaxis.Y;
				M[6] = (T)zaxis.Y;
				M[7] = 0;

				M[8] = (T)xaxis.Z;
				M[9] = (T)yaxis.Z;
				M[10] = (T)zaxis.Z;
				M[11] = 0;

				M[12] = (T)-xaxis.GetDot(position);
				M[13] = (T)-yaxis.GetDot(position);
				M[14] = (T)-zaxis.GetDot(position);
				M[15] = 1;

				m_DefinitelyIdentityMatrix = false;

				return *this;
			}

			/*!	Builds a right-handed look-at matrix.
			*/
			C_Matrix<T>& BuildCameraLookAtMatrixRH (const C_Vector3D<float>& position, const C_Vector3D<float>& target, const C_Vector3D<float>& upVector)
			{
				C_Vector3D<float> zaxis = position - target;
				zaxis.SetNormalized();

				C_Vector3D<float> xaxis = upVector.GetCross(zaxis);
				xaxis.SetNormalized();

				C_Vector3D<float> yaxis = zaxis.GetCross(xaxis);

				M[0] = (T)xaxis.X;
				M[1] = (T)yaxis.X;
				M[2] = (T)zaxis.X;
				M[3] = 0;

				M[4] = (T)xaxis.Y;
				M[5] = (T)yaxis.Y;
				M[6] = (T)zaxis.Y;
				M[7] = 0;

				M[8] = (T)xaxis.Z;
				M[9] = (T)yaxis.Z;
				M[10] = (T)zaxis.Z;
				M[11] = 0;

				M[12] = (T)-xaxis.GetDot(position);
				M[13] = (T)-yaxis.GetDot(position);
				M[14] = (T)-zaxis.GetDot(position);
				M[15] = 1;

				m_DefinitelyIdentityMatrix = false;

				return *this;
			}

			/*!	Creates a new matrix as interpolated matrix from two other ones.
			**	\param b: other matrix to interpolate with
			**	\param time: Must be a value between 0 and 1. 
			*/
			C_Matrix<T> Interpolate (const C_Matrix<T>& b, float time) const
			{
				C_Matrix<T> mat (MATRX_CONSTRUCTOR_EMPTY);

				for (unsigned int i = 0; i < 16; i += 4)
				{
					mat.M[i+0] = (T)(M[i+0] + ( b.M[i+0] - M[i+0] ) * time);
					mat.M[i+1] = (T)(M[i+1] + ( b.M[i+1] - M[i+1] ) * time);
					mat.M[i+2] = (T)(M[i+2] + ( b.M[i+2] - M[i+2] ) * time);
					mat.M[i+3] = (T)(M[i+3] + ( b.M[i+3] - M[i+3] ) * time);
				}
				return mat;
			}

			/*!	Gets transposed matrix
			*/
			C_Matrix<T> GetTransposed () const
			{
				C_Matrix<T> t (CF_EMPTY);

				GetTransposed (t);

				return t;
			}

			/*!	Gets transposed matrix
			*/	
			void GetTransposed (C_Matrix<T>& dest) const
			{
				dest[ 0] = M[ 0];
				dest[ 1] = M[ 4];
				dest[ 2] = M[ 8];
				dest[ 3] = M[12];

				dest[ 4] = M[ 1];
				dest[ 5] = M[ 5];
				dest[ 6] = M[ 9];
				dest[ 7] = M[13];

				dest[ 8] = M[ 2];
				dest[ 9] = M[ 6];
				dest[10] = M[10];
				dest[11] = M[14];

				dest[12] = M[ 3];
				dest[13] = M[ 7];
				dest[14] = M[11];
				dest[15] = M[15];

				dest.m_DefinitelyIdentityMatrix = m_DefinitelyIdentityMatrix;
			}

			/*!	Sets all matrix data members at once
			*/
			C_Matrix<T>& SetMatrixData (const T* data)
			{
				memcpy (M,data, 16*sizeof(T));

				m_DefinitelyIdentityMatrix = false;

				return *this;
			}

			/*!	Sets if the matrix is definitely identity matrix
			*/
			void SetDefinitelyIdentityMatrix (bool isDefinitelyIdentityMatrix)
			{
				m_DefinitelyIdentityMatrix = isDefinitelyIdentityMatrix;
			}

			/*!	Gets if the matrix is definitely identity matrix
			*/
			bool GetDefinitelyIdentityMatrix () const
			{
				return m_DefinitelyIdentityMatrix;
			}

			/*!	Compare two matrices using the equal method
			*/
			bool Equals (const C_Matrix<T>& other, const T tolerance = (T)C_MathUtils::ROUNDING_ERROR_FLOAT) const
			{
				if (m_DefinitelyIdentityMatrix && other.m_DefinitelyIdentityMatrix)
					return true;

				for (int i = 0; i < 16; ++i)
					if (!C_MathUtils::Equals(M[i], other.M[i], tolerance))
						return false;

				return true;
			}

		public:
			//!	Matrix data, stored in row-major order
			union
			{
				T M[16];
				T _M[4][4];

				struct  
				{
					T _m11, _m12, _m13, _m14;
					T _m21, _m22, _m23, _m24;
					T _m31, _m32, _m33, _m34;
					T _m41, _m42, _m43, _m44;
				};
			};
					
		private:
			//! Flag is this matrix is identity matrix
			mutable bool	m_DefinitelyIdentityMatrix;
		};

	}	//	namespace CORE
}	//	namespace REDEEMER

#endif	//	_R_CORE_MATRIX_H_
