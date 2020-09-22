/*
**	Redeemer Engine 2008-2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
***	R_Core_Convert.cpp
**		Converting routines
*/

#include <sstream>

#include "R_Core_Convert.h"

namespace REDEEMER
{
	namespace CORE
	{
		//------------------------------------------------------------------------------------------------------------------------
		bool C_Convert::ConvertUnicodeToChars(std::string *Out, const std::wstring &S, unsigned CodePage)
		{
			if (S.empty())
			{
				Out->clear(); 
				
				return true;
			}

			int Size = WideCharToMultiByte (CodePage, 0, S.data(), (int)S.length(), NULL, 0, NULL, NULL);

			if (Size == 0)
			{
				Out->clear(); 
				
				return false;
			}

			char *Buf = new char[Size];
			int R = WideCharToMultiByte (CodePage, 0, S.data(), (int)S.length(), Buf, (int)Size, NULL, NULL);

			if (R == 0)
			{
				Out->clear (); 
				
				return false;
			}
			
			Out->assign(Buf, Size);

			delete [] Buf;

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_Convert::ConvertUnicodeToChars (std::string *Out, const wchar_t *S, unsigned NumChars, unsigned CodePage)
		{
			if (NumChars == 0)
			{
				Out->clear(); 
				
				return true;
			}

			int Size = WideCharToMultiByte (CodePage, 0, S, (int)NumChars, NULL, 0, NULL, NULL);

			if (Size == 0)
			{
				Out->clear(); 
				
				return false;
			}

			char *Buf = new char[Size];
			int R = WideCharToMultiByte (CodePage, 0, S, (int)NumChars, Buf, Size, NULL, NULL);
			
			if (R == 0)
			{
				Out->clear(); 
				
				return false;
			}

			Out->assign(Buf, Size);

			delete [] Buf;
			
			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_Convert::ConvertCharsToUnicode(std::wstring *Out, const std::string &S, unsigned CodePage)
		{
			if (S.empty())
			{
				Out->clear(); 
				
				return true;
			}

			int Size = MultiByteToWideChar (CodePage, 0, S.data(), (int)S.length(), NULL, 0);

			if (Size == 0)
			{
				Out->clear(); 
				
				return false;
			}

			wchar_t *Buf = new wchar_t[Size];
			int R = MultiByteToWideChar (CodePage, 0, S.data(), (int)S.length(), Buf, Size);
			
			if (R == 0)
			{
				Out->clear();  
				
				return false;
			}

			Out->assign(Buf, Size);

			delete [] Buf;

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_Convert::ConvertCharsToUnicode(std::wstring *Out, const char *S, unsigned NumChars, unsigned CodePage)
		{
			if (NumChars == 0)
			{
				Out->clear(); 
				
				return true;
			}

			int Size = MultiByteToWideChar (CodePage, 0, S, (int)NumChars, NULL, 0);

			if (Size == 0)
			{
				Out->clear(); 
				
				return false;
			}

			wchar_t *Buf = new wchar_t[Size];
			int R = MultiByteToWideChar (CodePage, 0, S, (int)NumChars, Buf, Size);

			if (R == 0)
			{
				Out->clear(); 
				
				return false;
			}

			Out->assign(Buf, Size);

			delete [] Buf;

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_Convert::ConvertStringToInteger (std::string str)
		{
			return atoi (str.c_str ());
		}

		//------------------------------------------------------------------------------------------------------------------------

		double C_Convert::ConvertStringToDouble (std::string str)
		{
			return atof (str.c_str ());
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::string C_Convert::ConvertIntegerToString (int number)
		{
			std::string s;
			std::stringstream out;

			out << number;
			s = out.str();

			return s;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::string C_Convert::ConvertDoubleToString (double number)
		{
			std::string s;
			std::stringstream out;

			out << number;
			s = out.str();

			return s;
		}

		//------------------------------------------------------------------------------------------------------------------------
		
	}	//	namespace CORE
}	//	namespace REDEEMER
