/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_Utility.h
**		Various utility functions
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_CORE_UTILITY_H_
#define _R_CORE_UTILITY_H_

#include <string>
#include <vector>

namespace REDEEMER
{
	namespace CORE
	{
		/*!	\brief  Various utility functions
		*/
		class C_Utility
		{
		public:
			/*!	Trims string from whitespaces
			*/
			static void Trim (std::string& str, bool left = true, bool right = true);

			/*!	Breaks entire string into small tokens using delimiter
			*/
			static std::vector<std::string> TokenizeString (char* data, const char* delimiter);

			/*!	Reads short number from a string
			*/
			static short ParseShort (const std::string& value);

			/*!	Reads unsigned short number from a string
			*/
			static unsigned short ParseUnsignedShort (const std::string& value);

			/*!	Reads int number from a string
			*/
			static int ParseInt (const std::string& value);

			/*!	Reads unsigned int number from a string
			*/
			static unsigned int ParseUnsignedInt (const std::string& value);

			/*!	Reads size_t number from a string
			*/
			static size_t ParseSizeT (const std::string& value);

			/*!	Reads float number from a string
			*/
			static float ParseFloat (const std::string& value);

			/*!	Reads double number from a string
			*/
			static double ParseDouble (const std::string& value);

			/*!	Reads bool number from a string
			*/
			static bool ParseBool (const std::string& value);

			/*!	Reads unsigned char number from a string
			*/
			static unsigned char ParseUnsignedChar (const std::string& value);

			/*!	Reads char number from a string
			*/
			static char ParseChar (const std::string& value);

			/*!	Parses string to given value type
			*/
			template<typename T > static T ParseValue (const std::string& value)
			{
				std::istringstream stream (value);

				T result;

				stream >> result;

				if (stream.fail()) 
					return T ();
				else
				{
					int item = stream.get();

					while (item != -1)
					{
						if (item != ' ' && item != '\t') 
							return T ();

						item = stream.get();
					}
				}

				return result;
			}

			/*!	Converts given data to a string
			*/
			template<typename T> static std::string ToString (T p)
			{
				std::ostringstream stream;
				stream << p;
				return stream.str();
			}

			/*!	Converts given data to a string
			*/
			template<typename T1,  typename T2 >
			static inline std::string ToString (T1 p1, T2 p2)
			{
				std::ostringstream stream;
				stream << p1 << p2;
				return stream.str();
			}

			/*!	Converts given data to a string
			*/
			template<typename T1,  typename T2,  typename T3 >
			static inline std::string ToString (T1 p1, T2 p2, T3 p3)
			{
				std::ostringstream stream;
				stream << p1 << p2 << p3;
				return stream.str();
			}

			/*!	Converts given data to a string
			*/
			template<typename T1,  typename T2,  typename T3, typename T4 >
			static inline std::string ToString (T1 p1, T2 p2, T3 p3, T4 p4)
			{
				std::ostringstream stream;
				stream << p1 << p2 << p3 << p4;
				return stream.str();
			}

			/*!	Converts given data to a string
			*/
			template<typename T1,  typename T2,  typename T3, typename T4, typename T5 >
			static inline std::string ToString (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
			{
				std::ostringstream stream;
				stream << p1 << p2 << p3 << p4 << p5;
				return stream.str();
			}

			/*!	Converts given data to a string
			*/
			template<typename T1,  typename T2,  typename T3, typename T4, typename T5, typename T6 >
			static inline std::string ToString (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
			{
				std::ostringstream stream;
				stream << p1 << p2 << p3 << p4 << p5 << p6;
				return stream.str();
			}

			/*!	Converts given data to a string
			*/
			template<typename T1,  typename T2,  typename T3, typename T4, typename T5, typename T6, typename T7 >
			static inline std::string ToString (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
			{
				std::ostringstream stream;
				stream << p1 << p2 << p3 << p4 << p5 << p6 << p7;
				return stream.str();
			}

			/*!	Converts given data to a string
			*/
			template<typename T1,  typename T2,  typename T3, typename T4, typename T5, typename T6, typename T7, typename T8 >
			static inline std::string ToString (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
			{
				std::ostringstream stream;
				stream << p1 << p2 << p3 << p4 << p5 << p6 << p7 << p8;
				return stream.str();
			}

			/*!	Converts given data to a string
			*/
			template<typename T1,  typename T2,  typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9 >
			static inline std::string ToString (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9)
			{
				std::ostringstream stream;
				stream << p1 << p2 << p3 << p4 << p5 << p6 << p7 << p8 << p9;
				return stream.str();
			}
		};

	}	//	namespace CORE
}	//	namespace REDEEMER

#endif	//	_R_CORE_UTILITY_H_
