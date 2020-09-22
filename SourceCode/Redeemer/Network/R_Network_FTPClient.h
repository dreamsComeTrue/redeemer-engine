/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Network_FTPClient.h
**		Simple FTP client implementation
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_NETWORK_FTPCLIENT_H_
#define _R_NETWORK_FTPCLIENT_H_

#include "R_Network_TCPSocket.h"
#include "R_Network_IPAddress.h"

#include <map>

namespace REDEEMER
{
	namespace NETWORK
	{
		/*!
		**	\brief Simple FTP client implementation
		*/
		class C_FTPClient
		{
		public:
			/*!	Enumeration of transfer modes
			*/
			enum ETransferMode
			{
				TM_Binary, ///< Binary mode (file is transfered as a sequence of bytes)
				TM_Ascii,  ///< Text mode using ASCII encoding
				TM_Ebcdic  ///< Text mode using EBCDIC encoding
			};

			/*!	\brief Define a FTP response
			*/
			class C_Response
			{
			public:
				/*!	Status codes possibly returned by a FTP response
				*/
				enum EResponseStatus
				{
					// 1xx: the requested action is being initiated,
					// expect another reply before proceeding with a new command
					RS_RestartMarkerReply          = 110, ///< Restart marker reply
					RS_ServiceReadySoon            = 120, ///< Service ready in N minutes
					RS_DataConnectionAlreadyOpened = 125, ///< Data connection already opened, transfer starting
					RS_OpeningDataConnection       = 150, ///< File status ok, about to open data connection

					// 2xx: the requested action has been successfully completed
					RS_Ok                    = 200, ///< Command ok
					RS_PointlessCommand      = 202, ///< Command not implemented
					RS_SystemStatus          = 211, ///< System status, or system help reply
					RS_DirectoryStatus       = 212, ///< Directory status
					RS_FileStatus            = 213, ///< File status
					RS_HelpMessage           = 214, ///< Help message
					RS_SystemType            = 215, ///< NAME system type, where NAME is an official system name from the list in the Assigned Numbers document
					RS_ServiceReady          = 220, ///< Service ready for new user
					RS_ClosingConnection     = 221, ///< Service closing control connection
					RS_DataConnectionOpened  = 225, ///< Data connection open, no transfer in progress
					RS_ClosingDataConnection = 226, ///< Closing data connection, requested file action successful
					RS_EnteringPassiveMode   = 227, ///< Entering passive mode
					RS_LoggedIn              = 230, ///< User logged in, proceed. Logged out if appropriate
					RS_FileActionOk          = 250, ///< Requested file action ok
					RS_DirectoryOk           = 257, ///< PATHNAME created

					// 3xx: the command has been accepted, but the requested action
					// is dormant, pending receipt of further information
					RS_NeedPassword       = 331, ///< User name ok, need password
					RS_NeedAccountToLogIn = 332, ///< Need account for login
					RS_NeedInformation    = 350, ///< Requested file action pending further information

					// 4xx: the command was not accepted and the requested action did not take place,
					// but the error condition is temporary and the action may be requested again
					RS_ServiceUnavailable        = 421, ///< Service not available, closing control connection
					RS_DataConnectionUnavailable = 425, ///< Can't open data connection
					RS_TransferAborted           = 426, ///< Connection closed, transfer aborted
					RS_FileActionAborted         = 450, ///< Requested file action not taken
					RS_LocalError                = 451, ///< Requested action aborted, local error in processing
					RS_InsufficientStorageSpace  = 452, ///< Requested action not taken; insufficient storage space in system, file unavailable

					// 5xx: the command was not accepted and
					// the requested action did not take place
					RS_CommandUnknown          = 500, ///< Syntax error, command unrecognized
					RS_ParametersUnknown       = 501, ///< Syntax error in parameters or arguments
					RS_CommandNotImplemented   = 502, ///< Command not implemented
					RS_BadCommandSequence      = 503, ///< Bad sequence of commands
					RS_ParameterNotImplemented = 504, ///< Command not implemented for that parameter
					RS_NotLoggedIn             = 530, ///< Not logged in
					RS_NeedAccountToStore      = 532, ///< Need account for storing files
					RS_FileUnavailable         = 550, ///< Requested action not taken, file unavailable
					RS_PageTypeUnknown         = 551, ///< Requested action aborted, page type unknown
					RS_NotEnoughMemory         = 552, ///< Requested file action aborted, exceeded storage allocation
					RS_FilenameNotAllowed      = 553, ///< Requested action not taken, file name not allowed

					// 10xx: SFML custom codes
					RS_InvalidResponse  = 1000, ///< Response is not a valid FTP one
					RS_ConnectionFailed = 1001, ///< Connection with server failed
					RS_ConnectionClosed = 1002, ///< Connection with server closed
					RS_InvalidFile      = 1003  ///< Invalid file to upload / download
				};

				/*!	Default constructor
				**
				**	\param code    Response status code
				**	\param message Response message
				*/
				C_Response (EResponseStatus code = RS_InvalidResponse, const std::string& message = "");

				/*!	Check if the status code means a success
				**	This function is defined for convenience, it is equivalent to testing if the status code is < 400.
				*/
				bool IsOk () const;

				/*!	Get the status code of the response
				*/
				EResponseStatus GetStatus () const;

				/*!	Get the full message contained in the response
				*/
				const std::string& GetMessage () const;

			private:
				EResponseStatus	m_Status;  ///< Status code returned from the server
				std::string		m_Message; ///< Last message received from the server
			};

