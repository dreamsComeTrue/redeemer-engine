/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_FileSystem_Logger.h
**		Log various information to file
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_FILESYSTEM_LOGGER_H_
#define _R_FILESYSTEM_LOGGER_H_

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include <ctime>

namespace REDEEMER
{
	namespace FILESYSTEM
	{
		///	Modifiers for logging
		enum LOGGER_MODIFIER
		{
			MODIFIER_INFO,
			MODIFIER_DEBUG,
			MODIFIER_VERBOSE,
			MODIFIER_OK,
			MODIFIER_WARNING,
			MODIFIER_ERROR,
			MODIFIER_FATAL,
			MODIFIER_END_MESSAGE,
			MODIFIER_END_LINE
		};

		///	Various log levels - each one of them produces different output
		enum LOGGER_LEVEL
		{
			LEVEL_INFO,
			LEVEL_DEBUG,
			LEVEL_VERBOSE,
			LEVEL_OK,
			LEVEL_WARNING,
			LEVEL_ERROR,
			LEVEL_FATAL
		};

		///	Various definitions to help logging
#define LOG_ENDMESSAGE	FILESYSTEM::C_Logger::C_Modifier(FILESYSTEM::MODIFIER_END_MESSAGE)
#define LOG_ENDLINE		FILESYSTEM::C_Logger::C_Modifier(FILESYSTEM::MODIFIER_END_LINE)
#define LOG_DEBUG_INFO	"File: " << __FILE__ << LOG_ENDLINE << "Function: " << __FUNCTION__ << LOG_ENDLINE << "Line: " << __LINE__ << LOG_ENDLINE

#define LOGERROR(logFile,description)	logFile.FlushToFileWithError (__FILE__, __FUNCTION__, __LINE__, description)

#define LOG_INFO	FILESYSTEM::C_Logger::C_Modifier(FILESYSTEM::MODIFIER_INFO)
#define LOG_DEBUG	FILESYSTEM::C_Logger::C_Modifier(FILESYSTEM::MODIFIER_DEBUG)
#define LOG_VERBOSE	FILESYSTEM::C_Logger::C_Modifier(FILESYSTEM::MODIFIER_VERBOSE)
#define LOG_OK		FILESYSTEM::C_Logger::C_Modifier(FILESYSTEM::MODIFIER_OK)
#define LOG_WARNING	FILESYSTEM::C_Logger::C_Modifier(FILESYSTEM::MODIFIER_WARNING)
#define LOG_ERROR	FILESYSTEM::C_Logger::C_Modifier(FILESYSTEM::MODIFIER_ERROR)
#define LOG_FATAL	FILESYSTEM::C_Logger::C_Modifier(FILESYSTEM::MODIFIER_FATAL)

		/*!
		**	\brief C_Logger provides logging capabilities - it can output various information about engine's state to a file
		*/
		class C_Logger
		{
		public:
			/*!	\brief Internal class used to deal with log modifiers
			*/
			class C_Modifier
			{
			public:
				/*!	Constructor
				**	\param modifier LOGGER_MODIFIER to apply
				*/
				C_Modifier (LOGGER_MODIFIER modifier) :
				  m_Modifier (modifier)
				  {
					  //	EMPTY
				  }

				LOGGER_MODIFIER m_Modifier;	///<	Internally held modifier from LOGGER_MODIFIER enum
			};

			///	Constructor
			C_Logger ();

			///	Destructor
			virtual ~C_Logger ();

			/*!	Creates log file
			**	\param	fileName name (path) of the file to log to
			**	\return True if log file is created successfully
			*/
			bool CreateLogFile (const std::wstring& fileName);

			///	Flushes log content to assigned file
			void FlushToFile ();

			/*!	Flushes log content to a file and notifies about error
			**	\param	fileName file, in which error occurred
			**	\param	function function which raised error
			**	\param	line line of code
			**	\param	description some notes about error
			*/
			void FlushToFileWithError (const std::string& fileName, const std::string& function, int line, const std::wstring& description);

			/*!	Append 'bool' value
			**	\param value value to append
			*/
			C_Logger& operator<< (bool value);

			/*!	Append 'char' value
			**	\param value value to append
			*/
			C_Logger& operator<< (char value);

			/*!	Append 'short' value
			**	\param value value to append
			*/
			C_Logger& operator<< (short value);

			/*!	Append 'unsigned short' value
			**	\param value value to append
			*/
			C_Logger& operator<< (unsigned short value);

			/*!	Append 'int' value
			**	\param value value to append
			*/
			C_Logger& operator<< (int value);

			/*!	Append 'unsigned int' value
			**	\param value value to append
			*/
			C_Logger& operator<< (unsigned int value);

			/*!	Append 'long' value
			**	\param value value to append
			*/
			C_Logger& operator<< (long value);

			/*!	Append 'unsigned long' value
			**	\param value value to append
			*/
			C_Logger& operator<< (unsigned long value);

			/*!	Append '__int64' value
			**	\param value value to append
			*/
			C_Logger& operator<< (__int64 value);

			/*!	Append 'unsigned __int64' value
			**	\param value value to append
			*/
			C_Logger& operator<< (unsigned __int64 value);

			/*!	Append 'float' value
			**	\param value value to append
			*/
			C_Logger& operator<< (float value);

			/*!	Append 'double' value
			**	\param value value to append
			*/
			C_Logger& operator<< (double value);

			/*!	Append 'long double' value
			**	\param value value to append
			*/
			C_Logger& operator<< (long double value);

			/*!	Append 'char*' value
			**	\param value value to append
			*/
			C_Logger& operator<< (const char* value);

			/*!	Append 'const std::string' value
			**	\param value value to append
			*/
			C_Logger& operator<< (const std::string& value);

			/*!	Append 'wchar_t*' value
			**	\param value value to append
			*/
			C_Logger& operator<< (wchar_t* value);

			/*!	Append 'const std::wstring' value
			**	\param value value to append
			*/
			C_Logger& operator<< (const std::wstring& value);
							
			/*!	Append C_Modifier to the output
			**	\param modifier modifier to append
			*/
			C_Logger& operator<< (const C_Modifier& modifier);
			
		private:
			bool				m_IsAssigned;	///<	Is file assigned
			std::wstring		m_FileName;		///<	Name of the file
			std::wofstream		m_File;			///<	File to write data to
			std::wostringstream	m_Message;		///<	Message to append in log file
			LOGGER_LEVEL		m_Level;		///<	Currently used log level

			time_t				m_RawTime;
			struct tm*			m_TimeInfo;
		};
	}	//	namespace FILESYSTEM
}	//	namespace REDEEMER

#endif	//	_R_FILESYSTEM_LOGGER_H_
