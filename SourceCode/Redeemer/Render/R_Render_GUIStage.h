/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_GUIStage.h
**		Stage for processing GUI controls
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_RENDER_GUISTAGE_H_
#define _R_RENDER_GUISTAGE_H_

#include "R_Render_RenderStage.h"

namespace REDEEMER
{
	namespace RENDER
	{			
		/*!
		**	\brief Stage for processing GUI controls
		*/
		class C_GUIStage : public C_RenderStage
		{
		public:
			/*!	Constructor
			*/
			C_GUIStage ();

			/*!	Destructor
			*/
			virtual ~C_GUIStage ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize (int priority);

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

		private:
			/*!	Updates rendering stage
			*/
			virtual void Update ();
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_GUISTAGE_H_
