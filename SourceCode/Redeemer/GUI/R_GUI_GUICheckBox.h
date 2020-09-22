/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_GUI_GUICheckBox.h
**		Checkbox is like a button, but it can be used like a toggle switch 
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_GUI_GUICHECKBOX_H_
#define _R_GUI_GUICHECKBOX_H_

#include "R_GUI_GUIControl.h"
	
namespace REDEEMER
{
	namespace GUI
	{
		class C_GUIImage;
		class C_GUIPanel;


		/*!
		**	\brief CheckBox is like a button, but it can be used like a toggle switch 
		*/
		class C_GUICheckBox : public C_GUIControl
		{
		public:
			/*!	Constructor
			*/
			C_GUICheckBox (unsigned int ID, C_GUIControl* parent, CORE::C_Rectangle<int> dimensions, bool checked = false);

			/*!	Destructor
			*/
			virtual ~C_GUICheckBox ();	  

			/*!	Draws control on the screen
			*/
			virtual void Render ();

			/*!	Sets control's caption text
			*/
			virtual void SetText (const wchar_t* caption);

			/*!	Sets checkbox value (switch on or off)
			*/
			void SetChecked (bool checked);

			/*!	Retrieves if checkbox is checked
			*/
			bool IsChecked () const;

			/*!	Retrieves internal panel
			*/
			C_GUIPanel* GetInternalPanelControl ();

		private:
			void UpdateTransformations ();

		private:
			bool			m_IsPressed;			///<	Describes, in which state (Pressed or Released) is checkbox currently
			bool			m_IsChecked;			///<	Defines, if checkbox is switched on or off
			C_GUIImage*		m_GlyphImage;			///<	Image that is rendered left to the checkbox's text
			C_GUIPanel*		m_BackgroundPanel;		///<	Visible surface of the checkbox
		};

	}	//	namespace GUI
}	//	namespace REDEEMER

#endif	//	_R_GUI_GUICHECKBOX_H_
