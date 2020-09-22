/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Audio_EAX2System.h
**		EAX 2 is used for environmental effects handling within OpenAL
*/

//--------------------------------------------------------------------------------------------------------------------------------
#ifndef _R_AUDIO_EAX2SYSTEM_H_
#define _R_AUDIO_EAX2SYSTEM_H_

#include "..\\Core\\R_Core_BaseClass.h"

#include <windows.h>

/** unsigned 32-bit integer */
typedef unsigned int ALuint;

/** enumerated 32-bit value */
typedef int ALenum;

/** void type (for opaque pointers only) */
typedef void ALvoid;


typedef ALenum (*EAXSet)(const GUID*, ALuint, ALuint, ALvoid*, ALuint);
typedef ALenum (*EAXGet)(const GUID*, ALuint, ALuint, ALvoid*, ALuint);

namespace REDEEMER
{
	namespace FILESYSTEM
	{
		class C_XMLDocument;
	}

	namespace AUDIO
	{
		class C_SoundSource;

		/*!	Properties for EAX listener
		*/
		struct S_AudioEAXListenerProperties
		{
			long			Room;					///<	room effect level at low frequencies
			long			RoomHF;					///<	room effect high-frequency level re. low frequency level
			float			RoomRolloffFactor;		///<	like DS3D flRolloffFactor but for room effect
			float			DecayTime;				///<	reverberation decay time at low frequencies
			float			DecayHFRatio;			///<	high-frequency to low-frequency decay time ratio
			long			Reflections;			///<	early reflections level relative to room effect
			float			ReflectionsDelay;		///<	initial reflection delay time
			long			Reverb;					///<	late reverberation level relative to room effect
			float			ReverbDelay;			///<	late reverberation delay time relative to initial reflection
			unsigned long	Environment;			///<	sets all listener properties
			float			EnvironmentSize;		///<	environment size in meters
			float			EnvironmentDiffusion;	///<	environment diffusion
			float			AirAbsorptionHF;		///<	change in level per meter at 5 kHz
			unsigned long	Flags;					///<	modifies the behavior of properties
		};
			
		/*!	Properties for EAX source
		*/
		struct S_AudioEAXSourceProperties
		{
			long			Direct;					///<	direct path level
			long			DirectHF;				///<	direct path level at high frequencies
			long			Room;					///<	room effect level
			long			RoomHF;					///<	room effect level at high frequencies
			float			RoomRolloffFactor;		///<	like DS3D flRolloffFactor but for room effect
			long			Obstruction;			///<	main obstruction control (attenuation at high frequencies) 
			float			ObstructionLFRatio;		///<	obstruction low-frequency level re. main control
			long			Occlusion;				///<	main occlusion control (attenuation at high frequencies)
			float			OcclusionLFRatio;		///<	occlusion low-frequency level re. main control
			float			OcclusionRoomRatio;		///<	occlusion room effect level re. main control
			long			OutsideVolumeHF;		///<	outside sound cone level at high frequencies
			float			AirAbsorptionFactor;	///<	multiplies DSPROPERTY_EAXLISTENER_AIRABSORPTIONHF
			unsigned long	Flags;					///<	modifies the behavior of properties
		};

		// EAX_LISTENER_PROPERTIES Flags (use: Flags = A | B | ....)
#define EAX_LISTENERFLAGS_DECAYTIMESCALE        0x00000001		// reverberation decay time
#define EAX_LISTENERFLAGS_REFLECTIONSSCALE      0x00000002		// reflection level
#define EAX_LISTENERFLAGS_REFLECTIONSDELAYSCALE 0x00000004		// initial reflection delay time
#define EAX_LISTENERFLAGS_REVERBSCALE           0x00000008		// reflections level
#define EAX_LISTENERFLAGS_REVERBDELAYSCALE      0x00000010		// late reverberation delay time
#define EAX_LISTENERFLAGS_DECAYHFLIMIT          0x00000020		// this flag limits high-frequency decay time according to air absorption


		// EAX_SOURCE_PROPERTIES dwFlags (use: dwFlags = A | B | ....)
#define EAX_BUFFERFLAGS_DIRECTHFAUTO				0x00000001		// affects DSPROPERTY_EAXBUFFER_DIRECTHF
#define EAX_BUFFERFLAGS_ROOMAUTO					0x00000002		// affects DSPROPERTY_EAXBUFFER_ROOM
#define EAX_BUFFERFLAGS_ROOMHFAUTO					0x00000004		// affects DSPROPERTY_EAXBUFFER_ROOMHF

		/*!	Various predefined listener environments
		*/
		enum EListenerEnvironmentPreset
		{
			LEP_Generic,
			LEP_PaddedCell,
			LEP_Room,
			LEP_BathRoom,
			LEP_LivingRoom,
			LEP_StoneRoom,
			LEP_Auditorium,
			LEP_ConcertHall,
			LEP_Cave,
			LEP_Arena,
			LEP_Hangar,
			LEP_CarpetedHallway,
			LEP_Hallway,
			LEP_StoneCorridor,
			LEP_Alley,
			LEP_Forest,
			LEP_City,
			LEP_Mountains,
			LEP_Quarry,
			LEP_Plain,
			LEP_ParkingLot,
			LEP_ServerPipe,
			LEP_Underwater,
			LEP_Drugged,
			LEP_Dizzy,
			LEP_Psychotic,
			LEP_Count
		};

