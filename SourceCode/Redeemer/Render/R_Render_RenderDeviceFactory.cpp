/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_RenderDeviceFactory.cpp
**		Factory is used as a abstraction layer for providing render resources, like vertex buffers, formats etc.
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_RenderDeviceFactory.h"

namespace REDEEMER
{
	namespace RENDER
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_RenderDeviceFactory::C_RenderDeviceFactory ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_RenderDeviceFactory::~C_RenderDeviceFactory ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_RenderDeviceFactory::Initialize ()
		{
			C_BaseClass::Initialize();

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_RenderDeviceFactory::Finalize ()
		{
			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER

