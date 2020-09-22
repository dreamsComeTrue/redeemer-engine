/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Audio_SoundFile.cpp
**		Provide access to sound files
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Audio_SoundFile.h"
#include "R_Audio_SoundFormat.h"
#include "R_Audio_SoundFormatWAV.h"
#include "R_Audio_SoundFormatOGG.h"

#include "..\\FileSystem\\\R_FileSystem_FileSystemManager.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace AUDIO
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_SoundFile::C_SoundFile () : 
			m_FileFormat (NULL)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_SoundFile::~C_SoundFile ()
		{
			REDEEMER_SAFE_DELETE (m_FileFormat);
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_SoundFile::LoadFromFile (FILESYSTEM::IDataFile* file)
		{
			// Extract the extension
			std::wstring extension = L"wav";

			FILESYSTEM::C_FileSystemManager::FileUtility_GetExtension (file->GetFileName(), extension);

			//	Match every supported extension with its format constant
			if (extension == L"wav"   || extension == L"WAV" )
			{
				m_FileFormat = new C_SoundFormatWAV ();
			}

			if (extension == L"ogg"   || extension == L"OGG" )
			{
				m_FileFormat = new C_SoundFormatOGG ();
			}

			return m_FileFormat->Initialize(file);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_SoundFile::Seek (float timeOffset)
		{
			if (m_FileFormat)
			{
				int channelCount;
				int sampleSize;
				ESampleFormat format;

				GetFormat(channelCount, sampleSize, format);

				long int frameOffset = static_cast<long int>(timeOffset * sampleSize);

				m_FileFormat->Seek (frameOffset, FILESYSTEM::FSD_Begin);
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		long int C_SoundFile::Read (int frameCount, void* buffer)
		{
			return m_FileFormat->Read(frameCount, buffer);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_SoundFile::GetFormat (int& channelCount, int& sampleSize, ESampleFormat& sampleFormat)
		{
			if (m_FileFormat)
				m_FileFormat->GetFormat(channelCount, sampleSize, sampleFormat);
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned long C_SoundFile::GetLength () const
		{					 
			if (m_FileFormat)
			{
				return m_FileFormat->GetLength ();
			}

			return 0;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace AUDIO
}	//	namespace REDEEMER
