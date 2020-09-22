/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_FileSystem_FileSystemManager.h
**		FileSystem manager provides functionality layer between engine's file system and engine object
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_FILESYSTEM_FILESYSTEMMANAGER_H_
#define _R_FILESYSTEM_FILESYSTEMMANAGER_H_

#include "R_FileSystem_FileManager.h"
#include "..\\Core\\R_Core_Singleton.h"
#include "..\\Core\\R_Core_SmartPointer.h"
#include "..\\Core\\R_Core_BaseClass.h"

#include <vector>

namespace REDEEMER
{
	namespace FILESYSTEM
	{	 	
		/*!
		**	\brief C_FileSystemManager manager provides functionality layer between engine's file system and engine object
		*/
		class C_FileSystemManager : public CORE::C_Singleton<C_FileSystemManager>, public CORE::C_BaseClass
		{
		public:
			/*!	Constructor
			*/
			C_FileSystemManager ();

			/*!	Destructor
			*/
			virtual ~C_FileSystemManager ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize ();

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Returns reference to underlying file manager object
			*/
			C_FileManager& GetFileManager ();

			//	Some utility functions
			
			/*!	Returns the Path for a File Name.
			*/
			static bool FileUtility_GetPath (const std::wstring& strFileName, std::wstring& strPath);

			/*!	Returns the Name for a File Name.
			*/
			static bool FileUtility_GetName (const std::wstring& strFileName, std::wstring& strName);

			/*!	Returns the Base Name for a File Name.
			*/
			static bool FileUtility_GetBaseName (const std::wstring& strFileName, std::wstring& strBaseName);

			/*!	Returns the Extension for a File Name.
			*/
			static bool FileUtility_GetExtension (const std::wstring& strFileName, std::wstring& strExtension);

			/*!	Determines whether the specified file name is absolute.
			*/
			static bool FileUtility_IsAbsoluteFileName (const std::wstring& strFileName);

			/*!	(Always) removes a trailing Path Separator Char.
			*/
			static std::wstring FileUtility_WithoutTrailingSeparator (const std::wstring& strFileName, bool bForce);

			/*!	Determines if the specified File Name represents a Root Directory.
			*/
			static bool FileUtility_IsRootDir (const std::wstring& strFileName);

			/*!	Determines if given file exists
			*/
			static bool FileUtility_FileExists (const std::wstring& strFileName);

			/*!	Removes file
			*/
			static bool FileUtility_Unlink (std::wstring strAbsoluteFileName);	

			/*!	Renames file
			*/
			static bool FileUtility_Rename (std::wstring strAbsoluteFileName, std::wstring strTo);	

			/*!	Creates new directory
			*/
			static bool FileUtility_MakeDirectory (std::wstring strAbsoluteDirName);				

			/*!	Removes directory
			*/
			static bool FileUtility_RemoveDirectory (std::wstring strAbsoluteDirName);				

			/*!	Checks, if there is a directory
			*/
			static bool FileUtility_DirectoryExists (std::wstring strAbsoluteDirName);				

			/*!	Returns files contained within a folder
			*/
			std::vector<std::wstring> GetFileListInFolder(std::wstring strPath, std::wstring strExt);

		private:
			CORE::C_SmartPointer<C_FileManager>	m_FileManager;
		};

	}	//	namespace FILESYSTEM
}	//	namespace REDEEMER

#endif	//	_R_FILESYSTEM_FILESYSTEMMANAGER_H_
