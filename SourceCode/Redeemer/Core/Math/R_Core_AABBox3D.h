/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_AABBox3D.h
**		Template Axis Aligned Bounding Box class
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_CORE_AABBOX3D_H_
#define _R_CORE_AABBOX3D_H_

#include "R_Core_Plane3D.h"

namespace REDEEMER
{
	namespace CORE
	{
		/*!
		**	\brief Axis Aligned Bounding Boxes are used as bounding volumes for primitives in 3D space
		*/
		template<typename T> class C_AABBox3D
		{	
		public:	
			/*! Default Constructor.
			*/
			C_AABBox3D(): 
			  MinEdge(-1,-1,-1), 
			  MaxEdge(1,1,1) 
			  {
				  //	EMPTY
			  }

			/*! Constructor with min edge and max edge.
			*/
			C_AABBox3D(const C_Vector3D<T>& min, const C_Vector3D<T>& max): 
			  MinEdge(min), 
			  MaxEdge(max) 
			  {
				  //	EMPTY
			  }

			/*! Constructor with only one point.
			*/
			C_AABBox3D(const C_Vector3D<T>& init):
			  MinEdge(init), 
			  MaxEdge(init) 
			  {
				  //	EMPTY
			  }

			/*! Constructor with min edge and max edge as single values, not vectors.
			*/
			C_AABBox3D (T minx, T miny, T minz, T maxx, T maxy, T maxz): 
			  MinEdge(minx, miny, minz), 
			  MaxEdge(maxx, maxy, maxz) 
			  {
				  //	EMPTY
			  }

			/*! Equality operator
			**	\param other box to compare with.
			**	\return True if both boxes are equal, else false. 
			*/
			bool operator== (const C_AABBox3D<T>& other) const 
			{ 
				return (MinEdge == other.MinEdge && other.MaxEdge == MaxEdge);
			}

			/*! Inequality operator
			**	\param other box to compare with.
			**	\return True if both boxes are different, else false. 
			*/
			bool operator!= (const C_AABBox3D<T>& other) const
			{ 
				return !(MinEdge == other.MinEdge && other.MaxEdge == MaxEdge);
			}

			/*! Resets the bounding box to a one-point box.
			**	\param x X coord of the point.
			**	\param y Y coord of the point.
			**	\param z Z coord of the point. 
			*/
			void Reset (T x, T y, T z)
			{
				MaxEdge.Set (x, y, z);

				MinEdge = MaxEdge;
			}

			/*! Resets the bounding box.
			**	\param initValue New box to set this one to. 
			*/
			void Reset (const C_AABBox3D<T>& initValue)
			{
				*this = initValue;
			}

			/*! Resets the bounding box to a one-point box.
			**	\param initValue New point. 
			*/
			void Reset (const C_Vector3D<T>& initValue)
			{
				MaxEdge = initValue;
				MinEdge = initValue;
			}

			/*! Adds a point to the bounding box
			**	The box grows bigger, if point was outside of the box.
			**	\param p: Point to add into the box. 
			*/
			void AddInternalPoint (const C_Vector3D<T>& p)
			{
				AddInternalPoint (p.X, p.Y, p.Z);
			}

			/*! Adds another bounding box
			**	The box grows bigger, if the new box was outside of the box.
			**	\param b: Other bounding box to add into this box. 
			*/
			void AddInternalBox (const C_AABBox3D<T>& b)
			{
				addInternalPoint (b.MaxEdge);
				addInternalPoint (b.MinEdge);
			}

			/*! Adds a point to the bounding box
			**	The box grows bigger, if point is outside of the box.
			**	\param x X coordinate of the point to add to this box.
			**	\param y Y coordinate of the point to add to this box.
			**	\param z Z coordinate of the point to add to this box. 
			*/
			void AddInternalPoint (T x, T y, T z)
			{
				if (x>MaxEdge.X) MaxEdge.X = x;

				if (y>MaxEdge.Y) MaxEdge.Y = y;

				if (z>MaxEdge.Z) MaxEdge.Z = z;

				if (x<MinEdge.X) MinEdge.X = x;

				if (y<MinEdge.Y) MinEdge.Y = y;

				if (z<MinEdge.Z) MinEdge.Z = z;
			}

			/*! Get center of the bounding box
			**	\return Center of the bounding box. 
			*/
			C_Vector3D<T> GetCenter() const
			{
				return (MinEdge + MaxEdge) / 2;
			}

			/*!	Get extent of the box (maximal distance of two points in the box)
			**	\return Extent of the bounding box. 
			*/
			C_Vector3D<T> GetExtent() const
			{
				return MaxEdge - MinEdge;
			}

			/*! Check if the box is empty.
			**	This means that there is no space between the min and max edge.
			**	\return True if box is empty, else false. 
			*/
			bool IsEmpty() const
			{
				return MinEdge.Equals (MaxEdge);
			}

			/*! Get the volume enclosed by the box in cubed units
			*/
			T GetVolume() const
			{
				const C_Vector3D<T> e = GetExtent();

				return e.X * e.Y * e.Z;
			}

			/*! Get the surface area of the box in squared units
			*/
			T GetArea() const
			{
				const C_Vector3D<T> e = GetExtent();

				return 2*(e.X*e.Y + e.X*e.Z + e.Y*e.Z);
			}

			/*! Stores all 8 edges of the box into an array
			**	\param edges: Pointer to array of 8 edges. 
			*/
			void GetEdges(C_Vector3D<T> *edges) const
			{
				const C_Vector3D<T> middle = GetCenter();
				const C_Vector3D<T> diag = middle - MaxEdge;

				/*
				Edges are stored in this way:
				Hey, am I an ascii artist, or what? :) niko.
					   /3--------/7
					  / |       / |
					 /  |      /  |
					1---------5   |
					|  /2- - -|- -6
					| /       |  /
					|/        | /
					0---------4/
				*/

				edges[0].Set(middle.X + diag.X, middle.Y + diag.Y, middle.Z + diag.Z);
				edges[1].Set(middle.X + diag.X, middle.Y - diag.Y, middle.Z + diag.Z);
				edges[2].Set(middle.X + diag.X, middle.Y + diag.Y, middle.Z - diag.Z);
				edges[3].Set(middle.X + diag.X, middle.Y - diag.Y, middle.Z - diag.Z);
				edges[4].Set(middle.X - diag.X, middle.Y + diag.Y, middle.Z + diag.Z);
				edges[5].Set(middle.X - diag.X, middle.Y - diag.Y, middle.Z + diag.Z);
				edges[6].Set(middle.X - diag.X, middle.Y + diag.Y, middle.Z - diag.Z);
				edges[7].Set(middle.X - diag.X, middle.Y - diag.Y, middle.Z - diag.Z);
			}

			/*! Repairs the box.
			**	Necessary if for example MinEdge and MaxEdge are swapped. 
			*/
			void Repair ()
			{
				T t;

				if (MinEdge.X > MaxEdge.X)
				{ 
					t = MinEdge.X; 
					MinEdge.X = MaxEdge.X; 
					MaxEdge.X = t; 
				}

				if (MinEdge.Y > MaxEdge.Y)
				{
					t = MinEdge.Y; 
					MinEdge.Y = MaxEdge.Y; 
					MaxEdge.Y = t; 
				}

				if (MinEdge.Z > MaxEdge.Z)
				{
					t = MinEdge.Z; 
					MinEdge.Z = MaxEdge.Z; 
					MaxEdge.Z = t; 
				}
			}

			/*! Calculates a new interpolated bounding box.
			**	d=0 returns other, d=1 returns this, all other values blend between the two boxes.
			**	\param other Other box to interpolate between
			**	\param d Value between 0.0f and 1.0f.
			**	\return Interpolated box. 
			*/
			C_AABBox3D<T> GetInterpolated (const C_AABBox3D<T>& other, float d) const
			{
				float inv = 1.0f - d;

				return C_AABBox3D<T>((other.MinEdge*inv) + (MinEdge*d),	(other.MaxEdge*inv) + (MaxEdge*d));
			}

			/*! Determines if a point is within this box.
			**	Border is included (IS part of the box)!
			**	\param p: Point to check.
			**	\return True if the point is within the box and false if not 
			*/
			bool IsPointInside(const C_Vector3D<T>& p) const
			{
				return (p.X >= MinEdge.X && p.X <= MaxEdge.X && p.Y >= MinEdge.Y && p.Y <= MaxEdge.Y && p.Z >= MinEdge.Z && p.Z <= MaxEdge.Z);
			}

			/*! Determines if a point is within this box and not its borders.
			**	Border is excluded (NOT part of the box)!
			**	\param p: Point to check.
			**	\return True if the point is within the box and false if not. 
			*/
			bool IsPointTotalInside (const C_Vector3D<T>& p) const
			{
				return (p.X > MinEdge.X && p.X < MaxEdge.X && p.Y > MinEdge.Y && p.Y < MaxEdge.Y && p.Z > MinEdge.Z && p.Z < MaxEdge.Z);
			}

			/*! Check if this box is completely inside the 'other' box.
			**	\param other: Other box to check against.
			**	\return True if this box is completely inside the other box, otherwise false. 
			*/
			bool IsFullInside (const C_AABBox3D<T>& other) const
			{
				return (MinEdge.X >= other.MinEdge.X && MinEdge.Y >= other.MinEdge.Y && MinEdge.Z >= other.MinEdge.Z &&
					MaxEdge.X <= other.MaxEdge.X && MaxEdge.Y <= other.MaxEdge.Y && MaxEdge.Z <= other.MaxEdge.Z);
			}

			/*! Determines if the axis-aligned box intersects with another axis-aligned box.
			**	\param other: Other box to check a intersection with.
			**	\return True if there is an intersection with the other box, otherwise false. 
			*/
			bool IntersectsWithBox (const C_AABBox3D<T>& other) const
			{
				return (MinEdge.X <= other.MaxEdge.X && MinEdge.Y <= other.MaxEdge.Y && MinEdge.Z <= other.MaxEdge.Z &&
					MaxEdge.X >= other.MinEdge.X && MaxEdge.Y >= other.MinEdge.Y && MaxEdge.Z >= other.MinEdge.Z);
			}

			/*! Classifies a relation with a plane.
			**	\param plane Plane to classify relation to.
			**	\return Returns INTERSECTION_FRONT if the box is in front of the plane, INTERSECTION_BACK if the box is behind the plane, and
			**	INTERSECTION_CLIPPED if it is on both sides of the plane. 
			*/
			EIntersectionType ClassifyPlaneRelation(const C_Plane3D<T>& plane) const
			{
				C_Vector3D<T> nearPoint(MaxEdge);
				C_Vector3D<T> farPoint(MinEdge);

				if (plane.Normal.X > (T)0)
				{
					nearPoint.X = MinEdge.X;
					farPoint.X = MaxEdge.X;
				}

				if (plane.Normal.Y > (T)0)
				{
					nearPoint.Y = MinEdge.Y;
					farPoint.Y = MaxEdge.Y;
				}

				if (plane.Normal.Z > (T)0)
				{
					nearPoint.Z = MinEdge.Z;
					farPoint.Z = MaxEdge.Z;
				}

				if (plane.Normal.dotProduct(nearPoint) + plane.D > (T)0)
					return IT_Front;

				if (plane.Normal.dotProduct(farPoint) + plane.D > (T)0)
					return IT_Clipped;

				return IT_Back;
			}

		public:
			C_Vector3D<T> MinEdge;		///<! The near edge
			C_Vector3D<T> MaxEdge;		///<! The far edge
		};

	}	//	namespace CORE
}	//	namespace REDEEMER

#endif	//	_R_CORE_AABBOX3D_H_
