/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_Sprite.h
**		Sprites are used to draw 2D surfaces on the screen, mostly used in GUI
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_RENDER_SPRITE_H_
#define _R_RENDER_SPRITE_H_

#include "R_Render_Color.h"
#include "..\\Core\\Math\\R_Core_Rectangle.h"
#include "..\\Core\\Math\\R_Core_Vector2D.h"

namespace REDEEMER
{
	namespace RENDER
	{
		class C_Material;
		class C_VertexBuffer;


		/*!	\brief	Sprites are used to draw 2D surfaces on the screen, mostly used in GUI
		*/
		class C_Sprite
		{
		public:
			/*!	Constructor
			*/
			C_Sprite (CORE::C_Rectangle<float> dimension);

			/*!	Destructor
			*/
			virtual ~C_Sprite ();

			/*!	Sets position on the screen
			*/
			void SetPosition (CORE::C_Vector2D<float> position);

			/*!	Returns sprite position on the screen
			*/
			CORE::C_Vector2D<float> GetPosition () const;

			/*!	Sets sprite width
			*/
			void SetWidth (float width);

			/*!	Gets sprite width
			*/
			float GetWidth () const;

			/*!	Sets sprite height
			*/
			void SetHeight (float height);

			/*!	Gets sprite height
			*/
			float GetHeight () const;

			/*!	Sets vertices colors
			*/
			void SetVerticesColors (C_Color upperLeft, C_Color upperRight, C_Color lowerLeft, C_Color lowerRight);

			/*!	Returns vertices colors as an array of 4 C_Color's
			*/
			C_Color* GetVerticesColors ();

			/*!	Sets, which portion of texture rectangle should be used for rendering
			**	Values should be given as coordinates in screen-space
			*/
			void SetTextureRectangle (CORE::C_Rectangle<int> rectangle);

			/*!	Assigns material
			*/
			void SetMaterial (C_Material* material);

			/*!	Returns sprite's material
			*/
			C_Material* GetMaterial ();

			/*!	Render actual quad, remember to set 2D mode on renderer first, and the restore to 3D
			*/
			void Render ();

		private:
			void SetVertexData ();

		private:
			CORE::C_Rectangle<float>	m_Dimension;
			C_VertexBuffer*				m_VertexBuffer;	///<	Underlying vertex buffer
			C_Material*					m_Material;		///<	Assigned material
			C_Color						m_Colors[4];	///<	Vertices colors
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_SPRITE_H_

