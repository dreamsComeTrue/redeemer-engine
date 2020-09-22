/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_FileSystem_AudioManager.cpp
**		Engine's audio system communication layer
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Audio_AudioManager.h"
#include "R_Audio_AudioStream.h"
#include "R_Audio_AudioSound.h"
#include "R_Audio_SoundBuffer.h"
#include "..\\RedeemerEngine.h"

#include <AL\\alut.h>
#include <al.h>
#include <alc.h>

#include <Ogg\\ogg.h>
#include <Vorbis\\codec.h>
#include <Vorbis\\vorbisenc.h>
#include <Vorbis\\vorbisfile.h>

namespace REDEEMER
{
	namespace AUDIO
	{	 	
		DWORD WINAPI RedeemerAudio_AutoUpdaterThreadFunction (LPVOID n)
		{
			C_AudioManager* manager = static_cast<C_AudioManager*>(n);

			while (manager->m_AutoUpdate)
				manager->UpdateStreams ();

			return 0;
		}

		//------------------------------------------------------------------------------------------------------------------------
		C_AudioManager::C_AudioManager () :
			m_OpenALDevice (0),
			m_OpenALContext (0)
		{
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_AudioManager::~C_AudioManager ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_AudioManager::Initialize (ESoundSystem system)
		{
			C_BaseClass::Initialize();

			if (system == SS_NOSYSTEM) 
				return RESULT_FAIL;

			//	Initialize threading
			InitializeCriticalSection (&m_CriticalSection);

			m_AutoUpdateThreadHandle = CreateThread (NULL, 0, RedeemerAudio_AutoUpdaterThreadFunction, this, NULL, &m_AutoUpdateThreadID);
			m_AutoUpdate = true;

			m_SoundSystem = system;

			switch (m_SoundSystem)
			{						
			case SS_EAX2: 
				{
					if (!OpenALContextInit ("DirectSound3D"))
						return RESULT_FAIL;

					if (!m_EAX2System.Initialize())
					{
						alcDestroyContext (m_OpenALContext);
						
						m_OpenALContext = 0;

						alcCloseDevice (m_OpenALDevice); 
						
						m_OpenALDevice = 0;

						return RESULT_FAIL;
					}
				}

				break;

			case SS_DIRECTSOUND3D: 
				{
					if (OpenALContextInit("DirectSound3D") == RESULT_FAIL)
						return RESULT_FAIL;
				}

				break;

			case SS_DIRECTSOUND: 
				{
					if (OpenALContextInit("DirectSound") == RESULT_FAIL)
						return RESULT_FAIL;
				}

				break;

			case SS_NVIDIA_NFORCE_2: 
				{
					if (OpenALContextInit("NVIDIA nForce2") == RESULT_FAIL)
						return RESULT_FAIL;
				}

				break;

			case SS_CREATIVE_AUDIGY_2: 
				{
					if (OpenALContextInit("Creative Audigy 2") == RESULT_FAIL)
						return RESULT_FAIL;
				}

				break;

			case SS_MMSYSTEM:
				{
					if (OpenALContextInit("MMSYSTEM") == RESULT_FAIL)
						return RESULT_FAIL;
				}

				break;

			case SS_ALUT: 
				{
					if (alutInit (NULL, 0) == AL_TRUE)
					{
						alDistanceModel (AL_INVERSE_DISTANCE_CLAMPED);
						alListenerf (AL_GAIN, 1.0f);
					}
					else
						return RESULT_FAIL;
				}
			}

			REDEEMER_LOG << LOG_INFO << L"Audio Manager initialized!" << LOG_ENDMESSAGE;

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_AudioManager::Finalize ()
		{
			if (!(m_SoundSystem == SS_NOSYSTEM))
			{
				m_AutoUpdate = false;

				WaitForMultipleObjects (1, &m_AutoUpdateThreadHandle, true, INFINITE);
				CloseHandle (m_AutoUpdateThreadHandle);

				//	Free threading
				DeleteCriticalSection (&m_CriticalSection);

				//	Remove all resources
				for (unsigned int i = 0; i < m_StreamsList.size(); i ++)
				{
					REDEEMER_SAFE_DELETE (m_StreamsList[i]);
				}

				for (unsigned int i = 0; i < m_SoundsList.size(); i ++)
				{
					REDEEMER_SAFE_DELETE (m_SoundsList[i]);
				}

				for (unsigned int i = 0; i < m_SoundsBuffers.size(); i ++)
				{
					REDEEMER_SAFE_DELETE (m_SoundsBuffers[i]);
				}

				switch (m_SoundSystem)
				{
				case SS_EAX2: 
					{
						m_EAX2System.Finalize(); 

						OpenALContextDestroy ();
					}
					
					break;

				case SS_DIRECTSOUND3D: 
					{
						OpenALContextDestroy();
					}

					break;

				case SS_DIRECTSOUND: 
					{
						OpenALContextDestroy();
					}

					break;

				case SS_NVIDIA_NFORCE_2: 
					{
						OpenALContextDestroy();
					}

					break;

				case SS_CREATIVE_AUDIGY_2: 
					{
						OpenALContextDestroy();
					}

					break;

				case SS_MMSYSTEM: 
					{
						OpenALContextDestroy();
					}

					break;

				case SS_ALUT: 
					{
						alutExit ();
					}
					
					break;
				}

				m_SoundSystem = SS_NOSYSTEM;
			}

			REDEEMER_LOG << LOG_INFO << L"Audio Manager finalized!" << LOG_ENDMESSAGE;

			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_SoundBuffer* C_AudioManager::CreateSoundBufferFromFile (FILESYSTEM::IDataFile* file)
		{
			if (!m_OpenALDevice)
				return NULL;

			//	If buffer already exists
			if (!m_SoundsBuffers.empty())
			for (unsigned int i = 0; i < m_SoundsBuffers.size(); i ++)
				if (m_SoundsBuffers[i]->m_FileName == file->GetFileName())
					return m_SoundsBuffers[i];

			C_SoundBuffer* buffer = new C_SoundBuffer ();

			if (!buffer->LoadFromFile(file))
			{
				REDEEMER_SAFE_DELETE (buffer);

				return NULL;
			}

			m_SoundsBuffers.push_back(buffer);

			return buffer;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_AudioSound* C_AudioManager::CreateSoundFromBuffer (C_SoundBuffer* buffer)
		{
			if (!m_OpenALDevice)
				return NULL;

			//	If buffer already exists
			if (!m_SoundsList.empty())
			for (unsigned int i = 0; i < m_SoundsList.size(); i ++)
				if (m_SoundsList[i]->GetBuffer () == buffer)
					return m_SoundsList[i];

			C_AudioSound* sound = new C_AudioSound ();

			sound->SetBuffer(*buffer);

			m_SoundsList.push_back(sound);

			return sound;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_AudioStream* C_AudioManager::CreateStreamSoundFromFile (FILESYSTEM::IDataFile* file)
		{
			if (!m_OpenALDevice)
				return NULL;

			//	If buffer already exists
			if (!m_StreamsList.empty())
			for (unsigned int i = 0; i < m_StreamsList.size(); i ++)
				if (m_StreamsList[i]->m_FileName == file->GetFileName())
					return m_StreamsList[i];

			C_AudioStream* stream = new C_AudioStream ();

			if (!stream->OpenFromFile(file))
			{
				REDEEMER_SAFE_DELETE (stream);

				return NULL;
			}

			m_StreamsList.push_back(stream);

			return stream;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_AudioManager::OpenALContextInit(const char* strType)
		{
			if (alutInitWithoutContext (NULL, 0) == AL_FALSE)
				return false;

			m_OpenALDevice = alcOpenDevice (strType);

			if (m_OpenALDevice)
			{
				m_OpenALContext = alcCreateContext(m_OpenALDevice, NULL);

				if (m_OpenALContext)
				{
					alcMakeContextCurrent(m_OpenALContext);

					alDistanceModel (AL_INVERSE_DISTANCE_CLAMPED);
					alListenerf (AL_GAIN, 1.0f);
					
					return true;
				}
				else 
				{
					alcCloseDevice(m_OpenALDevice);
									
					m_OpenALDevice = 0;

					return false;
				}
			}

			return false;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_AudioManager::OpenALContextDestroy ()
		{
			alutExit ();

			if (m_OpenALContext) 
			{ 
				alcDestroyContext (m_OpenALContext); 
				
				m_OpenALContext = NULL;
			}

			if (m_OpenALDevice) 
			{ 
				alcCloseDevice (m_OpenALDevice); 
				
				m_OpenALDevice = NULL;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_AudioManager::UpdateStreams ()
		{
			if (!m_StreamsList.empty())
			{
				for (unsigned int i = 0; i < m_StreamsList.size(); i ++)
					m_StreamsList[i]->Update();
			}	
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_AudioManager::Listener_SetGlobalVolume (float volume)
		{
			alListenerf (AL_GAIN, volume * 0.01f);
		}

		//------------------------------------------------------------------------------------------------------------------------
		float C_AudioManager::Listener_GetGlobalVolume ()
		{
			float volume = 0.f;

			alGetListenerf(AL_GAIN, &volume);

			return volume;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_AudioManager::Listener_Set3DPosition (CORE::C_Vector3D<float>& position)
		{
			alListener3f (AL_POSITION, position.X, position.Y, position.Z);
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Vector3D<float> C_AudioManager::Listener_Get3DPosition ()
		{
			CORE::C_Vector3D<float> position;

			alGetListener3f (AL_POSITION, &position.X, &position.Y, &position.Z);

			return position;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_AudioManager::Listener_Set3DOrientation (CORE::C_Vector3D<float>& frontOrientation, CORE::C_Vector3D<float>& topOrientation)
		{
			float ov[6] = { frontOrientation.X, frontOrientation.Y, frontOrientation.Z, topOrientation.X, topOrientation.Y, topOrientation.Z };

			alListenerfv (AL_ORIENTATION, ov);
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Vector3D<float> C_AudioManager::Listener_Get3DTopOrientation ()
		{
			float orientation[6];

			alGetListenerfv (AL_ORIENTATION, orientation);

			return CORE::C_Vector3D<float>(orientation[3], orientation[4], orientation[5]);
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Vector3D<float> C_AudioManager::Listener_Ge3DFrontOrientation ()
		{
			float orientation[6];

			alGetListenerfv (AL_ORIENTATION, orientation);

			return CORE::C_Vector3D<float>(orientation[0], orientation[1], orientation[2]);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_AudioManager::Listener_Set3DVelocity (CORE::C_Vector3D<float>& velocity)
		{
			float vel[3] = {velocity.X, velocity.Y, velocity.Z};

			alListenerfv (AL_VELOCITY, vel);
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Vector3D<float> C_AudioManager::Listener_Get3DVelocity ()
		{
			return CORE::C_Vector3D<float>(0, 0, 0);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_EAX2System& C_AudioManager::GetEAX2System ()
		{
			return m_EAX2System;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace AUDIO
}	//	namespace REDEEMER
