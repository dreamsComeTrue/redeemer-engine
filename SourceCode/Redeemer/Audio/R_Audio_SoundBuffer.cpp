/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Audio_SoundBuffer.cpp
**		Storage for audio samples defining a sound
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Audio_SoundBuffer.h"
#include "R_Audio_AudioSound.h"
#include "R_Audio_SoundFile.h"

#include "..\\FileSystem\\R_FileSystem_DataFile.h"
#include "..\\RedeemerEngine.h"

#include "al.h"

namespace REDEEMER
{
	namespace AUDIO
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_SoundBuffer::C_SoundBuffer () :
			m_BufferID (0)
		{
			// Create the buffer
			alGenBuffers (1, &m_BufferID);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_SoundBuffer::C_SoundBuffer (const C_SoundBuffer& copy)
		{

		}

		//------------------------------------------------------------------------------------------------------------------------

		C_SoundBuffer::~C_SoundBuffer ()
		{
			//	First detach the buffer from the sounds that use it (to avoid OpenAL errors)
			for (std::set<C_AudioSound*>::const_iterator it = m_Sounds.begin(); it != m_Sounds.end(); ++it)
				(*it)->ResetBuffer();

			// Destroy the buffer
			if (m_BufferID)
				alDeleteBuffers (1, &m_BufferID);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_SoundBuffer& C_SoundBuffer::operator =(const C_SoundBuffer& right)
		{
			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_SoundBuffer::LoadFromFile (FILESYSTEM::IDataFile* file)
		{
			C_SoundFile soundFile;

			if (!soundFile.LoadFromFile(file))
				return false;

			m_FileName = file->GetFileName();

			long int samplesCount = soundFile.GetLength ();
			int channelCount = 0;
			int sampleSize = 0;
			ESampleFormat sampleFormat;

			soundFile.GetFormat(channelCount, sampleSize, sampleFormat);
			
			m_Samples.resize (samplesCount * channelCount * C_SoundFormat::GetSampleSize(sampleFormat));			

			long int i;

			if ((i = soundFile.Read (samplesCount, &m_Samples[0])) == samplesCount)
			{
				// Update the internal buffer with the new samples
				return Update (channelCount, sampleSize, sampleFormat);
			}
			else
			{
				return false;
			}	  
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_SoundBuffer::GetFormatFromChannelsCount (unsigned int channelsCount, ESampleFormat format)
		{
			// Find the good format according to the number of channels
			switch (channelsCount)
			{
			case 1:
				if (format == SF_U8)
					return AL_FORMAT_MONO8;
				else
					return AL_FORMAT_MONO16;

			case 2: 
				if (format == SF_U8)
					return AL_FORMAT_STEREO8;
				else
					return AL_FORMAT_STEREO16;

			case 4: 
				return alGetEnumValue ("AL_FORMAT_QUAD16");

			case 6: 
				return alGetEnumValue ("AL_FORMAT_51CHN16");

			case 7: 
				return alGetEnumValue ("AL_FORMAT_61CHN16");

			case 8: 
				return alGetEnumValue ("AL_FORMAT_71CHN16");

			default: 
				return 0;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_SoundBuffer::Update (unsigned int channelsCount, unsigned int sampleRate, ESampleFormat format)
		{
			// Check parameters
			if (!channelsCount || !sampleRate || m_Samples.empty())
				return false;

			// Find the good format according to the number of channels
			ALenum bufferFormat = GetFormatFromChannelsCount (channelsCount, format);

			// Check if the format is valid
			if (bufferFormat == 0)
			{
				REDEEMER_LOG << LOG_ERROR << L"Unsupported number of channels (" << channelsCount << ")" << LOG_ENDMESSAGE;

				return false;
			}
										 
			// Fill the buffer			  
			ALsizei size = static_cast<ALsizei> (m_Samples.size()) * sizeof(char);

			alBufferData (m_BufferID, bufferFormat, &m_Samples[0], size, sampleRate);

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_SoundBuffer::AttachSound (C_AudioSound* sound) const
		{
			 m_Sounds.insert (sound);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_SoundBuffer::DetachSound (C_AudioSound* sound) const
		{
			  m_Sounds.erase (sound);
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace AUDIO
}	//	namespace REDEEMER

