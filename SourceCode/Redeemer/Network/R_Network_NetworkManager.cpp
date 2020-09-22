/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Network_NetworManager.cpp
**		Communication layer between low level sockets and engine
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Network_NetworkManager.h"
#include "R_Network_HTTPClient.h"
#include "R_Network_FTPClient.h"
#include "R_Network_TCPSocket.h"
#include "R_Network_UDPSocket.h"
#include "R_Network_TCPListener.h"
#include "..\\RedeemerEngine.h"

#include <WinSock.h>

namespace REDEEMER
{
	namespace NETWORK
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_NetworkManager::C_NetworkManager ()  :
			m_HTTPClient (NULL),
			m_FTPClient (NULL),
			m_TCPListener (NULL)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_NetworkManager::~C_NetworkManager ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_NetworkManager::Initialize ()
		{
			C_BaseClass::Initialize();

			//	Initialize WinSock
			WSADATA init;
			WSAStartup(MAKEWORD(2, 2), &init);

			REDEEMER_LOG << LOG_INFO << L"Network Manager initialized!" << LOG_ENDMESSAGE;

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_NetworkManager::Finalize ()
		{
			//	Cleanup WinSock
			WSACleanup ();

			REDEEMER_SAFE_DELETE (m_HTTPClient);
			REDEEMER_SAFE_DELETE (m_FTPClient);
			REDEEMER_SAFE_DELETE (m_TCPListener);

			REDEEMER_LOG << LOG_INFO << L"Network Manager finalized!" << LOG_ENDMESSAGE;

			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_TCPListener& C_NetworkManager::GetTCPListener ()
		{
			if (!m_TCPListener)
				m_TCPListener = new C_TCPListener ();

			return *m_TCPListener;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_HTTPClient& C_NetworkManager::GetHttpClient ()
		{
			if (!m_HTTPClient)
				m_HTTPClient = new C_HTTPClient ();

			return *m_HTTPClient;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_FTPClient& C_NetworkManager::GetFTPClient ()
		{
			if (!m_FTPClient)
				m_FTPClient = new C_FTPClient ();

			return *m_FTPClient;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_TCPSocket* C_NetworkManager::CreateTCPSocket ()
		{
			return new C_TCPSocket ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_UDPSocket* C_NetworkManager::CreateUDPSocket ()
		{
			return new C_UDPSocket ();
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace NETWORK
}	//	namespace REDEEMER

