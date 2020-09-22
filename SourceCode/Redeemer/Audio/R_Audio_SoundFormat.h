/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Audio_SoundFormat.h
**		Base class for sound format
*/

#ifndef _R_AUDIO_SOUNDFORMAT_H_
#define _R_AUDIO_SOUNDFORMAT_H_

#include "..\\FileSystem\\R_FileSystem_DataFile.h"

//------------------------------------------------------------------------------------------------------------------------
namespace REDEEMER
{
	namespace AUDIO
	{
		class C_SoundBuffer;

		/*!	Storage formats for sample data.
		*/
		enum ESampleFormat 
		{
			SF_U8,  ///< unsigned 8-bit integer [0,255]
			SF_S16, ///< signed 16-bit integer in host endianness [-32768,32767]
		};

		/*!	Base class for sound format
		*/
		class C_SoundFormat
		{
			friend class C_SoundBuffer;

		public:
			/*!	Constructor
			*/
			C_SoundFormat ();

			/*!	Destructor
			*/
			virtual ~C_SoundFormat ();

			/*!	Initializes file data
			*/
			virtual bool Initialize (FILESYSTEM::IDataFile* file) = 0;

			/*!
			**	Retrieve the number of channels, sample rate, and sample format of the sample source.
			*/
			virtual void GetFormat (int& channelCount, int& sampleSize, ESampleFormat& sampleFormat) = 0;

			/*!	Read frame_count samples into buffer. Buffer must be at least |frame_count * GetSampleSize (format) * channelCount| bytes long.
			**	\param frameCount  number of frames to read
			**	\param buffer buffer to store samples in
			**	\return  number of frames actually read
			*/
			virtual long int Read (int frameCount, void* buffer) = 0;

			/*!	Seeks to given position
			*/
			virtual void Seek (int offset, FILESYSTEM::EFileSeekDirection direction) = 0;

			/*!	Returns sound file length in samples count
			*/
			virtual long int GetLength() = 0;

			/*!	Gets sample size based on it's format
			*/
			static int GetSampleSize (ESampleFormat format);
		};

	}	//	namespace AUDIO
}	//	namespace REDEEMER

#endif	//	_R_AUDIO_SOUNDFORMAT_H_
