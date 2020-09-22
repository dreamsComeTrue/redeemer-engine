/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_FileSystem_FileManager.cpp
**		File manager keeps tracks about currently opened files
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_FileSystem_FileManager.h"
#include "R_FileSystem_PhysicalFileANSI.h"
#include "R_FileSystem_PhysicalFileUNICODE.h"
#include "R_FileSystem_MemoryFile.h"
#include "R_FileSystem_PackFile.h"
#include "R_FileSystem_XMLDocument.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace FILESYSTEM
	{	 	
		//------------------------------------------------------------------------------------------------------------------------
		C_FileManager::C_FileManager ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_FileManager::~C_FileManager ()
		{
			CloseAllFiles ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		IDataFile* C_FileManager::OpenPhysicalFile (const std::wstring& fileName, DWORD mode, EPhysicalFileType type)
		{
			if (m_Files.find (fileName) != m_Files.end())
				return (m_Files[fileName]);

			switch (type)
			{
			case PFT_ANSI:
				m_Files[fileName] = new C_PhysicalFileANSI (fileName, mode);

				break;

			case PFT_Unicode:
				m_Files[fileName] = new C_PhysicalFileUNICODE (fileName, mode);

				break;
			}

			return (m_Files[fileName]);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_PackFile* C_FileManager::OpenPACKFile (const std::wstring& fileName, DWORD mode)
		{
			if (m_PACKFiles.find (fileName) != m_PACKFiles.end())
				return (m_PACKFiles [fileName]);
											 
			C_PackFile* packFile = new C_PackFile (fileName, mode);

			m_PACKFiles[fileName] = packFile;
			m_Files[fileName] = packFile;

			return packFile;
		}

		//------------------------------------------------------------------------------------------------------------------------

		IDataFile* C_FileManager::CreateMemoryFile (const std::wstring& fileName, DWORD mode)
		{
			if (m_Files.find (fileName) != m_Files.end())
				return (m_Files[fileName]);

			m_Files[fileName] = new C_MemoryFile (fileName, mode);

			return (m_Files[fileName]);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_FileManager::CloseFile (IDataFile* file)
		{
			// No file found, so exit
			for (std::map<std::wstring, IDataFile*>::iterator it = m_Files.begin(); it != m_Files.end (); it ++)
			{
				if (it->second == file)
				{
					it->second->Close ();				
					REDEEMER_SAFE_DELETE (it->second);
					m_Files.erase(it);

					if (m_PACKFiles.find(it->second->GetFileName()) != m_PACKFiles.end())
						m_PACKFiles.erase(m_PACKFiles.find (it->second->GetFileName()));

					break;
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_FileManager::CloseAllFiles ()
		{
			//	Close the files and then remove them
			if (!m_Files.empty())
			{
				for (std::map<std::wstring, IDataFile*>::iterator it = m_Files.begin(); it != m_Files.end (); it ++)
				{
					it->second->Close ();
					REDEEMER_SAFE_DELETE(it->second);
				}

				m_Files.clear ();
				m_PACKFiles.clear();
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_FileManager::AddFile (IDataFile* file)
		{
			if (m_Files.find (file->GetFileName()) != m_Files.end())
				return false;

			m_Files[file->GetFileName()] = file;
							 
			return  true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_FileManager::RemoveFile (IDataFile* file)
		{
			for (std::map<std::wstring, IDataFile*>::iterator it = m_Files.begin(); it != m_Files.end (); it ++)
			{
				if (it->second == file)
				{
					m_Files.erase(it);

					if (m_PACKFiles.find(it->second->GetFileName()) != m_PACKFiles.end())
						m_PACKFiles.erase(m_PACKFiles.find (it->second->GetFileName()));

					REDEEMER_SAFE_DELETE(it->second);

					return true;
				}
			}

			return false;			
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_FileManager::RemoveFile (std::wstring& fileName)
		{
			for (std::map<std::wstring, IDataFile*>::iterator it = m_Files.begin(); it != m_Files.end (); it ++)
			{
				if (it->first == fileName)
				{
					m_Files.erase(it);

					if (m_PACKFiles.find(it->second->GetFileName()) != m_PACKFiles.end())
						m_PACKFiles.erase(m_PACKFiles.find (it->second->GetFileName()));

					REDEEMER_SAFE_DELETE(it->second);

					return true;
				}
			}

			return false;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_XMLDocument* C_FileManager::CreateXMLDocument (IDataFile* file)
		{
			C_XMLDocument* document = new C_XMLDocument ();

			if (!document->OpenFile(file))
			{
				REDEEMER_SAFE_DELETE (document);

				return NULL;
			}

			return document;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace FILESYSTEM
}	//	namespace REDEEMER

