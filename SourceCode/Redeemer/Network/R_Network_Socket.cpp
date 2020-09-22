/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Network_Socket.cpp
**		Low level socket implementation for network system
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Network_Socket.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace NETWORK
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_Socket::C_Socket (ESocketType type) :
			m_Type (type),
			m_Socket (GetInvalidSocket ()),
			m_IsBlocking (true)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Socket::~C_Socket ()
		{
			Close ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		sockaddr_in C_Socket::CreateAddress (unsigned long address, unsigned short port)
		{
			sockaddr_in addr;

			memset(addr.sin_zero, 0, sizeof(addr.sin_zero));

			addr.sin_addr.s_addr = htonl(address);
			addr.sin_family      = AF_INET;
			addr.sin_port        = htons(port);

			return addr;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Socket::Create ()
		{
			// Don't create the socket if it already exists
			if (m_Socket == GetInvalidSocket ())
			{
				UINT_PTR handle = socket (PF_INET, m_Type == ST_Tcp ? SOCK_STREAM : SOCK_DGRAM, 0);

				Create (handle);
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Socket::Create (UINT_PTR handle)
		{
			// Don't create the socket if it already exists
			if (m_Socket == GetInvalidSocket ())
			{
				// Assign the new handle
				m_Socket = handle;

				// Set the current blocking state
				SetBlocking (m_IsBlocking);

				// To avoid the "Address already in use" error message when trying to bind to the same port
				int yes = 1;

				if (setsockopt (m_Socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&yes), sizeof(yes)) == -1)
				{
					REDEEMER_LOG << LOG_ERROR << L"Failed to set socket option \"SO_REUSEADDR\" ; " << "binding to a same port may fail if too fast" << LOG_ENDMESSAGE;
				}

				if (m_Type == ST_Tcp)
				{
					// Disable the Nagle algorithm (ie. removes buffering of TCP packets)
					if (setsockopt (m_Socket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char*>(&yes), sizeof(yes)) == -1)
					{
						REDEEMER_LOG << LOG_ERROR << L"Failed to set socket option \"TCP_NODELAY\" ; " << "all your TCP packets will be buffered" << LOG_ENDMESSAGE;
					}
				}
				else
				{
					// Enable broadcast by default for UDP sockets
					if (setsockopt (m_Socket, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char*>(&yes), sizeof(yes)) == -1)
					{
						REDEEMER_LOG << LOG_ERROR << L"Failed to enable broadcast on UDP socket" << LOG_ENDMESSAGE;
					}
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Socket::Close ()
		{
			// Close the socket
			if (m_Socket != GetInvalidSocket ())
			{
				 closesocket (m_Socket);

				m_Socket = GetInvalidSocket ();
			}

			// Reset the pending packet data
			m_PendingPacket = S_PendingPacket ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Socket::SetBlocking (bool isBlocking)
		{
			// Apply if the socket is already created
			if (m_Socket != GetInvalidSocket ())
			{
				unsigned long blocking = isBlocking ? 0 : 1;

				ioctlsocket (m_Socket, FIONBIO, &blocking);
			}

			m_IsBlocking = isBlocking;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_Socket::IsBlocking () const
		{
			return m_IsBlocking;
		}

		//------------------------------------------------------------------------------------------------------------------------

		UINT_PTR C_Socket::GetHandle () const
		{
			return m_Socket;
		}

		//------------------------------------------------------------------------------------------------------------------------

		UINT_PTR C_Socket::GetInvalidSocket ()
		{
			return INVALID_SOCKET;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Socket::ESocketStatus C_Socket::GetErrorStatus ()
		{
			switch (WSAGetLastError())
			{
			case WSAEWOULDBLOCK :  return C_Socket::SS_NotReady;
			case WSAECONNABORTED : return C_Socket::SS_Disconnected;
			case WSAECONNRESET :   return C_Socket::SS_Disconnected;
			case WSAETIMEDOUT :    return C_Socket::SS_Disconnected;
			case WSAENETRESET :    return C_Socket::SS_Disconnected;
			case WSAENOTCONN :     return C_Socket::SS_Disconnected;
			default :              return C_Socket::SS_Error;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_Socket::S_PendingPacket::S_PendingPacket () :
			Size (0),
			SizeReceived (0),
			Data ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace NETWORK
}	//	namespace REDEEMER

