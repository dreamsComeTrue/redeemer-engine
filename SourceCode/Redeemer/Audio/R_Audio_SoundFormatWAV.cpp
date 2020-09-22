/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Audio_SoundFormatWAV.cpp
**		WAV file format handling
*/

#include "R_Audio_SoundFormatWAV.h"
#include "..\\RedeemerEngine.h"
#include "..\\FileSystem\\R_FileSystem_Logger.h"
#include "..\\Core\\Math\\R_Core_MathUtils.h"


//------------------------------------------------------------------------------------------------------------------------
namespace REDEEMER
{
	namespace AUDIO
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_SoundFormatWAV::C_SoundFormatWAV () :
			m_File (NULL),
			m_ChannelCount (0),
			m_SampleRate (0),
			m_SampleFormat (SF_U8),
			m_DataChunkLocation (0),
			m_DataChunkLength (0),
			m_FramesLeftInChunk (0)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_SoundFormatWAV::~C_SoundFormatWAV ()
		{
			m_File = 0;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_SoundFormatWAV::Initialize(FILESYSTEM::IDataFile* file)
		{
			m_File = file;

			m_File->Seek(0, FILESYSTEM::FSD_Begin);

			// read the RIFF header
			char riffID[4];
			unsigned __int8 riffLengthBuffer[4];
			char riffDataType[4];

			unsigned __int32 size = 0;
			size += file->Read ((char*)riffID, 4);
			size += file->Read ((char*)riffLengthBuffer, 4);
			size += file->Read ((char*)riffDataType, 4);

			int riffLength = Read32 (riffLengthBuffer);

			if (size != 12 || memcmp (riffID, "RIFF", 4) != 0 || riffLength == 0 || memcmp (riffDataType, "WAVE", 4) != 0) 
			{
				REDEEMER_LOG << LOG_WARNING << "Couldn't read RIFF header for file: '" << file->GetFileName() << "'." << LOG_ENDMESSAGE;

				// so we don't destroy the file
				m_File = 0;
				
				return false;
			}

			if (FindFormatChunk() && FindDataChunk()) 
			{
				return true;
			} 
			else
			{
				m_File = 0;

				return false;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_SoundFormatWAV::GetFormat (int& channelCount, int& sampleSize, ESampleFormat& sampleFormat)
		{
			channelCount = m_ChannelCount;
			sampleSize   = m_SampleRate;
			sampleFormat = m_SampleFormat;
		}

		//------------------------------------------------------------------------------------------------------------------------

		long int C_SoundFormatWAV::GetLength() 
		{
			return m_DataChunkLength;
		}

		//------------------------------------------------------------------------------------------------------------------------
		std::ofstream plik;

		long int C_SoundFormatWAV::Read (int frameCount, void* buffer)
		{
			if (m_FramesLeftInChunk == 0) 
			{
				return 0;
			}

			const long int frames_to_read = CORE::C_MathUtils::Min<long int>(frameCount, m_FramesLeftInChunk);
			const long int frame_size = m_ChannelCount * GetSampleSize (m_SampleFormat);
			const long int bytes_to_read = frames_to_read * frame_size;

			const long int read = m_File->Read ((char*)buffer, bytes_to_read);
			const long int frames_read = read / frame_size;

			// assume that if we didn't get a full read, we're done
			if (read != bytes_to_read) 
			{
				m_FramesLeftInChunk = 0;

				return frames_read;
			}

			m_FramesLeftInChunk -= frames_read;

			return frames_read;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_SoundFormatWAV::Seek (int offset, FILESYSTEM::EFileSeekDirection direction)
		{
			m_File->Seek(offset, direction);
		}

		//------------------------------------------------------------------------------------------------------------------------
		unsigned __int16 C_SoundFormatWAV::Read16 (const unsigned char* b) 
		{												  
			return b[0] + (b[1] << 8);
		}

		//------------------------------------------------------------------------------------------------------------------------
		unsigned __int32 C_SoundFormatWAV::Read32 (const unsigned char* b) 
		{				
			return (b[0] + (b[1] << 8)) + (((b + 2)[0] + ((b + 2)[1] << 8)) << 16);
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_SoundFormatWAV::FindFormatChunk ()
		{
			// seek to just after the RIFF header
			m_File->Seek (12, FILESYSTEM::FSD_Begin);

			// search for a format chunk
			for (;;) 
			{
				char chunk_id[4];
				unsigned __int8 chunk_length_buffer[4];

				int size = m_File->Read ((char*)chunk_id, 4);
				size    += m_File->Read ((char*)chunk_length_buffer, 4);
				unsigned __int32 chunk_length = Read32(chunk_length_buffer);

				// if we couldn't read enough, we're done
				if (size != 8) 
				{
					return false;
				}

				// if we found a format chunk, excellent!
				if (memcmp (chunk_id, "fmt ", 4) == 0 && chunk_length >= 16) 
				{
					// read format chunk
					unsigned __int8 chunk[16];

					size = m_File->Read ((char*)chunk, 16);

					// could we read the entire format chunk?
					if (size < 16) 
					{
						return false;
					}

					chunk_length -= size;

					// parse the memory into useful information
					unsigned __int16 format_tag         = Read16 (chunk + 0);
					unsigned __int16 channel_count      = Read16 (chunk + 2);
					unsigned __int32 samples_per_second = Read32 (chunk + 4);
					//u32 bytes_per_second   = read32_le(chunk + 8);
					//u16 block_align        = read16_le(chunk + 12);
					unsigned __int16 bits_per_sample    = Read16 (chunk + 14);

					// format_tag must be 1 (WAVE_FORMAT_PCM)
					// we only support mono and stereo
					if (format_tag != 1 || channel_count > 2 ||	!((bits_per_sample == 8 || bits_per_sample == 16)))
					{
							REDEEMER_LOG << LOG_WARNING << "Invalid WAV file: '" << m_File->GetFileName() << "'." << LOG_ENDMESSAGE; 

							return false;
					}

					// skip the rest of the chunk
					if (!SkipBytes (chunk_length))
					{
						// oops, end of stream
						return false;
					}

					// figure out the sample format
					if (bits_per_sample == 8) 
					{
						m_SampleFormat = SF_U8;
					} 
					else 
						if (bits_per_sample == 16)
						{
							m_SampleFormat = SF_S16;
						} 
						else 
						{
							return false;
						}
									   
					// store the other important .wav attributes
					m_ChannelCount = channel_count;
					m_SampleRate   = samples_per_second;
					return true;

				}
				else 
				{
					// skip the rest of the chunk
					if (!SkipBytes (chunk_length)) 
					{
						// oops, end of stream
						return false;
					}
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_SoundFormatWAV::FindDataChunk ()
		{
			// seek to just after the RIFF header
			m_File->Seek (12, FILESYSTEM::FSD_Begin);

			// search for a data chunk
			while (true) 
			{
				char chunk_id[4];
				unsigned __int8   chunk_length_buffer[4];

				int size = m_File->Read ((char*)chunk_id, 4);
				size    += m_File->Read ((char*)chunk_length_buffer, 4);
				unsigned __int32 chunk_length = Read32 (chunk_length_buffer);

				// if we couldn't read enough, we're done
				if (size != 8) 
				{
					REDEEMER_LOG << LOG_WARNING << "Couldn't read chunk header: '" << m_File->GetFileName() << "'." << LOG_ENDMESSAGE; 

					return false;
				}

				// if we found a data chunk, excellent!
				if (memcmp(chunk_id, "data", 4) == 0) 
				{
					// calculate the frame size so we can truncate the data chunk
					int frame_size = m_ChannelCount * GetSampleSize (m_SampleFormat);

					m_DataChunkLocation  = m_File->Tell ();
					m_DataChunkLength    = chunk_length;// / frame_size;
					m_FramesLeftInChunk = m_DataChunkLength;

					return true;		 
				} 
				else 
				{
					// skip the rest of the chunk
					if (!SkipBytes (chunk_length))
					{
						// oops, end of stream
						return false;
					}
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_SoundFormatWAV::SkipBytes (int size)
		{
			m_File->Seek (size, FILESYSTEM::FSD_Current);

			return m_File->IsValid();
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace AUDIO
}	//	namespace REDEEMER
