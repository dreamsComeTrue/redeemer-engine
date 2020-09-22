/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_FileSystem_MemoryFile.cpp
**		Memory files are used as temporary files for holding data
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_FileSystem_MemoryFile.h"
#include "..\\RedeemerEngine.h"


// disable: deprecation warnings when using CRT calls in VC8
#pragma warning (disable: 4996)

namespace REDEEMER
{
	namespace FILESYSTEM
	{	 	
		//------------------------------------------------------------------------------------------------------------------------
		C_MemoryFile::C_MemoryFile() :
			m_IsOpen (false),
			m_Data (NULL)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_MemoryFile::C_MemoryFile (const std::wstring& fileName, DWORD mode)
		{
			Open (fileName, mode);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_MemoryFile::C_MemoryFile (const C_MemoryFile& src)
		{
			Open (src.m_FileName, src.m_OpenMode);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_MemoryFile& C_MemoryFile::operator= (const C_MemoryFile& rhs)
		{
			if (this == &rhs)
				return (*this);

			IDataFile::operator= (rhs);

			Open (rhs.m_FileName, rhs.m_OpenMode);

			return (*this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_MemoryFile::~C_MemoryFile ()
		{
			Close ();

			REDEEMER_SAFE_DELETE (m_Data);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_MemoryFile::Open (const std::wstring& fileName, DWORD mode)
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

			// Open stream
			m_Data = new std::wstringstream (openMode);

			if (!m_Data)
			{
				REDEEMER_LOG << LOG_ERROR << L"Memory file: " << m_FileName << " could not be opened!" << LOG_ENDMESSAGE;

				return;
			}

			m_IsOpen = true;

			m_Data->clear();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_MemoryFile::Close ()
		{
			if (!m_Data)
				return;

			m_OpenMode = 0;

			if (IsOpened ())
			{
				m_IsOpen = false;
			}				
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_MemoryFile::Write (const char* data, ...)
		{
			// Improper open mode
			if (!(m_OpenMode & FOM_Write))
			{
				REDEEMER_LOG << LOG_ERROR << L"Memory file: " << m_FileName << " is not opened for writing!" << LOG_ENDMESSAGE;

				return;
			}

			//	File is not valid
			if (!m_Data->good())
				return;

			char* tempMsg;
			int len = 0;
			va_list arg_ptr;

			va_start(arg_ptr, data);

			len = _vscprintf (data, arg_ptr) + 1;
			tempMsg = new char[len];

			vsprintf(tempMsg, data, arg_ptr);
			va_end(arg_ptr);

			(*m_Data) << tempMsg;

			REDEEMER_SAFE_DELETE_ARRAY (tempMsg);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_MemoryFile::Write (const wchar_t* data, ...)
		{
			// Improper open mode
			if (!(m_OpenMode & FOM_Write))
			{
				REDEEMER_LOG << LOG_ERROR << L"Memory file: " << m_FileName << " is not opened for writing!" << LOG_ENDMESSAGE;

				return;
			}

			//	File is not valid
			if (!m_Data->good ())
				return;

			wchar_t* tempMsg;
			int len = 0;
			va_list arg_ptr;

			va_start(arg_ptr, data);

			len = _vscwprintf(data, arg_ptr) + 1;
			tempMsg = new wchar_t[len];

			vswprintf(tempMsg, data, arg_ptr);
			va_end(arg_ptr);

			(*m_Data) << tempMsg;

			REDEEMER_SAFE_DELETE_ARRAY (tempMsg);
		}

		//------------------------------------------------------------------------------------------------------------------------

		long int C_MemoryFile::Read (wchar_t* buffer, unsigned int size)
		{
			if (!IsOpened() || !(m_OpenMode & FOM_Read))
			{
				REDEEMER_LOG << LOG_ERROR << L"Memory file: " << m_FileName << " is not opened for reading!" << LOG_ENDMESSAGE;

				return 0;
			}

			m_Data->read (buffer, size);

			return size;
		}

		//------------------------------------------------------------------------------------------------------------------------

		long int C_MemoryFile::Read (char* buffer, unsigned int size)
		{
			if (!IsOpened() || !(m_OpenMode & FOM_Read))
			{
				REDEEMER_LOG << LOG_ERROR << L"File: " << m_FileName << " is not opened for reading!" << LOG_ENDMESSAGE;

				return 0;
			}

			long int count = size / sizeof(char);
			wchar_t* data = new wchar_t[count];

			m_Data->read (data, size);

			wcstombs (buffer, data, size);

			REDEEMER_SAFE_DELETE_ARRAY (data);

			return size;
		}

		//------------------------------------------------------------------------------------------------------------------------

		long int C_MemoryFile::ReadLine (std::string& buffer, char delimiter)
		{
			if (!IsOpened() || !(m_OpenMode & FOM_Read))
			{
				REDEEMER_LOG << LOG_ERROR << L"File: " << m_FileName << " is not opened for reading!" << LOG_ENDMESSAGE;

				return 0;
			}

			std::wstring out;

			out.resize(FILE_BUFFER_SIZE);

			m_Data->getline(const_cast<wchar_t*>(out.c_str ()), GetSize(), (char)delimiter);

			buffer.assign(out.begin(), out.end());

			return buffer.size();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_MemoryFile::Put (const char* data, unsigned int size)
		{
			if (!IsOpened() || !(m_OpenMode & FOM_Write))
			{
				REDEEMER_LOG << LOG_ERROR << L"Memory file: " << m_FileName << " is not opened for writing!" << LOG_ENDMESSAGE;

				return;
			}

			long int count = size / sizeof(char);
			wchar_t* fileData = new wchar_t[count];

			mbstowcs (fileData, data, count);

			m_Data->write (fileData, count);

			REDEEMER_SAFE_DELETE_ARRAY (fileData);
		}

		//------------------------------------------------------------------------------------------------------------------------

		const std::wstring& C_MemoryFile::GetFileName () const
		{
			return m_FileName;
		}

		//------------------------------------------------------------------------------------------------------------------------

		DWORD C_MemoryFile::GetFileOpenMode () const
		{
			return m_OpenMode;
		}

		//------------------------------------------------------------------------------------------------------------------------

		long int C_MemoryFile::GetSize () const
		{
			// If the file is not opened, exit
			if (!IsOpened ())
				return 0;

			long int curPos = 0;

			if (m_OpenMode & FOM_Read)
			{
				long int oldPos = m_Data->tellg();

				m_Data->seekg (0, std::ios::end);
				curPos = m_Data->tellg ();
				m_Data->seekg (oldPos, std::ios::beg);
			}
			else 
				if (m_OpenMode & FOM_Write)
				{
					long int oldPos = m_Data->tellp();

					m_Data->seekp (0, std::ios::end);
					curPos = m_Data->tellp ();
					m_Data->seekp (oldPos, std::ios::beg);
				}

				return curPos;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::wstring C_MemoryFile::GetBuffer () const
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

			m_Data->read (pData, (std::streamsize)size);

			if (!(m_OpenMode & FOM_Binary))
				pData[m_Data->gcount()] = '\0';
			else
				pData[size] = '\0';

			std::wstring result = pData;

			REDEEMER_SAFE_DELETE_ARRAY (pData);

			m_Data->clear();

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::string C_MemoryFile::GetData ()
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

			m_Data->read (pData, (std::streamsize)size);

			if (!(m_OpenMode & FOM_Binary))
				pData[m_Data->gcount()] = '\0';
			else
				pData[size] = '\0';

			std::wstring tmp = pData;

			std::string result (tmp.begin(), tmp.end());

			REDEEMER_SAFE_DELETE_ARRAY (pData);

			m_Data->clear();

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_MemoryFile::Eof () const
		{
			return m_Data->eof ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_MemoryFile::IsValid () const
		{
			return (!m_Data->eof () && m_Data->good() && m_IsOpen);
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_MemoryFile::IsOpened () const
		{
			return m_IsOpen;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::ios::seekdir C_MemoryFile::GetSeekDirection (EFileSeekDirection direction) const
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

		void C_MemoryFile::Seek (int position, EFileSeekDirection direction)
		{
			//	If the file is not opened, exit
			if (!IsOpened())
				return;

			// Find iostream seek direction
			std::ios::seekdir dir = GetSeekDirection (direction);

			if (m_OpenMode & FOM_Write)
				m_Data->seekp (position, dir);
			else 
				if(m_OpenMode & FOM_Read)
					m_Data->seekg (position, dir);
		}

		//------------------------------------------------------------------------------------------------------------------------

		long int C_MemoryFile::Tell ()
		{
			//	If the file is not opened, exit
			if (!IsOpened ())
				return -1;

			//	Find iostream position
			if(m_OpenMode & FOM_Write)
				return m_Data->tellp();
			else 
				if(m_OpenMode & FOM_Read)
					return m_Data->tellg();
				else
					return -1;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace FILESYSTEM
}	//	namespace REDEEMER

