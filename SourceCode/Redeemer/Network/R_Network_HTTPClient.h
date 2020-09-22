/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Network_HTTPClient.h
**		Simple HTTP client implementation
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_NETWORK_HTTPCLIENT_H_
#define _R_NETWORK_HTTPCLIENT_H_

#include "R_Network_TCPSocket.h"
#include "R_Network_IPAddress.h"

#include <map>

namespace REDEEMER
{
	namespace NETWORK
	{
		/*!
		**	\brief Simple HTTP client implementation
		*/
		class C_HTTPClient
		{
		public:
			/*!	\brief Define a HTTP request
			*/
			class C_HTTPRequest
			{
			public:
				/*!	Enumerate the available HTTP methods for a request
				*/
				enum ERequestMethod
				{
					RM_Get,  ///< Request in get mode, standard method to retrieve a page
					RM_Post, ///< Request in post mode, usually to send data to a page
					RM_Head  ///< Request a page's header only
				};

				/*!	Default constructor
				**	This constructor creates a GET request, with the root URI ("/") and an empty body.
				**
				**	\param uri    Target URI
				**	\param method Method to use for the request
				**	\param body   Content of the request's body
				*/
				C_HTTPRequest (const std::string& uri = "/", ERequestMethod method = RM_Get, const std::string& body = "");

				/*!	Set the value of a field
				**	The field is created if it doesn't exist. The name of the field is case insensitive.
				**	By default, a request doesn't contain any field (but the mandatory fields are added later by the HTTP client when sending the request).
				**
				**	\param field Name of the field to set
				**	\param value Value of the field
				*/
				void SetField (const std::string& field, const std::string& value);

				/*!	Set the request method
				**
				**	\param method Method to use for the request
				*/
				void SetMethod (ERequestMethod method);

				/*!	Set the requested URI
				**
				**	\param uri URI to request, relative to the host
				*/
				void SetUri (const std::string& uri);

				/*!	Set the HTTP version fo the request
				**	The HTTP version is 1.0 by default.
				**
				**	\param major Major HTTP version number
				**	\param minor Minor HTTP version number
				*/
				void SetHttpVersion (unsigned int major, unsigned int minor);

				/*!	Set the body of the request
				**	The body of a request is optional and only makes sense for POST requests. It is ignored for all other methods.
				**
				**	\param body Content of the body
				*/
				void SetBody (const std::string& body);

			private:
				friend class C_HTTPClient;

				/*!	Prepare the final request to send to the server
				**
				**	\return String containing the request, ready to be sent
				*/
				std::string Prepare () const;

				/*!	Check if the request defines a field
				**
				**	\param field Name of the field to test
				*/
				bool HasField (const std::string& field) const;

				/*!	Type defining field entry in HTTP request
				*/
				typedef std::map<std::string, std::string> FieldTable;

				FieldTable			m_Fields;       ///< Fields of the header associated to their value
				ERequestMethod      m_Method;       ///< Method to use for the request
				std::string			m_URI;          ///< Target URI of the request
				unsigned int		m_MajorVersion; ///< Major HTTP version
				unsigned int		m_MinorVersion; ///< Minor HTTP version
				std::string			m_Body;         ///< Body of the request
			};

			/*!	\brief Define a HTTP response
			*/
			class C_HTTPResponse
			{
			public:
				/*!	Enumerate all the valid status codes for a response
				*/
				enum EResponseStatus
				{
					// 2xx: success
					RS_Ok             = 200, ///< Most common code returned when operation was successful
					RS_Created        = 201, ///< The resource has successfully been created
					RS_Accepted       = 202, ///< The request has been accepted, but will be processed later by the server
					RS_NoContent      = 204, ///< The server didn't send any data in return
					RS_ResetContent   = 205, ///< The server informs the client that it should clear the view (form) that caused the request to be sent
					RS_PartialContent = 206, ///< The server has sent a part of the resource, as a response to a partial GET request

					// 3xx: redirection
					RS_MultipleChoices  = 300, ///< The requested page can be accessed from several locations
					RS_MovedPermanently = 301, ///< The requested page has permanently moved to a new location
					RS_MovedTemporarily = 302, ///< The requested page has temporarily moved to a new location
					RS_NotModified      = 304, ///< For conditionnal requests, means the requested page hasn't changed and doesn't need to be refreshed

