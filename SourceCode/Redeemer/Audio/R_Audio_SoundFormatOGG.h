/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Audio_SoundFormatOG.h
**		OGG file format handling
*/

#ifndef _R_AUDIO_SOUNDFORMATOGG_H_
#define _R_AUDIO_SOUNDFORMATOGG_H_

#include "R_Audio_SoundFormat.h"

//------------------------------------------------------------------------------------------------------------------------
struct OggVorbis_File;
typedef __int64 ogg_int64_t;

namespace REDEEMER
{
	namespace AUDIO
	{
		/*!	OGG file format handling
		*/
		class C_SoundFormatOGG : public C_SoundFormat
		{
		public:
			/*!	Constructor
			*/
			C_SoundFormatOGG ();

			/*!	Destructor
			*/
			virtual ~C_SoundFormatOGG ();

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
			static size_t FileRead (void* buffer, size_t size, size_t n, void* opaque);
			static int    FileSeek (void* opaque, ogg_int64_t offset, int whence);
			static int    FileClose (void* opaque);
			static long   FileTell (void* opaque);

		private:
			OggVorbis_File*			m_VorbisFile;

			//	From format chunk
			int						m_ChannelCount;
			int						m_SampleRate;
			ESampleFormat			m_SampleFormat;

			FILESYSTEM::IDataFile*	m_File;
		};

	}	//	namespace AUDIO
}	//	namespace REDEEMER

#endif	//	_R_AUDIO_SOUNDFORMATOGG_H_
