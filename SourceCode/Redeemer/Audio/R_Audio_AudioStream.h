/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Audio_AudioStream.cpp
**		Streaming media handling
*/

#ifndef _R_AUDIO_AUDIOSTREAM_H_
#define _R_AUDIO_AUDIOSTREAM_H_

#include "R_Audio_SoundSource.h"
#include "R_Audio_SoundFormat.h"

#include <vector>

//------------------------------------------------------------------------------------------------------------------------
namespace REDEEMER
{
	namespace FILESYSTEM
	{
		class IDataFile;
	}

	namespace AUDIO
	{
		class C_SoundFile;
		class C_AudioManager;

		/*!	Streamed music played from an audio file
		*/
		class C_AudioStream : public C_SoundSource
		{
			friend class C_AudioManager;

		public:
			/*!	Structure defining a chunk of audio data to stream
			*/
			struct S_Chunk
			{
				const char*	Samples;		///<	Pointer to the audio samples
				std::size_t	SamplesCount;	///<	Number of samples pointed by Samples
			};


			/*!	Default constructor
			*/
			C_AudioStream ();

			/*!	Destructor
			*/
			virtual ~C_AudioStream ();

			/*!	Open a music from an audio file
			**	This function doesn't start playing the music (call Play () to do so).
			**	Currently supported audio formats are: WAV and OGG
			**	\param file File with the music file to open
			**	\return True if loading succeeded, false if it failed
			*/
			bool OpenFromFile (FILESYSTEM::IDataFile* file);

			/*!	Start or resume playing the audio stream
			**	This function starts the stream if it was stopped, resumes it if it was paused, and does nothing it is it already playing.
			*/
			void Play ();

			/*!	Pause the audio stream
			**	This function pauses the stream if it was playing, otherwise (stream already paused or stopped) it has no effect.
			*/
			void Pause ();

			/*!	Stop playing the audio stream
			**	This function stops the stream if it was playing or paused, and does nothing if it was already stopped.
			**	It also resets the playing position (unlike Pause ()).
			*/
			void Stop ();

			/*!	Return the number of channels of the stream
			**	1 channel means a mono sound, 2 means stereo, etc.
			*/
			unsigned int GetChannelsCount () const;

			/*!	Get the stream sample rate of the stream
			**	The sample rate is the number of audio samples played per second. The higher, the better the quality.
			**	\return Sample rate, in number of samples per second
			*/
			unsigned int GetSampleRate () const;

			/*!	Get the current status of the stream (stopped, paused, playing)
			*/
			C_SoundSource::ESourceStatus GetStatus () const;

			/*!	Change the current playing position of the stream
			**	The playing position can be changed when the stream is either paused or playing.
			**	\param timeOffset New playing position, in seconds
			*/
			void SetPlayingOffset (float timeOffset);

			/*!	Get the current playing position of the stream
			**	\return Current playing position, in seconds
			*/
			float GetPlayingOffset () const;

			/*!	Set whether or not the stream should loop after reaching the end
			**	If set, the stream will restart from beginning after reaching the end and so on, until it is stopped or SetLoop(false) is called.
			**	The default looping state for streams is false.
			**	\param loop True to play in loop, false to play once
			*/
			void SetLoop (bool loop);

			/*!	Tell whether or not the stream is in loop mode
			**	\return True if the stream is looping, false otherwise
			*/
			bool GetLoop () const;

			/*!	Get the total duration of the music
			**	\return Music duration, in seconds
			*/
			float GetDuration () const;

		private:
			void NewPlay ();
			void StopPlay ();

			/*!	Define the audio stream parameters
			**	\param channelsCount Number of channels of the stream
			**	\param sampleRate    Sample rate, in samples per second
			*/
			void Initialize (unsigned int channelsCount, unsigned int sampleRate);

			/*!	Function called as the entry point of the thread
			**	This function starts the streaming loop, and returns only when the sound is stopped.
			*/
			void Update ();

			/*!	Request a new chunk of audio samples from the stream source
			**	It is called continuously by the streaming loop, in a separate thread.
			**	The source can choose to stop the streaming loop at any time, by returning false to the caller.
			**
			**	\param data Chunk of data to fill
			**
			**	\return True to continue playback, false to stop
			*/
			bool GetData (S_Chunk& data);

			/*!	Change the current playing position in the stream source
			**	
			**	\param timeOffset New playing position, in seconds
			*/
			void Seek (float timeOffset);

			/*!	Fill a new buffer with audio samples, and append it to the playing queue
			**	This function is called as soon as a buffer has been fully consumed; it fills it again and inserts it back into the
			**	playing queue.
			**
			**	\param buffer Number of the buffer to fill (in [0, BuffersCount])
			**
			**	\return True if the stream source has requested to stop, false otherwise
			*/
			bool FillAndPushBuffer (unsigned int bufferNum);

			/*!	Fill the audio buffers and put them all into the playing queue
			**	This function is called when playing starts and the playing queue is empty.
			**
			**	\return True if class has requested to stop, false otherwise
			*/
			bool FillQueue();

			/*!	Clear all the audio buffers and empty the playing queue
			**	This function is called when the stream is stopped.
			*/
			void ClearQueue ();

			enum
			{
				BuffersCount = 3 ///< Number of audio buffers used by the streaming loop
			};

			bool				m_IsStreaming;              ///<	Streaming state (true = playing, false = stopped)
			unsigned int		m_Buffers[BuffersCount];    ///<	Sound buffers used to store temporary audio data
			unsigned int		m_ChannelsCount;            ///<	Number of channels (1 = mono, 2 = stereo, ...)
			unsigned int		m_SampleRate;               ///<	Frequency (samples / second)
			unsigned long		m_Format;                   ///<	Format of the internal sound buffers
			bool				m_Loop;                     ///<	Loop flag (true to loop, false to play once)
			unsigned int		m_SamplesProcessed;         ///<	Number of buffers processed since beginning of the stream
			bool				m_EndBuffers[BuffersCount]; ///<	Each buffer is marked as "end buffer" or not, for proper duration calculation
			float				m_Duration;
			bool				m_RequestStop;

			std::vector<char>	m_Samples;
			C_SoundFile*		m_SoundFile;  
			std::wstring		m_FileName;
			ESampleFormat		m_SampleFormat;
		};

	}	//	namespace AUDIO
}	//	namespace REDEEMER

#endif	//	_R_AUDIO_AUDIOSTREAM_H_
