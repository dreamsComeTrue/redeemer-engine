/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_GUI_GUIStaticText.h
**		GUI Control for rendering text
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_GUI_GUISTATICTEXT_H_
#define _R_GUI_GUISTATICTEXT_H_

#include "R_GUI_GUIControl.h"
#include "..\\Render\\R_Render_Color.h"
	
namespace REDEEMER
{
	namespace GUI
	{
		class C_GUIFont;
		class C_GUIImage;

		/*!
		**	\brief GUI Control for rendering text
		*/
		class C_GUIStaticText : public C_GUIControl
		{
		public:
			/*!	Constructor
			*/
			C_GUIStaticText (unsigned int ID, C_GUIControl* parent, CORE::C_Rectangle<int> dimensions);

			/*!	Destructor
			*/
			virtual ~C_GUIStaticText ();	  

			/*!	Draws control on the screen
			*/
			virtual void Render ();

			/*!	Sets control's caption text
			*/
			virtual void SetText (const wchar_t* caption);

			/*!	Sets font used for rendering text
			*/
			void SetFont (C_GUIFont* font);

			/*!	Gets font used for rendering purposes
			*/
			C_GUIFont* GetFont ();

			/*!	Sets font color
			*/
			void SetFontColor (RENDER::C_Color color);

			/*!	Gets font color
			*/
			RENDER::C_Color GetFontColor ();

			/*!	Sets word wrapping enabled
			*/
			void SetWordWrapEnabled (bool enable);

			/*!	Checks, if word wrapping is enabled
			*/
			bool IsWordWrapEnabled () const;

			/*!	Sets text vertical alignment inside control's borders
			*/
			void SetVerticalAlignment (EGUIControlAlignment alignment);

			/*! Gets text vertical alignment
			*/
			EGUIControlAlignment GetVerticalAlignment () const;

			/*!	Sets text horizontal alignment inside control's borders
			*/
			void SetHorizontalAlignment (EGUIControlAlignment alignment);

			/*! Gets text horizontal alignment
			*/
			EGUIControlAlignment GetHorizontalAlignment () const;

			/*!	Retrieves text width (in screen space, as non word-wrappable)
			*/
			int GetTextWidth () const;

			/*!	Worker method for updating text position)
			*/
			void RecalculatePositions ();

		private:
			void BreakText (int xPos, int YPos);

		private:
			C_GUIFont*				m_Font;					///<	Font used for rendering text
			C_GUIImage*				m_BackgroundImage;		///<	Surface used for drawing
			RENDER::C_Color			m_FontColor;			///<	Text color
			bool					m_WordWrap;				///<	Is word wrap enabled
			EGUIControlAlignment	m_VeritcalAlignment;	///<	Text vertical alignment
			EGUIControlAlignment	m_HorizontalAlignment;	///<	Text horizontal alignment
			int						m_TextWidth;			///<	Text width, in screen space
		};

	}	//	namespace GUI
}	//	namespace REDEEMER

#endif	//	_R_GUI_GUISTATICTEXT_H_
