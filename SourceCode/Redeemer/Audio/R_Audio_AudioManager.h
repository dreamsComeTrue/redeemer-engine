/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_FileSystem_AudioManager.h
**		Engine's audio system communication layer
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_AUDIO_AUDIOMANAGER_H_
#define _R_AUDIO_AUDIOMANAGER_H_

#include "R_Audio_EAX2System.h"
#include "..\\Core\\R_Core_Singleton.h"
#include "..\\Core\\R_Core_SmartPointer.h"
#include "..\\Core\\R_Core_BaseClass.h"
#include "..\\Core\\Math\\R_Core_Vector3D.h"

#include <windows.h>
#include <vector>

typedef struct ALCdevice_struct ALCdevice;
typedef struct ALCcontext_struct ALCcontext;

namespace REDEEMER
{
	namespace FILESYSTEM
	{
		class IDataFile;
	}

	namespace AUDIO
	{
		class C_SoundBuffer;
		class C_AudioSound;
		class C_AudioStream;


		DWORD WINAPI RedeemerAudio_AutoUpdaterThreadFunction (LPVOID n);

		/*!
		**	\brief C_AudioManager provides communication layer between engine and OpenAL system
		*/
		class C_AudioManager : public CORE::C_Singleton<C_AudioManager>, public CORE::C_BaseClass
		{
			friend DWORD WINAPI RedeemerAudio_AutoUpdaterThreadFunction (LPVOID n);
		public:
			/*!	Sound System types
			*/
			enum ESoundSystem
			{
				SS_EAX2,						// EAX 2.0 (Direct Sound 3D)
				SS_DIRECTSOUND3D,				// Direct Sound 3D
				SS_DIRECTSOUND,					// Direct Sound
				SS_NVIDIA_NFORCE_2,				// nVidia nForce 2
				SS_CREATIVE_AUDIGY_2,			// Creative Audigy 2
				SS_MMSYSTEM,					// Microsoft
				SS_ALUT,						// ALUT

				SS_NOSYSTEM						// no sound system
			};
		
			/*!	Constructor
			*/
			C_AudioManager ();

			/*!	Destructor
			*/
			virtual ~C_AudioManager ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize (ESoundSystem system = SS_EAX2);

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Creates new sound buffer from given file
			*/
			C_SoundBuffer* CreateSoundBufferFromFile (FILESYSTEM::IDataFile* file);

			/*!	Creates new static sound from given sound buffer
			*/
			C_AudioSound* CreateSoundFromBuffer (C_SoundBuffer* buffer);

			/*!	Creates new streamed sound from file
			*/
			C_AudioStream* CreateStreamSoundFromFile (FILESYSTEM::IDataFile* file);
															
			//-------------------------------------------------------------------------------------------------
			//	LISTENER section

			/*!	Sets listener master gain
			*/
			void Listener_SetGlobalVolume (float volume = 1.0);

			/*!	Returns listener master gain volume
			*/
			float Listener_GetGlobalVolume ();

			/*!	Sets listener position in 3D
			*/
			void Listener_Set3DPosition (CORE::C_Vector3D<float>& position);

			/*!	Returns listener position
			*/
			CORE::C_Vector3D<float> Listener_Get3DPosition ();

			/*!	Sets listener front and top orientation vectors
			*/
			void Listener_Set3DOrientation (CORE::C_Vector3D<float>& frontOrientation, CORE::C_Vector3D<float>& topOrientation);

			/*!	Returns listener's top orientation vector
			*/
			CORE::C_Vector3D<float> Listener_Get3DTopOrientation ();

			/*!	Returns listener's front orientation vector
			*/
			CORE::C_Vector3D<float> Listener_Ge3DFrontOrientation ();

			/*!	Sets listener velocity
			*/
			void Listener_Set3DVelocity (CORE::C_Vector3D<float>& velocity);

			/*!	Returns listener velocity
			*/
			CORE::C_Vector3D<float> Listener_Get3DVelocity ();

			/*!	Returns reference to EAX2 system
			*/
			C_EAX2System& GetEAX2System ();

		private:
			bool OpenALContextInit (const char* strType);
			void OpenALContextDestroy ();
			void UpdateStreams ();

			CRITICAL_SECTION			m_CriticalSection;			///<	Used for threading
			HANDLE						m_AutoUpdateThreadHandle;	///<	Handle for auto updating routine
			DWORD						m_AutoUpdateThreadID;		///<	ID number
			bool						m_AutoUpdate;			
			ESoundSystem				m_SoundSystem;				///<	Currently initialized sound system
			ALCdevice*					m_OpenALDevice;
			ALCcontext*					m_OpenALContext;
			C_EAX2System				m_EAX2System;				///<	Used for environmental effects
			std::vector<C_SoundBuffer*>	m_SoundsBuffers;			///<	Loaded sound buffers
			std::vector<C_AudioSound*>	m_SoundsList;				///<	List of all static sounds
			std::vector<C_AudioStream*>	m_StreamsList;				///<	List of all streamed sounds
		};

	}	//	namespace AUDIO
}	//	namespace REDEEMER

#endif	//	_R_AUDIO_AUDIOMANAGER_H_
