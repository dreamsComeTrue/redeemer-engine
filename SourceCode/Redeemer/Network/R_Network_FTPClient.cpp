/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Network_FTPClient.cpp
**		Simple FTP client implementation
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Network_FTPClient.h"

#include <algorithm>
#include <fstream>
#include <iterator>
#include <sstream>

namespace REDEEMER
{
	namespace NETWORK
	{
		//------------------------------------------------------------------------------------------------------------------------
		/*!	Data channel used as communication layer in FTP client
		*/
		class C_FTPClient::C_FTPDataChannel
		{
		public:
			/*!	Constructor
			*/
			C_FTPDataChannel (C_FTPClient& owner) :
				m_Ftp (owner)
			{
				//	EMPTY
			}

			/*!	Opens data channel
			*/
			C_FTPClient::C_Response Open (C_FTPClient::ETransferMode mode)
			{
				// Open a data connection in active mode (we connect to the server)
				C_FTPClient::C_Response response = m_Ftp.SendCommand ("PASV");

				if (response.IsOk())
				{
					// Extract the connection address and port from the response
					std::string::size_type begin = response.GetMessage().find_first_of("0123456789");

					if (begin != std::string::npos)
					{
						unsigned char data[6] = {0, 0, 0, 0, 0, 0};
						std::string str = response.GetMessage().substr(begin);
						std::size_t index = 0;

						for (int i = 0; i < 6; ++i)
						{
							// Extract the current number
							while (isdigit(str[index]))
							{
								data[i] = data[i] * 10 + (str[index] - '0');
								index++;
							}

							// Skip separator
							index++;
						}

						// Reconstruct connection port and address
						unsigned short port = data[4] * 256 + data[5];

						C_IPAddress address(static_cast<unsigned char>(data[0]), static_cast<unsigned char>(data[1]), static_cast<unsigned char>(data[2]),
							static_cast<unsigned char>(data[3]));

						// Connect the data channel to the server
						if (m_DataSocket.Connect (address, port) == C_Socket::SS_Done)
						{
							// Translate the transfer mode to the corresponding FTP parameter
							std::string modeStr;

							switch (mode)
							{
							case C_FTPClient::TM_Binary : modeStr = "I"; break;
							case C_FTPClient::TM_Ascii :  modeStr = "A"; break;
							case C_FTPClient::TM_Ebcdic : modeStr = "E"; break;
							}

							// Set the transfer mode
							response = m_Ftp.SendCommand("TYPE", modeStr);
						}
						else
						{
							// Failed to connect to the server
							response = C_FTPClient::C_Response(C_FTPClient::C_Response::RS_ConnectionFailed);
						}
					}
				}

				return response;
			}

			/*!	Send data across channel
			*/
			void Send (const std::vector<char>& data)
			{
				// Send data
				if (!data.empty())
					m_DataSocket.Send(&data[0], data.size());

				// Close the data socket
				m_DataSocket.Disconnect();
			}

			/*!	Receive data from channel
			*/
			void Receive (std::vector<char>& data)
			{
				// Receive data
				data.clear();

				char buffer[1024];
				std::size_t received;

				while (m_DataSocket.Receive(buffer, sizeof(buffer), received) == C_Socket::SS_Done)
				{
					std::copy(buffer, buffer + received, std::back_inserter(data));
				}

				// Close the data socket
				m_DataSocket.Disconnect();
			}

		private:
			C_FTPClient&    m_Ftp;        ///< Reference to the owner Ftp instance
			C_TCPSocket		m_DataSocket; ///< Socket used for data transfers
		};

