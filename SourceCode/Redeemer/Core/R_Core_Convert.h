/*
**	Redeemer Engine 2008-2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
***	R_Core_Convert.h
**		Various conversion routines
*/

#ifndef _R_CORE_CONVERT_H_
#define _R_CORE_CONVERT_H_

#include <windows.h>
#include <string>

namespace REDEEMER
{
	namespace CORE
	{
		/*!	\brief C_Convert class provides some useful static functions to convert between data formats
		*/
		class C_Convert
		{
		public:
			/*!	Convert from 'wstring' to 'string'
			**	\param dataOut buffer to write data to
			**	\param S data to be converted
			**	\param CodePage optional code page
			**	\return True if conversion succeed
			*/
			static bool ConvertUnicodeToChars(std::string *dataOut, const std::wstring &S, unsigned CodePage = CP_ACP);

			/*!	Convert from 'wchar_t*' to 'string'
			**	\param dataOut buffer to write data to
			**	\param S data to be converted
			**	\param NumChars number of characters in a string
			**	\param CodePage optional code page
			**	\return True if conversion succeed
			*/
			static bool ConvertUnicodeToChars(std::string *dataOut, const wchar_t* S, unsigned NumChars, unsigned CodePage = CP_ACP);

			/*!	Convert from 'string' to 'wstring'
			**	\param dataOut buffer to write data to
			**	\param S data to be converted
			**	\param CodePage optional code page
			**	\return True if conversion succeed
			*/
			static bool ConvertCharsToUnicode(std::wstring *dataOut, const std::string &S, unsigned CodePage = CP_ACP);

			/*!	Convert from 'char*' to 'wstring'
			**	\param dataOut buffer to write data to
			**	\param S data to be converted
			**	\param NumChars number of characters in a string
			**	\param CodePage optional code page
			**	\return True if conversion succeed
			*/
			static bool ConvertCharsToUnicode(std::wstring *dataOut, const char* S, unsigned NumChars, unsigned CodePage = CP_ACP);

			/*!	Convert from 'string' to 'integer'
			**	\param str data to be converted
			**	\return converted number
			*/
			static int ConvertStringToInteger (std::string str);

			/*!	Convert from 'integer' to 'string'
			**	\param number data to be converted
			**	\return converted string
			*/
			static std::string ConvertIntegerToString (int number);

			/*!	Convert from 'string' to 'double'
			**	\param str data to be converted
			**	\return converted number
			*/
			static double ConvertStringToDouble (std::string str);

			/*!	Convert from 'double' to 'string'
			**	\param number data to be converted
			**	\return converted string
			*/
			static std::string ConvertDoubleToString (double number);
		};
		
	}	//	namespace CORE
}	//	namespace REDEEMER

#endif	//	_R_CORE_CONVERT_H_
