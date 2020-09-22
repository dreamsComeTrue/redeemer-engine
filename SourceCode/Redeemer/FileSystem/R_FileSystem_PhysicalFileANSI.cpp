/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_FileSystem_PhysicalFileANSI.cpp
**		Physical file is raw file stored on a disk
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_FileSystem_PhysicalFileANSI.h"
#include "..\\RedeemerEngine.h"

#include <iostream>

// disable: deprecation warnings when using CRT calls in VC8
#pragma warning (disable: 4996)

namespace REDEEMER
{
	namespace FILESYSTEM
	{	 	
		//------------------------------------------------------------------------------------------------------------------------
		C_PhysicalFileANSI::C_PhysicalFileANSI ()  :
		m_OpenMode (0)
		{
			m_File = new std::fstream ();			
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PhysicalFileANSI::C_PhysicalFileANSI (const std::wstring& fileName, DWORD mode)
		{
			m_File = new std::fstream ();

			Open (fileName, mode);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PhysicalFileANSI::C_PhysicalFileANSI (const C_PhysicalFileANSI& src)
		{
			m_File = new std::fstream ();

			Open (src.m_FileName, src.m_OpenMode);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PhysicalFileANSI& C_PhysicalFileANSI::operator= (const C_PhysicalFileANSI& rhs)
		{
			if (this == &rhs)
				return (*this);

			IDataFile::operator= (rhs);

			Open (rhs.m_FileName, rhs.m_OpenMode);

			return (*this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PhysicalFileANSI::~C_PhysicalFileANSI ()
		{
			Close ();

			REDEEMER_SAFE_DELETE (m_File);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicalFileANSI::Open (const std::wstring& fileName, DWORD mode)
		{
			m_FileName = fileName;
			m_OpenMode = mode;

			// Set opening flags
			std::ios_base::openmode openMode = 0;

			if (m_OpenMode & FOM_Read)
				openMode |= std::ios::in;

			if (m_OpenMode & FOM_Write)
				openMode |= std::ios::out;

			if (m_OpenMode & FOM_Binary)
				openMode |= std::ios::binary;

			if (m_OpenMode & FOM_Append)
				openMode |= std::ios::app;

			if (m_OpenMode & FOM_NoCreate)
				openMode |= std::ios::_Nocreate;

			if (m_OpenMode & FOM_NoReplace)
				openMode |= std::ios::_Noreplace;

			if (m_OpenMode & FOM_Truncate)
				openMode |= std::ios::trunc;

			if (m_OpenMode & FOM_Ate)
				openMode |= std::ios::ate;

			// Open stream
			m_File->open (m_FileName.c_str(), openMode);

			if (!IsOpened ())
			{
				REDEEMER_LOG << LOG_ERROR << L"File: " << m_FileName << " could not be opened!" << LOG_ENDMESSAGE;

				return;
			}

			m_File->clear();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicalFileANSI::Close ()
		{
			if (!m_File)
				return;

			m_OpenMode = 0;

			if (IsOpened ())
				m_File->close ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicalFileANSI::Write (const char* data, ...)
		{
			// Improper open mode
			if (!(m_OpenMode & FOM_Write))
			{
				REDEEMER_LOG << LOG_ERROR << L"File: " << m_FileName << " is not opened for writing!" << LOG_ENDMESSAGE;

				return;
			}

			//	File is not valid
			if (!m_File->good())
				return;

			char* tempMsg;
			int len = 0;
			va_list arg_ptr;

			va_start(arg_ptr, data);

			len = _vscprintf (data, arg_ptr) + 1;
			tempMsg = new char[len];

			vsprintf(tempMsg, data, arg_ptr);
			va_end(arg_ptr);

			m_File->clear();
			(*m_File) << tempMsg;

			m_File->flush();

			REDEEMER_SAFE_DELETE_ARRAY (tempMsg);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicalFileANSI::Write (const wchar_t* data, ...)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		long int C_PhysicalFileANSI::Read (char* buffer, unsigned int size)
		{
			if (!IsOpened() || !(m_OpenMode & FOM_Read))
			{
				REDEEMER_LOG << LOG_ERROR << L"File: " << m_FileName << " is not opened for reading!" << LOG_ENDMESSAGE;

				return 0;
			}

			m_File->read (buffer, size);

			return size;
		}

		//------------------------------------------------------------------------------------------------------------------------

		long int C_PhysicalFileANSI::ReadLine(std::string& buffer, char delimiter)
		{
			if (!IsOpened() || !(m_OpenMode & FOM_Read))
			{
				REDEEMER_LOG << LOG_ERROR << L"File: " << m_FileName << " is not opened for reading!" << LOG_ENDMESSAGE;

				return 0;
			}

			buffer.clear();

			buffer.resize(FILE_BUFFER_SIZE);

			m_File->getline(const_cast<char*>(buffer.c_str ()), GetSize(), delimiter);

			return buffer.size();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicalFileANSI::Put (const char* data, unsigned int size)
		{
			if (!IsOpened() || !(m_OpenMode & FOM_Write))
			{
				REDEEMER_LOG << LOG_ERROR << L"File: " << m_FileName << " is not opened for writing!" << LOG_ENDMESSAGE;

				return;
			}

			m_File->clear();
			m_File->write (data, size);
			m_File->flush();
		}

		//------------------------------------------------------------------------------------------------------------------------

		const std::wstring& C_PhysicalFileANSI::GetFileName () const
		{
			return m_FileName;
		}

		//------------------------------------------------------------------------------------------------------------------------

		DWORD C_PhysicalFileANSI::GetFileOpenMode () const
		{
			return m_OpenMode;
		}

		//------------------------------------------------------------------------------------------------------------------------

		long int C_PhysicalFileANSI::GetSize () const
		{
			// If the file is not opened, exit
			if (!IsOpened ())
				return 0;

			long int curPos = 0;

			if (m_OpenMode & FOM_Read)
			{
				long int oldPos = m_File->tellg();

				m_File->seekg (0, std::ios::end);
				curPos = m_File->tellg ();
				m_File->seekg (oldPos, std::ios::beg);
			}
			else 
				if (m_OpenMode & FOM_Write)
				{
					long int oldPos = m_File->tellp();

					m_File->seekp (0, std::ios::end);
					curPos = m_File->tellp ();
					m_File->seekp (oldPos, std::ios::beg);
				}

				return curPos;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::wstring C_PhysicalFileANSI::GetBuffer () const
		{
			//	Improper open mode
			if (!(m_OpenMode & FOM_Read))
			{
				REDEEMER_LOG << LOG_ERROR << L"File: " << m_FileName << " is not opened for reading!" << LOG_ENDMESSAGE;

				return L"";
			}

			// Get file size in bytes
			long int size = GetSize();

			// If the file is not opened, exit
			if (!size)
				return L"";

			// Read file as a null-terminated string
			char* pData = new char[size + 1];

			m_File->read (pData, (std::streamsize)size);

			if (!(m_OpenMode & FOM_Binary))
				pData[m_File->gcount()] = '\0';
			else
				pData[size] = '\0';

			std::string temp = pData;

			std::wstring result(temp.begin(), temp.end());

			REDEEMER_SAFE_DELETE_ARRAY (pData);

			m_File->clear();

			return result;
		}

		std::string C_PhysicalFileANSI::GetData ()
		{
			//	Improper open mode
			if (!(m_OpenMode & FOM_Read))
			{
				REDEEMER_LOG << LOG_ERROR << L"File: " << m_FileName << " is not opened for reading!" << LOG_ENDMESSAGE;

				return "";
			}

			// Get file size in bytes
			long int size = GetSize();

			// If the file is not opened, exit
			if (!size)
				return "";

			Seek (0, FSD_Begin);

			// Read file as a null-terminated string
			char* pData = new char[size + 1];
							
			m_File->read ((char*)pData, size);
																 
			if (!(m_OpenMode & FOM_Binary))
				pData[m_File->gcount()] = '\0';
			else
				pData[size] = '\0';

			std::string temp = pData;
			REDEEMER_SAFE_DELETE_ARRAY (pData);

			m_File->clear();

			return temp;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_PhysicalFileANSI::Eof () const
		{
			return m_File->eof ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_PhysicalFileANSI::IsValid () const
		{
			return (!m_File->eof () && m_File->good() && m_File->is_open ());
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_PhysicalFileANSI::IsOpened () const
		{
			return m_File->is_open ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::ios::seekdir C_PhysicalFileANSI::GetSeekDirection (EFileSeekDirection direction) const
		{
			std::ios::seekdir dir = std::ios::beg;

			switch (direction)
			{
			case FSD_Current: dir = std::ios::cur; break;
			case FSD_Begin  : dir = std::ios::beg; break;
			case FSD_End	 : dir = std::ios::end; break;
			}

			return dir;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicalFileANSI::Seek (int position, EFileSeekDirection direction)
		{
			//	If the file is not opened, exit
			if (!IsOpened())
				return;

			// Find iostream seek direction
			std::ios::seekdir dir = GetSeekDirection (direction);

			m_File->clear();

			if (m_OpenMode & FOM_Write)
				m_File->seekp (position, dir);

			if(m_OpenMode & FOM_Read)
				m_File->seekg (position, dir);
		}

		//------------------------------------------------------------------------------------------------------------------------

		long int C_PhysicalFileANSI::Tell ()
		{
			//	If the file is not opened, exit
			if (!IsOpened ())
				return -1;

			//	Find iostream position
			if(m_OpenMode & FOM_Write)
				return m_File->tellp();
			else 
				if(m_OpenMode & FOM_Read)
					return m_File->tellg();
				else
					return -1;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace FILESYSTEM
}	//	namespace REDEEMER

