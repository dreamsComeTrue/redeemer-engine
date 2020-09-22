/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Audio_AudioSound.h
**		Regular sound that can be played in the audio environment
*/

#ifndef _R_AUDIO_AUDIOSOUND_H_
#define _R_AUDIO_AUDIOSOUND_H_

//------------------------------------------------------------------------------------------------------------------------
#include "R_Audio_SoundSource.h"
#include "R_Audio_SoundBuffer.h"

namespace REDEEMER
{
	namespace AUDIO
	{
		/*!	Regular sound that can be played in the audio environment
		*/
		class C_AudioSound : public C_SoundSource
		{
		public:
			/*!	Default constructor
			*/
			C_AudioSound ();

			/*!	Construct the sound with parameters
			**	\param buffer   Sound buffer containing the audio data to play with the sound
			**	\param loop     Should the sound loop?
			**	\param pitch    Pitch of the sound
			**	\param volume   Volume of the sound, in the range [0, 100]
			**	\param position 3D position of the sound source in the audio scene
			*/
			C_AudioSound (C_SoundBuffer& buffer, bool loop = false, float pitch = 1.f, float volume = 100.f, 
				const CORE::C_Vector3D<float>& position = CORE::C_Vector3D<float>(0, 0, 0));

			/*!	Copy constructor
			*/
			C_AudioSound (const C_AudioSound& copy);

			/*!	Destructor
			*/
			virtual ~C_AudioSound ();

			/*!	Overload of assignment operator
			**	\param right Instance to assign
			*/
			C_AudioSound& operator =(const C_AudioSound& right);

			/*!	Start or resume playing the sound
			**	This function starts the sound if it was stopped, resumes it if it was paused, and does nothing it is it already playing.
			*/
			void Play ();

			/*!	Pause the sound
			**	This function pauses the sound if it was playing, otherwise (sound already paused or stopped) it has no effect.
			*/
			void Pause ();

			/*!	Stop playing the sound
			**	This function stops the sound if it was playing or paused, and does nothing if it was already stopped.
			**	It also resets the playing position (unlike Pause()).
			*/
			void Stop ();

			/*!	Set the source buffer containing the audio data to play
			**	It is important to note that the sound buffer is not copied, thus the C_SoundBuffer instance must remain alive as long
			**	as it is attached to the sound.
			**	\param buffer Sound buffer to attach to the sound
			*/
			void SetBuffer (C_SoundBuffer& buffer);

			/*!	Set whether or not the sound should loop after reaching the end
			**	If set, the sound will restart from beginning after reaching the end and so on, until it is stopped or SetLoop(false) is called.
			**	The default looping state for sound is false.
			**	\param loop True to play in loop, false to play once
			*/
			void SetLoop (bool loop);

			/*!	Change the current playing position of the sound
			**	The playing position can be changed when the sound is either paused or playing.
			**	\param timeOffset New playing position, in seconds
			*/
			void SetPlayingOffset (float timeOffset);

			/*!	Get the audio buffer attached to the sound
			*/
			const C_SoundBuffer* GetBuffer () const;

			/*!	Tell whether or not the sound is in loop mode
			*/
			bool IsLooping () const;

			/*!	Get the current playing position of the sound (in seconds)
			*/
			float GetPlayingOffset () const;

			/*!	Get the current status of the sound (stopped, paused, playing)
			*/
			ESourceStatus GetStatus () const;

			/*!	Reset the internal buffer of the sound
			**	This function is for internal use only, you don't have to use it. It is called by the C_SoundBuffer that
			**	this sound uses, when it is destroyed in order to prevent the sound from using a dead buffer.
			*/
			void ResetBuffer ();

		private:
			C_SoundBuffer*	m_SoundBuffer;	///< Sound buffer bound to the source
		};

	}	//	namespace AUDIO
}	//	namespace REDEEMER

#endif	//	_R_AUDIO_AUDIOSOUND_H_
