/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Network_HTTPClient.cpp
**		Simple HTTP client implementation
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Network_HTTPClient.h"

#include <ctype.h>
#include <algorithm>
#include <iterator>
#include <sstream>

namespace REDEEMER
{
	namespace NETWORK
	{
		namespace
		{
			// Convert a string to lower case
			std::string ToLower(std::string str)
			{
				for (std::string::iterator i = str.begin(); i != str.end(); ++i)
					*i = static_cast<char>(tolower(*i));
				return str;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------
		C_HTTPClient::C_HTTPRequest::C_HTTPRequest (const std::string& uri, ERequestMethod method, const std::string& body)
		{
			SetMethod (method);
			SetUri (uri);
			SetHttpVersion (1, 0);
			SetBody (body);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_HTTPClient::C_HTTPRequest::SetField (const std::string& field, const std::string& value)
		{
			m_Fields[ToLower(field)] = value;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_HTTPClient::C_HTTPRequest::SetMethod (ERequestMethod method)
		{
			m_Method = method;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_HTTPClient::C_HTTPRequest::SetUri (const std::string& uri)
		{
			m_URI = uri;

			// Make sure it starts with a '/'
			if (m_URI.empty() || (m_URI[0] != '/'))
				m_URI.insert(0, "/");
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_HTTPClient::C_HTTPRequest::SetHttpVersion (unsigned int major, unsigned int minor)
		{
			m_MajorVersion = major;
			m_MinorVersion = minor;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_HTTPClient::C_HTTPRequest::SetBody (const std::string& body)
		{
			m_Body = body;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::string C_HTTPClient::C_HTTPRequest::Prepare () const
		{
			std::ostringstream out;

			// Convert the method to its string representation
			std::string method;

			switch (m_Method)
			{
			default :
			case RM_Get :  method = "GET";  break;
			case RM_Post : method = "POST"; break;
			case RM_Head : method = "HEAD"; break;
			}

			// Write the first line containing the request type
			out << method << " " << m_URI << " ";
			out << "HTTP/" << m_MajorVersion << "." << m_MinorVersion << "\r\n";

			// Write fields
			for (FieldTable::const_iterator i = m_Fields.begin(); i != m_Fields.end(); ++i)
			{
				out << i->first << ": " << i->second << "\r\n";
			}

			// Use an extra \r\n to separate the header from the body
			out << "\r\n";

			// Add the body
			out << m_Body;

			return out.str();
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_HTTPClient::C_HTTPRequest::HasField (const std::string& field) const
		{
			 return m_Fields.find(field) != m_Fields.end();
		}		   
		
		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_HTTPClient::C_HTTPResponse::C_HTTPResponse () :
			m_Status (RS_ConnectionFailed),
			m_MajorVersion(0),
			m_MinorVersion(0)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		const std::string& C_HTTPClient::C_HTTPResponse::GetField (const std::string& field) const
		{
			FieldTable::const_iterator it = m_Fields.find(ToLower(field));

			if (it != m_Fields.end())
			{
				return it->second;
			}
			else
			{
				static const std::string empty = "";
				return empty;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_HTTPClient::C_HTTPResponse::EResponseStatus C_HTTPClient::C_HTTPResponse::GetStatus () const
		{
			return m_Status;
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned int C_HTTPClient::C_HTTPResponse::GetMajorHttpVersion () const
		{
			return m_MajorVersion;
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned int C_HTTPClient::C_HTTPResponse::GetMinorHttpVersion () const
		{
			return m_MinorVersion;
		}

		//------------------------------------------------------------------------------------------------------------------------

		const std::string& C_HTTPClient::C_HTTPResponse::GetBody () const
		{
			return m_Body;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_HTTPClient::C_HTTPResponse::Parse (const std::string& data)
		{
			std::istringstream in(data);

			// Extract the HTTP version from the first line
			std::string version;

			if (in >> version)
			{
				if ((version.size() >= 8) && (version[6] == '.') && (ToLower(version.substr(0, 5)) == "http/") && isdigit(version[5]) && isdigit(version[7]))
				{
					m_MajorVersion = version[5] - '0';
					m_MinorVersion = version[7] - '0';
				}
				else
				{
					// Invalid HTTP version
					m_Status = RS_InvalidResponse;

					return;
				}
			}

			// Extract the status code from the first line
			int status;

			if (in >> status)
			{
				m_Status = static_cast<EResponseStatus>(status);
			}
			else
			{
				// Invalid status code
				m_Status = RS_InvalidResponse;
				return;
			}

			// Ignore the end of the first line
			in.ignore (10000, '\n');

			// Parse the other lines, which contain fields, one by one
			std::string line;

			while (std::getline(in, line) && (line.size() > 2))
			{
				std::string::size_type pos = line.find(": ");

				if (pos != std::string::npos)
				{
					// Extract the field name and its value
					std::string field = line.substr(0, pos);
					std::string value = line.substr(pos + 2);

					// Remove any trailing \r
					if (!value.empty() && (*value.rbegin() == '\r'))
						value.erase(value.size() - 1);

					// Add the field
					m_Fields[ToLower(field)] = value;
				}
			}

			// Finally extract the body
			m_Body.clear();

			std::copy(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>(), std::back_inserter(m_Body));
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_HTTPClient::C_HTTPClient () :
			m_Host(),
			m_Port(0)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_HTTPClient::C_HTTPClient (const std::string& host, unsigned short port)
		{
			SetHost (host, port);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_HTTPClient::SetHost (const std::string& host, unsigned short port)
		{
			// Detect the protocol used
			std::string protocol = ToLower(host.substr(0, 8));

			if (protocol.substr(0, 7) == "http://")
			{
				// HTTP protocol
				m_HostName = host.substr(7);
				m_Port     = (port != 0 ? port : 80);
			}
			else if (protocol == "https://")
			{
				// HTTPS protocol
				m_HostName = host.substr(8);
				m_Port     = (port != 0 ? port : 443);
			}
			else
			{
				// Undefined protocol - use HTTP
				m_HostName = host;
				m_Port     = (port != 0 ? port : 80);
			}

			// Remove any trailing '/' from the host name
			if (!m_HostName.empty() && (*m_HostName.rbegin() == '/'))
				m_HostName.erase(m_HostName.size() - 1);

			m_Host = C_IPAddress (m_HostName);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_HTTPClient::C_HTTPResponse C_HTTPClient::SendRequest (const C_HTTPRequest& request, float timeout)
		{
			// First make sure that the request is valid -- add missing mandatory fields
			C_HTTPRequest toSend (request);

			if (!toSend.HasField("From"))
			{
				toSend.SetField("From", "user@redeemer-engine.org");
			}
			if (!toSend.HasField("User-Agent"))
			{
				toSend.SetField("User-Agent", "redeemer-engine-network/2.x");
			}
			if (!toSend.HasField("Host"))
			{
				toSend.SetField("Host", m_HostName);
			}
			if (!toSend.HasField("Content-Length"))
			{
				std::ostringstream out;

				out << toSend.m_Body.size();
				toSend.SetField("Content-Length", out.str());
			}
			if ((toSend.m_Method == C_HTTPRequest::RM_Post) && !toSend.HasField("Content-Type"))
			{
				toSend.SetField("Content-Type", "application/x-www-form-urlencoded");
			}
			if ((toSend.m_MajorVersion * 10 + toSend.m_MinorVersion >= 11) && !toSend.HasField("Connection"))
			{
				toSend.SetField("Connection", "close");
			}

			// Prepare the response
			C_HTTPResponse received;

			// Connect the socket to the host
			if (m_Connection.Connect (m_Host, m_Port, timeout) == C_Socket::SS_Done)
			{
				// Convert the request to string and send it through the connected socket
				std::string requestStr = toSend.Prepare();

				if (!requestStr.empty())
				{
					// Send it through the socket
					if (m_Connection.Send(requestStr.c_str(), requestStr.size()) == C_Socket::SS_Done)
					{
						// Wait for the server's response
						std::string receivedStr;
						std::size_t size = 0;
						char buffer[1024];

						while (m_Connection.Receive(buffer, sizeof(buffer), size) == C_Socket::SS_Done)
						{
							receivedStr.append(buffer, buffer + size);
						}

						// Build the Response object from the received data
						received.Parse(receivedStr);
					}
				}

				// Close the connection
				m_Connection.Disconnect();
			}

			return received;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace NETWORK
}	//	namespace REDEEMER

