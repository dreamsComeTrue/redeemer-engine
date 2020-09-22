		  /*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_FileSystem_PackFile.h
**		A C_PackFiles are used as archives containing data
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_FILESYSTEM_PACKFILE_H_
#define _R_FILESYSTEM_PACKFILE_H_

#include "R_FileSystem_PhysicalFileANSI.h"
#include "R_FileSystem_CompressFile.h"

#include <map>
#include <vector>

namespace REDEEMER
{
	namespace FILESYSTEM
	{	 	
		/*!
		**	\brief A C_PackFiles are used as archives containing data
		**
			Default file archive is pretty easy and use following format:
			[File header section]
			version ID - 5 chars
			total number of files - 4 bytes

			[File table section]
			File1 name length - 2 bytes
			File1 name - File1 name length bytes
			File1 data offset - 4 bytes
			File1 size - 4 bytes

			File2 name length - 2 bytes
			File2 name - File2 name length bytes
			File2 data offset - 4 bytes
			File2 size - 4 bytes
			...
			FileN name length - 2 bytes
			FileN name - FileN name length bytes
			FileN data offset - 4 bytes
			FileN size - 4 bytes

			[File data section]
			File1 data - File1 size bytes
			File2 data - File2 size bytes
			...
			FileN data - FileN size bytes
		*/
		class C_PackFile : public C_PhysicalFileANSI
		{
		public:
			/*!	This structure describes file header.
			*/
			struct S_FileHeader
			{
				unsigned long int offset;		///<	Offset in file
				unsigned long int length;		///<	Length of the file
				unsigned long int packedSize;	///<	Packed size of the file
				unsigned long int entry;		///<	Entry ID
			};

			/*!	Constructs the File and opens it.
			**	\param fileName wide char file name.
			**	\param mode combination of flags of FileOpenMode type. You can combine any of them.
			*/
			C_PackFile (const std::wstring& fileName, DWORD mode);

			/*!	Copy constructor.
			*/
			C_PackFile (const C_PackFile& src);

			/*!	Assignment operator.
			*/
			C_PackFile& operator= (const C_PackFile& rhs);

			/*!  Default destructor. 
			*/
			virtual ~C_PackFile ();

			/*!	Opens the File.
			**	\param fileName name of the file to open.
			**	\param mode combination of flags of OPEN_MODE type. You
				can combine any of them.
			*/
			virtual void Open (const std::wstring& fileName, DWORD mode);

			/*!	Closes the file.
			**	\remarks This function is automatically called on object's destruction.
			*/
			virtual void Close ();

			/*!	Returns C_CompressFile entry withing given name
			**	\param fileName name of the file to retrieve
			*/
			C_CompressFile* GetCompressFile (const std::string& fileName);

			/*!	Adds file to archive
			**	\param	file file to add
			*/
			void AddFile (IDataFile* file);

			/*!	Returns true if file is available in the archive
			**	\param fileName file name to search
			*/
			bool IsFileAvailable (const std::string& fileName);

			/*!	Returns list of files' names
			*/
			std::vector<std::string> GetFilesNames () const;

			/*!	Returns number of files within the archive
			*/
			unsigned int GetFilesCount () const;

		private:
			unsigned int							m_FilesCount;				///<	How many files are there in a PACK file
			unsigned long int						m_DataOffset;
			char									m_ArchiveVersion[6];		///<	Version ID
			std::map<std::string, S_FileHeader>		m_FilesHeaders;				///<	Headers of files within the archive
			std::map<std::string, C_CompressFile*>	m_OpenedFiles;
			std::vector<C_CompressFile>				m_Files;					///<	Actual files
													
			static const char						s_CurrentArchiveVersion[6];	///<	Currently supported archive number

		};

	}	//	namespace FILESYSTEM
}	//	namespace REDEEMER

#endif	//	_R_FILESYSTEM_PACKFILE_H_
