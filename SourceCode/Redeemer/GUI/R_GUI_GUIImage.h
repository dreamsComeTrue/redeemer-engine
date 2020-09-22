/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_GUI_GUImage.h
**		GUI Control which uses C_Sprite to render an image
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_GUI_GUIIMAGE_H_
#define _R_GUI_GUIIMAGE_H_

#include "R_GUI_GUIControl.h"	  
#include "..\\Render\\R_Render_Color.h"
	
namespace REDEEMER
{
	namespace RENDER
	{
		class C_Texture;
		class C_Sprite;
		class C_Material;
	}

	namespace GUI
	{
		/*!
		**	\brief GUI Control which uses C_Sprite to render an image
		*/
		class C_GUIImage : public C_GUIControl
		{
		public:
			/*!	Constructor
			*/
			C_GUIImage (unsigned int ID, C_GUIControl* parent, CORE::C_Rectangle<int> dimensions);

			/*!	Destructor
			*/
			virtual ~C_GUIImage ();

			/*! Sets a texture used for drawing
			*/
			void SetImage (RENDER::C_Texture* texture);

			/*!	Sets if the image should scale to fit the element
			*/
			void SetScaleImage (bool scale = true);

			/*! Returns true if the image is scaled to fit, false if not
			*/
			bool IsImageScaled () const;

			/*!	Draws control on the screen
			*/
			virtual void Render ();

			/*!	Sets, which portion of texture rectangle should be used for rendering
			**	Values should be given as coordinates in screen-space
			*/
			void SetTextureRectangle (CORE::C_Rectangle<int> rectangle);

			/*!	Sets vertices colors
			*/
			void SetVerticesColors (RENDER::C_Color upperLeft, RENDER::C_Color upperRight, RENDER::C_Color lowerLeft, RENDER::C_Color lowerRight);

			/*!	Returns vertices colors as an array of 4 C_Color's
			*/
			RENDER::C_Color* GetVerticesColors ();

		private:
			RENDER::C_Sprite*	m_Sprite;			///<	Surface used for drawing
			RENDER::C_Texture*	m_OldTexture;
			RENDER::C_Texture*	m_Texture;
			RENDER::C_Material*	m_GUIMaterial;
			bool				m_ScaleImage;
		};

	}	//	namespace GUI
}	//	namespace REDEEMER

#endif	//	_R_GUI_GUIIMAGE_H_
