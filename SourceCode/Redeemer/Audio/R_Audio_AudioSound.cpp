/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Audio_AudioSound.cpp
**		Regular sound that can be played in the audio environment
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Audio_AudioSound.h"

#include "al.h"

#include <windows.h>

namespace REDEEMER
{
	namespace AUDIO
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_AudioSound::C_AudioSound ()  :
			m_SoundBuffer (NULL)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_AudioSound::C_AudioSound (C_SoundBuffer& buffer, bool loop, float pitch, float volume, const CORE::C_Vector3D<float>& position) :
			m_SoundBuffer (NULL)
		{
			SetBuffer (buffer);
			SetLoop (loop);
			SetPitch (pitch);
			SetVolume (volume);
			SetPosition (position);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_AudioSound::C_AudioSound (const C_AudioSound& copy) :
			C_SoundSource (copy),
			m_SoundBuffer (NULL)
		{
			if (copy.m_SoundBuffer)
				SetBuffer(*copy.m_SoundBuffer);

			SetLoop (copy.IsLooping());
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_AudioSound::~C_AudioSound ()
		{
			Stop ();

			if (m_SoundBuffer)
				m_SoundBuffer->DetachSound (this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_AudioSound& C_AudioSound::operator =(const C_AudioSound& right)
		{
			// Here we don't use the copy-and-swap idiom, because it would mess up
			// the list of sound instances contained in the buffers

			// Detach the sound instance from the previous buffer (if any)
			if (m_SoundBuffer)
			{
				Stop ();

				m_SoundBuffer->DetachSound(this);
				m_SoundBuffer = NULL;
			}

			// Copy the sound attributes
			if (right.m_SoundBuffer)
				SetBuffer(*right.m_SoundBuffer);

			SetLoop (right.IsLooping());
			SetPitch (right.GetPitch());
			SetVolume (right.GetVolume());
			SetPosition (right.GetPosition());
			SetRelativeToListener (right.IsRelativeToListener());
			SetMinDistance (right.GetMinDistance());
			SetAttenuation (right.GetAttenuation());

			return *this;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_AudioSound::Play ()
		{
			  alSourcePlay (m_SourceID);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_AudioSound::Pause ()
		{
			  alSourcePause (m_SourceID);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_AudioSound::Stop ()
		{
			 alSourceStop (m_SourceID);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_AudioSound::SetBuffer (C_SoundBuffer& buffer)
		{
			// First detach from the previous buffer
			if (m_SoundBuffer)
			{
				Stop();
				m_SoundBuffer->DetachSound(this);
			}

			// Assign and use the new buffer
			m_SoundBuffer = &buffer;
			m_SoundBuffer->AttachSound(this);

			alSourcei (m_SourceID, AL_BUFFER, m_SoundBuffer->m_BufferID);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_AudioSound::SetLoop (bool loop)
		{
		    alSourcei (m_SourceID, AL_LOOPING, loop);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_AudioSound::SetPlayingOffset (float timeOffset)
		{
			alSourcef (m_SourceID, AL_SEC_OFFSET, timeOffset);
		}

		//------------------------------------------------------------------------------------------------------------------------

		const C_SoundBuffer* C_AudioSound::GetBuffer () const
		{
			  return m_SoundBuffer;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_AudioSound::IsLooping () const
		{
			ALint loop;

			alGetSourcei (m_SourceID, AL_LOOPING, &loop);

			return loop != 0;
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_AudioSound::GetPlayingOffset () const
		{
			ALfloat seconds = 0.f;

			alGetSourcef (m_SourceID, AL_SEC_OFFSET, &seconds);

			return seconds;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_SoundSource::ESourceStatus C_AudioSound::GetStatus () const
		{
			return C_SoundSource::GetStatus ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_AudioSound::ResetBuffer ()
		{
			// First stop the sound in case it is playing
			Stop ();

			// Detach the buffer
			alSourcei (m_SourceID, AL_BUFFER, 0);

			m_SoundBuffer = NULL;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace AUDIO
}	//	namespace REDEEMER

