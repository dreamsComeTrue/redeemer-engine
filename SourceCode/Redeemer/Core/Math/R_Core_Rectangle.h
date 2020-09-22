/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_Rectangle.h
**		Rectangles are commonly used as a part of the GUI
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_CORE_RECTANGLE_H_
#define _R_CORE_RECTANGLE_H_

#include "R_Core_Vector2D.h"

namespace REDEEMER
{
	namespace CORE
	{
		/*!
		**	\brief Rectangle consists of two coordinates
		*/
		template<typename T> class C_Rectangle
		{	
		public:	
			/*!	Default constructor creating empty rectangle at (0,0)
			*/
			C_Rectangle() : 
			  UpperLeftCorner(0,0), 
			  LowerRightCorner(0,0) 
			  {
				  //	EMPTY
			  }

			/*!	Constructor with two corners
			*/
			C_Rectangle (T x, T y, T x2, T y2) : 
			  UpperLeftCorner (x,y), 
			  LowerRightCorner(x2,y2) 
			  {
				  //	EMPTY
			  }

			/*!	Constructor with two corners
			*/
			C_Rectangle (const C_Vector2D<T>& upperLeft, const C_Vector2D<T>& lowerRight) : 
			  UpperLeftCorner(upperLeft), 
			  LowerRightCorner(lowerRight) 
			  {
				  //	EMPTY
			  }

			/*!	Constructor with upper left corner and dimension
			*/
			template <class U> C_Rectangle (const C_Vector2D<T>& position, const int width, const int height) : 
				UpperLeftCorner(position),
				LowerRightCorner (position.X + width, position.Y + height) 
				{
					//	EMPTY
				}

			/*!	Move right by given numbers
			*/
			C_Rectangle<T> operator+ (const C_Vector2D<T>& pos) const
			{
				C_Rectangle<T> ret(*this);

				return ret += pos;
			}

			/*! Move right by given numbers
			*/
			C_Rectangle<T>& operator += (const C_Vector2D<T>& pos)
			{
				UpperLeftCorner += pos;
				LowerRightCorner += pos;

				return *this;
			}

			/*! Move left by given numbers
			*/
			C_Rectangle<T> operator- (const C_Vector2D<T>& pos) const
			{
				C_Rectangle<T> ret(*this);

				return ret -= pos;
			}

			/*!	Move left by given numbers
			*/
			C_Rectangle<T>& operator-= (const C_Vector2D<T>& pos)
			{
				UpperLeftCorner -= pos;
				LowerRightCorner -= pos;

				return *this;
			}

			/*! Equality operator
			*/
			bool operator== (const C_Rectangle<T>& other) const
			{
				return (UpperLeftCorner == other.UpperLeftCorner && LowerRightCorner == other.LowerRightCorner);
			}

			/*! Inequality operator
			*/
			bool operator!= (const C_Rectangle<T>& other) const
			{
				return (UpperLeftCorner != other.UpperLeftCorner || LowerRightCorner != other.LowerRightCorner);
			}

			/*! Compares size of rectangles
			*/
			bool operator< (const C_Rectangle<T>& other) const
			{
				return GetArea() < other.GetArea();
			}

			/*! Returns size of rectangle
			*/
			T GetArea() const
			{
				return GetWidth() * GetHeight();
			}

			/*! Returns if a 2d point is within this rectangle.
			**	\param pos Position to test if it lies within this rectangle.
			**	\return True if the position is within the rectangle, false if not. 
			*/
			bool IsPointInside (const C_Vector2D<T>& pos) const
			{
				return (UpperLeftCorner.X <= pos.X && UpperLeftCorner.Y <= pos.Y && LowerRightCorner.X >= pos.X && LowerRightCorner.Y >= pos.Y);
			}

			/*! Check if the rectangle collides with another rectangle.
			**	\param other Rectangle to test collision with
			**	\return True if the rectangles collide. 
			*/
			bool IsRectCollided (const C_Rectangle<T>& other) const
			{
				return (LowerRightCorner.Y > other.UpperLeftCorner.Y &&
					UpperLeftCorner.Y < other.LowerRightCorner.Y &&
					LowerRightCorner.X > other.UpperLeftCorner.X &&
					UpperLeftCorner.X < other.LowerRightCorner.X);
			}

			/*! Clips this rectangle with another one.
			**	\param other Rectangle to clip with 
			*/
			void ClipAgainst (const C_Rectangle<T>& other)
			{
				if (other.LowerRightCorner.X < LowerRightCorner.X)
					LowerRightCorner.X = other.LowerRightCorner.X;

				if (other.LowerRightCorner.Y < LowerRightCorner.Y)
					LowerRightCorner.Y = other.LowerRightCorner.Y;

				if (other.UpperLeftCorner.X > UpperLeftCorner.X)
					UpperLeftCorner.X = other.UpperLeftCorner.X;

				if (other.UpperLeftCorner.Y > UpperLeftCorner.Y)
					UpperLeftCorner.Y = other.UpperLeftCorner.Y;

				// correct possible invalid rect resulting from clipping
				if (UpperLeftCorner.Y > LowerRightCorner.Y)
					UpperLeftCorner.Y = LowerRightCorner.Y;

				if (UpperLeftCorner.X > LowerRightCorner.X)
					UpperLeftCorner.X = LowerRightCorner.X;
			}

			/*! Moves this rectangle to fit inside another one.
			**	\return True on success, false if not possible 
			*/
			bool ConstrainTo (const C_Rectangle<T>& other)
			{
				if (other.GetWidth() < GetWidth() || other.GetHeight() < GetHeight())
					return false;

				T diff = other.LowerRightCorner.X - LowerRightCorner.X;

				if (diff < 0)
				{
					LowerRightCorner.X += diff;
					UpperLeftCorner.X  += diff;
				}

				diff = other.LowerRightCorner.Y - LowerRightCorner.Y;

				if (diff < 0)
				{
					LowerRightCorner.Y += diff;
					UpperLeftCorner.Y  += diff;
				}

				diff = UpperLeftCorner.X - other.UpperLeftCorner.X;

				if (diff < 0)
				{
					UpperLeftCorner.X  -= diff;
					LowerRightCorner.X -= diff;
				}

				diff = UpperLeftCorner.Y - other.UpperLeftCorner.Y;

				if (diff < 0)
				{
					UpperLeftCorner.Y  -= diff;
					LowerRightCorner.Y -= diff;
				}

				return true;
			}

			/*! Get width of rectangle.
			*/
			T GetWidth() const
			{
				return LowerRightCorner.X - UpperLeftCorner.X;
			}

			/*! Get height of rectangle.
			*/
			T GetHeight() const
			{
				return LowerRightCorner.Y - UpperLeftCorner.Y;
			}

			/*! If the lower right corner of the rect is smaller then the upper left, the points are swapped.
			*/
			void Repair ()
			{
				if (LowerRightCorner.X < UpperLeftCorner.X)
				{
					T t = LowerRightCorner.X;

					LowerRightCorner.X = UpperLeftCorner.X;
					UpperLeftCorner.X = t;
				}

				if (LowerRightCorner.Y < UpperLeftCorner.Y)
				{
					T t = LowerRightCorner.Y;

					LowerRightCorner.Y = UpperLeftCorner.Y;
					UpperLeftCorner.Y = t;
				}
			}

			/*!	Returns if the rect is valid to draw.
			**	It would be invalid if the UpperLeftCorner is lower or more right than the LowerRightCorner. 
			*/
			bool IsValid () const
			{
				return ((LowerRightCorner.X >= UpperLeftCorner.X) && (LowerRightCorner.Y >= UpperLeftCorner.Y));
			}

			/*!	Get the center of the rectangle
			*/
			C_Vector2D<T> GetCenter () const
			{
				return C_Vector2D<T>((UpperLeftCorner.X + LowerRightCorner.X) / 2, (UpperLeftCorner.Y + LowerRightCorner.Y) / 2);
			}

			/*! Get the dimensions of the rectangle
			*/
			CORE::C_Vector2D<T> GetSize() const
			{
				return CORE::C_Vector2D<T>(GetWidth(), GetHeight());
			}
													   
			/*! Adds a point to the rectangle
			**	Causes the rectangle to grow bigger if point is outside of the box
			**	\param p Point to add to the box. 
			*/
			void AddInternalPoint(const C_Vector2D<T>& p)
			{
				AddInternalPoint (p.X, p.Y);
			}

			/*! Adds a point to the bounding rectangle
			**	Causes the rectangle to grow bigger if point is outside of the box
			**	\param x X-Coordinate of the point to add to this box.
			**	\param y Y-Coordinate of the point to add to this box. 
			*/
			void AddInternalPoint (T x, T y)
			{
				if (x > LowerRightCorner.X)
					LowerRightCorner.X = x;

				if (y > LowerRightCorner.Y)
					LowerRightCorner.Y = y;

				if (x < UpperLeftCorner.X)
					UpperLeftCorner.X = x;

				if (y < UpperLeftCorner.Y)
					UpperLeftCorner.Y = y;
			}

		public:
			//! Upper left corner
			C_Vector2D<T> UpperLeftCorner;

			//! Lower right corner
			C_Vector2D<T> LowerRightCorner;
		};

	}	//	namespace CORE
}	//	namespace REDEEMER

#endif	//	_R_CORE_VECTOR2D_H_
