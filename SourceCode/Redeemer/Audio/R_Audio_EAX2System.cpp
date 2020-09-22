/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Audio_EAX2System.cpp
**		EAX 2 is used for environmental effects handling within OpenAL
*/

#include "R_Audio_EAX2System.h"
#include "R_Audio_SoundSource.h"
#include "..\\Core\\R_Core_Utility.h"
#include "..\\FileSystem\\R_FileSystem_XMLDocument.h"

#include <al.h>
#include "eax.h"

namespace REDEEMER
{
	namespace AUDIO
	{
		//--------------------------------------------------------------------------------------------------------------------------------
		C_EAX2System::C_EAX2System () :
			m_OpenALEAXGet (0),
			m_OpenALEAXSet (0),
			m_DataSizeListener ((ALuint)sizeof(S_AudioEAXListenerProperties)),
			m_DataSizeBuffer ((ALuint)sizeof(S_AudioEAXSourceProperties))
		{
			//	EMPTY
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		C_EAX2System::~C_EAX2System()
		{
			if (IsInitialized())
				Finalize();
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		int C_EAX2System::Initialize ()
		{
			C_BaseClass::Initialize();

			if (alIsExtensionPresent ("EAX2.0") == AL_FALSE)
				return RESULT_FAIL;

			m_OpenALEAXSet = (EAXSet)alGetProcAddress("EAXSet");
			m_OpenALEAXGet = (EAXGet)alGetProcAddress("EAXGet");

			if (!m_OpenALEAXSet || !m_OpenALEAXGet)
				return RESULT_FAIL;

			m_OpenALEAXGet (&DSPROPSETID_EAX_ListenerProperties, DSPROPERTY_EAXLISTENER_ALLPARAMETERS, 0, &m_DefaultListener, m_DataSizeListener);

			return RESULT_OK;
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		int C_EAX2System::Finalize ()
		{
			SetListenerDefaultEnvironment();

			m_OpenALEAXGet = m_OpenALEAXSet = 0;

			return C_BaseClass::Finalize();
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		void C_EAX2System::SetListenerEnvironment(S_AudioEAXListenerProperties* lpData)
		{
			if (!m_OpenALEAXSet) 
				return;

			memcpy (&m_CurrentListener, lpData, m_DataSizeListener);

			m_OpenALEAXSet (&DSPROPSETID_EAX_ListenerProperties, DSPROPERTY_EAXLISTENER_ALLPARAMETERS, 0, lpData, m_DataSizeListener);
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		void C_EAX2System::SetListenerEnvironmentPreset (EListenerEnvironmentPreset preset)
		{
			S_AudioEAXListenerProperties properties = PresetListenerEnvironment (preset);

			SetListenerEnvironment (&properties);
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		void C_EAX2System::SetListenerDefaultEnvironment()
		{
			SetListenerEnvironment (&m_DefaultListener);
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		S_AudioEAXListenerProperties C_EAX2System::GetListenerEnvironment()
		{
			return m_CurrentListener;
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		void C_EAX2System::LoadListenerEnvironmentFromXMLFile (FILESYSTEM::C_XMLDocument* file)
		{
			S_AudioEAXListenerProperties properties = PresetListenerEnvironment (LEP_Generic);
			FILESYSTEM::C_XMLDocument::C_XMLNode* root = file->GetRootNode();

			if (root == NULL || root->GetName().compare("ListenerEnvironment") != 0)
				return;

			FILESYSTEM::C_XMLDocument::C_XMLNodeEnumerator node = root->GetNodeEnumerator();

			while (node.Next ())
			{
				std::string content = node->GetContent();

				if (node->GetName().compare("Environment") == 0)
				{
					if (content == "GENERIC") { properties.Environment = EAX_ENVIRONMENT_GENERIC; continue; }
					if (content == "PADDEDCELL") { properties.Environment = EAX_ENVIRONMENT_PADDEDCELL; continue; }
					if (content == "ROOM") { properties.Environment = EAX_ENVIRONMENT_ROOM; continue; }
					if (content == "LIVINGROOM") { properties.Environment = EAX_ENVIRONMENT_LIVINGROOM; continue; }
					if (content == "STONEROOM") { properties.Environment = EAX_ENVIRONMENT_STONEROOM; continue; }
					if (content == "AUDITORIUM") { properties.Environment = EAX_ENVIRONMENT_AUDITORIUM; continue; }
					if (content == "CONCERTHALL") { properties.Environment = EAX_ENVIRONMENT_CONCERTHALL; continue; }
					if (content == "CAVE") { properties.Environment = EAX_ENVIRONMENT_CAVE; continue; }
					if (content == "ARENA") { properties.Environment = EAX_ENVIRONMENT_ARENA; continue; }
					if (content == "HANGAR") { properties.Environment = EAX_ENVIRONMENT_HANGAR; continue; }
					if (content == "CARPETEDHALLWAY") { properties.Environment = EAX_ENVIRONMENT_CARPETEDHALLWAY; continue; }
					if (content == "HALLWAY") { properties.Environment = EAX_ENVIRONMENT_HALLWAY; continue; }
					if (content == "STONECORRIDOR") { properties.Environment = EAX_ENVIRONMENT_STONECORRIDOR; continue; }
					if (content == "ALLEY") { properties.Environment = EAX_ENVIRONMENT_ALLEY; continue; }
					if (content == "FOREST") { properties.Environment = EAX_ENVIRONMENT_FOREST; continue; }
					if (content == "CITY") { properties.Environment = EAX_ENVIRONMENT_CITY; continue; }
					if (content == "MOUNTAINS") { properties.Environment = EAX_ENVIRONMENT_MOUNTAINS; continue; }
					if (content == "QUARRY") { properties.Environment = EAX_ENVIRONMENT_QUARRY; continue; }
					if (content == "PLAIN") { properties.Environment = EAX_ENVIRONMENT_PLAIN; continue; }
					if (content == "PARKINGLOT") { properties.Environment = EAX_ENVIRONMENT_PARKINGLOT; continue; }
					if (content == "SEWERPIPE") { properties.Environment = EAX_ENVIRONMENT_SEWERPIPE; continue; }
					if (content == "UNDERWATER") { properties.Environment = EAX_ENVIRONMENT_UNDERWATER; continue; }
					if (content == "DRUGGED") { properties.Environment = EAX_ENVIRONMENT_DRUGGED; continue; }
					if (content == "DIZZY") { properties.Environment = EAX_ENVIRONMENT_DIZZY; continue; }
					if (content == "PSYCHOTIC") { properties.Environment = EAX_ENVIRONMENT_PSYCHOTIC; continue; }
				}

				if (node->GetName().compare("EnvironmentSize") == 0)
				{
					properties.EnvironmentSize = CORE::C_Utility::ParseFloat(content.c_str()); 
					
					continue; 
				}

				if (node->GetName().compare("EnvironmentDiffusion") == 0)
				{
					properties.EnvironmentDiffusion = CORE::C_Utility::ParseFloat(content.c_str()); 
					continue; 
				}

				if (node->GetName().compare("Room") == 0)
				{
					properties.Room = (long)CORE::C_Utility::ParseInt(content.c_str()); 
					
					continue; 
				}

				if (node->GetName().compare("RoomHF") == 0)
				{
					properties.RoomHF = (long)CORE::C_Utility::ParseInt(content.c_str()); 
					
					continue; 
				}

				if (node->GetName().compare("RoomRolloffFactor") == 0)
				{
					properties.RoomRolloffFactor = CORE::C_Utility::ParseFloat(content.c_str()); 
					
					continue; 
				}

				if (node->GetName().compare("AirAbsorptionHF") == 0)
				{
					properties.AirAbsorptionHF = CORE::C_Utility::ParseFloat(content.c_str()); 
					
					continue; 
				}

				if (node->GetName().compare("Reflections") == 0)
				{
					properties.Reflections = (long)CORE::C_Utility::ParseInt(content.c_str());
					
					continue; 
				}

				if (node->GetName().compare("ReflectionsDelay") == 0)
				{
					properties.ReflectionsDelay = CORE::C_Utility::ParseFloat(content.c_str()); 
					
					continue; 
				}

				if (node->GetName().compare("Reverb") == 0)
				{
					properties.Reverb = (long)CORE::C_Utility::ParseInt(content.c_str()); 
					
					continue; 
				}

				if (node->GetName().compare("ReverbDelay") == 0)
				{
					properties.ReverbDelay = CORE::C_Utility::ParseFloat(content.c_str()); 
					
					continue; 
				}

				if (node->GetName().compare("DecayTime") == 0)
				{
					properties.DecayTime = CORE::C_Utility::ParseFloat(content.c_str()); 
					
					continue; 
				}

				if (node->GetName().compare("DecayHFRatio") == 0)
				{
					properties.DecayHFRatio = CORE::C_Utility::ParseFloat(content.c_str()); 
					
					continue; 
				}

				if (node->GetName().compare("Flags") == 0)
				{
					FILESYSTEM::C_XMLDocument::VectorAttributes attributes = node->GetAttributes();

					for (unsigned int i = 0; i < attributes.size(); i ++)
					{
						if (attributes[i].second == "DECAYTIMESCALE") 
						{
							properties.Flags |= EAXLISTENERFLAGS_DECAYTIMESCALE; 
							continue; 
						}

						if (attributes[i].second == "REFLECTIONSSCALE") 
						{
							properties.Flags |= EAXLISTENERFLAGS_REFLECTIONSSCALE; 
							continue; 
						}

						if (attributes[i].second == "REFLECTIONSDELAYSCALE")
						{
							properties.Flags |= EAXLISTENERFLAGS_REFLECTIONSDELAYSCALE;
							continue; 
						}

						if (attributes[i].second == "REVERBSCALE") 
						{
							properties.Flags |= EAXLISTENERFLAGS_REVERBSCALE; 
							continue; 
						}

						if (attributes[i].second == "REVERBDELAYSCALE") 
						{
							properties.Flags |= EAXLISTENERFLAGS_REVERBDELAYSCALE; 
							continue; 
						}

						if (attributes[i].second == "DECAYHFLIMIT")
						{
							properties.Flags |= EAXLISTENERFLAGS_DECAYHFLIMIT; 
							continue; 
						}
					}
				}
			}

			SetListenerEnvironment (&properties);
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		void C_EAX2System::LoadSourceEnvironmentFromXMLFile (FILESYSTEM::C_XMLDocument* file, C_SoundSource* source)
		{
			S_AudioEAXSourceProperties properties;

			memset (&properties, 0, sizeof (S_AudioEAXSourceProperties));

			FILESYSTEM::C_XMLDocument::C_XMLNode* root = file->GetRootNode();

			if (root == NULL || root->GetName().compare("SourceEnvironment") != 0)
				return;

			FILESYSTEM::C_XMLDocument::C_XMLNodeEnumerator node = root->GetNodeEnumerator();

			while (node.Next ())
			{
				std::string content = node->GetContent();

				if (node->GetName().compare("Direct") == 0)
				{
					properties.Direct = CORE::C_Utility::ParseInt(content.c_str()); 

					continue; 
				}

				if (node->GetName().compare("DirectHF") == 0)
				{
					properties.DirectHF = CORE::C_Utility::ParseInt(content.c_str()); 
					continue; 
				}

				if (node->GetName().compare("Room") == 0)
				{
					properties.Room = (long)CORE::C_Utility::ParseInt(content.c_str()); 

					continue; 
				}

				if (node->GetName().compare("RoomHF") == 0)
				{
					properties.RoomHF = (long)CORE::C_Utility::ParseInt(content.c_str()); 

					continue; 
				}

				if (node->GetName().compare("RoomRolloffFactor") == 0)
				{
					properties.RoomRolloffFactor = CORE::C_Utility::ParseFloat(content.c_str()); 

					continue; 
				}

				if (node->GetName().compare("Obstruction") == 0)
				{
					properties.Obstruction = CORE::C_Utility::ParseInt(content.c_str()); 

					continue; 
				}

				if (node->GetName().compare("ObstructionLFRatio") == 0)
				{
					properties.ObstructionLFRatio = CORE::C_Utility::ParseFloat(content.c_str());

					continue; 
				}

				if (node->GetName().compare("Occlusion") == 0)
				{
					properties.Occlusion = CORE::C_Utility::ParseInt(content.c_str()); 

					continue; 
				}

				if (node->GetName().compare("OcclusionLFRatio") == 0)
				{
					properties.ObstructionLFRatio = CORE::C_Utility::ParseFloat(content.c_str()); 

					continue; 
				}

				if (node->GetName().compare("OcclusionRoomRatio") == 0)
				{
					properties.OcclusionRoomRatio = CORE::C_Utility::ParseFloat(content.c_str()); 

					continue; 
				}

				if (node->GetName().compare("OutsideVolumeHF") == 0)
				{
					properties.OutsideVolumeHF = CORE::C_Utility::ParseInt(content.c_str()); 

					continue; 
				}

				if (node->GetName().compare("AirAbsorptionFactor") == 0)
				{
					properties.AirAbsorptionFactor = CORE::C_Utility::ParseFloat(content.c_str()); 

					continue; 
				}

				if (node->GetName().compare("Flags") == 0)
				{
					FILESYSTEM::C_XMLDocument::VectorAttributes attributes = node->GetAttributes();

					for (unsigned int i = 0; i < attributes.size(); i ++)
					{
						if (attributes[i].second == "DIRECTHFAUTO") 
						{
							properties.Flags |= EAXBUFFERFLAGS_DIRECTHFAUTO; 
							continue; 
						}

						if (attributes[i].second == "ROOMAUTO") 
						{
							properties.Flags |= EAXBUFFERFLAGS_ROOMAUTO; 
							continue; 
						}

						if (attributes[i].second == "ROOMHFAUTO")
						{
							properties.Flags |= EAXBUFFERFLAGS_ROOMHFAUTO;
							continue; 
						}
					}
				}
			}

			SetSourceEnvironmentalSettings (&properties, source);
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		void C_EAX2System::SetSourceEnvironmentalSettings (S_AudioEAXSourceProperties* data, C_SoundSource* source)
		{
			if (!m_OpenALEAXSet) 
				return;

			m_OpenALEAXSet (&DSPROPSETID_EAX_BufferProperties, DSPROPERTY_EAXBUFFER_ALLPARAMETERS, source->GetSourceID(), data, m_DataSizeBuffer);
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		S_AudioEAXSourceProperties C_EAX2System::GetSourceEnvironmentalSettings (C_SoundSource* source)
		{
			S_AudioEAXSourceProperties ret;

			if (!m_OpenALEAXGet)
			{
				memset(&ret, 0, sizeof(S_AudioEAXSourceProperties));

				return ret;
			}

			m_OpenALEAXGet (&DSPROPSETID_EAX_BufferProperties, DSPROPERTY_EAXBUFFER_ALLPARAMETERS, source->GetSourceID(), &ret, m_DataSizeBuffer);

			return ret;
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		void C_EAX2System::SetSourceDefaultEnvironmentalSettings (C_SoundSource* source)
		{
			S_AudioEAXSourceProperties ret;

			ret.Direct = 0;
			ret.DirectHF = 0;
			ret.Room = 0;
			ret.RoomHF = 0;
			ret.Obstruction = 0;
			ret.ObstructionLFRatio = 0.0f;
			ret.Occlusion = 0;
			ret.OcclusionLFRatio = 0.25f;
			ret.OcclusionRoomRatio = 0.5f;
			ret.RoomRolloffFactor = 0.0f;
			ret.AirAbsorptionFactor = 1.0f;
			ret.OutsideVolumeHF = 0;
			ret.Flags = EAX_BUFFERFLAGS_DIRECTHFAUTO | EAX_BUFFERFLAGS_ROOMAUTO | EAX_BUFFERFLAGS_ROOMHFAUTO;

			SetSourceEnvironmentalSettings (&ret, source);
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		S_AudioEAXListenerProperties C_EAX2System::PresetListenerEnvironment (EListenerEnvironmentPreset preset)
		{
			//	Some magic values :)

			switch (preset)
			{
			case LEP_Generic:			
				return CreateListenerEnvironment (EAX_ENVIRONMENT_GENERIC,7.5f,1.0f,-1000,-100,0.0f,-5.0f,-2602,0.007f,200,0.011f,1.49f,0.83f);
				break;

			case LEP_PaddedCell:		
				return CreateListenerEnvironment (EAX_ENVIRONMENT_PADDEDCELL,1.4f,1.0f,-1000,-6000,0.0f,-5.0f,-1204,0.002f,207,0.02f,0.17f,0.1f); 
				break;

			case LEP_Room:
				return CreateListenerEnvironment (EAX_ENVIRONMENT_ROOM,1.9f,1.0f,-1000,-454,0.0f,-5.0f,-1646,0.002f,53,0.003f,0.4f,0.83f); 
				break;

			case LEP_BathRoom:
				return CreateListenerEnvironment (EAX_ENVIRONMENT_BATHROOM,1.4f,1.0f,-1000,-1200,0.0f,-5.0f,-370,0.007f,1030,0.011f,1.49f,0.54f); 
				break;

			case LEP_LivingRoom:	
				return CreateListenerEnvironment (EAX_ENVIRONMENT_LIVINGROOM,2.5f,1.0f,-1000,-6000,0.0f,-5.0f,-1376,0.003f,-1104,0.004f,0.5f,0.1f); 
				break;

			case LEP_StoneRoom:
				return CreateListenerEnvironment (EAX_ENVIRONMENT_STONEROOM,11.6f,1.0f,-1000,-300,0.0f,-5.0f,-711,0.012f,83,0.017f,2.31f,0.64f); 
				break;

			case LEP_Auditorium:
				return CreateListenerEnvironment (EAX_ENVIRONMENT_AUDITORIUM,21.6f,1.0f,-1000,-476,0.0f,-5.0f,-789,0.02f,-289,0.03f,4.32f,0.59f); 
				break;

			case LEP_ConcertHall:
				return CreateListenerEnvironment (EAX_ENVIRONMENT_CONCERTHALL,19.6f,1.0f,-1000,-500,0.0f,-5.0f,-1230,0.02f,-2,0.029f,3.92f,0.7f);
				break;

			case LEP_Cave:
				return CreateListenerEnvironment (EAX_ENVIRONMENT_CAVE,14.6f,1.0f,-1000,0,0.0f,-5.0f,-602,0.015f,-302,0.022f,2.91f,1.3f); 
				break;

			case LEP_Arena:
				return CreateListenerEnvironment (EAX_ENVIRONMENT_ARENA,36.2f,1.0f,-1000,-698,0.0f,-5.0f,-1166,0.02f,16,0.03f,7.24f,0.33f); 
				break;

			case LEP_Hangar:
				return CreateListenerEnvironment (EAX_ENVIRONMENT_HANGAR,50.3f,1.0f,-1000,-1000,0.0f,-5.0f,-602,0.02f,198,0.03f,10.05f,0.23f); 
				break;

			case LEP_CarpetedHallway:
				return CreateListenerEnvironment (EAX_ENVIRONMENT_CARPETEDHALLWAY,1.9f,1.0f,-1000,-4000,0.0f,-5.0f,-1831,0.002f,-1630,0.03f,0.3f,0.1f); 
				break;

			case LEP_Hallway:
				return CreateListenerEnvironment (EAX_ENVIRONMENT_HALLWAY,1.8f,1.0f,-1000,-300,0.0f,-5.0f,-1219,0.007f,441,0.011f,1.49f,0.59f); 
				break;

			case LEP_StoneCorridor:
				return CreateListenerEnvironment (EAX_ENVIRONMENT_STONECORRIDOR,13.5f,1.0f,-1000,-237,0.0f,-5.0f,-1214,0.013f,395,0.02f,2.7f,0.79f);
				break;

			case LEP_Alley:	
				return CreateListenerEnvironment (EAX_ENVIRONMENT_ALLEY,7.5f,0.3f,-1000,-270,0.0f,-5.0f,-1204,0.007f,-4,0.011f,1.49f,0.86f); 
				break;

			case LEP_Forest:
				return CreateListenerEnvironment (EAX_ENVIRONMENT_FOREST,38.0f,0.3f,-1000,-33000,0.0f,-5.0f,-2560,0.162f,-229,0.088f,1.49f,0.54f);
				break;

			case LEP_City:
				return CreateListenerEnvironment (EAX_ENVIRONMENT_CITY,7.5f,0.5f,-1000,-800,0.0f,-5.0f,-2273,0.007f,-1691,0.011f,1.49f,0.67f);
				break;

			case LEP_Mountains:
				return CreateListenerEnvironment (EAX_ENVIRONMENT_MOUNTAINS,100.0f,0.27f,-1000,-2500,0.0f,-5.0f,-2780,0.3f,-1434,0.1f,1.49f,0.21f);
				break;

			case LEP_Quarry:	
				return CreateListenerEnvironment (EAX_ENVIRONMENT_QUARRY,17.5f,1.0f,-1000,-1000,0.0f,-5.0f,-1000,0.061f,500,0.025f,1.49f,0.83f);
				break;

			case LEP_Plain:		
				return CreateListenerEnvironment (EAX_ENVIRONMENT_PLAIN,42.5f,0.21f,-1000,-2000,0.0f,-5.0f,-2466,0.179f,-1925,0.1f,1.49f,0.5f); 
				break;

			case LEP_ParkingLot:	
				return CreateListenerEnvironment (EAX_ENVIRONMENT_PARKINGLOT,8.3f,1.0f,-1000,0,0.0f,-5.0f,-1363,0.008f,-1153,0.012f,1.65f,1.5f);
				break;

			case LEP_ServerPipe:	
				return CreateListenerEnvironment (EAX_ENVIRONMENT_SEWERPIPE,1.7f,0.8f,-1000,-1000,0.0f,-5.0f,429,0.014f,1023,0.021f,2.81f,0.14f);
				break;

			case LEP_Underwater:
				return CreateListenerEnvironment (EAX_ENVIRONMENT_UNDERWATER,1.8f,1.0f,-1000,-4000,0.0f,-5.0f,-449,0.007f,1700,0.011f,1.49f,0.1f); 
				break;

			case LEP_Drugged:		
				return CreateListenerEnvironment (EAX_ENVIRONMENT_DRUGGED,1.9f,0.5f,-1000,0,0.0f,-5.0f,-115,0.002f,985,0.03f,8.39f,1.39f);
				break;

			case LEP_Dizzy:
				return CreateListenerEnvironment (EAX_ENVIRONMENT_DIZZY,1.8f,0.6f,-1000,-400,0.0f,-5.0f,-1713,0.02f,-613,0.03f,17.23f,0.56f); 
				break;

			case LEP_Psychotic:
				return CreateListenerEnvironment (EAX_ENVIRONMENT_PSYCHOTIC,1.0f,0.5f,-1000,-151,0.0f,-5.0f,-626,0.02f,774,0.03f,7.56f,0.91f); 
				break;
			}

			S_AudioEAXListenerProperties env;

			memset (&env, 0, sizeof (S_AudioEAXListenerProperties));

			return env;
		}

		//--------------------------------------------------------------------------------------------------------------------------------

		S_AudioEAXListenerProperties C_EAX2System::CreateListenerEnvironment (unsigned long Environment, float EnvironmentSize, float EnvironmentDiffusion, 
			long Room,	long RoomHF, float RoomRolloffFactor, float AirAbsorptionHF, long Reflections, float ReflectionsDelay,
			long Reverb, float ReverbDelay, float DecayTime, float DecayHFRatio)
		{
			S_AudioEAXListenerProperties ret;

			ret.Room = Room;
			ret.RoomHF = RoomHF;
			ret.RoomRolloffFactor = RoomRolloffFactor;
			ret.DecayTime = DecayTime;
			ret.DecayHFRatio = DecayHFRatio;
			ret.Reflections = Reflections;
			ret.ReflectionsDelay = ReflectionsDelay;
			ret.Reverb = Reverb;
			ret.ReverbDelay = ReverbDelay;
			ret.Environment = Environment;
			ret.EnvironmentSize = EnvironmentSize;
			ret.EnvironmentDiffusion = EnvironmentDiffusion;
			ret.AirAbsorptionHF = AirAbsorptionHF;
			ret.Flags = EAXLISTENER_DEFAULTFLAGS;

			return ret;
		}

		//--------------------------------------------------------------------------------------------------------------------------------

	}	//	namespace AUDIO
}	//	namespace REDEEMER

