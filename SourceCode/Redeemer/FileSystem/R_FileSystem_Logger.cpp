/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_FileSystem_Logger.cpp
**		Log various information to file
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_FileSystem_Logger.h"
#include "..\\Core\\R_Core_Convert.h"

#include <iomanip>

// disable: deprecation warnings when using CRT calls in VC8
#pragma warning (disable: 4996)

namespace REDEEMER
{
	namespace FILESYSTEM
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_Logger::C_Logger () :
			m_IsAssigned (false)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Logger::~C_Logger ()
		{
			if (m_IsAssigned)
			{
				m_File <<	"</table>"	<<	std::endl;
				m_File << std::endl;
				m_File <<	"<div class=\"close\">"	<<	std::endl;

#ifdef _DEBUG
				m_File << "DEBUG Log procedure ended at ";
#else
				m_File << "Log procedure ended at ";
#endif

				time (&m_RawTime);
				m_TimeInfo = localtime (&m_RawTime);

				m_File << asctime (m_TimeInfo);
				m_File <<	"</div>"	<<	std::endl;
				m_File <<	"</div>"	<<	std::endl;
				m_File <<	"</div>"	<<	std::endl;
				m_File <<	"</div>"	<<	std::endl;
				m_File <<	"</body>"	<<	std::endl;
				m_File <<	"</html>"	<<	std::endl;

				m_File.close();
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_Logger::CreateLogFile (const std::wstring& fileName)
		{
			if (m_IsAssigned)
				return false;

			m_File.open (fileName.c_str());

			if (!m_File.is_open ())
				return false;

			m_File <<	"<?xml version=\"1.0\" encoding=\"utf-16\"?>"	<<	std::endl;
			m_File <<	"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\""	<<	std::endl;
			m_File <<	"		\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">"	<<	std::endl;
			m_File <<	std::endl;
			m_File <<	"<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\">"	<<	std::endl;
			m_File <<	"<head>"	<<	std::endl;
			m_File <<	"	<meta name=\"generator\" content=\"Engine\" />"	<<	std::endl;
			m_File <<	"	<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-16\" />"	<<	std::endl;
			m_File <<	"	<meta name=\"author\" content=\"Engine\" />"	<<	std::endl;
			m_File <<	"	<meta name=\"owner\" content=\"Engine\" />"	<<	std::endl;
			m_File <<	std::endl;
			m_File <<	"	<title>Log file</title>"	<<	std::endl;
			m_File <<	"	<style type=\"text/css\">"	<<	std::endl;
			m_File <<	"		/*<![CDATA[*/"	<<	std::endl;
			m_File <<	"		.strip		{ background-color:#A8D3FF; height:8px; border-bottom:1px solid #8898BF; }"	<<	std::endl;
			m_File <<	"		.container	{ font-family: anonymous; font-size: small; border: 1px solid #8898BF; }"	<<	std::endl;
			m_File <<	"		.log		{ padding: 5px; }"	<<	std::endl;
			m_File <<	"		.messages	{ border: 1px solid #8898BF; }"	<<	std::endl;
			m_File <<	std::endl;
			m_File <<	"		.open		{ background-color:#A8D3FF; padding: 0 0 0 1em; font-size: small; }"	<<	std::endl;
			m_File <<	"		.close		{ background-color:#A8D3FF; padding: 0 0 0 1em; font-size: small; }"	<<	std::endl;
			m_File <<	std::endl;
			m_File <<	"		h1			{ margin: 0; background: #FFFFFF; font-family: Verdana; font-size: xx-large; font-weight: bold; padding: 5px; border-bottom:1px solid #8898BF; }"	<<	std::endl;
			m_File <<	"		h2			{ margin: 0; background: #C8D2DC; font-family: Verdana; font-size: small; font-weight: normal; padding: 0 0 0 5em; border-bottom:1px solid #8898BF; }"	<<	std::endl;
			m_File <<	std::endl;
			m_File <<	"		table		{ border:1px solid #8898BF; border-width:1px 0px 0px 0px;  width: 100%;}"	<<	std::endl;
			m_File <<	"		td			{ border:1px solid #8898BF; border-width:0 0px 1px 0; }"	<<	std::endl;
			m_File <<	std::endl;
			m_File <<	"		.time		{ width: 6em; text-align: center; background: #99cc99; vertical-align: top;}"	<<	std::endl;
			m_File <<	"		.message	{ text-align: left; }"	<<	std::endl;
			m_File <<	std::endl;
			m_File <<	"		.i_info		{ border-left:1px solid #8898BF; border-right:1px solid #8898BF; width: 32px; height: 32px; background-color: #87CEFA; background-repeat: no-repeat; background-image: url(\"GameDocumentation/Help/Log/info.png\"); }"	<<	std::endl;
			m_File <<	"		.i_debug	{ border-left:1px solid #8898BF; border-right:1px solid #8898BF; width: 32px; height: 32px; background-color: #D8E1B9; background-repeat: no-repeat; background-image: url(\"GameDocumentation/Help/Log/debug.png\");	}"	<<	std::endl;
			m_File <<	"		.i_verbose	{ border-left:1px solid #8898BF; border-right:1px solid #8898BF; width: 32px; height: 32px; background-color: #2cb7b7; background-repeat: no-repeat; background-image: url(\"GameDocumentation/Help/Log/verbose.png\");	}"	<<	std::endl;
			m_File <<	"		.i_ok		{ border-left:1px solid #8898BF; border-right:1px solid #8898BF; width: 32px; height: 32px; background-color: #90EE90; background-repeat: no-repeat; background-image: url(\"GameDocumentation/Help/Log/ok.png\");		}"	<<	std::endl;
			m_File <<	"		.i_warning	{ border-left:1px solid #8898BF; border-right:1px solid #8898BF; width: 32px; height: 32px; background-color: #FFCA7F; background-repeat: no-repeat; background-image: url(\"GameDocumentation/Help/Log/warning.png\");	}"	<<	std::endl;
			m_File <<	"		.i_error	{ border-left:1px solid #8898BF; border-right:1px solid #8898BF; width: 32px; height: 32px; background-color: #FF7F7F; background-repeat: no-repeat; background-image: url(\"GameDocumentation/Help/Log/error.png\");	}"	<<	std::endl;
			m_File <<	"		.i_fatal	{ border-left:1px solid #8898BF; border-right:1px solid #8898BF; width: 32px; height: 32px; background-color: #DC143C; background-repeat: no-repeat; background-image: url(\"GameDocumentation/Help/Log/fatal.png\");	}"	<<	std::endl;
			m_File <<	std::endl;
			m_File <<	"		.info		{ background-color:#87CEFA; padding-left: 1em; }"	<<	std::endl;
			m_File <<	"		.debug		{ background-color:#D8E1B9; padding-left: 1em; }"	<<	std::endl;
			m_File <<	"		.verbose	{ background-color:#2cb7b7; padding-left: 1em; }"	<<	std::endl;
			m_File <<	"		.ok			{ background-color:#90EE90; padding-left: 1em; }"	<<	std::endl;
			m_File <<	"		.warning	{ background-color:#FFCA7F; padding-left: 1em; }"	<<	std::endl;
			m_File <<	"		.error		{ background-color:#FF7F7F; padding-left: 1em; }"	<<	std::endl;
			m_File <<	"		.fatal		{ background-color:#DC143C; padding-left: 1em; }"	<<	std::endl;
			m_File <<	std::endl;
			m_File <<	"		/*]]>*/"	<<	std::endl;
			m_File <<	"	</style>"	<<	std::endl;
			m_File <<	"</head>"	<<	std::endl;
			m_File <<	std::endl;
			m_File <<	"<body>"	<<	std::endl;
			m_File <<	"<div class=\"container\">"	<<	std::endl;
			m_File <<	"<div class=\"strip\"></div>"	<<	std::endl;
			m_File <<	"<h1>Log file</h1>"	<<	std::endl;
			m_File <<	"<h2>Logger data [generated by Redeemer (C) Engine]</h2>"	<<	std::endl;
			m_File <<	"<div class=\"log\">"	<<	std::endl;
			m_File <<	"<div class=\"messages\">"	<<	std::endl;
			m_File <<	"<div class=\"open\">"	<<	std::endl;

#ifdef _DEBUG
			m_File << "DEBUG Log procedure started at ";
#else
			m_File << "Log procedure started at ";
#endif

			time (&m_RawTime);
			m_TimeInfo = localtime (&m_RawTime);

			m_File << asctime (m_TimeInfo);
			m_File <<	"</div>"	<<	std::endl;
			m_File <<	std::endl;
			m_File <<	"<table cellspacing=\"0\" cellpadding=\"0\">"	<<	std::endl;

			m_File.flush();

			m_IsAssigned = true;

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Logger::FlushToFile ()
		{
			time (&m_RawTime);
			m_TimeInfo = localtime (&m_RawTime);

			std::wostringstream	t;

			t	<< L"[" << std::setfill(L'0') << std::setw(2) << m_TimeInfo->tm_hour
				<< L":" << std::setfill(L'0') << std::setw(2) << m_TimeInfo->tm_min
				<< L":" << std::setfill(L'0') << std::setw(2) << m_TimeInfo->tm_sec << "]";

			m_File <<	"<tr><td class=\"time\">";

			switch (m_Level)
			{
			case LEVEL_INFO:
				m_File << t.str() << "<br />INFO</td><td class=\"i_info\">&nbsp;</td><td class=\"info\">";
				break;

			case LEVEL_DEBUG:
				m_File << t.str() << "<br />DEBUG</td><td class=\"i_debug\">&nbsp;</td><td class=\"debug\">";
				break;

			case LEVEL_VERBOSE:
				m_File << t.str() << "<br />VERBOSE</td><td class=\"i_verbose\">&nbsp;</td><td class=\"verbose\">";
				break;

			case LEVEL_OK:
				m_File << t.str() << "<br />OK</td><td class=\"i_ok\">&nbsp;</td><td class=\"ok\">";
				break;

			case LEVEL_WARNING:
				m_File << t.str() << "<br />WARNING</td><td class=\"i_warning\">&nbsp;</td><td class=\"warning\">";
				break;

			case LEVEL_ERROR:
				m_File << t.str() << "<br />ERROR</td><td class=\"i_error\">&nbsp;</td><td class=\"error\">";
				break;

			case LEVEL_FATAL:
				m_File << t.str() << "<br />FATAL</td><td class=\"i_fatal\">&nbsp;</td><td class=\"fatal\">";
				break;
			}

			m_File << m_Message.str() << "</td></tr>"	<<	std::endl;

			m_Message.str(L"");
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Logger::FlushToFileWithError (const std::string& fileName, const std::string& function, int line, const std::wstring& description)
		{
			m_Level = LEVEL_ERROR;

			std::wstring fileNameTmp, functionTmp;

			CORE::C_Convert::ConvertCharsToUnicode (&fileNameTmp, fileName);
			CORE::C_Convert::ConvertCharsToUnicode (&functionTmp, function);

			m_Message << L"File: "			<< fileNameTmp
				<< L"<br />Function: "		<< functionTmp
				<< L"<br />Line: "			<< line
				<< L"<br />Description: "	<< description;

			FlushToFile ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Logger& C_Logger::operator<< (bool value)
		{
			if (value)
				m_Message << "true";
			else
				m_Message << "false";

			return (*this);
		}
	 
		//------------------------------------------------------------------------------------------------------------------------

		C_Logger& C_Logger::operator<< (char value)
		{
			m_Message << value;

			return (*this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Logger& C_Logger::operator<< (short value)
		{
			m_Message << value;

			return (*this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Logger& C_Logger::operator<< (unsigned short value)
		{
			m_Message << value;

			return (*this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Logger& C_Logger::operator<< (int value)
		{
			m_Message << value;

			return (*this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Logger& C_Logger::operator<< (unsigned int value)
		{
			m_Message << value;

			return (*this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Logger& C_Logger::operator<< (long value)
		{
			m_Message << value;

			return (*this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Logger& C_Logger::operator<< (unsigned long value)
		{
			m_Message << value;

			return (*this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Logger& C_Logger::operator<< (__int64 value)
		{
			m_Message << value;

			return (*this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Logger& C_Logger::operator<< (unsigned __int64 value)
		{
			m_Message << value;

			return (*this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Logger& C_Logger::operator<< (float value)
		{
			m_Message << value;

			return (*this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Logger& C_Logger::operator<< (double value)
		{
			m_Message << value;

			return (*this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Logger& C_Logger::operator<< (long double value)
		{
			m_Message << value;

			return (*this);
		}

		//------------------------------------------------------------------------------------------------------------------------
		C_Logger& C_Logger::operator<< (const char* value)
		{
			m_Message << value;

			return (*this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Logger& C_Logger::operator<< (const std::string& value)
		{
			m_Message << value.c_str();

			return (*this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Logger& C_Logger::operator<< (wchar_t* value)
		{
			m_Message << value;

			return (*this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Logger& C_Logger::operator<< (const std::wstring& value)
		{
			m_Message << value;

			return (*this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Logger& C_Logger::operator<< (const C_Modifier& modifier)
		{
			switch (modifier.m_Modifier)
			{
			case MODIFIER_INFO:		m_Level = LEVEL_INFO;		break;
			case MODIFIER_DEBUG:	m_Level = LEVEL_DEBUG;		break;
			case MODIFIER_VERBOSE:	m_Level = LEVEL_VERBOSE;	break;
			case MODIFIER_OK:		m_Level = LEVEL_OK;			break;
			case MODIFIER_WARNING:	m_Level = LEVEL_WARNING;	break;
			case MODIFIER_ERROR:	m_Level = LEVEL_ERROR;		break;
			case MODIFIER_FATAL:	m_Level = LEVEL_FATAL;		break;

			case MODIFIER_END_MESSAGE:
				FlushToFile ();
				break;

			case MODIFIER_END_LINE:
				m_Message << "<br />";
				break;
			}

			return (*this);
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace FILESYSTEM
}	//	namespace REDEEMER
