/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_GUI_GUIEventHandler.cpp
**		Interface for handling GUI events
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_GUI_GUIEventHandler.h"
#include "R_GUI_GUISystem.h"
#include "..\\Core\\Math\\R_Core_Vector2D.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace GUI
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_GUIInputEventHandler::C_GUIInputEventHandler ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUIInputEventHandler::~C_GUIInputEventHandler ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUIInputEventHandler::KeyPressed (const INPUT::C_KeyboardEvent &event)
		{
			S_GUIEvent e;

			e.Source = GUIES_Keyboard;
			e.KeyboardData = &const_cast<INPUT::C_KeyboardEvent &>(event);
			e.KeyboardPressed = true;

			C_RedeemerEngine::GetSingleton().GetGUIManager()->GetGUISystem()->PostEventFromUser(e);

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUIInputEventHandler::KeyReleased (const INPUT::C_KeyboardEvent &event)
		{
			S_GUIEvent e;

			e.Source = GUIES_Keyboard;
			e.KeyboardData = &const_cast<INPUT::C_KeyboardEvent &>(event);
			e.KeyboardPressed = false;

			C_RedeemerEngine::GetSingleton().GetGUIManager()->GetGUISystem()->PostEventFromUser(e);

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUIInputEventHandler::MouseMoved (const INPUT::C_MouseEvent &event)
		{
			S_GUIEvent e;

			e.Source = GUIES_Mouse;
			e.MouseData = &const_cast<INPUT::C_MouseEvent &>(event);

			C_RedeemerEngine::GetSingleton().GetGUIManager()->GetGUISystem()->PostEventFromUser(e);

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUIInputEventHandler::MousePressed (const INPUT::C_MouseEvent &event, INPUT::EMouseButtonID buttonID)
		{
			S_GUIEvent e;

			e.Source = GUIES_Mouse;
			e.MouseData = &const_cast<INPUT::C_MouseEvent &>(event);

			C_RedeemerEngine::GetSingleton().GetGUIManager()->GetGUISystem()->PostEventFromUser(e);

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUIInputEventHandler::MouseReleased (const INPUT::C_MouseEvent &event, INPUT::EMouseButtonID buttonID)
		{
			S_GUIEvent e;

			e.Source = GUIES_Mouse;
			e.MouseData = &const_cast<INPUT::C_MouseEvent &>(event);

			C_RedeemerEngine::GetSingleton().GetGUIManager()->GetGUISystem()->PostEventFromUser(e);

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUIInputEventHandler::ButtonPressed (const INPUT::C_JoyStickEvent &event, int button)
		{
			//	JoySticks are not used in GUI...yet :)
			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUIInputEventHandler::ButtonReleased (const INPUT::C_JoyStickEvent &event, int button)
		{
			//	JoySticks are not used in GUI...yet :)
			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUIInputEventHandler::AxisMoved (const INPUT::C_JoyStickEvent &event, int axis)
		{
			//	JoySticks are not used in GUI...yet :)
			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUIInputEventHandler::SliderMoved (const INPUT::C_JoyStickEvent &event, int index)
		{
			//	JoySticks are not used in GUI...yet :)
			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUIInputEventHandler::PovMoved (const INPUT::C_JoyStickEvent &event, int index)
		{
			//	JoySticks are not used in GUI...yet :)
			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUIInputEventHandler::Vector3Moved (const INPUT::C_JoyStickEvent &event, int index)
		{
			//	JoySticks are not used in GUI...yet :)
			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace GUI
}	//	namespace REDEEMER