		// Used by DSPROPERTY_EAXLISTENER_FLAGS
		//
		// Note: The number and order of flags may change in future EAX versions.
		//       It is recommended to use the flag defines as follows:
		//              myFlags = EAXLISTENERFLAGS_DECAYTIMESCALE | EAXLISTENERFLAGS_REVERBSCALE;
		//       instead of:
		//              myFlags = 0x00000009;
		//
		// These flags determine what properties are affected by environment size.
#define EAXLISTENERFLAGS_DECAYTIMESCALE        0x00000001 // reverberation decay time
#define EAXLISTENERFLAGS_REFLECTIONSSCALE      0x00000002 // reflection level
#define EAXLISTENERFLAGS_REFLECTIONSDELAYSCALE 0x00000004 // initial reflection delay time
#define EAXLISTENERFLAGS_REVERBSCALE           0x00000008 // reflections level
#define EAXLISTENERFLAGS_REVERBDELAYSCALE      0x00000010 // late reverberation delay time

		// This flag limits high-frequency decay time according to air absorption.
#define EAXLISTENERFLAGS_DECAYHFLIMIT          0x00000020

#define EAXLISTENERFLAGS_RESERVED              0xFFFFFFC0 // reserved future use

		// property ranges and defaults:

#define EAXLISTENER_MINROOM                       (-10000)
#define EAXLISTENER_MAXROOM                       0
#define EAXLISTENER_DEFAULTROOM                   (-1000)

#define EAXLISTENER_MINROOMHF                     (-10000)
#define EAXLISTENER_MAXROOMHF                     0
#define EAXLISTENER_DEFAULTROOMHF                 (-100)

#define EAXLISTENER_MINROOMROLLOFFFACTOR          0.0f
#define EAXLISTENER_MAXROOMROLLOFFFACTOR          10.0f
#define EAXLISTENER_DEFAULTROOMROLLOFFFACTOR      0.0f

#define EAXLISTENER_MINDECAYTIME                  0.1f
#define EAXLISTENER_MAXDECAYTIME                  20.0f
#define EAXLISTENER_DEFAULTDECAYTIME              1.49f

#define EAXLISTENER_MINDECAYHFRATIO               0.1f
#define EAXLISTENER_MAXDECAYHFRATIO               2.0f
#define EAXLISTENER_DEFAULTDECAYHFRATIO           0.83f

#define EAXLISTENER_MINREFLECTIONS                (-10000)
#define EAXLISTENER_MAXREFLECTIONS                1000
#define EAXLISTENER_DEFAULTREFLECTIONS            (-2602)

#define EAXLISTENER_MINREFLECTIONSDELAY           0.0f
#define EAXLISTENER_MAXREFLECTIONSDELAY           0.3f
#define EAXLISTENER_DEFAULTREFLECTIONSDELAY       0.007f

#define EAXLISTENER_MINREVERB                     (-10000)
#define EAXLISTENER_MAXREVERB                     2000
#define EAXLISTENER_DEFAULTREVERB                 200

#define EAXLISTENER_MINREVERBDELAY                0.0f
#define EAXLISTENER_MAXREVERBDELAY                0.1f
#define EAXLISTENER_DEFAULTREVERBDELAY            0.011f

#define EAXLISTENER_MINENVIRONMENT                0
#define EAXLISTENER_MAXENVIRONMENT                (EAX_ENVIRONMENT_COUNT-1)
#define EAXLISTENER_DEFAULTENVIRONMENT            EAX_ENVIRONMENT_GENERIC

#define EAXLISTENER_MINENVIRONMENTSIZE            1.0f
#define EAXLISTENER_MAXENVIRONMENTSIZE            100.0f
#define EAXLISTENER_DEFAULTENVIRONMENTSIZE        7.5f

#define EAXLISTENER_MINENVIRONMENTDIFFUSION       0.0f
#define EAXLISTENER_MAXENVIRONMENTDIFFUSION       1.0f
#define EAXLISTENER_DEFAULTENVIRONMENTDIFFUSION   1.0f

#define EAXLISTENER_MINAIRABSORPTIONHF            (-100.0f)
#define EAXLISTENER_MAXAIRABSORPTIONHF            0.0f
#define EAXLISTENER_DEFAULTAIRABSORPTIONHF        (-5.0f)

#define EAXLISTENER_DEFAULTFLAGS                  (EAXLISTENERFLAGS_DECAYTIMESCALE |        \
	EAXLISTENERFLAGS_REFLECTIONSSCALE |      \
	EAXLISTENERFLAGS_REFLECTIONSDELAYSCALE | \
	EAXLISTENERFLAGS_REVERBSCALE |           \
	EAXLISTENERFLAGS_REVERBDELAYSCALE |      \
	EAXLISTENERFLAGS_DECAYHFLIMIT)


