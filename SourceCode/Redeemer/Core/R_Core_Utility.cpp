/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_Utility.cpp
**		Various utility functions
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Core_Utility.h"

#include <sstream>

// disable: deprecation warnings when using CRT calls in VC8
#pragma warning (disable: 4996)

namespace REDEEMER
{
	namespace CORE
	{
		//------------------------------------------------------------------------------------------------------------------------
		void C_Utility::Trim (std::string& str, bool left, bool right)
		{
			if (right)
				str.erase (str.find_last_not_of(" \t\r") + 1);

			if (left)
				str.erase (0, str.find_first_not_of(" \t\r"));
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::vector<std::string> C_Utility::TokenizeString (char* data, const char* delimiter)
		{
			std::vector<std::string> strArray;

			char* pch = strtok(data, delimiter);

			while (pch)
			{
				std::string t = pch;

				strArray.push_back (t);

				pch = strtok (NULL, delimiter);
			}

			return strArray;
		}

		//------------------------------------------------------------------------------------------------------------------------

		short C_Utility::ParseShort (const std::string& value)
		{
			return ParseValue<short> (value);
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned short C_Utility::ParseUnsignedShort (const std::string& value)
		{
			return ParseValue<unsigned short> (value);
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_Utility::ParseInt (const std::string& value)
		{
			return ParseValue<int> (value);
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned int C_Utility::ParseUnsignedInt (const std::string& value)
		{
			return ParseValue<unsigned int> (value);
		}

		//------------------------------------------------------------------------------------------------------------------------

		size_t C_Utility::ParseSizeT (const std::string& value)
		{
			return ParseValue<size_t> (value);
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_Utility::ParseFloat (const std::string& value)
		{
			return ParseValue<float> (value);
		}

		//------------------------------------------------------------------------------------------------------------------------

		double C_Utility::ParseDouble (const std::string& value)
		{
			return ParseValue<double> (value);
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_Utility::ParseBool (const std::string& value)
		{
			return ParseValue<bool> (value);
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned char C_Utility::ParseUnsignedChar (const std::string& value)
		{
			return ParseValue<unsigned char> (value);
		}

		//------------------------------------------------------------------------------------------------------------------------

		char C_Utility::ParseChar (const std::string& value)
		{
			return ParseValue<char> (value);
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace CORE
}	//	namespace REDEEMER

