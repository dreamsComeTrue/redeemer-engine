/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Audio_SoundSource.cpp
**		Base class defining a sound's properties
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Audio_SoundSource.h"

#include <al.h>

namespace REDEEMER
{
	namespace AUDIO
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_SoundSource::C_SoundSource ()
		{				
			alGenSources (1, &m_SourceID);
			alSourcei (m_SourceID, AL_BUFFER, 0);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_SoundSource::C_SoundSource (const C_SoundSource& copy)
		{
			alGenSources (1, &m_SourceID);
			alSourcei (m_SourceID, AL_BUFFER, 0);

			SetPitch (copy.GetPitch ());
			SetVolume (copy.GetVolume ());
			SetPosition (copy.GetPosition ());
			SetRelativeToListener (copy.IsRelativeToListener ());
			SetMinDistance (copy.GetMinDistance ());
			SetAttenuation (copy.GetAttenuation ());
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_SoundSource::~C_SoundSource ()
		{
			alSourcei (m_SourceID, AL_BUFFER, 0);
			alDeleteSources (1, &m_SourceID);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_SoundSource::SetPitch (float pitch)
		{
			 alSourcef (m_SourceID, AL_PITCH, pitch);
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_SoundSource::GetPitch () const
		{
			ALfloat pitch;

			alGetSourcef (m_SourceID, AL_PITCH, &pitch);

			return pitch;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_SoundSource::SetVolume (float volume)
		{
			alSourcef (m_SourceID, AL_GAIN, volume * 0.01f);
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_SoundSource::GetVolume () const
		{
			ALfloat gain;

			alGetSourcef (m_SourceID, AL_GAIN, &gain);

			return gain * 100.f;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_SoundSource::SetPosition (const CORE::C_Vector3D<float>& position)
		{
			alSource3f (m_SourceID, AL_POSITION, position.X, position.Y, position.Z);
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Vector3D<float> C_SoundSource::GetPosition () const
		{
			CORE::C_Vector3D<float> position;

			alGetSource3f (m_SourceID, AL_POSITION, &position.X, &position.Y, &position.Z);

			return position;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_SoundSource::SetRelativeToListener (bool relative)
		{
			alSourcei (m_SourceID, AL_SOURCE_RELATIVE, relative);
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_SoundSource::IsRelativeToListener () const
		{
			ALint relative;

			alGetSourcei (m_SourceID, AL_SOURCE_RELATIVE, &relative);

			return relative != 0;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_SoundSource::SetMinDistance (float distance)
		{
			alSourcef (m_SourceID, AL_REFERENCE_DISTANCE, distance);
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_SoundSource::GetMinDistance () const
		{
			ALfloat distance;

			alGetSourcef (m_SourceID, AL_REFERENCE_DISTANCE, &distance);

			return distance;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_SoundSource::SetAttenuation (float attenuation)
		{
			alSourcef (m_SourceID, AL_ROLLOFF_FACTOR, attenuation);
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_SoundSource::GetAttenuation () const
		{
			ALfloat attenuation;

			alGetSourcef (m_SourceID, AL_ROLLOFF_FACTOR, &attenuation);

			return attenuation;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_SoundSource::ESourceStatus C_SoundSource::GetStatus () const
		{
			ALint status;

			alGetSourcei (m_SourceID, AL_SOURCE_STATE, &status);

			switch (status)
			{
			case AL_INITIAL:
			case AL_STOPPED: 
				return SS_Stopped;

			case AL_PAUSED: 
				return SS_Paused;

			case AL_PLAYING: 
				return SS_Playing;
			}

			return SS_Stopped;
		}

		//------------------------------------------------------------------------------------------------------------------------

		ALuint C_SoundSource::GetSourceID () const
		{
			return m_SourceID;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace AUDIO
}	//	namespace REDEEMER