		/*!	EAX 2 is used for environmental effects handling within OpenAL
		*/
		class C_EAX2System : public CORE::C_BaseClass
		{
		public:
			/*!	Constructor
			*/
			C_EAX2System ();

			/*!	Destructor
			*/
			virtual ~C_EAX2System();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize ();

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Sets environmental effects for a listener
			*/
			void SetListenerEnvironment (S_AudioEAXListenerProperties* data);

			/*!	Sets environment for listener from predefined presets
			*/
			void SetListenerEnvironmentPreset (EListenerEnvironmentPreset preset);

			/*!	Sets default environmental effects for a listener
			*/
			void SetListenerDefaultEnvironment();

			/*!	Returns environmental effects for a listener
			*/
			S_AudioEAXListenerProperties GetListenerEnvironment();

			/*!	Sets environmental settings to correct source
			*/
			void SetSourceEnvironmentalSettings (S_AudioEAXSourceProperties* data, C_SoundSource* source);

			/*!	Returns environmental settings for given source
			*/
			S_AudioEAXSourceProperties GetSourceEnvironmentalSettings (C_SoundSource* source);

			/*!	Loads listener environmental settings from an XML file
			**
			**	Structure of the file is given:
			**
				<?xml version="1.0" encoding="UTF-8"?>
					<ListenerEnvironment Name="NameOfTheEnvironment">
						<Environment>PSYCHOTIC</Environment>
						<EnvironmentSize>1.8</EnvironmentSize>
						<EnvironmentDiffusion>1.8</EnvironmentDiffusion>
						<Room>-1000</Room>
						<RoomHF>-400</RoomHF>
						<RoomRolloffFactor>0.0</RoomRolloffFactor>
						<AirAbsorptionHF>-5.0</AirAbsorptionHF>
						<Reflections>-1713</Reflections>
						<ReflectionsDelay>0.02</ReflectionsDelay>
						<Reverb>-613</Reverb>
						<ReverbDelay>0.03</ReverbDelay>
						<DecayTime>17.23</DecayTime>
						<DecayHFRatio>0.56</DecayHFRatio>
						<Flags flag1="DECAYTIMESCALE" flag2="REFLECTIONSSCALE" flag3="REFLECTIONSDELAYSCALE" flag4="REVERBSCALE" flag5="REVERBDELAYSCALE"/>
					</ListenerEnvironment>
			**
			**	Warning - names MUST match!
			*/
			void LoadListenerEnvironmentFromXMLFile (FILESYSTEM::C_XMLDocument* file);

			/*!	Loads source environmental settings from an XML file
			**
			**	Structure of the file is given:
			**
				<?xml version="1.0" encoding="UTF-8"?>
					<SourceEnvironment Name="NameOfTheEnvironment">
						<Direct>1</Direct>
						<DirectHF>1</DirectHF>
						<Room>1</Room>
						<RoomHF>-1000</RoomHF>
						<RoomRolloffFactor>0.0</RoomRolloffFactor>
						<Obstruction>5</Obstruction>
						<ObstructionLFRatio>-1713</ObstructionLFRatio>
						<Occlusion>0.02</Occlusion>
						<OcclusionLFRatio>-613</OcclusionLFRatio>
						<OcclusionRoomRatio>0.03</OcclusionRoomRatio>
						<OutsideVolumeHF>17.23</OutsideVolumeHF>
						<AirAbsorptionFactor>0.56</AirAbsorptionFactor>
						<Flags flag1="DIRECTHFAUTO" flag2="ROOMAUTO" flag3="ROOMHFAUTO"/>
					</SourceEnvironment>
			**
			**	Warning - names MUST match!
			*/
			void LoadSourceEnvironmentFromXMLFile (FILESYSTEM::C_XMLDocument* file, C_SoundSource* source);

			/*!	Sets default environmental settings to given source
			*/
			void SetSourceDefaultEnvironmentalSettings (C_SoundSource* source);

		private:
			/*!	Returns listener properties based on preset
			*/
			S_AudioEAXListenerProperties PresetListenerEnvironment (EListenerEnvironmentPreset preset);

			/*!	Helper method for creating listener presets
			*/
			S_AudioEAXListenerProperties CreateListenerEnvironment (unsigned long Environment, float EnvironmentSize, float EnvironmentDiffusion, 
				long Room,	long RoomHF, float RoomRolloffFactor, float AirAbsorptionHF, long Reflections, float lReflectionsDelay,
				long Reverb, float ReverbDelay, float DecayTime, float DecayHFRatio);

		private:
			EAXSet							m_OpenALEAXSet;
			EAXGet							m_OpenALEAXGet;

			ALuint							m_DataSizeListener;
			ALuint							m_DataSizeBuffer;
			S_AudioEAXListenerProperties	m_DefaultListener;
			S_AudioEAXListenerProperties	m_CurrentListener;
		};

	}	//	namespace AUDIO
}	//	namespace REDEEMER

#endif	//	_R_AUDIO_EAX2SYSTEM_H_
