/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Network_TCPListener.h
**		Socket that listens to new TCP connections
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_NETWORK_TCPLISTENER_H_
#define _R_NETWORK_TCPLISTENER_H_

#include "R_Network_Socket.h"

namespace REDEEMER
{
	namespace NETWORK
	{
		class C_TCPSocket;


		/*!
		**	\brief Socket that listens to new TCP connections
		*/
		class C_TCPListener : public C_Socket
		{
		public:
			/*!	Constructor
			*/
			C_TCPListener();

			/*!	Get the port to which the socket is bound locally
			**	If the socket is not listening to a port, this function returns 0.
			*/
			unsigned short GetLocalPort () const;

			/*!	Start listening for connections
			**	This functions makes the socket listen to the specified port, waiting for new connections.
			**	If the socket was previously listening to another port, it will be stopped first and bound to the new port.
			**
			**	\param port Port to listen for new connections
			*/
			ESocketStatus Listen (unsigned short port);

			/*!	Accept a new connection
			**	If the socket is in blocking mode, this function will not return until a connection is actually received.
			**
			**	\param socket Socket that will hold the new connection
			*/
			ESocketStatus Accept (C_TCPSocket& socket);
		};

	}	//	namespace NETWORK
}	//	namespace REDEEMER

#endif	//	_R_NETWORK_TCPLISTENER_H_
