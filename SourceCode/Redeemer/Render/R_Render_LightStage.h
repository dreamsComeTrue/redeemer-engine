/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_LightStage.h
**		Light stage uses data collected from geometry stage to lit the surfaces
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_RENDER_LIGHTSTAGE_H_
#define _R_RENDER_LIGHTSTAGE_H_

#include "R_Render_RenderStage.h"

namespace REDEEMER
{
	namespace RENDER
	{
		class C_Sprite;

		/*!
		**	\brief Light stage uses data collected from geometry stage to lit the surfaces
		*/
		class C_LightStage : public C_RenderStage
		{
		public:
			/*!	Constructor
			*/
			C_LightStage ();

			/*!	Destructor
			*/
			virtual ~C_LightStage ();

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

		private:
			C_Sprite*								m_ScreenSprite;		///<	Screen surface
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_LIGHTSTAGE_H_
