/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_PostProcessStage.h
**		This stage can add various effects on output rendered image
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_RENDER_POSTPROCESSSTAGE_H_
#define _R_RENDER_POSTPROCESSSTAGE_H_

#include "R_Render_RenderStage.h"
#include "R_Render_RenderQueue.h"

#include <map>

namespace REDEEMER
{
	namespace RENDER
	{
		class C_Material;
		class C_Sprite;

		/*!
		**	\brief This stage can add various effects on output rendered image
		*/
		class C_PostProcessStage : public C_RenderStage
		{
		public:
			/*!	Constructor
			*/
			C_PostProcessStage ();

			/*!	Destructor
			*/
			virtual ~C_PostProcessStage ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize (int priority);

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Adds new render material to queue. It is important to set it's priority, so materials with lower priorities will be rendered first
			*/
			void AddToRenderQueue (unsigned int priority, C_Material* material);

		private:
			/*!	Updates rendering stage
			*/
			virtual void Update ();

		private:
			std::map<unsigned int, C_Material*>		m_Materials;		///<	Contains all materials with postprocess effects
			C_Sprite*								m_ScreenSprite;		///<	Screen surface
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_POSTPROCESSSTAGE_H_
