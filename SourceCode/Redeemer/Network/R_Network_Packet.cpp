/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Network_Packet.cpp
**		Packets are used to interchange data between computers in the internet
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Network_Packet.h"

#include <winsock.h>

namespace REDEEMER
{
	namespace NETWORK
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_Packet::C_Packet () :
			m_ReadPosition (0),
			m_IsValid (true)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet::~C_Packet ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Packet::Append (const void* data, std::size_t sizeInBytes)
		{
			if (data && (sizeInBytes > 0))
			{
				std::size_t start = m_Data.size();

				m_Data.resize (start + sizeInBytes);

				memcpy (&m_Data[start], data, sizeInBytes);
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Packet::Clear ()
		{
			m_Data.clear();

			m_ReadPosition = 0;
			m_IsValid = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		const char* C_Packet::GetData () const
		{
			return !m_Data.empty() ? &m_Data[0] : NULL;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::size_t C_Packet::GetDataSize () const
		{
			return m_Data.size();
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_Packet::EndOfPacket () const
		{
			return m_ReadPosition >= m_Data.size();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet::operator bool() const
		{
			return m_IsValid;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet& C_Packet::operator >>(bool& data)
		{
			unsigned char value;

			if (*this >> value)
				data = (value != 0);

			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet& C_Packet::operator >>(char& data)
		{
			if (CheckSize (sizeof (data)))
			{
				data = *reinterpret_cast<const char*>(GetData() + m_ReadPosition);
				m_ReadPosition += sizeof(data);
			}

			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet& C_Packet::operator >>(unsigned char& data)
		{
			if (CheckSize (sizeof (data)))
			{
				data = *reinterpret_cast<const unsigned char*>(GetData() + m_ReadPosition);
				m_ReadPosition += sizeof(data);
			}

			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet& C_Packet::operator >>(short& data)
		{
			if (CheckSize (sizeof(data)))
			{
				data = ntohs (*reinterpret_cast<const short*>(GetData() + m_ReadPosition));
				m_ReadPosition += sizeof(data);
			}

			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet& C_Packet::operator >>(unsigned short& data)
		{
			if (CheckSize (sizeof(data)))
			{
				data = ntohs(*reinterpret_cast<const unsigned short*>(GetData() + m_ReadPosition));
				m_ReadPosition += sizeof(data);
			}

			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet& C_Packet::operator >>(int& data)
		{
			if (CheckSize (sizeof(data)))
			{
				data = ntohl(*reinterpret_cast<const int*>(GetData() + m_ReadPosition));
				m_ReadPosition += sizeof(data);
			}

			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet& C_Packet::operator >>(unsigned int& data)
		{
			if (CheckSize (sizeof(data)))
			{
				data = ntohl(*reinterpret_cast<const unsigned int*>(GetData() + m_ReadPosition));
				m_ReadPosition += sizeof(data);
			}

			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet& C_Packet::operator >>(float& data)
		{
			if (CheckSize (sizeof(data)))
			{
				data = *reinterpret_cast<const float*>(GetData() + m_ReadPosition);
				m_ReadPosition += sizeof(data);
			}

			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet& C_Packet::operator >>(double& data)
		{
			if (CheckSize (sizeof(data)))
			{
				data = *reinterpret_cast<const double*>(GetData () + m_ReadPosition);
				m_ReadPosition += sizeof(data);
			}

			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet& C_Packet::operator >>(char* data)
		{
			// First extract string length
			unsigned int length = 0;

			*this >> length;

			if ((length > 0) && CheckSize (length))
			{
				// Then extract characters
				memcpy(data, GetData() + m_ReadPosition, length);
				data[length] = '\0';

				// Update reading position
				m_ReadPosition += length;
			}

			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet& C_Packet::operator >>(std::string& data)
		{
			// First extract string length
			unsigned int length = 0;

			*this >> length;

			data.clear();

			if ((length > 0) && CheckSize (length))
			{
				// Then extract characters
				data.assign(GetData() + m_ReadPosition, length);

				// Update reading position
				m_ReadPosition += length;
			}

			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet& C_Packet::operator >>(wchar_t* data)
		{
			// First extract string length
			unsigned int length = 0;

			*this >> length;

			if ((length > 0) && CheckSize(length * sizeof(unsigned int)))
			{
				// Then extract characters
				for (unsigned int i = 0; i < length; ++i)
				{
					unsigned int character = 0;

					*this >> character;
					data[i] = static_cast<wchar_t>(character);
				}

				data[length] = L'\0';
			}

			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet& C_Packet::operator >>(std::wstring& data)
		{
			// First extract string length
			unsigned int length = 0;

			*this >> length;

			data.clear();

			if ((length > 0) && CheckSize(length * sizeof(unsigned int)))
			{
				// Then extract characters
				for (unsigned int i = 0; i < length; ++i)
				{
					unsigned int character = 0;

					*this >> character;
					data += static_cast<wchar_t>(character);
				}
			}

			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet& C_Packet::operator <<(bool data)
		{
			*this << static_cast<unsigned char>(data);

			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet& C_Packet::operator <<(char data)
		{
			Append (&data, sizeof(data));

			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet& C_Packet::operator <<(unsigned char data)
		{
			Append (&data, sizeof(data));

			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet& C_Packet::operator <<(short data)
		{
			short toWrite = htons(data);

			Append (&toWrite, sizeof(toWrite));

			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet& C_Packet::operator <<(unsigned short data)
		{
			unsigned short toWrite = htons(data);

			Append (&toWrite, sizeof(toWrite));

			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet& C_Packet::operator <<(int data)
		{
			int toWrite = htonl(data);

			Append (&toWrite, sizeof(toWrite));

			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet& C_Packet::operator <<(unsigned int data)
		{
			unsigned int toWrite = htonl(data);

			Append (&toWrite, sizeof(toWrite));

			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet& C_Packet::operator <<(float data)
		{
			Append (&data, sizeof(data));

			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet& C_Packet::operator <<(double data)
		{
			Append (&data, sizeof(data));

			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet& C_Packet::operator <<(const char* data)
		{
			// First insert string length
			unsigned int length = 0;

			for (const char* c = data; *c != '\0'; ++c)
				++length;

			*this << length;

			// Then insert characters
			Append (data, length * sizeof(char));

			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet& C_Packet::operator <<(const std::string& data)
		{
			// First insert string length
			unsigned int length = static_cast<unsigned int>(data.size());

			*this << length;

			// Then insert characters
			if (length > 0)
			{
				Append (data.c_str(), length * sizeof(std::string::value_type));
			}

			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet& C_Packet::operator <<(const wchar_t* data)
		{
			// First insert string length
			unsigned int length = 0;

			for (const wchar_t* c = data; *c != L'\0'; ++c)
				++length;

			*this << length;

			// Then insert characters
			for (const wchar_t* c = data; *c != L'\0'; ++c)
				*this << static_cast<unsigned int>(*c);

			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Packet& C_Packet::operator <<(const std::wstring& data)
		{
			// First insert string length
			unsigned int length = static_cast<unsigned int>(data.size());

			*this << length;

			// Then insert characters
			if (length > 0)
			{
				for (std::wstring::const_iterator c = data.begin(); c != data.end(); ++c)
					*this << static_cast<unsigned int>(*c);
			}

			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_Packet::CheckSize(std::size_t size)
		{
			m_IsValid = m_IsValid && (m_ReadPosition + size <= m_Data.size());

			return m_IsValid;
		}

		//------------------------------------------------------------------------------------------------------------------------

		const char* C_Packet::OnSend(std::size_t& size)
		{
			size = GetDataSize ();

			return GetData();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Packet::OnReceive (const char* data, std::size_t size)
		{
			 Append (data, size);
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace NETWORK
}	//	namespace REDEEMER

