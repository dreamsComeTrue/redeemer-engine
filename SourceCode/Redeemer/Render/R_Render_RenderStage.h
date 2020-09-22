/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_RenderStage.h
**		Stages are independent units in rendering pipeline. This class is a base class for all stages, such as Geometry, Lights, etc.
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_RENDER_RENDERSTAGE_H_
#define _R_RENDER_RENDERSTAGE_H_

#include "..\\Core\\R_Core_BaseClass.h"

namespace REDEEMER
{
	namespace RENDER
	{
		class C_RenderPipeline;


		/*!
		**	\brief Stages are independent units in rendering pipeline. This class is a base class for all stages, such as Geometry, Lights, etc.
		*/
		class C_RenderStage : public CORE::C_BaseClass
		{
			friend class C_RenderPipeline;
			friend bool operator< (const C_RenderStage& left, const C_RenderStage& right);

		public:
			/*!	Constructor
			*/
			C_RenderStage ();

			/*!	Destructor
			*/
			virtual ~C_RenderStage ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize (int priority);

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Sets stage enabled
			*/
			void SetEnabled (bool enabled = true);

			/*!	Checks, if stage is enabled
			*/
			bool IsEnabled () const;

		protected:
			/*!	Updates rendering stage
			*/
			virtual void Update () = 0;

		private:
			unsigned int	m_Priority;		///<	Priority of the stage
			bool			m_IsEnabled;	///<	Is stage turned "on"?
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_RENDERSTAGE_H_
