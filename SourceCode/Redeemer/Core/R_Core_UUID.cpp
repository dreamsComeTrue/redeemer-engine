/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_UUID.cpp
**		Tool class for Universal Unique IDentifier.
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Core_UUID.h"

#include <sstream>

// disable: deprecation warnings when using CRT calls in VC8
#pragma warning (disable: 4996)

namespace REDEEMER
{
	namespace CORE
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_UUID::C_UUID () :
			m_Data1(0),
			m_Data2(0), 
			m_Data3(0)
		{
			m_Data4[0] = m_Data4[1] = m_Data4[2] = m_Data4[3] = m_Data4[4] = m_Data4[5] = m_Data4[6] = m_Data4[7] = 0;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_UUID::C_UUID (unsigned long iD1, unsigned short iD2, unsigned short iD3, unsigned char iD40, unsigned char iD41, unsigned char iD42,
			unsigned char iD43, unsigned char iD44, unsigned char iD45, unsigned char iD46, unsigned char iD47) :
			m_Data1(iD1),
			m_Data2(iD2), 
			m_Data3(iD3)	
		{
			m_Data4[0] = iD40;
			m_Data4[1] = iD41;
			m_Data4[2] = iD42;
			m_Data4[3] = iD43;
			m_Data4[4] = iD44;
			m_Data4[5] = iD45;
			m_Data4[6] = iD46;
			m_Data4[7] = iD47;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_UUID::C_UUID (const char * iprString) : 
			m_Data1(0),
			m_Data2(0), 
			m_Data3(0)
		{
			m_Data4[0] = m_Data4[1] = m_Data4[2] = m_Data4[3] = m_Data4[4] = m_Data4[5] = m_Data4[6] = m_Data4[7] = 0;

			if (NULL == iprString)
				return;

			std::string s(iprString);
			std::stringstream stream;

			stream << "0x" << std::string(s, 1, 8) << '\n';
			stream >> std::hex >> m_Data1;

			stream << "0x" << std::string(s, 10, 4) << '\n';
			stream >> std::hex >> m_Data2;

			stream << "0x" << std::string(s, 15, 4) << '\n';
			stream >> std::hex >> m_Data3;

			unsigned long tmp;
			stream << "0x" << std::string(s, 20, 2) << '\n';
			stream >> std::hex >> tmp; m_Data4[0] = (unsigned char)tmp;

			stream << "0x" << std::string(s, 22, 2) << '\n';
			stream >> std::hex >> tmp; m_Data4[1] = (unsigned char)tmp;

			stream << "0x" << std::string(s, 25, 2) << '\n';
			stream >> std::hex >> tmp; m_Data4[2] = (unsigned char)tmp;

			stream << "0x" << std::string(s, 27, 2) << '\n';
			stream >> std::hex >> tmp; m_Data4[3] = (unsigned char)tmp;

			stream << "0x" << std::string(s, 29, 2) << '\n';
			stream >> std::hex >> tmp; m_Data4[4] = (unsigned char)tmp;

			stream << "0x" << std::string(s, 31, 2) << '\n';
			stream >> std::hex >> tmp; m_Data4[5] = (unsigned char)tmp;

			stream << "0x" << std::string(s, 33, 2) << '\n';
			stream >> std::hex >> tmp; m_Data4[6] = (unsigned char)tmp;

			stream << "0x" << std::string(s, 35, 2) << '\n';
			stream >> std::hex >> tmp; m_Data4[7] = (unsigned char)tmp;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_UUID::C_UUID (const C_UUID& iUUID)
		{
			::memcpy (this, &iUUID, sizeof(C_UUID));
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_UUID::operator= (const C_UUID& iUUID)
		{
			 ::memcpy (this, &iUUID, sizeof(C_UUID));
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_UUID::operator == (const C_UUID& iUUID) const
		{
			 return (0 == ::memcmp (this, &iUUID, sizeof(C_UUID)));
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_UUID::operator != (const C_UUID& iUUID) const
		{
			return (0 != ::memcmp (this, &iUUID, sizeof(C_UUID)));
		}

		//------------------------------------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------
		//           1         2         3   
		// 012345678901234567890123456789012345678
		// {xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}
		void C_UUID::GetString (char oString[39]) const
		{
			::sprintf (oString, "{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
				m_Data1, m_Data2, m_Data3, m_Data4[0], m_Data4[1],
				m_Data4[2], m_Data4[3], m_Data4[4], m_Data4[5], m_Data4[6], m_Data4[7]);
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace CORE
}	//	namespace REDEEMER

