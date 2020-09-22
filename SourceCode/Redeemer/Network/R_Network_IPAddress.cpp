/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Network_IPAddress.cpp
**		Encapsulate an IPv4 network address
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Network_IPAddress.h"
#include "R_Network_Socket.h"

#include <WinSock.h>

namespace REDEEMER
{
	namespace NETWORK
	{
		const C_IPAddress C_IPAddress::None;
		const C_IPAddress C_IPAddress::LocalHost (127, 0, 0, 1);

		//------------------------------------------------------------------------------------------------------------------------
		C_IPAddress::C_IPAddress ()	:
			m_Address(INADDR_NONE)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_IPAddress::C_IPAddress (const std::string& address)
		{
			// First try to convert it as a byte representation ("xxx.xxx.xxx.xxx")
			m_Address = inet_addr(address.c_str());

			// If not successful, try to convert it as a host name
			if (m_Address == INADDR_NONE)
			{
				hostent* host = gethostbyname(address.c_str());

				if (host)
				{
					// Host found, extract its IP address
					m_Address = reinterpret_cast<in_addr*>(host->h_addr)->s_addr;
				}
				else
				{
					// Host name not found on the network
					m_Address = INADDR_NONE;
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_IPAddress::C_IPAddress (const char* address)
		{
			// First try to convert it as a byte representation ("xxx.xxx.xxx.xxx")
			m_Address = inet_addr(address);

			// If not successful, try to convert it as a host name
			if (m_Address == INADDR_NONE)
			{
				hostent* host = gethostbyname(address);

				if (host)
				{
					// Host found, extract its IP address
					m_Address = reinterpret_cast<in_addr*>(host->h_addr)->s_addr;
				}
				else
				{
					// Host name not found on the network
					m_Address = INADDR_NONE;
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_IPAddress::C_IPAddress (unsigned char byte0, unsigned char byte1, unsigned char byte2, unsigned char byte3)
		{
			 m_Address = htonl((byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_IPAddress::C_IPAddress (unsigned int address)
		{
			m_Address = htonl(address);
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::string C_IPAddress::ToString () const
		{
			in_addr address;

			address.s_addr = m_Address;

			return inet_ntoa(address);
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned int C_IPAddress::ToInteger () const
		{
			 return ntohl (m_Address);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_IPAddress C_IPAddress::GetLocalAddress()
		{
			// The method here is to connect a UDP socket to anyone (here to localhost),
			// and get the local socket address with the getsockname function.
			// UDP connection will not send anything to the network, so this function won't cause any overhead.

			C_IPAddress localAddress;

			// Create the socket
			UINT_PTR sock = socket(PF_INET, SOCK_DGRAM, 0);

			if (sock == C_Socket::GetInvalidSocket())
				return localAddress;

			// Connect the socket to localhost on any port

			sockaddr_in address = C_Socket::CreateAddress (INADDR_LOOPBACK, 0);

			if (connect (sock, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == -1)
			{
				closesocket(sock);

				return localAddress;
			}

			// Get the local address of the socket connection
			int size = sizeof(address);

			if (getsockname (sock, reinterpret_cast<sockaddr*>(&address), &size) == -1)
			{
				closesocket(sock);

				return localAddress;
			}

			// Close the socket
			closesocket(sock);

			// Finally build the IP address
			localAddress = C_IPAddress (ntohl (address.sin_addr.s_addr));

			return localAddress;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_IPAddress C_IPAddress::GetPublicAddress (float timeout)
		{
			return C_IPAddress ();
		}	

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		bool operator ==(const C_IPAddress& left, const C_IPAddress& right)
		{
			  return left.ToInteger() == right.ToInteger();
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool operator !=(const C_IPAddress& left, const C_IPAddress& right)
		{
			  return !(left == right);
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool operator <(const C_IPAddress& left, const C_IPAddress& right)
		{
			 return left.ToInteger() < right.ToInteger();
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool operator >(const C_IPAddress& left, const C_IPAddress& right)
		{
			 return right < left;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool operator <=(const C_IPAddress& left, const C_IPAddress& right)
		{
			return !(right < left);
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool operator >=(const C_IPAddress& left, const C_IPAddress& right)
		{
			return !(left < right);
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::istream& operator >>(std::istream& stream, C_IPAddress& address)
		{
			std::string str;

			stream >> str;
			address = C_IPAddress(str);

			return stream;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::ostream& operator <<(std::ostream& stream, const C_IPAddress& address)
		{
			return stream << address.ToString();
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace NETWORK
}	//	namespace REDEEMER