		//------------------------------------------------------------------------------------------------------------------------
		C_FTPClient::C_Response::C_Response (EResponseStatus code, const std::string& message) :
			m_Status (code),
			m_Message(message)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_FTPClient::C_Response::IsOk () const
		{
			return m_Status < 400;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_FTPClient::C_Response::EResponseStatus C_FTPClient::C_Response::GetStatus () const
		{
			return m_Status;
		}

		//------------------------------------------------------------------------------------------------------------------------

		const std::string& C_FTPClient::C_Response::GetMessage () const
		{
			return m_Message;
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_FTPClient::C_DirectoryResponse::C_DirectoryResponse (const C_Response& response) :
			C_FTPClient::C_Response (response)
		{
			if (IsOk ())
			{
				// Extract the directory from the server response
				std::string::size_type begin = GetMessage().find('"', 0);
				std::string::size_type end   = GetMessage().find('"', begin + 1);

				m_Directory = GetMessage().substr(begin + 1, end - begin - 1);
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		const std::string& C_FTPClient::C_DirectoryResponse::GetDirectory () const
		{
			return m_Directory;
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_FTPClient::C_ListingResponse::C_ListingResponse (const C_Response& response, const std::vector<char>& data) :
			C_FTPClient::C_Response (response)
		{
			if (IsOk ())
			{
				// Fill the array of strings
				std::string paths(data.begin(), data.end());
				std::string::size_type lastPos = 0;

				for (std::string::size_type pos = paths.find("\r\n"); pos != std::string::npos; pos = paths.find("\r\n", lastPos))
				{
					m_Filenames.push_back(paths.substr(lastPos, pos - lastPos));

					lastPos = pos + 2;
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		const std::vector<std::string>& C_FTPClient::C_ListingResponse::GetFileNames () const
		{
			return m_Filenames;
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_FTPClient::~C_FTPClient ()
		{
			Disconnect();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_FTPClient::C_Response C_FTPClient::Connect (const C_IPAddress& server, unsigned short port, float timeout)
		{
			// Connect to the server
			if (m_CommandSocket.Connect (server, port, timeout) != C_Socket::SS_Done)
				return C_Response(C_Response::RS_ConnectionFailed);

			// Get the response to the connection
			return GetResponse ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_FTPClient::C_Response C_FTPClient::Disconnect ()
		{
			// Send the exit command
			C_Response response = SendCommand("QUIT");

			if (response.IsOk())
				m_CommandSocket.Disconnect();

			return response;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_FTPClient::C_Response C_FTPClient::Login ()
		{
			return Login ("anonymous", "password");
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_FTPClient::C_Response C_FTPClient::Login (const std::string& name, const std::string& password)
		{
			C_Response response = SendCommand ("USER", name);

			if (response.IsOk())
				response = SendCommand("PASS", password);

			return response;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_FTPClient::C_Response C_FTPClient::KeepAlive ()
		{
			 return SendCommand ("NOOP");
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_FTPClient::C_DirectoryResponse C_FTPClient::GetWorkingDirectory ()
		{
			return C_DirectoryResponse (SendCommand("PWD"));
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_FTPClient::C_ListingResponse C_FTPClient::GetDirectoryListing (const std::string& directory)
		{
			// Open a data channel on default port (20) using ASCII transfer mode
			std::vector<char> directoryData;
			C_FTPDataChannel data(*this);
			C_Response response = data.Open (TM_Ascii);

			if (response.IsOk())
			{
				// Tell the server to send us the listing
				response = SendCommand("NLST", directory);

				if (response.IsOk())
				{
					// Receive the listing
					data.Receive(directoryData);

					// Get the response from the server
					response = GetResponse();
				}
			}

			return C_ListingResponse (response, directoryData);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_FTPClient::C_Response C_FTPClient::ChangeDirectory (const std::string& directory)
		{
			  return SendCommand ("CWD", directory);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_FTPClient::C_Response C_FTPClient::ParentDirectory ()
		{
			return SendCommand ("CDUP");
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_FTPClient::C_Response C_FTPClient::CreateDirectory (const std::string& name)
		{
			 return SendCommand ("MKD", name);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_FTPClient::C_Response C_FTPClient::DeleteDirectory (const std::string& name)
		{
			return SendCommand ("RMD", name);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_FTPClient::C_Response C_FTPClient::RenameFile (const std::string& file, const std::string& newName)
		{
			C_Response response = SendCommand ("RNFR", file);

			if (response.IsOk())
				response = SendCommand ("RNTO", newName);

			return response;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_FTPClient::C_Response C_FTPClient::DeleteFile (const std::string& name)
		{
			return SendCommand ("DELE", name);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_FTPClient::C_Response C_FTPClient::Download (const std::string& remoteFile, const std::string& localPath, ETransferMode mode)
		{
			// Open a data channel using the given transfer mode
			C_FTPDataChannel data(*this);
			C_Response response = data.Open(mode);

			if (response.IsOk())
			{
				// Tell the server to start the transfer
				response = SendCommand ("RETR", remoteFile);

				if (response.IsOk())
				{
					// Receive the file data
					std::vector<char> fileData;

					data.Receive(fileData);

					// Get the response from the server
					response = GetResponse();

					if (response.IsOk())
					{
						// Extract the filename from the file path
						std::string filename = remoteFile;
						std::string::size_type pos = filename.find_last_of("/\\");

						if (pos != std::string::npos)
							filename = filename.substr(pos + 1);

						// Make sure the destination path ends with a slash
						std::string path = localPath;

						if (!path.empty() && (path[path.size() - 1] != '\\') && (path[path.size() - 1] != '/'))
							path += "/";

						// Create the file and copy the received data into it
						std::ofstream file((path + filename).c_str(), std::ios_base::binary);

						if (!file)
							return C_Response(C_Response::RS_InvalidFile);

						if (!fileData.empty())
							file.write(&fileData[0], static_cast<std::streamsize>(fileData.size()));
					}
				}
			}

			return response;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_FTPClient::C_Response C_FTPClient::Upload (const std::string& localFile, const std::string& remotePath, ETransferMode mode)
		{
			// Get the contents of the file to send
			std::ifstream file(localFile.c_str(), std::ios_base::binary);

			if (!file)
				return C_Response (C_Response::RS_InvalidFile);

			file.seekg(0, std::ios::end);

			std::size_t length = file.tellg();

			file.seekg(0, std::ios::beg);

			std::vector<char> fileData(length);

			if (length > 0)
				file.read(&fileData[0], static_cast<std::streamsize>(length));

			// Extract the filename from the file path
			std::string filename = localFile;
			std::string::size_type pos = filename.find_last_of("/\\");

			if (pos != std::string::npos)
				filename = filename.substr(pos + 1);

			// Make sure the destination path ends with a slash
			std::string path = remotePath;

			if (!path.empty() && (path[path.size() - 1] != '\\') && (path[path.size() - 1] != '/'))
				path += "/";

			// Open a data channel using the given transfer mode
			C_FTPDataChannel data(*this);
			C_Response response = data.Open(mode);

			if (response.IsOk())
			{
				// Tell the server to start the transfer
				response = SendCommand ("STOR", path + filename);

				if (response.IsOk())
				{
					// Send the file data
					data.Send (fileData);

					// Get the response from the server
					response = GetResponse();
				}
			}

			return response;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_FTPClient::C_Response C_FTPClient::SendCommand (const std::string& command, const std::string& parameter)
		{
			// Build the command string
			std::string commandStr;

			if (parameter != "")
				commandStr = command + " " + parameter + "\r\n";
			else
				commandStr = command + "\r\n";

			// Send it to the server
			if (m_CommandSocket.Send(commandStr.c_str(), commandStr.length()) != C_Socket::SS_Done)
				return C_Response (C_Response::RS_ConnectionClosed);

			// Get the response
			return GetResponse();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_FTPClient::C_Response C_FTPClient::GetResponse ()
		{
			// We'll use a variable to keep track of the last valid code.
			// It is useful in case of multi-lines responses, because the end of such a response
			// will start by the same code
			unsigned int lastCode  = 0;
			bool isInsideMultiline = false;
			std::string message;

			for (;;)
			{
				// Receive the response from the server
				char buffer[1024];
				std::size_t length;

				if (m_CommandSocket.Receive(buffer, sizeof(buffer), length) != C_Socket::SS_Done)
					return C_Response (C_Response::RS_ConnectionClosed);

				// There can be several lines inside the received buffer, extract them all
				std::istringstream in(std::string(buffer, length), std::ios_base::binary);

				while (in)
				{
					// Try to extract the code
					unsigned int code;

					if (in >> code)
					{
						// Extract the separator
						char separator;
						in.get(separator);

						// The '-' character means a multiline response
						if ((separator == '-') && !isInsideMultiline)
						{
							// Set the multiline flag
							isInsideMultiline = true;

							// Keep track of the code
							if (lastCode == 0)
								lastCode = code;

							// Extract the line
							std::getline(in, message);

							// Remove the ending '\r' (all lines are terminated by "\r\n")
							message.erase(message.length() - 1);
							message = separator + message + "\n";
						}
						else
						{
							// We must make sure that the code is the same, otherwise it means
							// we haven't reached the end of the multiline response
							if ((separator != '-') && ((code == lastCode) || (lastCode == 0)))
							{
								// Clear the multiline flag
								isInsideMultiline = false;

								// Extract the line
								std::string line;
								std::getline(in, line);

								// Remove the ending '\r' (all lines are terminated by "\r\n")
								line.erase(line.length() - 1);

								// Append it to the message
								if (code == lastCode)
								{
									std::ostringstream out;
									out << code << separator << line;
									message += out.str();
								}
								else
								{
									message = separator + line;
								}

								// Return the response code and message
								return C_Response (static_cast<C_Response::EResponseStatus>(code), message);
							}
							else
							{
								// The line we just read was actually not a response,
								// only a new part of the current multiline response

								// Extract the line
								std::string line;
								std::getline(in, line);

								if (!line.empty())
								{
									// Remove the ending '\r' (all lines are terminated by "\r\n")
									line.erase(line.length() - 1);

									// Append it to the current message
									std::ostringstream out;
									out << code << separator << line << "\n";
									message += out.str();
								}
							}
						}
					}
					else if (lastCode != 0)
					{
						// It seems we are in the middle of a multiline response

						// Clear the error bits of the stream
						in.clear();

						// Extract the line
						std::string line;
						std::getline(in, line);

						if (!line.empty())
						{
							// Remove the ending '\r' (all lines are terminated by "\r\n")
							line.erase(line.length() - 1);

							// Append it to the current message
							message += line + "\n";
						}
					}
					else
					{
						// Error : cannot extract the code, and we are not in a multiline response
						return C_Response(C_Response::RS_InvalidResponse);
					}
				}
			}

			// We never reach there
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace NETWORK
}	//	namespace REDEEMER

