/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Audio_SoundFormatOG.cpp
**		OGG file format handling
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Audio_SoundFormatOGG.h"
#include "..\\FileSystem\\R_FileSystem_DataFile.h"
#include "..\\RedeemerEngine.h"

#include <vorbis\\vorbisfile.h>

namespace REDEEMER
{
	namespace AUDIO
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_SoundFormatOGG::C_SoundFormatOGG ()  :
			m_File (0),
			m_VorbisFile (0),
			m_ChannelCount (0),
			m_SampleRate (0),
			m_SampleFormat (SF_S16)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_SoundFormatOGG::~C_SoundFormatOGG ()
		{
			if (m_File) 
			{
				ov_clear (m_VorbisFile);

				REDEEMER_SAFE_DELETE (m_VorbisFile);
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_SoundFormatOGG::Initialize (FILESYSTEM::IDataFile* file)
		{
			m_File = file;

			m_File->Seek(0, FILESYSTEM::FSD_Begin);

			m_VorbisFile = new OggVorbis_File ();

			// Custom ogg vorbis callbacks
			ov_callbacks callbacks;

			callbacks.read_func  = FileRead;
			callbacks.seek_func  = FileSeek;
			callbacks.close_func = FileClose;
			callbacks.tell_func  = FileTell;

			// open ogg vorbis stream
			int result = ov_open_callbacks ((void*)m_File, m_VorbisFile, 0, 0, callbacks);

			if (result) 
			{
				m_File = 0;
				REDEEMER_SAFE_DELETE (m_VorbisFile);

				return false;
			}

			// calculate stream type
			vorbis_info* vi = ov_info (m_VorbisFile, -1);

			if (!vi) 
			{
				ov_clear (m_VorbisFile);

				m_File = 0;
				REDEEMER_SAFE_DELETE (m_VorbisFile);

				return false;
			}

			m_ChannelCount = vi->channels;
			m_SampleRate   = vi->rate;
			m_SampleFormat = SF_S16; // see constructor

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_SoundFormatOGG::GetFormat (int& channelCount, int& sampleSize, ESampleFormat& sampleFormat)
		{
			channelCount = m_ChannelCount;
			sampleSize   = m_SampleRate;
			sampleFormat = m_SampleFormat;
		}

		//------------------------------------------------------------------------------------------------------------------------

		long int C_SoundFormatOGG::Read (int frameCount, void* buffer)
		{
			long int sample_size = m_ChannelCount * GetSampleSize (m_SampleFormat);

			unsigned char* out = (unsigned char*)buffer;
			long int samples_left = frameCount;
			long int total_read = 0;

			while (samples_left > 0) 
			{
				// check to see if the stream format has changed
				// if so, treat it as an EndOfStream
				vorbis_info* vi = ov_info (m_VorbisFile, -1);

				if (vi && (m_SampleRate != vi->rate || m_ChannelCount != vi->channels)) 
				{
					break;
				}

				int bitstream;

				long result = ov_read (m_VorbisFile, (char*)out, samples_left * sample_size, 0,	2, 1, &bitstream);

				if (result < 0) 
				{
					// if error, ignore it
					continue;
				} 
				else 
					if (result == 0) 
					{
						break;
					}

				unsigned long samples_read = (unsigned long)(result / sample_size);

				out += samples_read * sample_size;
				samples_left -= samples_read;
				total_read   += samples_read;
			}

			return total_read;	   
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_SoundFormatOGG::Seek(int offset, FILESYSTEM::EFileSeekDirection direction)
		{
			ov_pcm_seek (m_VorbisFile, offset);
		}

		//------------------------------------------------------------------------------------------------------------------------

		long int C_SoundFormatOGG::GetLength ()
		{
			 return static_cast<long int>(ov_pcm_total(m_VorbisFile, -1));
		}

		//------------------------------------------------------------------------------------------------------------------------

		size_t C_SoundFormatOGG::FileRead (void* buffer, size_t size, size_t n, void* opaque)
		{
			FILESYSTEM::IDataFile* file = reinterpret_cast<FILESYSTEM::IDataFile*>(opaque);

			return file->Read ((char*)buffer, size * n) / size;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_SoundFormatOGG::FileSeek (void* opaque, ogg_int64_t offset, int whence)
		{
			FILESYSTEM::IDataFile* file = reinterpret_cast<FILESYSTEM::IDataFile*> (opaque);

			FILESYSTEM::EFileSeekDirection type;

			switch (whence) 
			{
				case SEEK_SET: 
					type = FILESYSTEM::FSD_Begin;   
					break;

				case SEEK_CUR: 
					type = FILESYSTEM::FSD_Current; 
					break;

				case SEEK_END: 
					type = FILESYSTEM::FSD_End;     
					break;

				default: 
					return -1;
			}

			file->Seek ((int)offset, type);

			return (file->IsValid () ? 0 : -1);
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_SoundFormatOGG::FileClose (void* opaque)
		{
			// we don't have to do anything
			// (read: don't trust ogg vorbis with handling file closes)
			return 0;
		}

		//------------------------------------------------------------------------------------------------------------------------

		long C_SoundFormatOGG::FileTell (void* opaque)
		{
			FILESYSTEM::IDataFile* file = reinterpret_cast<FILESYSTEM::IDataFile*>(opaque);

			return file->Tell ();
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace AUDIO
}	//	namespace REDEEMER

