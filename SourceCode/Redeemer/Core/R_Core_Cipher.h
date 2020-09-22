/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_Cipher.h
**		Provides some methods for encoding/decoding streams
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_CORE_CIPHER_H_
#define _R_CORE_CIPHER_H_

namespace REDEEMER
{
	namespace CORE
	{
		/*!	C_Cipher class provides some methods for encoding/decoding streams
		*/
		class C_Cipher
		{
		public:
			/*!	The simplest of all. In this you increase or decrease a char by a number.
			*/
			static void BinaryShift (char* stream, int shift);

			/*!	XOR
			**	Another type of encryption. Exclusive-OR encryption, is almost unbreakable through brute force methods.
			**	It is susceptible to patterns, but this weakness can be avoided through first compressing 
			**	the file (so as to remove patterns). This encryption while extremely simple, is nearly unbreakable.
			*/
			static void XOR (char *string, const char *key);
		};

	}	//	namespace CORE
}	//	namespace REDEEMER

#endif	//	_R_CORE_CIPHER_H_
