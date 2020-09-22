/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_GUI_GUIButton.h
**		Button is a widget that can respond to the user events
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_GUI_GUIBUTTON_H_
#define _R_GUI_GUIBUTTON_H_

#include "R_GUI_GUIPanel.h"
	
namespace REDEEMER
{
	namespace RENDER
	{
		class C_Texture;
	}

	namespace GUI
	{
		/*!	Glyph alignment around the button's text
		*/
		enum EGlyphAlignment
		{
			GA_Left,
			GA_Right,
			GA_Top,
			GA_Bottom
		};

		/*!
		**	\brief Button is a widget that can respond to the user events
		*/
		class C_GUIButton : public C_GUIControl
		{
		public:
			/*!	Constructor
			*/
			C_GUIButton (unsigned int ID, C_GUIControl* parent, CORE::C_Rectangle<int> dimensions);

			/*!	Destructor
			*/
			virtual ~C_GUIButton ();	  

			/*!	Draws control on the screen
			*/
			virtual void Render ();

			/*!	Sets control's caption text
			*/
			virtual void SetText (const wchar_t* caption);

			/*!	Sets button to pushable state - if true, it can be either in two states (Pressed or Released), described by m_IsPressed
			*/
			void SetPushable (bool pushable = true);

			/*!	Checks, if button is in pushable state
			*/
			bool IsPushable () const;

			/*!	Sets button as pressed
			*/
			void SetPressed ();

			/*!	Sets button as released 
			*/
			void SetReleased ();

			/*!	Changes pressed button state - from Pressed to Release and vice versa
			*/
			void ChangePressedState ();

			/*!	Checks, if button is Pressed
			*/
			bool IsPressed () const;

			/*! Sets a texture used for drawing glyph icon
			*/
			void SetGlyphImage (RENDER::C_Texture* texture);

			/*!	Sets if glyph image should be rendered
			*/
			void SetGlyphImageRendered (bool enable);

			/*!	Checks, if glyph image is rendered
			*/
			bool IsGlyphImageRendered () const;

			/*!	Sets glyph alignment around the text
			*/
			void SetGlyphAlignment (EGlyphAlignment alignment);

			/*!	Receives glyph alignment around the text
			*/
			EGlyphAlignment GetGlyphAlignment () const;

			/*!	Sets spacing between glyph and button's text	
			*/
			void SetGlyphSpacing (int spacing);

			/*!	Retrieves spacing between glyph and button's text
			*/
			int GetGlyphSpacing () const;

			/*!	Sets glyph scaling factor from [0..100] as percent of button's width/height
			*/
			void SetGlyphScalingFactor (int scaling);

			/*!	Retrieves glyph scaling factor from [0..100] as percent of button's width/height
			*/
			int GetGlyphScalingFactor () const;

			/*!	Retrieves internal panel
			*/
			C_GUIPanel* GetInternalPanelControl ();

		protected:
			void UpdateTransformations ();

			bool			m_IsPushable;			///<	If true, button can be in two states, described by m_IsPressed state (Pressed or Released)
			bool			m_IsPressed;			///<	Describes, in which state (Pressed or Released) is button currently
			C_GUIImage*		m_GlyphImage;			///<	Image that is rendered near/instead the button's text
			C_GUIPanel*		m_BackgroundPanel;		///<	Visible surface of the button
			bool			m_RenderGlyphImage;		///<	Draw glyph image?
			EGlyphAlignment	m_GlyphAlignment;		///<	Glyph alignment around the text
			int				m_GlyphSpacing;			///<	Spacing between glyph and button's text
			int				m_GlyphScaling;			///<	Glyph scaling factor
		};

	}	//	namespace GUI
}	//	namespace REDEEMER

#endif	//	_R_GUI_GUIBUTTON_H_
