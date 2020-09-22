/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Audio_SoundFormatWAV.h
**		WAV file format handling
*/

#ifndef _R_AUDIO_SOUNDFORMATWAV_H_
#define _R_AUDIO_SOUNDFORMATWAV_H_

#include "R_Audio_SoundFormat.h"

//------------------------------------------------------------------------------------------------------------------------
namespace REDEEMER
{
	namespace AUDIO
	{
		/*!	WAV file format handling
		*/
		class C_SoundFormatWAV : public C_SoundFormat
		{
		public:
			/*!	Constructor
			*/
			C_SoundFormatWAV ();

			/*!	Destructor
			*/
			virtual ~C_SoundFormatWAV ();

			/*!	Initializes file data
			*/
			bool Initialize (FILESYSTEM::IDataFile* file);

			/*!
			**	Retrieve the number of channels, sample rate, and sample format of the sample source.
			*/
			virtual void GetFormat (int& channelCount, int& sampleSize, ESampleFormat& sampleFormat);

			/*!	Read frame_count samples into buffer. Buffer must be at least |frame_count * GetSampleSize (format) * channelCount| bytes long.
			**	\param frameCount  number of frames to read
			**	\param buffer buffer to store samples in
			**	\return  number of frames actually read
			*/
			virtual long int Read (int frameCount, void* buffer);

			/*!	Seeks to given position
			*/
			virtual void Seek (int offset, FILESYSTEM::EFileSeekDirection direction);

			/*!	Returns sound file length in samples count
			*/
			virtual long int GetLength ();

		private:
			unsigned __int16 Read16 (const unsigned char* b);
			unsigned __int32 Read32 (const unsigned char* b);
			bool FindFormatChunk ();
			bool FindDataChunk ();
			bool SkipBytes (int size);			

			//	From format chunk
			int						m_ChannelCount;
			int						m_SampleRate;
			ESampleFormat			m_SampleFormat;

			FILESYSTEM::IDataFile*	m_File;

			// from data chunk
			int						m_DataChunkLocation; // bytes
			int						m_DataChunkLength;   // in frames
			int						m_FramesLeftInChunk;
		};

	}	//	namespace AUDIO
}	//	namespace REDEEMER

#endif	//	_R_AUDIO_SOUNDFORMATWAV_H_
