/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Audio_SoundSource.h
**		Base class defining a sound's properties
*/

#ifndef _R_AUDIO_SOUNDSOURCE_H_
#define _R_AUDIO_SOUNDSOURCE_H_

//------------------------------------------------------------------------------------------------------------------------
#include "..\\Core\\Math\\R_Core_Vector3D.h"

/** unsigned 32-bit integer */
typedef unsigned int ALuint;

namespace REDEEMER
{
	namespace AUDIO
	{
		class C_EAX2System;


		/*!	Base class defining a sound's properties
		*/
		class C_SoundSource
		{
			friend class C_EAX2System;

		public:

			/*!	Sound source status
			*/
			enum ESourceStatus
			{
				SS_Stopped,		///< Sound is not playing
				SS_Paused,		///< Sound is paused
				SS_Playing		///< Sound is playing
			};

			/*!	Copy constructor
			*/
			C_SoundSource (const C_SoundSource& copy);

			/*!	Destructor
			*/
			virtual ~C_SoundSource ();

			/*!	Set the pitch of the sound
			**	The default value for the pitch is 1.
			**	\param pitch New pitch to apply to the sound
			*/
			void SetPitch (float pitch);

			/*!	Get the pitch of the sound
			*/
			float GetPitch () const;

			/*!	Set the volume of the sound
			**	The volume is a value between 0 (mute) and 100 (full volume).
			**	The default value for the volume is 100.
			**	\param volume Volume of the sound
			*/
			void SetVolume (float volume);
			
			/*!	Get the volume of the sound
			**	\return Volume of the sound, in the range [0, 100]
			*/
			float GetVolume () const;

			/*!	Set the 3D position of the sound in the audio scene
			**	Only sounds with one channel (mono sounds) can be spatialized.
			**	The default position of a sound is (0, 0, 0).
			**	\param position Position of the sound in the scene
			*/
			void SetPosition (const CORE::C_Vector3D<float>& position);
			
			/*!	Get the 3D position of the sound in the audio scene
			*/
			CORE::C_Vector3D<float> GetPosition () const;

			/*!	Make the sound's position relative to the listener or absolute
			**	Making a sound relative to the listener will ensure that it will always
			**	be played the same way regardless the position of the listener.
			**	The default value is false (position is absolute).
			**	\param relative True to set the position relative, false to set it absolute
			*/
			void SetRelativeToListener (bool relative);
			
			/*!	Tell whether the sound's position is relative to the listener or is absolute
			*/
			bool IsRelativeToListener () const;

			/*!	Set the minimum distance of the sound
			**	The "minimum distance" of a sound is the maximum distance at which it is heard at its maximum volume. Further
			**	than the minimum distance, it will start to fade out according to its attenuation factor. A value of 0 ("inside the head
			**	of the listener") is an invalid value and is forbidden.
			**	The default value of the minimum distance is 1.
			**	\param distance New minimum distance of the sound
			*/
			void SetMinDistance (float distance);
			
			/*!	Get the minimum distance of the sound
			*/
			float GetMinDistance () const;

			/*!	Set the attenuation factor of the sound
			**	The attenuation is a multiplicative factor which makes the sound more or less loud according to its distance
			**	from the listener. An attenuation of 0 will produce a non-attenuated sound, i.e. its volume will always be the same
			**	whether it is heard from near or from far. On the other hand, an attenuation value such as 100 will make the sound fade out
			**	very quickly as it gets further from the listener.
			**	The default value of the attenuation is 1.
			**	\param attenuation New attenuation factor of the sound
			*/
			void SetAttenuation (float attenuation);			 

			/*!	Get the attenuation factor of the sound
			*/
			float GetAttenuation () const;

		protected:
			/*!	Default constructor
			**	This class can ONLY be derived
			*/
			C_SoundSource ();

			/*!	Get the current status of the sound (stopped, paused, playing)
			*/
			ESourceStatus GetStatus () const;

			/*!	Returns OpenAL ID - used by EAX
			*/
			ALuint GetSourceID () const;

		protected:
			ALuint	 m_SourceID;	///<	OpenAL source identifier
		};

	}	//	namespace AUDIO
}	//	namespace REDEEMER

#endif	//	_R_AUDIO_SOUNDSOURCE_H_
