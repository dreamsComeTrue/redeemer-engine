/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Network_TCPSocket.cpp
**		Specialized socket using the TCP protocol
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Network_TCPSocket.h"
#include "R_Network_Packet.h"
#include "R_Network_IPAddress.h"
#include "..\\RedeemerEngine.h"

#include <WinSock.h>

namespace REDEEMER
{
	namespace NETWORK
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_TCPSocket::C_TCPSocket() :
			C_Socket (ST_Tcp)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned short C_TCPSocket::GetLocalPort () const
		{
			if (GetHandle () != GetInvalidSocket())
			{
				// Retrieve informations about the local end of the socket
				sockaddr_in address;
				int size = sizeof(address);

				if (getsockname (GetHandle (), reinterpret_cast<sockaddr*>(&address), &size) != -1)
				{
					return ntohs (address.sin_port);
				}
			}

			// We failed to retrieve the port
			return 0;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_IPAddress C_TCPSocket::GetRemoteAddress () const
		{
			if (GetHandle() != GetInvalidSocket ())
			{
				// Retrieve informations about the remote end of the socket
				sockaddr_in address;
				int size = sizeof(address);

				if (getpeername (GetHandle (), reinterpret_cast<sockaddr*>(&address), &size) != -1)
				{
					return C_IPAddress (ntohl(address.sin_addr.s_addr));
				}
			}

			// We failed to retrieve the address
			return C_IPAddress::None;
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned short C_TCPSocket::GetRemotePort () const
		{
			if (GetHandle() != GetInvalidSocket ())
			{
				// Retrieve informations about the remote end of the socket
				sockaddr_in address;
				int size = sizeof(address);

				if (getpeername (GetHandle(), reinterpret_cast<sockaddr*>(&address), &size) != -1)
				{
					return ntohs(address.sin_port);
				}
			}

			// We failed to retrieve the port
			return 0;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Socket::ESocketStatus C_TCPSocket::Connect (const C_IPAddress& remoteAddress, unsigned short remotePort, float timeout)
		{
			// Create the internal socket if it doesn't exist
			Create ();

			// Create the remote address
			sockaddr_in address = CreateAddress(remoteAddress.ToInteger(), remotePort);

			if (timeout <= 0)
			{
				// ----- We're not using a timeout: just try to connect -----

				// Connect the socket
				if (connect (GetHandle(), reinterpret_cast<sockaddr*>(&address), sizeof(address)) == -1)
					return GetErrorStatus();

				// Connection succeeded
				return SS_Done;
			}
			else
			{
				// ----- We're using a timeout: we'll need a few tricks to make it work -----

				// Save the previous blocking state
				bool blocking = IsBlocking ();

				// Switch to non-blocking to enable our connection timeout
				if (blocking)
					SetBlocking (false);

				// Try to connect to the remote address
				if (connect (GetHandle(), reinterpret_cast<sockaddr*>(&address), sizeof(address)) >= 0)
				{
					// We got instantly connected! (it may no happen a lot...)
					return SS_Done;
				}

				// Get the error status
				ESocketStatus status = GetErrorStatus();

				// If we were in non-blocking mode, return immediately
				if (!blocking)
					return status;

				// Otherwise, wait until something happens to our socket (success, timeout or error)
				if (status == C_Socket::SS_NotReady)
				{
					// Setup the selector
					fd_set selector;

					FD_ZERO(&selector);
					FD_SET(GetHandle (), &selector);

					// Setup the timeout
					timeval time;
					time.tv_sec  = static_cast<long>(timeout);
					time.tv_usec = (static_cast<long>(timeout * 1000) % 1000) * 1000;

					// Wait for something to write on our socket (which means that the connection request has returned)
					if (select (static_cast<int> (GetHandle() + 1), NULL, &selector, NULL, &time) > 0)
					{
						// At this point the connection may have been either accepted or refused.
						// To know whether it's a success or a failure, we must check the address of the connected peer
						if (GetRemoteAddress() != C_IPAddress::None)
						{
							// Connection accepted
							status = SS_Done;
						}
						else
						{
							// Connection refused
							status = GetErrorStatus();
						}
					}
					else
					{
						// Failed to connect before timeout is over
						status = GetErrorStatus();
					}
				}

				// Switch back to blocking mode
				SetBlocking (true);

				return status;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_TCPSocket::Disconnect ()
		{
			// Simply close the socket
			Close ();
		}

		//------------------------------------------------------------------------------------------------------------------------
		
		C_Socket::ESocketStatus C_TCPSocket::Send (const char* data, std::size_t size)
		{
			// Check the parameters
			if (!data || (size == 0))
			{
				REDEEMER_LOG << LOG_ERROR << L"NETWORK: Cannot send data over the network (no data to send)" << LOG_ENDMESSAGE;

				return SS_Error;
			}

			// Loop until every byte has been sent
			int sent = 0;
			int sizeToSend = static_cast<int>(size);

			for (int length = 0; length < sizeToSend; length += sent)
			{
				// Send a chunk of data
				sent = send (GetHandle(), data + length, sizeToSend - length, 0);

				// Check for errors
				if (sent < 0)
					return GetErrorStatus();
			}

			return SS_Done;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Socket::ESocketStatus C_TCPSocket::Receive (char* data, std::size_t size, std::size_t& received)
		{
			// First clear the variables to fill
			received = 0;

			// Check the destination buffer
			if (!data)
			{
				REDEEMER_LOG << LOG_ERROR << L"NETWORK: Cannot receive data from the network (the destination buffer is invalid)" << LOG_ENDMESSAGE;

				return SS_Error;
			}

			// Receive a chunk of bytes
			int sizeReceived = recv (GetHandle(), data, static_cast<int>(size), 0);

			// Check the number of bytes received
			if (sizeReceived > 0)
			{
				received = static_cast<std::size_t>(sizeReceived);

				return SS_Done;
			}
			else if (sizeReceived == 0)
			{
				return C_Socket::SS_Disconnected;
			}
			else
			{
				return GetErrorStatus();
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Socket::ESocketStatus C_TCPSocket::Send (C_Packet& packet)
		{
			// TCP is a stream protocol, it doesn't preserve messages boundaries.
			// This means that we have to send the packet size first, so that the
			// receiver knows the actual end of the packet in the data stream.

			// Get the data to send from the packet
			std::size_t size = 0;
			const char* data = packet.OnSend(size);

			// First send the packet size
			unsigned int packetSize = htonl(static_cast<unsigned long>(size));
			ESocketStatus status = Send (reinterpret_cast<const char*>(&packetSize), sizeof(packetSize));

			// Make sure that the size was properly sent
			if (status != SS_Done)
				return status;

			// Send the packet data
			if (packetSize > 0)
			{
				return Send (data, size);
			}
			else
			{
				return SS_Done;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Socket::ESocketStatus C_TCPSocket::Receive (C_Packet& packet)
		{
			// First clear the variables to fill
			packet.Clear();

			// We start by getting the size of the incoming packet
			unsigned int packetSize = 0;
			std::size_t received = 0;

			if (m_PendingPacket.SizeReceived < sizeof(m_PendingPacket.Size))
			{
				// Loop until we've received the entire size of the packet
				// (even a 4 bytes variable may be received in more than one call)
				while (m_PendingPacket.SizeReceived < sizeof(m_PendingPacket.Size))
				{
					char* data = reinterpret_cast<char*>(&m_PendingPacket.Size) + m_PendingPacket.SizeReceived;
					ESocketStatus status = Receive(data, sizeof(m_PendingPacket.Size) - m_PendingPacket.SizeReceived, received);

					m_PendingPacket.SizeReceived += received;

					if (status != SS_Done)
						return status;
				}

				// The packet size has been fully received
				packetSize = ntohl(m_PendingPacket.Size);
			}
			else
			{
				// The packet size has already been received in a previous call
				packetSize = ntohl(m_PendingPacket.Size);
			}

			// Loop until we receive all the packet data
			char buffer[1024];

			while (m_PendingPacket.Data.size() < packetSize)
			{
				// Receive a chunk of data
				std::size_t sizeToGet = min(static_cast<std::size_t>(packetSize - m_PendingPacket.Data.size()), sizeof(buffer));
				ESocketStatus status = Receive (buffer, sizeToGet, received);

				if (status != SS_Done)
					return status;

				// Append it into the packet
				if (received > 0)
				{
					m_PendingPacket.Data.resize(m_PendingPacket.Data.size() + received);

					char* begin = &m_PendingPacket.Data[0] + m_PendingPacket.Data.size() - received;

					memcpy(begin, buffer, received);
				}
			}

			// We have received all the packet data: we can copy it to the user packet
			if (!m_PendingPacket.Data.empty())
				packet.OnReceive(&m_PendingPacket.Data[0], m_PendingPacket.Data.size());

			// Clear the pending packet data
			m_PendingPacket = S_PendingPacket();

			return SS_Done;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace NETWORK
}	//	namespace REDEEMER

