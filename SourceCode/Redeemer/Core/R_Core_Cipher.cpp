/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_Cipher.cpp
**		Provides some methods for encoding/decoding streams
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Core_Cipher.h"

#include <string>

namespace REDEEMER
{
	namespace CORE
	{
		//------------------------------------------------------------------------------------------------------------------------
		void C_Cipher::BinaryShift (char* stream, int shift)
		{
			for (int i = 0; stream[i] != 0; i++)
			{
				stream[i] += shift;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Cipher::XOR (char *string, const char *key)
		{
			unsigned int l = 0;

			for (int x = 0; string[x] != '\0'; x ++)
			{		
				string [x] = string[x] ^ key[l];
				l++;

				if (l >= strlen(key)) 
					l = 0;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace CORE
}	//	namespace REDEEMER

