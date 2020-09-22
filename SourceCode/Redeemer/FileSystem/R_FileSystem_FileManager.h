/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_FileSystem_FileManager.h
**		File manager keeps tracks about currently opened files
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_FILESYSTEM_FILEMANAGER_H_
#define _R_FILESYSTEM_FILEMANAGER_H_

#include "R_FileSystem_DataFile.h"
#include "..\\Core\\R_Core_Singleton.h"

#include <map>

namespace REDEEMER
{
	namespace FILESYSTEM
	{	 	
		class C_PackFile;
		class C_XMLDocument;

		/*!	Type of file to open within file manager
		*/
		enum EPhysicalFileType
		{
			PFT_Unicode,
			PFT_ANSI
		};

		/*!
		**	\brief C_FileManager keeps tracks about currently opened files, physical files, memory files or archives
		*/
		class C_FileManager : public CORE::C_Singleton<C_FileManager>
		{
		public:
			/*!	Constructor
			*/
			C_FileManager ();

			/*!	Destructor
			*/
			virtual ~C_FileManager ();

			/*! Opens a file physically stored on a disk
			**	\remarks File opened using this method has to be closed using C_FileManager::CloseFile () method.
			*/
			IDataFile* OpenPhysicalFile (const std::wstring& fileName, DWORD mode, EPhysicalFileType = PFT_Unicode);

			/*! Opens an PACK archive file
			**	\remarks File opened using this method has to be closed using C_FileManager::CloseFile () method.
			*/
			C_PackFile* OpenPACKFile (const std::wstring& fileName, DWORD mode);

			/*!	Creates new memory file
			**	\remarks File opened using this method has to be closed using C_FileManager::CloseFile () method.
			*/
			IDataFile* CreateMemoryFile (const std::wstring& fileName, DWORD mode);

			/*!	Adds new file to the manager
			**	\return True if file can be added
			*/
			bool AddFile (IDataFile* file);

			/*!	Removes file from the the manager
			**	\return True if file can be removed
			*/
			bool RemoveFile (IDataFile* file);

			/*!	Removes file from the the manager
			**	\return True if file can be removed
			*/
			bool RemoveFile (std::wstring& fileName);

			/*!	Closes the file.
			*/
			void CloseFile (IDataFile* file);

			/*!	Close all files.
			**	This function is called automatically on C_FileManager destruction.
			*/
			void CloseAllFiles ();

			/*!	Creates new XML file and tries to parse it			
			*/
			C_XMLDocument* CreateXMLDocument (IDataFile* file);

		private:			
			std::map <std::wstring, IDataFile*>		m_Files;		///<	Map holding all currently opened files
			std::map <std::wstring, C_PackFile*>	m_PACKFiles;	///<	Map holding all currently opened archives
		};

	}	//	namespace FILESYSTEM
}	//	namespace REDEEMER

#endif	//	_R_FILESYSTEM_FILEMANAGER_H_