			/*!	\brief Specialization of FTP response returning a directory
			*/
			class C_DirectoryResponse : public C_Response
			{
			public:
				/*!	Default constructor
				**
				**	\param response Source response
				*/
				C_DirectoryResponse (const C_Response& response);

				/*!	Get the directory returned in the response
				*/
				const std::string& GetDirectory () const;

			private:
				std::string m_Directory; ///< Directory extracted from the response message
			};


			/*!	\brief Specialization of FTP response returning a filename listing
			*/
			class C_ListingResponse : public C_Response
			{
			public:
				/*!	Default constructor
				**
				**	\param response  Source response
				**	\param data      Data containing the raw listing
				*/
				C_ListingResponse (const C_Response& response, const std::vector<char>& data);

				/*!	Return the array of filenames
				*/
				const std::vector<std::string>& GetFileNames () const;

			private:
				std::vector<std::string> m_Filenames; ///< Filenames extracted from the data
			};
					   
			/*!	Destructor
			**	Automatically closes the connection with the server if it is still opened.
			*/
			~C_FTPClient ();

			/*!	Connect to the specified FTP server
			**	The port has a default value of 21, which is the standard port used by the FTP protocol. You shouldn't use a different
			**	value, unless you really know what you do. This function tries to connect to the server so it may take a while to complete, 
			**	especially if the server is not reachable. To avoid blocking your application for too long, you can use a timeout. 
			**	The default value, 0, means that the system timeout will be used (which is usually pretty long).
			**
			**	\param server  Name or address of the FTP server to connect to
			**	\param port    Port used for the connection
			**	\param timeout Maximum time to wait, in seconds
			*/
			C_Response Connect (const C_IPAddress& server, unsigned short port = 21, float timeout = 0.f);

			/*!	Close the connection with the server
			*/
			C_Response Disconnect ();

			/*!	Log in using an anonymous account
			**	Logging in is mandatory after connecting to the server. Users that are not logged in cannot perform any operation.
			*/
			C_Response Login ();

			/*!	Log in using a username and a password
			**
			**	\param name     User name
			**	\param password Password
			*/
			C_Response Login (const std::string& name, const std::string& password);

			/*!	Send a null command to keep the connection alive
			**	This command is useful because the server may close the connection automatically if no command is sent.
			*/
			C_Response KeepAlive ();

			/*!	Get the current working directory
			**	The working directory is the root path for subsequent operations involving directories and/or filenames.
			*/
			C_DirectoryResponse GetWorkingDirectory ();

			/*!	Get the contents of the given directory
			**	This function retrieves the sub-directories and files contained in the given directory. It is not recursive.
			**	The \a directory parameter is relative to the current working directory.
			**
			**	\param directory Directory to list
			*/
			C_ListingResponse GetDirectoryListing (const std::string& directory = "");

			/*!	Change the current working directory
			**	The new directory must be relative to the current one.
			**
			**	\param directory New working directory
			*/
			C_Response ChangeDirectory (const std::string& directory);

			/*!	Go to the parent directory of the current one
			*/
			C_Response ParentDirectory ();

			/*!	Create a new directory
			**	The new directory is created as a child of the current working directory.
			**
			**	\param name Name of the directory to create
			*/
			C_Response CreateDirectory (const std::string& name);

			/*!	Remove an existing directory
			**	The directory to remove must be relative to the current working directory.
			**
			**	\param name Name of the directory to remove
			*/
			C_Response DeleteDirectory (const std::string& name);

			/*!	Rename an existing file
			**	The filenames must be relative to the current working directory.
			**
			**	\param file    File to rename
			**	\param newName New name of the file
			*/
			C_Response RenameFile (const std::string& file, const std::string& newName);

			/*!	Remove an existing file
			**	The file name must be relative to the current working directory.
			**
			**	\param name File to remove
			*/
			C_Response DeleteFile (const std::string& name);

			/*!	Download a file from the server
			**	The filename of the distant file is relative to the current working directory of the server, and the local destination path is 
			**	relative to the current directory of your application.
			**	
			**	\param remoteFile Filename of the distant file to download
			**	\param localPath  Where to put to file on the local computer
			**	\param mode       Transfer mode
			*/
			C_Response Download (const std::string& remoteFile, const std::string& localPath, ETransferMode mode = TM_Binary);

			/*!	Upload a file to the server
			**	The name of the local file is relative to the current working directory of your application, and the
			**	remote path is relative to the current directory of the FTP server.
			**
			**	\param localFile  Path of the local file to upload
			**	\param remotePath Where to put to file on the server
			**	\param mode       Transfer mode
			*/
			C_Response Upload (const std::string& localFile, const std::string& remotePath, ETransferMode mode = TM_Binary);

		private:
			/*!	Send a command to the FTP server
			**
			**	\param command   Command to send
			**	\param parameter Command parameter
			*/
			C_Response SendCommand (const std::string& command, const std::string& parameter = "");

			/*!	Receive a response from the server
			**	This function must be called after each call to SendCommand that expects a response.
			*/
			C_Response GetResponse ();

			/*!	\brief Utility class for exchanging datas with the server on the data channel
			*/
			class C_FTPDataChannel;

			friend class C_FTPDataChannel;

		private:
			C_TCPSocket m_CommandSocket; ///< Socket holding the control connection with the server
		};

	}	//	namespace NETWORK
}	//	namespace REDEEMER

#endif	//	_R_NETWORK_FTPCLIENT_H_
