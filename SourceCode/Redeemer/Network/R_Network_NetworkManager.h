/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Network_NetworManager.h
**		Communication layer between low level sockets and engine
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_NETWORK_NETWORKMANAGER_H_
#define _R_NETWORK_NETWORKMANAGER_H_

#include "..\\Core\\R_Core_Singleton.h"
#include "..\\Core\\R_Core_BaseClass.h"

namespace REDEEMER
{
	namespace NETWORK
	{
		class C_HTTPClient;
		class C_FTPClient;
		class C_UDPSocket;
		class C_TCPSocket;
		class C_TCPListener;

		/*!
		**	\brief Communication layer between low level sockets and engine
		*/
		class C_NetworkManager : public CORE::C_Singleton<C_NetworkManager>, public CORE::C_BaseClass
		{
		public:
			/*!	Constructor
			*/
			C_NetworkManager ();

			/*!	Destructor
			*/
			virtual ~C_NetworkManager ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize ();

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Returns reference to TCP listener (server)
			*/
			C_TCPListener& GetTCPListener ();

			/*!	Returns reference to HTTP Client
			*/
			C_HTTPClient& GetHttpClient ();

			/*!	Returns reference to FTP Client
			*/
			C_FTPClient& GetFTPClient ();

			/*!	Creates new TCP type socket
			*/
			C_TCPSocket* CreateTCPSocket ();

			/*!	Creates new UDP type socket
			*/
			C_UDPSocket* CreateUDPSocket ();

		private:	
			C_HTTPClient*	m_HTTPClient;
			C_FTPClient*	m_FTPClient;
			C_TCPListener*	m_TCPListener;
		};

	}	//	namespace NETWORK
}	//	namespace REDEEMER

#endif	//	_R_NETWORK_NETWORKMANAGER_H_
