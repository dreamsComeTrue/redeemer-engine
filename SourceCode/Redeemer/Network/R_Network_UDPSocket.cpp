/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Network_UDPSocket.cpp
**		Specialized socket using the UDP protocol
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Network_UDPSocket.h"
#include "R_Network_IPAddress.h"
#include "R_Network_Packet.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace NETWORK
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_UDPSocket::C_UDPSocket() :
			C_Socket (ST_Udp)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned short C_UDPSocket::GetLocalPort () const
		{
			if (GetHandle() != GetInvalidSocket())
			{
				// Retrieve informations about the local end of the socket
				sockaddr_in address;

				int size = sizeof(address);

				if (getsockname (GetHandle(), reinterpret_cast<sockaddr*>(&address), &size) != -1)
				{
					return ntohs(address.sin_port);
				}
			}

			// We failed to retrieve the port
			return 0;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Socket::ESocketStatus C_UDPSocket::Bind (unsigned short port)
		{
			// Create the internal socket if it doesn't exist
			Create ();

			// Bind the socket
			sockaddr_in address = CreateAddress(INADDR_ANY, port);

			if (bind (GetHandle(), reinterpret_cast<sockaddr*>(&address), sizeof(address)) == -1)
			{
				REDEEMER_LOG << LOG_ERROR << "Network: Failed to bind socket to port: " << port << LOG_ENDMESSAGE;

				return SS_Error;
			}

			return SS_Done;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_UDPSocket::Unbind ()
		{
			// Simply close the socket
			Close ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Socket::ESocketStatus C_UDPSocket::Send (const char* data, std::size_t size, const C_IPAddress& remoteAddress, unsigned short remotePort)
		{
			// Create the internal socket if it doesn't exist
			Create ();

			// Make sure that all the data will fit in one datagram
			if (size > MaxDatagramSize)
			{
				REDEEMER_LOG << LOG_ERROR << "Network: Cannot send data over the network (the number of bytes to send is greater than" <<
					"C_UDPSocket::MaxDatagramSize)" << LOG_ENDMESSAGE;

				return SS_Error;
			}

			// Build the target address
			sockaddr_in address = CreateAddress(remoteAddress.ToInteger(), remotePort);

			// Send the data (unlike TCP, all the data is always sent in one call)
			int sent = sendto (GetHandle(), data, static_cast<int>(size), 0, reinterpret_cast<sockaddr*>(&address), sizeof(address));

			// Check for errors
			if (sent < 0)
				return GetErrorStatus();

			return SS_Done;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Socket::ESocketStatus C_UDPSocket::Receive (char* data, std::size_t size, std::size_t& received, C_IPAddress& remoteAddress, unsigned short& remotePort)
		{
			// First clear the variables to fill
			received      = 0;
			remoteAddress = C_IPAddress ();
			remotePort    = 0;

			// Check the destination buffer
			if (!data)
			{
				REDEEMER_LOG << LOG_ERROR << "Network: Cannot receive data from the network (the destination buffer is invalid)" << LOG_ENDMESSAGE;

				return SS_Error;
			}

			// Data that will be filled with the other computer's address
			sockaddr_in address = CreateAddress(INADDR_ANY, 0);

			// Receive a chunk of bytes
			int addressSize = sizeof(address);
			int sizeReceived = recvfrom (GetHandle(), data, static_cast<int>(size), 0, reinterpret_cast<sockaddr*>(&address), &addressSize);

			// Check for errors
			if (sizeReceived < 0)
				return GetErrorStatus();

			// Fill the sender informations
			received      = static_cast<std::size_t>(sizeReceived);
			remoteAddress = C_IPAddress(ntohl(address.sin_addr.s_addr));
			remotePort    = ntohs(address.sin_port);

			return SS_Done;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Socket::ESocketStatus C_UDPSocket::Send (C_Packet& packet, const C_IPAddress& remoteAddress, unsigned short remotePort)
		{
			// As the UDP protocol preserves datagrams boundaries, we don't have to
			// send the packet size first (it would even be a potential source of bug, if
			// that size arrives corrupted), but we must split the packet into multiple
			// pieces if data size is greater than the maximum datagram size.

			// Get the data to send from the packet
			std::size_t size = 0;
			const char* data = packet.OnSend(size);

			// If size is greater than MaxDatagramSize, the data must be split into multiple datagrams
			while (size >= MaxDatagramSize)
			{
				ESocketStatus status = Send(data, MaxDatagramSize, remoteAddress, remotePort);

				if (status != SS_Done)
					return status;

				data += MaxDatagramSize;
				size -= MaxDatagramSize;
			}

			// It is important to send a final datagram with a size < MaxDatagramSize,
			// even if it is zero, to mark the end of the packet
			return Send (data, size, remoteAddress, remotePort);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Socket::ESocketStatus C_UDPSocket::Receive (C_Packet& packet, C_IPAddress& remoteAddress, unsigned short& remotePort)
		{
			// First clear the variables to fill
			packet.Clear();

			remoteAddress = C_IPAddress();
			remotePort    = 0;

			// Receive datagrams
			std::size_t received = 0;

			do
			{
				// Make room in the data buffer for a new datagram
				std::size_t size = m_PendingPacket.Data.size();

				m_PendingPacket.Data.resize(size + MaxDatagramSize);

				char* data = &m_PendingPacket.Data[0] + size;

				// Receive the datagram
				ESocketStatus status = Receive (data, MaxDatagramSize, received, remoteAddress, remotePort);

				// Check for errors
				if (status != SS_Done)
					return status;
			}
			while (received == MaxDatagramSize);

			// We have received all the packet data: we can copy it to the user packet
			std::size_t actualSize = m_PendingPacket.Data.size() - MaxDatagramSize + received;

			if (actualSize > 0)
				packet.OnReceive(&m_PendingPacket.Data[0], actualSize);

			// Clear the pending packet data
			m_PendingPacket = S_PendingPacket();

			return SS_Done;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace NETWORK
}	//	namespace REDEEMER

