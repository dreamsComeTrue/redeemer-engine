/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_RenderManager.h
**		Engine's render system communication layer
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_RENDER_RENDERMANAGER_H_
#define _R_RENDER_RENDERMANAGER_H_

#include "..\\Core\\R_Core_Singleton.h"
#include "..\\Core\\R_Core_BaseClass.h"

namespace REDEEMER
{
	namespace RENDER
	{
		class C_RenderDevice;

		/*!
		**	\brief Engine's render system communication layer
		*/
		class C_RenderManager : public CORE::C_Singleton<C_RenderManager>, public CORE::C_BaseClass
		{
		public:
			/*!	Constructor
			*/
			C_RenderManager ();

			/*!	Destructor
			*/
			virtual ~C_RenderManager ();
			
			/*!	\brief	Initialization code
			*/
			virtual int Initialize ();

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Returns render device
			*/
			C_RenderDevice*	GetRenderDevice ();

		private:
			C_RenderDevice*		m_RenderDevice;		///<	Render device object
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_RENDERMANAGER_H_
