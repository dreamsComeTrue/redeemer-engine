/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_FileSystem_FileSystemManager.cpp
**		FileSystem manager provides functionality layer between engine's file system and engine object
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_FileSystem_FileSystemManager.h"
#include "R_FileSystem_PhysicalFileUNICODE.h"
#include "..\\RedeemerEngine.h"

#include <windows.h>

#define FILESYSTEM_PATH_SEPARATOR L'\\'

namespace REDEEMER
{
	namespace FILESYSTEM
	{	 	
		//------------------------------------------------------------------------------------------------------------------------
		C_FileSystemManager::C_FileSystemManager ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_FileSystemManager::~C_FileSystemManager ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_FileSystemManager::Initialize ()
		{
			C_BaseClass::Initialize();

			m_FileManager = new C_FileManager ();

			REDEEMER_LOG << LOG_INFO << L"FileSystem Manager initialized!" << LOG_ENDMESSAGE;

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_FileSystemManager::Finalize ()
		{		  
			REDEEMER_LOG << LOG_INFO << L"FileSystem Manager finalized!" << LOG_ENDMESSAGE;

			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_FileManager& C_FileSystemManager::GetFileManager ()
		{
			return *m_FileManager;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_FileSystemManager::FileUtility_GetPath (const std::wstring& strFileName, std::wstring& strPath)
		{
			//	Following exceptions:
			//	Path of "/" is ""
			//	Path of (char)":/" is (""
			if ((strFileName.size() == 1 && strFileName[0] == FILESYSTEM_PATH_SEPARATOR) ||
				(strFileName.size() == 3 && strFileName[1] == L':' && strFileName[2] == FILESYSTEM_PATH_SEPARATOR))
			{
				strPath = L"";
				return true;
			}

			strPath = FileUtility_WithoutTrailingSeparator (strFileName, false);

			//	Is there a trailing backslash?
			if (strPath.rfind (FILESYSTEM_PATH_SEPARATOR) != std::wstring::npos)
			{
				//	Remove all the text starting from the trailing backslash.
				strPath = strPath.substr (0, strPath.rfind (FILESYSTEM_PATH_SEPARATOR));
			}
			else
			{
				strPath = L"";
			}

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_FileSystemManager::FileUtility_GetName (const std::wstring& strFileName, std::wstring& strName)
		{
			//	Following exceptions:
			//	Name of "/" is "/"
			//	Name of (char)":/" is (char)":/"
			if ((strFileName.size() == 1 && strFileName[0] == FILESYSTEM_PATH_SEPARATOR) ||
				( strFileName.size() == 3 && strFileName[1] == L':' && strFileName[2] == FILESYSTEM_PATH_SEPARATOR))
			{
				strName = strFileName;
				return true;
			}

			strName = FileUtility_WithoutTrailingSeparator (strFileName, true);

			//	Is there a trailing backslash?
			if (strName.rfind (FILESYSTEM_PATH_SEPARATOR) != std::wstring::npos)
			{
				//	Remove all the text from the beginning to the trailing backslash.
				strName = strName.substr (strName.rfind (FILESYSTEM_PATH_SEPARATOR) + 1);
			}

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_FileSystemManager::FileUtility_GetBaseName (const std::wstring& strFileName, std::wstring& strBaseName)
		{
			// Get the Name.
			if (!FileUtility_GetName (strFileName, strBaseName))
				return false;

			//	Is there a point in the string
			if (strBaseName.rfind (L'.') != std::wstring::npos)
			{
				//	Remove the Extension.
				strBaseName = strBaseName.substr (0, strBaseName.rfind (L'.'));
			}

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_FileSystemManager::FileUtility_GetExtension (const std::wstring& strFileName, std::wstring& strExtension)
		{
			// Get the Name.
			if (!FileUtility_GetName (strFileName, strExtension))
				return false;

			//	Is there a point in the string
			if (strExtension.rfind (L'.') != std::wstring::npos)
			{
				//	Remove the Extension.
				strExtension = strExtension.substr (strExtension.rfind (L'.') + 1);
			}
			else
			{
				strExtension = L"";
			}

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_FileSystemManager::FileUtility_IsAbsoluteFileName (const std::wstring& strFileName)
		{
			//	There are two possibilities for an absolute File Name.
			//	- <path separator>.......
			//	- <drive letter><path separator>.......
			if (strFileName.size() == 0)
				return false;

			if (strFileName [0] == FILESYSTEM_PATH_SEPARATOR)
				return true;

			if (strFileName.size() < 3)
				return false;

			if (isalpha (strFileName[0]) && strFileName[1] == L':' && strFileName[2] == FILESYSTEM_PATH_SEPARATOR)
				return true;

			return false;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::wstring C_FileSystemManager::FileUtility_WithoutTrailingSeparator (const std::wstring& strFileName, bool bForce)
		{
			std::wstring strResult = strFileName;

			//	If we are in the root directory, then do just nothing :)
			if (strFileName.size() == 0 || (!bForce && FileUtility_IsRootDir (strFileName)))
				return strResult;

			if (*(strResult.end() -1) == FILESYSTEM_PATH_SEPARATOR)
				return strResult.substr (0, strResult.size() - 1);

			return strResult;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_FileSystemManager::FileUtility_IsRootDir (const std::wstring& strFileName)
		{
			if (strFileName.size() == 0)
				return false;

			if (strFileName.size() == 1 && strFileName[1] == FILESYSTEM_PATH_SEPARATOR)
				return true;

			if (strFileName.size() == 3 && isalpha (strFileName[0] ) && strFileName[1] == L':' && strFileName[2] == FILESYSTEM_PATH_SEPARATOR)
				return true;

			return false;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_FileSystemManager::FileUtility_FileExists (const std::wstring& strFileName)
		{
			C_PhysicalFileUNICODE file (strFileName, FOM_Read);

			if (file.IsOpened())
			{
				file.Close();

				return true;
			}

			return false;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_FileSystemManager::FileUtility_Unlink (std::wstring strAbsoluteFileName)
		{
			return (( _wunlink ((strAbsoluteFileName ).c_str()) == 0 ) ? true : false);
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_FileSystemManager::FileUtility_Rename (std::wstring strAbsoluteFileName, std::wstring strTo)
		{
			return ((_wrename ((strAbsoluteFileName ).c_str(), strTo.c_str()) == 0) ? true : false);
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_FileSystemManager::FileUtility_MakeDirectory (std::wstring strAbsoluteDirName)
		{
			return ((_wmkdir ((strAbsoluteDirName ).c_str()) == 0) ? true : false);
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_FileSystemManager::FileUtility_RemoveDirectory (std::wstring strAbsoluteDirName)
		{
			return ((_wrmdir ((strAbsoluteDirName).c_str()) == 0) ? true : false);
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_FileSystemManager::FileUtility_DirectoryExists (std::wstring strAbsoluteDirName)
		{
			return ((GetFileAttributesW ((strAbsoluteDirName ).c_str()) != 0xFFFFFFFF && 
				(GetFileAttributesW (strAbsoluteDirName.c_str()) & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) ? true : false);
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::vector<std::wstring> C_FileSystemManager::GetFileListInFolder(std::wstring strPath, std::wstring strExt)
		{
			std::vector<std::wstring> strArray;

			WIN32_FIND_DATA data;
			wchar_t strFiles[MAX_PATH];
			wchar_t strFile[MAX_PATH];

			wsprintf (strFiles, L"%s*.%s", strPath.c_str(), strExt.c_str());

			HANDLE hfile = FindFirstFile (strFiles, &data);

			do
			{
				if (hfile == INVALID_HANDLE_VALUE) 
					break;

				wsprintf (strFile, L"%s%s", strPath, data.cFileName);
				strArray.push_back (strFile);

			} while (FindNextFile (hfile, &data));

			FindClose (hfile);

			return strArray;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace FILESYSTEM
}	//	namespace REDEEMER
