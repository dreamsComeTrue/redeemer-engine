/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_GUI_GUIManager.h
**		Graphical User Interface manager
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_GUI_GUIMANAGER_H_
#define _R_GUI_GUIMANAGER_H_

#include "..\\Core\\R_Core_Singleton.h"
#include "..\\Core\\R_Core_BaseClass.h"

namespace REDEEMER
{
	namespace GUI
	{
		class C_GUISystem;

		/*!
		**	\brief Graphical User Interface manager
		*/
		class C_GUIManager : public CORE::C_Singleton<C_GUIManager>, public CORE::C_BaseClass
		{
		public:
			/*!	Constructor
			*/
			C_GUIManager ();

			/*!	Destructor
			*/
			virtual ~C_GUIManager ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize ();

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Returns reference to GUI system object
			*/
			C_GUISystem* GetGUISystem ();

		private:			
			C_GUISystem*		m_GUISystem;		///<	GUI system object
		};

	}	//	namespace GUI
}	//	namespace REDEEMER

#endif	//	_R_GUI_GUIMANAGER_H_
