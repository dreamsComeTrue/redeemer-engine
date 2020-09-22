/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_GUI_GUIPanel.h
**		Panel is a simple area for placing other controls
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_GUI_GUIPANEL_H_
#define _R_GUI_GUIPANEL_H_

#include "R_GUI_GUIControl.h"
#include "..\\Render\\R_Render_Color.h"
	
namespace REDEEMER
{
	namespace GUI
	{
		class C_GUIStaticText;
		class C_GUIFont;
		class C_GUIImage;

		/*!
		**	\brief Panel is a simple area for placing other controls
		*/
		class C_GUIPanel : public C_GUIControl
		{
		public:
			/*!	Constructor
			*/
			C_GUIPanel (unsigned int ID, C_GUIControl* parent, CORE::C_Rectangle<int> dimensions);

			/*!	Destructor
			*/
			virtual ~C_GUIPanel ();	  

			/*!	Draws control on the screen
			*/
			virtual void Render ();

			/*!	Sets control's caption text
			*/
			virtual void SetText (const wchar_t* caption);

			/*!	Sets font used for rendering text
			*/
			void SetCaptionFont (C_GUIFont* font);

			/*!	Gets font used for rendering purposes
			*/
			C_GUIFont* GetCaptionFont ();

			/*!	Sets panel color. When skin is attached it is used instead of color
			*/
			void SetColor (RENDER::C_Color color);

			/*!	Gets panel color
			*/
			RENDER::C_Color GetColor ();

			/*!	Sets font color
			*/
			void SetCaptionColor (RENDER::C_Color color);

			/*!	Gets font color
			*/
			RENDER::C_Color GetCaptionColor ();

			/*!	Sets text vertical alignment for text inside control's borders
			*/
			void SetCaptionVerticalAlignment (EGUIControlAlignment alignment);

			/*! Gets text vertical alignment
			*/
			EGUIControlAlignment GetCaptionVerticalAlignment () const;

			/*!	Sets text horizontal alignment inside control's borders
			*/
			void SetCaptionHorizontalAlignment (EGUIControlAlignment alignment);

			/*! Gets text horizontal alignment
			*/
			EGUIControlAlignment GetCaptionHorizontalAlignment () const;

			/*!	Gets internal static text control
			*/
			C_GUIStaticText* GetInternalTextControl ();

			/*!	Gets internal image control
			*/
			C_GUIImage* GetInternalImageControl ();

		protected:
			C_GUIStaticText*		m_BackgroundText;		///<	Text as appear inside panel
			C_GUIImage*				m_BackgroundImage;		///<	Surface used for drawing
			RENDER::C_Color			m_Color;
		};

	}	//	namespace GUI
}	//	namespace REDEEMER

#endif	//	_R_GUI_GUIPANEL_H_
