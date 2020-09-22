/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Network_UDPSocket.h
**		Specialized socket using the UDP protocol
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_NETWORK_UDPSOCKET_H_
#define _R_NETWORK_UDPSOCKET_H_

#include "R_Network_Socket.h"

namespace REDEEMER
{
	namespace NETWORK
	{
		class C_IPAddress;
		class C_Packet;


		/*!
		**	\brief Specialized socket using the UDP protocol
		*/
		class C_UDPSocket : public C_Socket
		{
		public:
			enum
			{
				MaxDatagramSize = 65507 ///< The maximum number of bytes that can be sent in a single UDP datagram
			};

			/*!	Constructor
			*/
			C_UDPSocket();

			/*!	Get the port to which the socket is bound locally
			**	If the socket is not connected, this function returns 0.
			*/
			unsigned short GetLocalPort () const;

			/*!	Bind the socket to a specific port
			**	Binding the socket to a port is necessary for being able to receive data on that port.
			**	You can use the special value C_Socket::AnyPort to tell the system to automatically pick an available port, and then
			**	call GetLocalPort to retrieve the chosen port.
			**
			**	\param port Port to bind the socket to
			*/
			ESocketStatus Bind (unsigned short port);

			/*!	Unbind the socket from the local port to which it is bound
			**	The port that the socket was previously using is immediately available after this function is called. If the
			**	socket is not bound to a port, this function has no effect.
			*/
			void Unbind ();

			/*!	Send raw data to the remote peer
			**	Make sure that \a size is not greater than C_UDPSocket::MaxDatagramSize, otherwise this function will fail and no data will be sent.
			**
			**	\param data          Pointer to the sequence of bytes to send
			**	\param size          Number of bytes to send
			**	\param remoteAddress Address of the receiver
			**	\param remotePort    Port of the receiver to send the data to
			*/
			ESocketStatus Send (const char* data, std::size_t size, const C_IPAddress& remoteAddress, unsigned short remotePort);

			/*!	Receive raw data from a remote peer
			**	In blocking mode, this function will wait until some bytes are actually received.
			**	Be careful to use a buffer which is large enough for the data that you intend to receive, if it is too small then an error will be 
			**	returned and *all* the data will be lost.
			**
			**	\param data          Pointer to the array to fill with the received bytes
			**	\param size          Maximum number of bytes that can be received
			**	\param received      This variable is filled with the actual number of bytes received
			**	\param remoteAddress Address of the peer that sent the data
			**	\param remotePort    Port of the peer that sent the data
			*/
			ESocketStatus Receive (char* data, std::size_t size, std::size_t& received, C_IPAddress& remoteAddress, unsigned short& remotePort);

			/*!	Send a formatted packet of data to a remote peer
			**	Unlike the other version of Send, this function can accept data sizes greater than C_UDPSocket::MaxDatagramSize.
			**	If necessary, the data will be split and sent in multiple datagrams.
			**
			**	\param packet        Packet to send
			**	\param remoteAddress Address of the receiver
			**	\param remotePort    Port of the receiver to send the data to
			*/
			ESocketStatus Send (C_Packet& packet, const C_IPAddress& remoteAddress, unsigned short remotePort);

			/*!	Receive a formatted packet of data from a remote peer
			**	In blocking mode, this function will wait until the whole packet has been received.
			**	Warning: this function doesn't properly handle mixed data received from multiple peers.
			**
			**	\param packet        Packet to fill with the received data
			**	\param remoteAddress Address of the peer that sent the data
			**	\param remotePort    Port of the peer that sent the data
			*/
			ESocketStatus Receive (C_Packet& packet, C_IPAddress& remoteAddress, unsigned short& remotePort);
		};

	}	//	namespace NETWORK
}	//	namespace REDEEMER

#endif	//	_R_NETWORK_UDPSOCKET_H_
