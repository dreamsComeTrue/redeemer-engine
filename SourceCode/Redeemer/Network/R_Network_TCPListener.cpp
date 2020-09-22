/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Network_TCPListener.cpp
**		Socket that listens to new TCP connections
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Network_TCPListener.h"
#include "R_Network_TCPSocket.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace NETWORK
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_TCPListener::C_TCPListener () :
			C_Socket (ST_Tcp)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned short C_TCPListener::GetLocalPort () const
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

		C_Socket::ESocketStatus C_TCPListener::Listen (unsigned short port)
		{
			// Create the internal socket if it doesn't exist
			Create ();

			// Bind the socket to the specified port
			sockaddr_in address = CreateAddress(INADDR_ANY, port);

			if (bind (GetHandle(), reinterpret_cast<sockaddr*>(&address), sizeof(address)) == -1)
			{
				// Not likely to happen, but...
				REDEEMER_LOG << LOG_ERROR << "Network: Failed to bind listener socket to port: " << port << LOG_ENDMESSAGE;

				return SS_Error;
			}

			// Listen to the bound port
			if (listen (GetHandle(), 0) == -1)
			{
				// Oops, socket is deaf
				REDEEMER_LOG << LOG_ERROR << "Network: Failed to listen to port: " << port << LOG_ENDMESSAGE;

				return SS_Error;
			}

			return SS_Done;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Socket::ESocketStatus C_TCPListener::Accept (C_TCPSocket& socket)
		{
			// Make sure that we're listening
			if (GetHandle() == GetInvalidSocket())
			{
				REDEEMER_LOG << LOG_ERROR << "Network: Failed to accept a new connection, the socket is not listening" << LOG_ENDMESSAGE;

				return SS_Error;
			}

			// Accept a new connection
			sockaddr_in address;
			int length = sizeof(address);

			INT_PTR remote = accept (GetHandle(), reinterpret_cast<sockaddr*>(&address), &length);

			// Check for errors
			if (GetInvalidSocket())
				return GetErrorStatus();

			// Initialize the new connected socket
			socket.Close();
			socket.Create(remote);

			return SS_Done;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace NETWORK
}	//	namespace REDEEMER

