		  /*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_FileSystem_PackFile.cpp
**		A C_PackFiles are used as archives containing data
*/

#include "R_FileSystem_PackFile.h"
#include "..\\Core\\R_Core_Cipher.h"
#include "..\\Core\\R_Core_Convert.h"
#include "..\\RedeemerEngine.h"

#include "zlib.h"

#define COMPRESS_BLOCK_SIZE 64

namespace REDEEMER
{
	namespace FILESYSTEM
	{	 	
		//------------------------------------------------------------------------------------------------------------------------
		const char C_PackFile::s_CurrentArchiveVersion[6] = {"PACK1"};

		//------------------------------------------------------------------------------------------------------------------------
		C_PackFile::C_PackFile (const std::wstring& fileName, DWORD mode) : 
			C_PhysicalFileANSI (fileName, mode),
			m_DataOffset (0),
			m_FilesCount (0)
		{				 
			Open (fileName, mode);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PackFile::C_PackFile (const C_PackFile& src) :
			C_PhysicalFileANSI (src),
			m_DataOffset (0),
			m_FilesCount (0)
		{
			Open (src.m_FileName, src.m_OpenMode);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PackFile& C_PackFile::operator= (const C_PackFile& rhs)
		{
			if(this == &rhs)
				return (*this);

			C_PhysicalFileANSI::operator=(rhs);

			Open (rhs.m_FileName, rhs.m_OpenMode);

			m_DataOffset = 0;
			m_FilesCount = 0;
			
			return (*this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PackFile::~C_PackFile ()
		{			
			//	Close the files and then remove them
			if (!m_OpenedFiles.empty())
			{
				for (std::map<std::string, C_CompressFile*>::iterator it = m_OpenedFiles.begin(); it != m_OpenedFiles.end (); it ++)
				{
					it->second->Close ();
					REDEEMER_SAFE_DELETE(it->second);
				}

				m_OpenedFiles.clear();
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PackFile::Open (const std::wstring& fileName, DWORD mode)
		{				 
			if (mode & FOM_Read)
			{
				memset (m_ArchiveVersion,  0, sizeof (m_ArchiveVersion));

				Read ((char*)&m_ArchiveVersion, 5 * sizeof(char));
				Read ((char*)&m_FilesCount, sizeof(unsigned int));	 

				//	Are we reading correct archive version?
				if (strcmp (m_ArchiveVersion, s_CurrentArchiveVersion) != 0)
				{
					REDEEMER_LOG << LOG_WARNING << L"Archive version number: '" << m_ArchiveVersion << "' differs that archive version currently" << 
						" supported by engine!" << LOG_ENDMESSAGE;
				}				
					
				//	Now, we can retrieve files' headers
				for (unsigned int i = 0; i < m_FilesCount; i ++)
				{
					short fileNameLength = NULL;

					Read ((char*)&fileNameLength, sizeof(short));

					char* archiveFileName = new char[fileNameLength + 1];

					Read(archiveFileName, fileNameLength);

					archiveFileName[fileNameLength] = '\0';

					//	We need to decipher file name
					CORE::C_Cipher::XOR (archiveFileName, s_CurrentArchiveVersion);

					S_FileHeader header;

					Read ((char*)&header.offset, sizeof(unsigned long int));
					Read ((char*)&header.length, sizeof(unsigned long int));
					Read ((char*)&header.packedSize, sizeof(unsigned long int));

					m_FilesHeaders[archiveFileName] = header;

					REDEEMER_SAFE_DELETE_ARRAY (archiveFileName);
				}
			}

			m_DataOffset = sizeof (unsigned int) + 5 * sizeof(char);		
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PackFile::Close()
		{
			if (m_OpenMode & FOM_Write)
			{			
				C_PhysicalFileANSI::Seek (0, FSD_Begin);

				//	Write archive file header
				Put ((char*)&s_CurrentArchiveVersion, 5 * sizeof(char));
				Put ((char*)&m_FilesCount, sizeof (unsigned int));

				//	Compress & write each file
				for (std::map<std::string, S_FileHeader>::iterator it = m_FilesHeaders.begin(); it != m_FilesHeaders.end(); it ++)
				{
					C_CompressFile& compressFile = m_Files[it->second.entry];

					S_FileHeader& header = it->second;

					header.length = compressFile.GetSize();

					compressFile.Seek(0, FSD_Begin);

					long int length = compressFile.GetSize();
					unsigned long destLength = static_cast<unsigned long>(header.length * 1.1 + 12);

					char* outputBlock = new char[destLength];			
					char* inputBlock = new char[length + 1];

					memset (outputBlock, 0, destLength);					
					memcpy (inputBlock, compressFile.GetDataPointer(), length);

					int zError = compress ((unsigned char*)outputBlock, &destLength, (unsigned char*)inputBlock, length);

					switch (zError)
					{
					//	no enough memory
					case Z_MEM_ERROR:
						REDEEMER_LOG << LOG_ERROR << L"Archive compression: '" << compressFile.GetFileName() << "' - not enough memory!" << LOG_ENDMESSAGE;
						break;

					//	no enough output buffer
					case Z_BUF_ERROR:
						REDEEMER_LOG << LOG_ERROR << L"Archive compression: '" << compressFile.GetFileName() << "' - not enough output buffer!" << LOG_ENDMESSAGE;
						break;

						//	no error
					case Z_OK:
						break;
					}			

					compressFile.Put (outputBlock, destLength);

					REDEEMER_SAFE_DELETE_ARRAY (inputBlock);
					REDEEMER_SAFE_DELETE_ARRAY (outputBlock);
				}

				// Write files' headers
				unsigned int previousFileSize = 0;

				for (std::map<std::string, S_FileHeader>::iterator it = m_FilesHeaders.begin(); it != m_FilesHeaders.end(); it ++)
				{
					short fileNameLength = (short)it->first.length();

					Put ((char*)&fileNameLength, sizeof(short));

					//	We need to cipher file name
					CORE::C_Cipher::XOR (const_cast<char*>(it->first.c_str()), s_CurrentArchiveVersion);

					Put (it->first.c_str(), fileNameLength);

					S_FileHeader& header = it->second;

					header.offset = m_DataOffset + previousFileSize;
					header.packedSize = m_Files[it->second.entry].GetSize();

					Put((char*)&header.offset, sizeof(unsigned long int));					
					Put((char*)&header.length, sizeof(unsigned long int));
					Put((char*)&header.packedSize, sizeof(unsigned long int));

					previousFileSize += header.packedSize;
				}

				//	Write files' entries
				for (std::map<std::string, S_FileHeader>::iterator it = m_FilesHeaders.begin(); it != m_FilesHeaders.end(); it ++)
				{
					C_CompressFile& compressFile = m_Files [it->second.entry];

					Put((char*)compressFile.GetDataPointer(), compressFile.GetSize());
				}
			}
							   
			C_PhysicalFileANSI::Close ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_CompressFile* C_PackFile::GetCompressFile (const std::string& fileName)
		{
			if (!IsFileAvailable (fileName))
			{
				REDEEMER_LOG << LOG_ERROR << L"File: '" << fileName << "' does not exist in the archive!" << LOG_ENDMESSAGE;

				return NULL;
			}

			S_FileHeader& fileHeader = m_FilesHeaders[fileName];

			// Read archive entry
			Seek (fileHeader.offset, FSD_Begin);
					
			char* data = new char[fileHeader.packedSize];

			Read ((char*)data, fileHeader.packedSize);
					   
			C_CompressFile* compressFile = new C_CompressFile (fileName, data, fileHeader.packedSize);

			REDEEMER_SAFE_DELETE_ARRAY (data);

			compressFile->Seek(0, FSD_Begin);

			unsigned long destLength = fileHeader.length;
			char* inputBlock = new char[fileHeader.packedSize];
			char* outputBlock = new char[destLength];

			memset (inputBlock, 0, sizeof (inputBlock));
			memset (outputBlock, 0, sizeof (inputBlock));

			int readed = compressFile->Read(inputBlock, fileHeader.packedSize);

			int zError = uncompress ((unsigned char*)outputBlock, &destLength, (unsigned char*)inputBlock, fileHeader.packedSize);

			switch (zError)
			{
				//	no enough memory
			case Z_MEM_ERROR:
				REDEEMER_LOG << LOG_ERROR << L"Archive decompression: '" << compressFile->GetFileName() << "' - not enough memory!" << LOG_ENDMESSAGE;
				break;

				//	no enough output buffer
			case Z_BUF_ERROR:
				REDEEMER_LOG << LOG_ERROR << L"Archive decompression: '" << compressFile->GetFileName() << "' - not enough output buffer!" << LOG_ENDMESSAGE;
				break;

				//	no error
			case Z_OK:
				compressFile->Put (outputBlock, destLength);
				break;
			}

			REDEEMER_SAFE_DELETE_ARRAY (inputBlock);
			REDEEMER_SAFE_DELETE_ARRAY (outputBlock);

			m_OpenedFiles[fileName] = compressFile;
					  
			return compressFile;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_PackFile::AddFile (IDataFile* file)
		{
			S_FileHeader header;

			header.length = file->GetSize ();
			header.offset = 0;

			char* data = new char [header.length];

			file->Read (data, header.length);

			std::string fileName (file->GetFileName().begin(), file->GetFileName().end());

			C_CompressFile compressFile (fileName, data, header.length);

			m_Files.push_back(compressFile);

			header.entry = m_FilesCount;			

			m_FilesHeaders[fileName] = header;
								   
			m_DataOffset += sizeof (short) + fileName.length() * sizeof(char) + 3 * sizeof(unsigned long int);

			++m_FilesCount;

			REDEEMER_SAFE_DELETE_ARRAY (data);
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_PackFile::IsFileAvailable (const std::string& fileName)
		{
			return (m_FilesHeaders.find (fileName) != m_FilesHeaders.end ());
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned int C_PackFile::GetFilesCount () const
		{
			return m_FilesCount;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::vector<std::string> C_PackFile::GetFilesNames () const
		{
			std::vector<std::string> out;

			for (std::map<std::string, S_FileHeader>::const_iterator it = m_FilesHeaders.begin(); it != m_FilesHeaders.end(); it ++)
			{
				out.push_back((*it).first);
			}

			return out;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace FILESYSTEM
}	//	namespace REDEEMER

