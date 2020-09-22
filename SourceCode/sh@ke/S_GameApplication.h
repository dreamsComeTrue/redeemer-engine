/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	S_GameApplication.h
**		Main application object.
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Framework_Application.h"
#include "S_GameWindow.h"

namespace REDEEMER
{
	namespace SHAKE
	{
		/*!
		**	\brief A S_GameApplication class controls all message's processing for a game
		*/
		class S_GameApplication : public FRAMEWROK::C_Application
		{
		public:
			///	Constructor
			S_GameApplication ();

			///	Destructor
			virtual ~S_GameApplication ();

			///	Initialization code
			virtual int Initialize ();

			///	Finalization code
			virtual int Finalize ();

		private:
			CORE::C_SmartPointer<S_GameWindow>			m_GameWindow;			///<	Game window
		};
	}	//	namespace SHAKE
}	//	namespace REDEEMER