					// 4xx: client error
					RS_BadRequest          = 400, ///< The server couldn't understand the request (syntax error)
					RS_Unauthorized        = 401, ///< The requested page needs an authentification to be accessed
					RS_Forbidden           = 403, ///< The requested page cannot be accessed at all, even with authentification
					RS_NotFound            = 404, ///< The requested page doesn't exist
					RS_RangeNotSatisfiable = 407, ///< The server can't satisfy the partial GET request (with a "Range" header field)

					// 5xx: server error
					RS_InternalServerError = 500, ///< The server encountered an unexpected error
					RS_NotImplemented      = 501, ///< The server doesn't implement a requested feature
					RS_BadGateway          = 502, ///< The gateway server has received an error from the source server
					RS_ServiceNotAvailable = 503, ///< The server is temporarily unavailable (overloaded, in maintenance, ...)
					RS_GatewayTimeout      = 504, ///< The gateway server couldn't receive a response from the source server
					RS_VersionNotSupported = 505, ///< The server doesn't support the requested HTTP version

					// 10xx: SFML custom codes
					RS_InvalidResponse  = 1000, ///< Response is not a valid HTTP one
					RS_ConnectionFailed = 1001  ///< Connection with server failed
				};

				/*!	Default constructor
				*/
				C_HTTPResponse ();

				/*!	Get the value of a field
				**	If the field \a field is not found in the response header, the empty string is returned. This function uses case-insensitive comparisons.
				**
				**	\param field Name of the field to get
				**	\return Value of the field, or empty string if not found
				*/
				const std::string& GetField (const std::string& field) const;

				/*!	Get the response status code
				**	The status code should be the first thing to be checked after receiving a response, it defines whether it is a
				**	success, a failure or anything else (see the EResponseStatus enumeration).
				*/
				EResponseStatus GetStatus () const;

				/*!	Get the major HTTP version number of the response
				*/
				unsigned int GetMajorHttpVersion () const;

				/*!	Get the minor HTTP version number of the response
				*/
				unsigned int GetMinorHttpVersion () const;

				/*!	Get the body of the response
				*/
				const std::string& GetBody () const;

			private:
				friend class C_HTTPClient;

				/*!	Construct the header from a response string
				**
				**	\param data Content of the response to parse
				*/
				void Parse (const std::string& data);

				typedef std::map<std::string, std::string> FieldTable;

				FieldTable			m_Fields;       ///< Fields of the header
				EResponseStatus     m_Status;       ///< Status code
				unsigned int		m_MajorVersion; ///< Major HTTP version
				unsigned int		m_MinorVersion; ///< Minor HTTP version
				std::string			m_Body;         ///< Body of the response
			};

			/*!	Default constructor
			*/
			C_HTTPClient ();

			/*!	Construct the HTTP client with the target host
			**	This is equivalent to calling SetHost(host, port). The port has a default value of 0, which means that the
			**	HTTP client will use the right port according to the protocol used (80 for HTTP, 443 for HTTPS). You should
			**	leave it like this unless you really need a port other than the standard one, or use an unknown protocol.
			**
			**	\param host Web server to connect to
			**	\param port Port to use for connection
			*/
			C_HTTPClient (const std::string& host, unsigned short port = 0);

			/*!	Set the target host
			**	This function just stores the host address and port, it doesn't actually connect to it until you send a request.
			**	The port has a default value of 0, which means that the HTTP client will use the right port according to the
			**	protocol used (80 for HTTP, 443 for HTTPS). You should leave it like this unless you really need a port other
			**	than the standard one, or use an unknown protocol.
			**
			**	\param host Web server to connect to
			**	\param port Port to use for connection
			*/
			void SetHost (const std::string& host, unsigned short port = 0);

			/*!	Send a HTTP request and return the server's response.
			**	You must have a valid host before sending a request (see SetHost). Any missing mandatory header field in the request will be added
			**	with an appropriate value. Warning: this function waits for the server's response and may not return instantly; use a thread if you 
			**	don't want to block your application, or use a timeout to limit the time to wait. A value of 0 means that the client will use the system 
			**	defaut timeout (which is usually pretty long).
			**
			**	\param request Request to send
			**	\param timeout Maximum time to wait, in seconds
			*/
			C_HTTPResponse SendRequest (const C_HTTPRequest& request, float timeout = 0.f);

		private:
			C_TCPSocket     m_Connection; ///< Connection to the host
			C_IPAddress		m_Host;       ///< Web host address
			std::string		m_HostName;   ///< Web host name
			unsigned short	m_Port;       ///< Port used for connection with host
		};

	}	//	namespace NETWORK
}	//	namespace REDEEMER

#endif	//	_R_NETWORK_HTTPCLIENT_H_
