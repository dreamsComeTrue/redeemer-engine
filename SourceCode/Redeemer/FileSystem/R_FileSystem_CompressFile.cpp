/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_FileSystem_CompressFile.cpp
**		Compress files are files contained in PACK archives
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_FileSystem_CompressFile.h"
#include "..\\RedeemerEngine.h"
#include "..\\Core\\\R_Core_Convert.h"

namespace REDEEMER
{
	namespace FILESYSTEM
	{	 	
		//------------------------------------------------------------------------------------------------------------------------
		C_CompressFile::C_CompressFile() :
			m_Data (NULL),
			m_FilePointer (NULL),
			m_Size (0),
			m_Position (0)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_CompressFile::C_CompressFile (const std::string& fileName, void* data, unsigned int size)
		{						   
			m_Size = size;
			m_Position = 0;
			m_Data = new char[m_Size];
			m_FilePointer = m_Data;
			m_FileName.assign(fileName.begin(), fileName.end ());
			memcpy (m_Data, data, m_Size);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_CompressFile::C_CompressFile (const C_CompressFile& src)
		{
			m_Size = src.m_Size;
			m_Data = new char[m_Size];

			memcpy(m_Data, src.m_Data, m_Size);

			m_FileName = src.m_FileName;
			m_FilePointer = src.m_FilePointer;
			m_Position = src.m_Position;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_CompressFile& C_CompressFile::operator= (const C_CompressFile& rhs)
		{
			if (this == &rhs)
				return (*this);

			m_Size = rhs.m_Size;

			REDEEMER_SAFE_DELETE_ARRAY (m_Data);

			m_Data = new char [m_Size];

			memcpy (m_Data, rhs.m_Data, m_Size);

			m_FileName = rhs.m_FileName;
			m_FilePointer = rhs.m_FilePointer;
			m_Position = rhs.m_Position;

			return (*this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_CompressFile::~C_CompressFile ()
		{
			REDEEMER_SAFE_DELETE_ARRAY (m_Data);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_CompressFile::Open (const std::wstring& fileName, DWORD mode)
		{
			m_FilePointer = m_Data;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_CompressFile::Close ()
		{
			REDEEMER_SAFE_DELETE_ARRAY (m_Data);

			m_Size = 0;
			m_FilePointer = NULL;
			m_Position = 0;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_CompressFile::Write (const char* data, ...)
		{
			//	Can't write to archive files
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_CompressFile::Write (const wchar_t* data, ...)
		{
			//	Can't write to archive files
		}

		//------------------------------------------------------------------------------------------------------------------------

		long int C_CompressFile::Read (wchar_t* buffer, unsigned int size)
		{
			char* charBuffer = new char[size];
			std::wstring out;

			memcpy (charBuffer, m_FilePointer, size);

			m_FilePointer += size;
			m_Position += size;

			CORE::C_Convert::ConvertCharsToUnicode(&out, charBuffer, size);

			memcpy (buffer,out.c_str(), size);

			return size;
		}

		//------------------------------------------------------------------------------------------------------------------------

		long int C_CompressFile::Read (char* buffer, unsigned int size)
		{
			if (m_Position + size > (unsigned long int)m_Size)
			{
				memcpy (buffer, m_FilePointer, m_Size - m_Position);

				long int readed = m_Size - m_Position;

				m_FilePointer += m_Size - m_Position;
				m_Position += m_Size - m_Position;

				return readed;
			}
			else
			{
				memcpy (buffer, m_FilePointer, size);

				m_FilePointer += size;
				m_Position += size;

				return size;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		long int C_CompressFile::ReadLine(std::string& buffer, char delimiter)
		{
			long int readed = 0;

			buffer.clear();
			
			while (*m_FilePointer != delimiter)
			{
				buffer += (*m_FilePointer);

				m_FilePointer ++;
				m_Position ++;
				readed ++;
			}

			return readed;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_CompressFile::Put (const char* data, unsigned int size)
		{
			if (size != m_Size)
			{
				REDEEMER_SAFE_DELETE_ARRAY (m_Data);

				m_Size = size;
				m_Data = new char[m_Size];
				m_FilePointer = m_Data;

				memcpy (m_FilePointer, data, size);
			}
			else
			{
				memcpy (m_FilePointer, data, size);

				m_FilePointer += size;
				m_Position += size;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		const std::wstring& C_CompressFile::GetFileName () const
		{
			return m_FileName;
		}

		//------------------------------------------------------------------------------------------------------------------------

		DWORD C_CompressFile::GetFileOpenMode () const
		{
			return (FOM_Write | FOM_Read | FOM_Binary);
		}

		//------------------------------------------------------------------------------------------------------------------------

		long int C_CompressFile::GetSize () const
		{
			return m_Size;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::wstring C_CompressFile::GetBuffer () const
		{
			std::wstring out;

			CORE::C_Convert::ConvertCharsToUnicode (&out, m_Data, m_Size);
			
			return out;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::string C_CompressFile::GetData ()
		{
			std::string result (m_Data);

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		char* C_CompressFile::GetDataPointer ()
		{
			return m_Data;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_CompressFile::Eof () const
		{
			if (m_Position >= m_Size)
				return true;
			else
				return false;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_CompressFile::IsValid () const
		{
			return !Eof ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_CompressFile::IsOpened () const
		{
			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_CompressFile::Seek (int position, EFileSeekDirection direction)
		{
			switch (direction)
			{
			case FSD_Begin: 
				m_FilePointer = m_Data + position;
				m_Position = position;
				break;

			case FSD_Current: 
				m_FilePointer += position;
				m_Position += position;
				break;

			case FSD_End: 
				m_FilePointer = m_Data + m_Size - position;
				m_Position = m_Size - m_Position;
				break;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		long int C_CompressFile::Tell ()
		{	
			return m_Position;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace FILESYSTEM
}	//	namespace REDEEMER

