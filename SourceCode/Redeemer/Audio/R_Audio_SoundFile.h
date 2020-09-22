/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Audio_SoundFile.h
**		Provide access to sound files
*/

#ifndef _R_AUDIO_SOUNDFILE_H_
#define _R_AUDIO_SOUNDFILE_H_

//------------------------------------------------------------------------------------------------------------------------
#include "R_Audio_SoundFormat.h"

namespace REDEEMER
{
	namespace FILESYSTEM
	{
		class IDataFile;
	}

	namespace AUDIO
	{
		/*!	Provide access to sound files
		*/
		class C_SoundFile
		{
		public:
			/*!	Default constructor
			*/
			C_SoundFile ();

			/*!	Destructor
			*/
			virtual ~C_SoundFile ();

			/*!	Load the sound buffer from a file
			**	Currently supported formats are: WAV and OGG
			**	\param file File with the sound data
			**	\return True if loading succeeded, false if it failed
			*/
			bool LoadFromFile (FILESYSTEM::IDataFile* file);

			/*!	Read frame_count samples into buffer. 
			**	\param frameCount  number of frames to read
			**	\param buffer buffer to store samples in
			**	\return  number of frames actually read
			*/
			long int Read (int frameCount, void* buffer);

			/*!
			**	Retrieve the number of channels, sample rate, and sample format of the sample source.
			*/
			void GetFormat (int& channelCount, int& sampleSize, ESampleFormat& sampleFormat);
			
			/*!	Returns sound's length - in samples count
			*/
			unsigned long GetLength () const;

			/*!	Seeks to given position
			*/
			void Seek (float timeOffset);

		private:
			/*!	Copy constructor
			*/
			C_SoundFile (const C_SoundFile& copy);

			/*!	Overload of assignment operator
			**	\param right Instance to assign
			*/
			C_SoundFile& operator =(const C_SoundFile& right);

			C_SoundFormat*	m_FileFormat;
		};

	}	//	namespace AUDIO
}	//	namespace REDEEMER

#endif	//	_R_AUDIO_SOUNDFILE_H_
