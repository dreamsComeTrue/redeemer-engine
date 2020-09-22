/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Framework_Application.h
**		Main application object.
*/

//------------------------------------------------------------------------------------------------------------------------
#include "RedeemerEngine.h"

namespace REDEEMER
{
	namespace FRAMEWROK
	{
		/*!
		**	\brief A C_Application class controls all message's processing for an application
		*/
		class C_Application : public CORE::C_BaseClass
		{
		public:
			///	Constructor
			C_Application ();

			///	Destructor
			virtual ~C_Application ();

			///	Initialization code
			int Initialize ();

			///	Finalization code
			int Finalize ();

		protected:
			CORE::C_SmartPointer<C_RedeemerEngine>	m_Engine;	///<	Engine object
		};
	}	//	namespace FRAMEWROK
}	//	namespace REDEEMER