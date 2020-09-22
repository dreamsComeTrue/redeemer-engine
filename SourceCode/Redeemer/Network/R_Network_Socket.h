/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Network_Socket.h
**		Low level socket implementation for network system
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_NETWORK_SOCKET_H_
#define _R_NETWORK_SOCKET_H_

#include <BaseTsd.h>
#include <WinSock.h>
#include <vector>

namespace REDEEMER
{
	namespace NETWORK
	{
		class C_TCPListener;


		/*!
		**	\brief Low level socket implementation for network system
		*/
		class C_Socket
		{
			friend class C_TCPListener;

		public:
			/*!	Sockets may return different status codes
			*/
			enum ESocketStatus
			{
				SS_Done,         ///< The socket has sent / received the data
				SS_NotReady,     ///< The socket is not ready to send / receive data yet
				SS_Disconnected, ///< The TCP socket has been disconnected
				SS_Error         ///< An unexpected error happened
			};

			/*!	Valid socket ports
			*/
			enum ESocketPort
			{
				SP_AnyPort = 0 ///< Special value that tells the system to pick any available port
			};

			/*!	Destructor
			*/
			virtual ~C_Socket ();

			/*!	Create an internal sockaddr_in address
			**
			**	\param address Target address
			**	\param port    Target port
			**	
			**	\return sockaddr_in ready to be used by socket functions
			*/
			static sockaddr_in CreateAddress (unsigned long address, unsigned short port);

			/*!	Sets the socket to a blocking mode. By default, all sockets are blocking.
			**	\param isBlocking True to set the socket as blocking, false for non-blocking
			*/
			void SetBlocking (bool isBlocking);

			/*!	Checks, if socket is in blocking state
			*/
			bool IsBlocking () const;

			/*!	Returns INVALID socket
			*/
			static UINT_PTR GetInvalidSocket ();

		protected:
			/*!	Sockets can be of those types: TCP or UDP
			*/
			enum ESocketType
			{
				ST_Tcp, ///< TCP protocol
				ST_Udp  ///< UDP protocol
			};

			/*!	Structure holding the data of a pending packet
			*/
			struct S_PendingPacket
			{
				/*!	Constructor
				*/
				S_PendingPacket ();

				unsigned int      Size;         ///< Data of packet size
				std::size_t       SizeReceived; ///< Number of size bytes received so far
				std::vector<char> Data;         ///< Data of the packet
			};

			/*!	Default constructor
			*/
			C_Socket (ESocketType type);

			/*!	Return the internal handle of the socket
			*/
			UINT_PTR GetHandle () const;

			/*!	Create the internal representation of the socket
			*/
			void Create ();

			/*!	Create the internal representation of the socket from a socket handle
			*/
			void Create (UINT_PTR handle);

			/*!	Close the socket gracefully
			*/
			void Close ();

			/*!	Get the last socket error status
			*/
			static ESocketStatus GetErrorStatus ();

		protected:
			S_PendingPacket m_PendingPacket;	///< Temporary data of the packet currently being received

		private:
			ESocketType     m_Type;				///< Type of the socket (TCP or UDP)
			UINT_PTR		m_Socket;			///< Socket descriptor
			bool			m_IsBlocking;		///< Current blocking mode of the socket
		};

	}	//	namespace NETWORK
}	//	namespace REDEEMER

#endif	//	_R_NETWORK_SOCKET_H_
