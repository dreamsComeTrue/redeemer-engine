/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_Plane.h
**		Template plane class
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_CORE_PLANE3D_H_
#define _R_CORE_PLANE3D_H_

#include "R_Core_Vector3D.h"

namespace REDEEMER
{
	namespace CORE
	{
		/*! Enumeration for intersection relations of 3d objects
		*/
		enum EIntersectionType
		{
			IT_Front = 0,
			IT_Back,
			IT_Planar,
			IT_Spanning,
			IT_Clipped
		};		   

		/*!
		**	\brief Template plane class
		*/
		template<typename T> class C_Plane3D
		{	
		public:	
			/*! Constructor
			*/
			C_Plane3D () : 
			  Normal (0, 1, 0) 
			  { 
				  RecalculateD (C_Vector3D<T> (0, 0, 0)); 
			  }
			
			/*! Constructor
			*/
			C_Plane3D (const C_Vector3D<T>& MPoint, const C_Vector3D<T>& Normal) : 
			  Normal(Normal) 
			  { 
				  RecalculateD(MPoint); 
			  }
			
			/*! Constructor
			*/
			C_Plane3D (T px, T py, T pz, T nx, T ny, T nz) : 
			  Normal(nx, ny, nz) 
			  { 
				  RecalculateD (C_Vector3D<T> (px, py, pz)); 
			  }
			
			/*! Constructor
			*/
			C_Plane3D(const C_Vector3D<T>& point1, const C_Vector3D<T>& point2, const C_Vector3D<T>& point3)
			{ 
				SetPlane (point1, point2, point3); 
			}
			
			/*! Constructor
			*/
			C_Plane3D(const C_Vector3D<T> & normal, const T d) : 
				Normal(normal), 
				D(d) 
				{
					//	EMPTY
				}

			/*!	Equality operator
			*/
			bool operator== (const C_Plane3D<T>& other) const 
			{ 
				return (Equals (D, other.D) && Normal == other.Normal);
			}

			/*!	Inequality operator
			*/
			bool operator!= (const C_Plane3D<T>& other) const 
			{ 
				return !(*this == other);
			}

			/*!	Sets plane data
			**	\param point Point on plane
			**	\param nvector normal vector
			*/
			void SetPlane(const C_Vector3D<T>& point, const C_Vector3D<T>& nvector)
			{
				Normal = nvector;

				RecalculateD(point);
			}

			/*!	Sets plane data
			**	\param nvect normal vector
			**	\param d distance from origin
			*/
			void SetPlane(const C_Vector3D<T>& nvect, T d)
			{
				Normal = nvect;
				D = d;
			}

			/*!	Sets plane data
			**	\param point1 part of the plane
			**	\param point2 part of the plane
			**	\param point3 part of the plane
			*/
			void SetPlane (const C_Vector3D<T>& point1, const C_Vector3D<T>& point2, const C_Vector3D<T>& point3)
			{
				// creates the plane from 3 memberpoints
				Normal = (point2 - point1).GetCross (point3 - point1);
				Normal.SetNormalized ();

				RecalculateD(point1);
			}
					   
			/*! Get an intersection with a 3d line.
			**	\param lineVect Vector of the line to intersect with.
			**	\param linePoint Point of the line to intersect with.
			**	\param outIntersection Place to store the intersection point, if there is one.
			**	\return True if there was an intersection, false if there was not.
			*/
			bool GetIntersectionWithLine (const C_Vector3D<T>& linePoint, const C_Vector3D<T>& lineVect, C_Vector3D<T>& outIntersection) const
			{
				T t2 = Normal.GetDot(lineVect);

				if (t2 == 0)
					return false;

				T t =- (Normal.GetDot(linePoint) + D) / t2;
				outIntersection = linePoint + (lineVect * t);

				return true;
			}

			/*! Classifies the relation of a point to this plane.
			**	\param point Point to classify its relation.
			**	\return INTERSECTION_FRONT if the point is in front of the plane, INTERSECTION_BACK if the point is behind of the plane, and
			**	INTERSECTION_PLANAR if the point is within the plane. 
			*/
			EIntersectionType ClassifyPointRelation (const C_Vector3D<T>& point) const
			{
				const T d = Normal.GetDot(point) + D;

				if (d < -ROUNDING_ERROR_f32)
					return IT_Back;

				if (d > ROUNDING_ERROR_f32)
					return IT_Front;

				return IT_Planar;
			}

			/*! Recalculates the distance from origin by applying a new member point to the plane.
			*/
			void RecalculateD (const C_Vector3D<T>& MPoint)
			{
				D = - MPoint.GetDot (Normal);
			}

			/*! Gets a member point of the plane.
			*/
			C_Vector3D<T> GetMemberPoint () const
			{
				return Normal * -D;
			}

			/*! Tests if there is an intersection with the other plane
			**	\return True if there is a intersection. 
			*/
			bool ExistsIntersection (const C_Plane3D<T>& other) const
			{
				C_Vector3D<T> cross = other.Normal.GetCross (Normal);

				return cross.GetMagnitude() > CORE::ROUNDING_ERROR_f32;
			}

			/*! Get the distance to a point.
			**	Note that this only works if the normal is normalized. 
			*/
			T GetDistanceTo (const C_Vector3D<T>& point) const
			{
				return point.GetDot (Normal) + D;
			}


		public:			
			C_Vector3D<T>	Normal;		///< Normal vector of the plane.
			T				D;			///< Distance from origin.
		};

	}	//	namespace CORE
}	//	namespace REDEEMER

#endif	//	_R_CORE_PLANE3D_H_
