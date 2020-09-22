/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Audio_SoundBuffer.h
**		Storage for audio samples defining a sound
*/

#ifndef _R_AUDIO_SOUNDBUFFER_H_
#define _R_AUDIO_SOUNDBUFFER_H_

//------------------------------------------------------------------------------------------------------------------------
#include "R_Audio_SoundFormat.h"

#include <set>
#include <vector>

/** unsigned 32-bit integer */
typedef unsigned int ALuint;

namespace REDEEMER
{
	namespace FILESYSTEM
	{
		class IDataFile;
	}

	namespace AUDIO
	{
		class C_AudioSound;
		class C_AudioStream;
		class C_AudioManager;

		/*!	Storage for audio samples defining a sound
		*/
		class C_SoundBuffer
		{
		public:
			/*!	Default constructor
			*/
			C_SoundBuffer ();

			/*!	Copy constructor
			*/
			C_SoundBuffer (const C_SoundBuffer& copy);

			/*!	Destructor
			*/
			virtual ~C_SoundBuffer ();

			/*!	Overload of assignment operator
			**	\param right Instance to assign
			*/
			C_SoundBuffer& operator =(const C_SoundBuffer& right);

			/*!	Load the sound buffer from a file
			**	Currently supported formats are: WAV and OGG
			**	\param file File with the sound data
			**	\return True if loading succeeded, false if it failed
			*/
			bool LoadFromFile (FILESYSTEM::IDataFile* file);

		private:			
			friend class C_AudioSound;
			friend class C_AudioStream;
			friend class C_AudioManager;

			static int GetFormatFromChannelsCount (unsigned int channelsCount, ESampleFormat format);

			/*!	Update the internal buffer with the cached audio samples
			**	\param channelsCount Number of channels
			**	\param sampleRate    Sample rate (number of samples per second)
			**	\return True on success, false if any error happened
			*/
			bool Update(unsigned int channelsCount, unsigned int sampleRate, ESampleFormat format);

			/*!	Add a sound to the list of sounds that use this buffer
			**	\param sound Sound instance to attach
			*/
			void AttachSound (C_AudioSound* sound) const;

			/*!	Remove a sound from the list of sounds that use this buffer
			**	\param sound Sound instance to detach
			*/
			void DetachSound (C_AudioSound* sound) const;

			ALuint							m_BufferID;	///<	OpenAL buffer identifier
			std::vector<char>				m_Samples;  ///<	Samples buffer
			float							myDuration; ///<	Sound duration, in seconds
			mutable std::set<C_AudioSound*>	m_Sounds;   ///<	List of sounds that are using this buffer
			std::wstring					m_FileName;	///<	Name of the loaded file
		};

	}	//	namespace AUDIO
}	//	namespace REDEEMER

#endif	//	_R_AUDIO_SOUNDBUFFER_H_
