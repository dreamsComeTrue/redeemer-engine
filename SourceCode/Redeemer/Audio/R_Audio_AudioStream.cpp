/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Audio_AudioStream.cpp
**		Streaming media handling
*/

#include "R_Audio_AudioStream.h"
#include "R_Audio_SoundBuffer.h"
#include "R_Audio_SoundFile.h"
#include "..\\RedeemerEngine.h"

#include "al.h"

#define BUFFER_SIZE (4096 * 4)

//--------------------------------------------------------------------------------------------------------------------------------
namespace REDEEMER
{
	namespace AUDIO
	{		 
		//--------------------------------------------------------------------------------------------------------------------------------
		C_AudioStream::C_AudioStream ()	:
			m_IsStreaming     (false),
			m_ChannelsCount   (0),
			m_SampleRate      (0),
			m_Format          (0),
			m_Loop            (false),
			m_SamplesProcessed(0),
			m_RequestStop (false),
			m_SoundFile (new C_SoundFile ())
		{
			//	EMPTY
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		C_AudioStream::~C_AudioStream ()
		{
			// Stop the sound if it was playing
			Stop ();

			REDEEMER_SAFE_DELETE (m_SoundFile);
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		bool C_AudioStream::OpenFromFile (FILESYSTEM::IDataFile* file)
		{
			// First stop the music if it was already running
			Stop ();

			// Create the sound file implementation, and open it in read mode
			if (!m_SoundFile->LoadFromFile (file))
			{
				REDEEMER_LOG << LOG_ERROR << "Failed to open \"" << file->GetFileName() << "\" for reading" << LOG_ENDMESSAGE;

				return false;
			}

			m_FileName = file->GetFileName();

			int channelCount;
			int sampleSize;
			ESampleFormat format;

			m_SoundFile->GetFormat(channelCount, sampleSize, format);

			// Compute the duration
			m_Duration = static_cast<float>(m_SoundFile->GetLength()) / sampleSize / channelCount;

			// Resize the internal buffer so that it can contain 1 second of audio samples
			m_Samples.resize (BUFFER_SIZE * channelCount * C_SoundFormat::GetSampleSize(format));

			// Initialize the stream
			Initialize (channelCount, sampleSize);

			return true;
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		void C_AudioStream::Play ()
		{
			// Check if the sound parameters have been set
			if (m_Format == 0)
			{
				REDEEMER_LOG << LOG_ERROR << "Failed to play audio stream: sound parameters have not been initialized (call Initialize first)" << LOG_ENDMESSAGE;

				return;
			}

			// If the sound is already playing (probably paused), just resume it
			if (m_IsStreaming)
			{
				alSourcePlay (m_SourceID);

				return;
			}

			// Move to the beginning
			Seek (0);

			// Start updating the stream in a separate thread to avoid blocking the application
			m_SamplesProcessed = 0;
			m_IsStreaming = true;

			NewPlay ();
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		void C_AudioStream::Pause ()
		{
			alSourcePause (m_SourceID);
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		void C_AudioStream::Stop ()
		{
			// Wait for the thread to terminate
			m_IsStreaming = false;

			StopPlay();
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		unsigned int C_AudioStream::GetChannelsCount () const
		{
			return m_ChannelsCount;
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		unsigned int C_AudioStream::GetSampleRate () const
		{
			return m_SampleRate;
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		C_SoundSource::ESourceStatus C_AudioStream::GetStatus () const
		{
			ESourceStatus status = C_SoundSource::GetStatus ();

			// To compensate for the lag between Play() and alSourcePlay()
			if ((status == SS_Stopped) && m_IsStreaming)
				status = SS_Playing;

			return status;
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		void C_AudioStream::SetPlayingOffset (float timeOffset)
		{
			// Stop the stream
			Stop ();

			// Let the derived class update the current position
			Seek (timeOffset);

			// Restart streaming
			m_SamplesProcessed = static_cast<unsigned int>(timeOffset * m_SampleRate * m_ChannelsCount);
			m_IsStreaming = true;

			NewPlay();
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		float C_AudioStream::GetPlayingOffset () const
		{
			ALfloat seconds = 0.f;

			alGetSourcef (m_SourceID, AL_SEC_OFFSET, &seconds);

			return seconds + static_cast<float>(m_SamplesProcessed) / m_SampleRate / m_ChannelsCount;
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		void C_AudioStream::SetLoop (bool loop)
		{
			m_Loop = loop;
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		bool C_AudioStream::GetLoop () const
		{
			return m_Loop;
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		float C_AudioStream::GetDuration () const
		{
			 return m_Duration;
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		void C_AudioStream::NewPlay ()
		{
			// Create the buffers
			alGenBuffers (BuffersCount, m_Buffers);

			for (int i = 0; i < BuffersCount; i ++)
				m_EndBuffers[i] = false;

			// Fill the queue
			m_RequestStop = FillQueue();

			// Play the sound
			alSourcePlay (m_SourceID);	
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		void C_AudioStream::StopPlay ()
		{
			// Stop the playback
			alSourceStop (m_SourceID);

			// Unqueue any buffer left in the queue
			ClearQueue();

			// Delete the buffers
			alSourcei (m_SourceID, AL_BUFFER, 0);
			alDeleteBuffers (BuffersCount, m_Buffers);
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		void C_AudioStream::Initialize (unsigned int channelsCount, unsigned int sampleRate)
		{
			m_ChannelsCount = channelsCount;
			m_SampleRate    = sampleRate;
			m_SampleFormat =  SF_S16;

			//	Deduce the format from the number of channels
			m_Format = C_SoundBuffer::GetFormatFromChannelsCount (channelsCount, SF_S16);

			//	Check if the format is valid
			if (m_Format == 0)
			{
				m_ChannelsCount = 0;
				m_SampleRate    = 0;

				REDEEMER_LOG << LOG_ERROR << L"Unsupported number of channels (" << channelsCount << ")" << LOG_ENDMESSAGE;
			}
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		void C_AudioStream::Update ()
		{
			if (m_IsStreaming)
			{
				// The stream has been interrupted!
				if (C_SoundSource::GetStatus() == SS_Stopped)
				{
					if (!m_RequestStop)
					{
						// Just continue
						alSourcePlay (m_SourceID);
					}
					else
					{
						// End streaming
						Stop ();
					}
				}

				// Get the number of buffers that have been processed (ie. ready for reuse)
				ALint nbProcessed;

				alGetSourcei (m_SourceID, AL_BUFFERS_PROCESSED, &nbProcessed);

				while (nbProcessed--)
				{
					// Pop the first unused buffer from the queue
					ALuint buffer;
					
					alSourceUnqueueBuffers (m_SourceID, 1, &buffer);

					// Find its number
					unsigned int bufferNum = 0;

					for (int i = 0; i < BuffersCount; ++i)
						if (m_Buffers[i] == buffer)
						{
							bufferNum = i;
							break;
						}

						// Retrieve its size and add it to the samples count
						if (m_EndBuffers[bufferNum])
						{
							// This was the last buffer: reset the sample count
							m_SamplesProcessed = 0;
							m_EndBuffers[bufferNum] = false;
						}
						else
						{
							ALint size, bits;

							alGetBufferi (buffer, AL_SIZE, &size);
							alGetBufferi (buffer, AL_BITS, &bits);

							m_SamplesProcessed += size / (bits / 8);
						}

						// Fill it and push it back into the playing queue
						if (!m_RequestStop)
						{
							if (FillAndPushBuffer(bufferNum))
								m_RequestStop = true;
						}
				}
			}
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		bool C_AudioStream::GetData (S_Chunk& data)
		{				 
			// Fill the chunk parameters
			data.Samples = &m_Samples[0];

			data.SamplesCount = m_SoundFile->Read (BUFFER_SIZE, &m_Samples[0]);

			// Check if we have reached the end of the audio file
			return data.SamplesCount == BUFFER_SIZE;//m_Samples.size();
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		void C_AudioStream::Seek (float timeOffset)
		{
			if (m_SoundFile)
				m_SoundFile->Seek (timeOffset);
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		bool C_AudioStream::FillAndPushBuffer (unsigned int bufferNum)
		{
			bool requestStop = false;

			// Acquire audio data
			S_Chunk data = {NULL, 0};

			if (!GetData (data))
			{
				// Mark the buffer as the last one (so that we know when to reset the playing position)
				m_EndBuffers[bufferNum] = true;

				// Check if the stream must loop or stop
				if (m_Loop)
				{
					// Return to the beginning of the stream source
					Seek(0);

					// If we previously had no data, try to fill the buffer once again
					if (!data.Samples || (data.SamplesCount == 0))
					{
						return FillAndPushBuffer (bufferNum);
					}
				}
				else
				{
					// Not looping: request stop
					requestStop = true;
				}
			}

			// Fill the buffer if some data was returned
			if (data.Samples && data.SamplesCount)
			{
				unsigned int buffer = m_Buffers[bufferNum];

				// Fill the buffer
				ALsizei size = static_cast<ALsizei>(data.SamplesCount) * sizeof(char);

				alBufferData (buffer, m_Format, data.Samples, size * m_ChannelsCount * C_SoundFormat::GetSampleSize(m_SampleFormat), m_SampleRate);

				// Push it into the sound queue
				alSourceQueueBuffers (m_SourceID, 1, &buffer);
			}

			return requestStop;
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		bool C_AudioStream::FillQueue ()
		{
			// Fill and enqueue all the available buffers
			bool requestStop = false;

			for (int i = 0; (i < BuffersCount) && !requestStop; ++i)
			{
				if (FillAndPushBuffer (i))
					requestStop = true;
			}

			return requestStop;
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		void C_AudioStream::ClearQueue ()
		{
			//	Get the number of buffers still in the queue
			ALint nbQueued;
			
			alGetSourcei (m_SourceID, AL_BUFFERS_QUEUED, &nbQueued);

			// Unqueue them all
			ALuint buffer;

			for (ALint i = 0; i < nbQueued; ++i)
				alSourceUnqueueBuffers (m_SourceID, 1, &buffer);
		}

		//--------------------------------------------------------------------------------------------------------------------------------

	}	//	namespace AUDIO
}	//	namespace REDEEMER

