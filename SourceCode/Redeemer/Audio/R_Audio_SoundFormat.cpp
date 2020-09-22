/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Audio_SoundFormat.cpp
**		Base class for sound format
*/

#include "R_Audio_SoundFormat.h"

//------------------------------------------------------------------------------------------------------------------------
namespace REDEEMER
{
	namespace AUDIO
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_SoundFormat::C_SoundFormat ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_SoundFormat::~C_SoundFormat ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		int	C_SoundFormat::GetSampleSize (ESampleFormat format)
		{
			switch (format) 
			{
			case SF_U8:
				return 1;

			case SF_S16:
				return 2;

			default:
				return 0;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace AUDIO
}	//	namespace REDEEMER
