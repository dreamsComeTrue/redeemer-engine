/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Network_TCPSocket.h
**		Specialized socket using the TCP protocol
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_NETWORK_TCPSOCKET_H_
#define _R_NETWORK_TCPSOCKET_H_

#include "R_Network_Socket.h"

namespace REDEEMER
{
	namespace NETWORK
	{
		class C_IPAddress;
		class C_Packet;


		/*!
		**	\brief Specialized socket using the TCP protocol
		*/
		class C_TCPSocket : public C_Socket
		{
		public:
			/*!	Constructor
			*/
			C_TCPSocket();

			/*!	Get the port to which the socket is bound locally
			**	If the socket is not connected, this function returns 0.
			*/
			unsigned short GetLocalPort () const;

			/*!	Get the address of the connected peer
			**	If the socket is not connected, this function returns C_IPAddress::None.
			*/
			C_IPAddress GetRemoteAddress () const;

			/*!	Get the port of the connected peer to which the socket is connected
			**	If the socket is not connected, this function returns 0.
			*/
			unsigned short GetRemotePort () const;

			/*!	Connect the socket to a remote peer
			**	In blocking mode, this function may take a while, especially if the remote peer is not reachable. The last parameter allows
			**	you to stop trying to connect after a given timeout. If the socket was previously connected, it is first disconnected.
			**	\param remoteAddress Address of the remote peer
			**	\param remotePort    Port of the remote peer
			**	\param timeout       Optional maximum time to wait, in seconds
			*/
			ESocketStatus Connect (const C_IPAddress& remoteAddress, unsigned short remotePort, float timeout = 0.f);

			/*!	Disconnect the connect from its remote peer
			**	This function gracefully closes the connection. If the socket is not connected, this function has no effect.
			*/
			void Disconnect ();

			/*!	Send raw data to the remote peer
			**	This function will fail if the socket is not connected.
			**	\param data Pointer to the sequence of bytes to send
			**	\param size Number of bytes to send
			*/
			ESocketStatus Send (const char* data, std::size_t size);

			/*!	Receive raw data from the remote peer
			**	In blocking mode, this function will wait until some bytes are actually received.
			**	This function will fail if the socket is not connected.
			**
			**	\param data     Pointer to the array to fill with the received bytes
			**	\param size     Maximum number of bytes that can be received
			**	\param received This variable is filled with the actual number of bytes received
			*/
			ESocketStatus Receive (char* data, std::size_t size, std::size_t& received);

			/*!	Send a formatted packet of data to the remote peer
			**	
			**	\param packet Packet to send
			*/
			ESocketStatus Send (C_Packet& packet);

			/*!	Receive a formatted packet of data from the remote peer
			**	In blocking mode, this function will wait until the whole packet has been received.
			**
			**	\param packet Packet to fill with the received data
			*/
			ESocketStatus Receive (C_Packet& packet);
		};

	}	//	namespace NETWORK
}	//	namespace REDEEMER

#endif	//	_R_NETWORK_TCPSOCKET_H_
