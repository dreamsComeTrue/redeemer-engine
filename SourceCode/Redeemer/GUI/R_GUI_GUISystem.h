/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_GUI_GUISystem.h
**		GUI system manages all GUI controls
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_GUI_GUISYSTEM_H_
#define _R_GUI_GUISYSTEM_H_

#include "..\\Core\\R_Core_BaseClass.h"
#include "R_GUI_GUIControl.h"

namespace REDEEMER
{
	namespace RENDER
	{
		class C_Material;
	}

	namespace GUI
	{
		struct FT_LibraryRec_;

		class C_GUISkinManager;
		class C_GUIFontManager;
		class C_GUIEventHandler;
		class C_GUIFont;

		/*!
		**	\brief GUI system manages all GUI controls
		*/
		class C_GUISystem : public CORE::C_BaseClass, public C_GUIControl
		{
			friend class C_GUIFont;

		public:
			/*!	Constructor
			*/
			C_GUISystem ();

			/*!	Destructor
			*/
			virtual ~C_GUISystem ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize ();

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Updates all controls on the screen
			*/
			void UpdateControls ();

			/*!	Gets access to skin manager
			*/
			C_GUISkinManager* GetSkinManager ();

			/*!	Gets access to font manager
			*/
			C_GUIFontManager* GetFontManager ();

			/*!	Sets material used by GUI system
			*/
			void SetGUIMaterial (RENDER::C_Material* material);

			/*!	Gets access material used by GUI system
			*/
			RENDER::C_Material* GetGUIMaterial ();

			/*! Sets the focus to a control.
			**	GUIET_ControlLostFocus event will be fired followed by a GUIET_ControlGainedFocus event.
			*/
			bool SetFocus (C_GUIControl* control);

			/*! Removes the focus from a control.
			**	Will fire the GUIET_ControlLostFocus event
			*/
			bool RemoveFocus (C_GUIControl* control);

			/*!	Returns if the control has focus
			*/
			bool HasFocus (C_GUIControl* control) const;

			/*! Returns the control with the focus
			*/
			C_GUIControl* GetFocus () const;

			/*! Removes all children controls 
			*/
			void ClearControls ();

			/*!	Returns first control added to the system - it is the most parent of all controls
			*/
			C_GUIControl* GetRootControl ();

			/*!	Assigns GUI event handler as a descendant of C_GUIEventHandler class
			*/
			void SetEventHandler (C_GUIEventHandler* handler);

			/*!	Returns GUI event handler assigned by an user
			*/
			C_GUIEventHandler* GetEventHandler ();

			/*! Method called when the event happened
			*/
			virtual bool OnEvent (const S_GUIEvent& event);

			/*! Posts an input event to the environment
			*/
			virtual bool PostEventFromUser(const S_GUIEvent& event);

		private:
			/*!	Sets control under mouse control as hovered
			*/
			void UpdateMouseOverControl (CORE::C_Vector2D<int> mousePosition);

			/*! Returns the next control in the tab group starting at the focused element
			*/
			C_GUIControl* GetNextControl (bool reverse = false, bool group = false);

			/*!	Used internally by C_GUIFont
			*/
			FT_LibraryRec_* GetFreeTypeLibrary ();

		private:
			C_GUISkinManager*		m_SkinManager;				///<	Container for all GUI skins			
			C_GUIFontManager*		m_FontManager;				///<	Container for all fonts
			C_GUIControl*			m_HoveredControl;			///<	On which control is currently mouse cursor?
			CORE::C_Vector2D<int>	m_LastHoveredMousePosition;	///<	Position of the mouse when last hovered control was changed
			C_GUIControl*			m_FocusedControl;			///<	Which control has focus?
			C_GUIEventHandler*		m_EventHandler;				///<	GUI event handler assigned by an user
			C_GUIInputEventHandler*	m_InputEventHandler;		///<	Used for passing data from keyboard and mice to GUI controls
			RENDER::C_Material*		m_GUIMaterial;
			FT_LibraryRec_*			m_FreeTypeLibrary;
		};

	}	//	namespace GUI
}	//	namespace REDEEMER

#endif	//	_R_GUI_GUISYSTEM_H_
