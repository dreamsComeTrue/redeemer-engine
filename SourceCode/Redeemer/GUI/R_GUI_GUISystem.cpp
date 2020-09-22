/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_GUI_GUISystem.cpp
**		GUI system manages all GUI controls
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_GUI_GUISystem.h"
#include "R_GUI_GUISkinManager.h"
#include "R_GUI_GUIFontManager.h"
#include "..\\Render\\R_Render_RenderDevice.h"
#include "..\\RedeemerEngine.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace REDEEMER
{
	namespace GUI
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_GUISystem::C_GUISystem () :
			C_GUIControl (0, GUICT_System, NULL, CORE::C_Rectangle<int>(CORE::C_Vector2D<int>(0, 0),
				CORE::C_Vector2D<int>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetRenderVideoMode().ScreenResolution.X,
					   				  C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetRenderVideoMode().ScreenResolution.Y))),
			m_SkinManager (NULL),
			m_FontManager (NULL),
			m_FreeTypeLibrary (NULL),
			m_GUIMaterial (NULL),
			m_HoveredControl (NULL),
			m_FocusedControl (NULL),
			m_EventHandler (NULL),
			m_InputEventHandler (NULL)
		{
			SetTabGroup (true);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUISystem::~C_GUISystem ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_GUISystem::Initialize ()
		{
			C_BaseClass::Initialize();

			m_SkinManager = new C_GUISkinManager ();
			m_FontManager = new C_GUIFontManager ();

			m_SkinManager->Initialize();
			m_FontManager->Initialize();

			m_InputEventHandler = new C_GUIInputEventHandler ();

			C_RedeemerEngine::GetSingleton().GetInputManager()->GetKeyboard().AttachKeyboardListener(m_InputEventHandler);
			C_RedeemerEngine::GetSingleton().GetInputManager()->GetMouse().AttachMouseListener(m_InputEventHandler);

			//	Initialize FreeType library
			int error = FT_Init_FreeType ((FT_Library*)&m_FreeTypeLibrary);

			if ( error )
			{
				REDEEMER_LOG << LOG_ERROR << L"An error occurred during initialization of FreeType2 library." << LOG_ENDMESSAGE;

				return RESULT_FAIL;
			}

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_GUISystem::Finalize ()
		{
			m_FontManager->Finalize();
			m_SkinManager->Finalize();

			REDEEMER_SAFE_DELETE (m_InputEventHandler);
			REDEEMER_SAFE_DELETE (m_SkinManager);
			REDEEMER_SAFE_DELETE (m_FontManager);

			//	Release FreeType library
			FT_Done_FreeType ((FT_Library)m_FreeTypeLibrary);

			return C_BaseClass::Finalize();
		}																														  

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUISystem::UpdateControls ()
		{
			//	Check if screen resolution changed and resize environment
			CORE::C_Vector2D<int> screenSize(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetRenderVideoMode().ScreenResolution.X,
				C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetRenderVideoMode().ScreenResolution.Y);

			if (screenSize.Width != m_AbsoluteRectangle.LowerRightCorner.X || screenSize.Height != m_AbsoluteRectangle.LowerRightCorner.Y)
			{
				m_DesiredRectangle.LowerRightCorner = screenSize;
				m_ClippingRectangle = m_DesiredRectangle;
				m_AbsoluteRectangle = m_DesiredRectangle;

				UpdateAbsolutePosition(true);
			}							 
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUISkinManager* C_GUISystem::GetSkinManager ()
		{
			return m_SkinManager;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUIFontManager* C_GUISystem::GetFontManager ()
		{
			return m_FontManager;
		}

		//------------------------------------------------------------------------------------------------------------------------

		FT_LibraryRec_* C_GUISystem::GetFreeTypeLibrary ()
		{
			return m_FreeTypeLibrary;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUISystem::SetGUIMaterial (RENDER::C_Material* material)
		{
			m_GUIMaterial = material;
		}

		//------------------------------------------------------------------------------------------------------------------------

		RENDER::C_Material* C_GUISystem::GetGUIMaterial ()
		{
			return m_GUIMaterial;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUISystem::SetFocus (C_GUIControl* control)
		{
			if (m_FocusedControl == control)
				return false;

			//  GUI System should not get the focus
			if (control == this)
				control = 0;

			if (m_FocusedControl)
			{
				S_GUIEvent event;

				event.Caller = m_FocusedControl;
				event.Control = control;
				event.EventType = GUIET_ControlLostFocus;

				if (m_FocusedControl->OnEvent (event))
					return false;
			}

			if (control)
			{
				// send focused event
				S_GUIEvent event;

				event.Caller = control;
				event.Control = m_FocusedControl;
				event.EventType = GUIET_ControlGainedFocus;

				if (control->OnEvent (event))
					return false;
			}

			m_FocusedControl = control;

			return true;		
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUISystem::RemoveFocus (C_GUIControl* control)
		{
			if (m_FocusedControl && m_FocusedControl == control)
			{
				S_GUIEvent event;

				event.Caller = m_FocusedControl;
				event.Control = 0;
				event.EventType = GUIET_ControlLostFocus;

				if (m_FocusedControl->OnEvent(event))
					return false;
			}

			if (m_FocusedControl)
			{
				m_FocusedControl = NULL;
			}

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUISystem::HasFocus (C_GUIControl* control) const
		{
			return (m_FocusedControl == control);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUIControl* C_GUISystem::GetFocus () const
		{
			return m_FocusedControl;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUISystem::ClearControls ()
		{
			// get the root's children in case the root changes in future
			std::list<C_GUIControl*>& children = const_cast<std::list<C_GUIControl*>& >(GetRootControl()->GetChildrenControls ());

			while (!children.empty())
				children.pop_back();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUIControl* C_GUISystem::GetRootControl ()
		{
			return this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUISystem::SetEventHandler(C_GUIEventHandler* handler)
		{
			m_EventHandler = handler;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUIEventHandler* C_GUISystem::GetEventHandler()
		{
			return m_EventHandler;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUISystem::OnEvent (const S_GUIEvent& event)
		{
			bool ret = false;
			
			if (m_EventHandler)
				ret = m_EventHandler->OnEvent(event);

			return ret;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUISystem::UpdateMouseOverControl (CORE::C_Vector2D<int> mousePosition)
		{
			C_GUIControl* lastHovered = m_HoveredControl;
			m_LastHoveredMousePosition = mousePosition;

			m_HoveredControl = GetControlAtPosition(mousePosition);

			if (m_HoveredControl && m_HoveredControl != lastHovered)
			{
				//	Send appropriate events
				S_GUIEvent event;

				if (lastHovered)
				{
					event.Caller = lastHovered;
					event.EventType = GUIET_ControlLostHovered;

					lastHovered->OnEvent(event);
				}

				event.Caller = m_HoveredControl;
				event.Control = m_HoveredControl;
				event.EventType = GUIET_ControlHovered;

				m_HoveredControl->OnEvent(event);
			}	
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUISystem::PostEventFromUser(const S_GUIEvent& event)
		{
			switch (event.Source)
			{
			case GUIES_GUI:
				//	We don't want to resend GUI events
				break;

			case GUIES_Keyboard:
				if (event.KeyboardPressed && event.KeyboardData->Key == INPUT::KEY_TAB)
				{
					bool shift = C_RedeemerEngine::GetSingleton().GetInputManager()->GetKeyboard().IsModifierDown(INPUT::C_KeyboardDevice::KM_Shift);
					bool control = C_RedeemerEngine::GetSingleton().GetInputManager()->GetKeyboard().IsModifierDown(INPUT::C_KeyboardDevice::KM_Ctrl);

					C_GUIControl *next = GetNextControl (shift, control);

					if (next && next != m_FocusedControl)
					{
						if (SetFocus (next))
							return true;
					}
				}

				if (m_FocusedControl)
				{
					return m_FocusedControl->OnEvent(event);
				}

				break;

			case GUIES_Mouse:
				CORE::C_Vector2D<int> position = CORE::C_Vector2D<int>(event.MouseData->State.X.Abs, event.MouseData->State.Y.Abs);

				UpdateMouseOverControl (position);

				if (event.MouseData->State.IsButtonDown(INPUT::MB_Left))
				{
					if ((m_HoveredControl && m_HoveredControl != m_FocusedControl) || !m_FocusedControl)
						SetFocus(m_HoveredControl);
				}

				//	Send also this event to focused control
				if (m_FocusedControl && m_FocusedControl->OnEvent(event))
					return true;

				if (m_HoveredControl && !m_FocusedControl)
				{
					return m_HoveredControl->OnEvent(event);
				}

				break;
			}

			return false;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUIControl* C_GUISystem::GetNextControl (bool reverse, bool group)
		{
			// start the search at the root of the current tab group
			C_GUIControl *startPos = m_FocusedControl ? m_FocusedControl->GetTabGroup() : 0;

			int startOrder = -1;

			// if we're searching for a group
			if (group && startPos)
			{
				startOrder = startPos->GetTabOrder();
			}
			else
				if (!group && m_FocusedControl && !m_FocusedControl->IsTabGroup())
				{
					startOrder = m_FocusedControl->GetTabOrder();

					if (startOrder == -1)
					{
						// this element is not part of the tab cycle, but its parent might be...
						C_GUIControl *el = m_FocusedControl;

						while (el && el->GetParent() && startOrder == -1)
						{
							el = el->GetParent();
							startOrder = el->GetTabOrder();
						}

					}
				}

				if (group || !startPos)
					startPos = this; // start at the root

				// find the element
				C_GUIControl *closest = 0;
				C_GUIControl *first = 0;

				startPos->GetNextElement (startOrder, reverse, group, first, closest);

				if (closest)
					return closest; // we found an element
				else if (first)
					return first; // go to the end or the start
				else if (group)
					return this; // no group found? root group
				else
					return 0;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace GUI
}	//	namespace REDEEMER
