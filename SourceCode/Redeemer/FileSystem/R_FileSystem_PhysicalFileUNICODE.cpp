/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_FileSystem_PhysicalFile.cpp
**		Physical file is raw file stored on a disk
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_FileSystem_PhysicalFileUNICODE.h"
#include "..\\RedeemerEngine.h"

#include <iostream>

// disable: deprecation warnings when using CRT calls in VC8
#pragma warning (disable: 4996)

namespace REDEEMER
{
	namespace FILESYSTEM
	{	 	
		//------------------------------------------------------------------------------------------------------------------------
		C_PhysicalFileUNICODE::C_PhysicalFileUNICODE ()  :
			m_OpenMode (0)
		{
			m_File = new std::wfstream ();			
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PhysicalFileUNICODE::C_PhysicalFileUNICODE (const std::wstring& fileName, DWORD mode)
		{
			m_File = new std::wfstream ();

			Open (fileName, mode);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PhysicalFileUNICODE::C_PhysicalFileUNICODE (const C_PhysicalFileUNICODE& src)
		{
			m_File = new std::wfstream ();

			Open (src.m_FileName, src.m_OpenMode);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PhysicalFileUNICODE& C_PhysicalFileUNICODE::operator= (const C_PhysicalFileUNICODE& rhs)
		{
			if (this == &rhs)
				return (*this);

			IDataFile::operator= (rhs);

			Open (rhs.m_FileName, rhs.m_OpenMode);

			return (*this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PhysicalFileUNICODE::~C_PhysicalFileUNICODE ()
		{
			Close ();

			REDEEMER_SAFE_DELETE (m_File);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicalFileUNICODE::Open (const std::wstring& fileName, DWORD mode)
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

		void C_PhysicalFileUNICODE::Close ()
		{
			if (!m_File)
				return;

			m_OpenMode = 0;

			if (IsOpened ())
				m_File->close ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicalFileUNICODE::Write (const char* data, ...)
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

		void C_PhysicalFileUNICODE::Write (const wchar_t* data, ...)
		{
			// Improper open mode
			if (!(m_OpenMode & FOM_Write))
			{
				REDEEMER_LOG << LOG_ERROR << L"File: " << m_FileName << " is not opened for writing!" << LOG_ENDMESSAGE;

				return;
			}

			//	File is not valid
			if (!m_File->is_open ())
				return;

			wchar_t* tempMsg;
			int len = 0;
			va_list arg_ptr;

			va_start(arg_ptr, data);

			len = _vscwprintf(data, arg_ptr) + 1;
			tempMsg = new wchar_t[len];

			vswprintf(tempMsg, data, arg_ptr);
			va_end(arg_ptr);

			m_File->clear();
			(*m_File) << tempMsg;

			m_File->flush();

			REDEEMER_SAFE_DELETE_ARRAY (tempMsg);
		}

		//------------------------------------------------------------------------------------------------------------------------

		long int C_PhysicalFileUNICODE::Read (wchar_t* buffer, unsigned int size)
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

		long int C_PhysicalFileUNICODE::ReadLine(std::string& buffer, char delimiter)
		{
			if (!IsOpened() || !(m_OpenMode & FOM_Read))
			{
				REDEEMER_LOG << LOG_ERROR << L"File: " << m_FileName << " is not opened for reading!" << LOG_ENDMESSAGE;

				return 0;
			}

			std::wstring out;

			out.resize (FILE_BUFFER_SIZE);

			m_File->getline(const_cast<wchar_t*>(out.c_str ()), GetSize(), (char)delimiter);

			buffer.assign(out.begin(), out.end());

			return buffer.size();
		}

		//------------------------------------------------------------------------------------------------------------------------

		long int C_PhysicalFileUNICODE::Read (char* buffer, unsigned int size)
		{
			if (!IsOpened() || !(m_OpenMode & FOM_Read))
			{
				REDEEMER_LOG << LOG_ERROR << L"File: " << m_FileName << " is not opened for reading!" << LOG_ENDMESSAGE;

				return 0;
			}

			long int count = size / sizeof(char);
			wchar_t* data = new wchar_t[count];

			m_File->read(data, size);

			wcstombs (buffer, data, size);

			REDEEMER_SAFE_DELETE_ARRAY (data);

			return size;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PhysicalFileUNICODE::Put (const char* data, unsigned int size)
		{
			if (!IsOpened() || !(m_OpenMode & FOM_Write))
			{
				REDEEMER_LOG << LOG_ERROR << L"File: " << m_FileName << " is not opened for writing!" << LOG_ENDMESSAGE;

				return;
			}

			long int count = size / sizeof(char);
			wchar_t* fileData = new wchar_t[count];

			mbstowcs (fileData, data, count);

			m_File->clear();
			m_File->write (fileData, count);
			m_File->flush();

			REDEEMER_SAFE_DELETE_ARRAY (fileData);
		}

		//------------------------------------------------------------------------------------------------------------------------

		const std::wstring& C_PhysicalFileUNICODE::GetFileName () const
		{
			return m_FileName;
		}

		//------------------------------------------------------------------------------------------------------------------------

		DWORD C_PhysicalFileUNICODE::GetFileOpenMode () const
		{
			return m_OpenMode;
		}

		//------------------------------------------------------------------------------------------------------------------------

		long int C_PhysicalFileUNICODE::GetSize () const
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

		std::wstring C_PhysicalFileUNICODE::GetBuffer () const
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
			wchar_t* pData = new wchar_t[size + 1];

			m_File->read (pData, (std::streamsize)size);

			if (!(m_OpenMode & FOM_Binary))
				pData[m_File->gcount()] = '\0';
			else
				pData[size] = '\0';

			std::wstring result = pData;

			REDEEMER_SAFE_DELETE_ARRAY (pData);

			m_File->clear();

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::string C_PhysicalFileUNICODE::GetData ()
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

			// Read file as a null-terminated string
			wchar_t* pData = new wchar_t[size + 1];

			Seek (0, FSD_Begin);

			m_File->read (pData, (std::streamsize)size);

			if (!(m_OpenMode & FOM_Binary))
				pData[m_File->gcount()] = '\0';
			else
				pData[size] = '\0';

			std::wstring tmp = pData;

			std::string result (tmp.begin(), tmp.end());

			REDEEMER_SAFE_DELETE_ARRAY (pData);

			m_File->clear();

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_PhysicalFileUNICODE::Eof () const
		{
			return m_File->eof ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_PhysicalFileUNICODE::IsValid () const
		{
			return (!m_File->eof () && m_File->good() && m_File->is_open ());
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_PhysicalFileUNICODE::IsOpened () const
		{
			return m_File->is_open ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::ios::seekdir C_PhysicalFileUNICODE::GetSeekDirection (EFileSeekDirection direction) const
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

		void C_PhysicalFileUNICODE::Seek (int position, EFileSeekDirection direction)
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

		long int C_PhysicalFileUNICODE::Tell ()
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

