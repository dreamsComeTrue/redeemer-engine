/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_TextureSampler.cpp
**		Class used for handling various parameters for textures, such as addressing modes, etc.
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_TextureSampler.h"
#include "R_Render_RenderDevice.h"
#include "..\\FileSystem\\R_FileSystem_PhysicalFileANSI.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_TextureSampler::C_TextureSampler () :
			m_Texture (0),
			m_TextureStage (0),
			m_ForceSet (true),
			m_MinFilteringMode (TFM_Linear),
			m_MagFilteringMode (TFM_Linear),
			m_MipFilteringMode (TFM_Linear),
			m_AddressingModeU (TAM_Clamp),
			m_AddressingModeV (TAM_Clamp)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_TextureSampler::~C_TextureSampler ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_TextureSampler::SetFilterState (ETextureFilteringType type, ETextureFilteringMode mode)
		{
			switch (type)
			{
			case TFT_Min:
				m_MinFilteringMode = mode;
				break;

			case TFT_Mag:
				m_MagFilteringMode = mode;
				break;

			case TFT_Mip:
				m_MipFilteringMode = mode;
				break;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_TextureSampler::SetAddressingState (ETextureAddressingType type, ETextureAddressingMode mode)
		{
			switch (type)
			{
			case TAT_AddressU:
				m_AddressingModeU = mode;
				break;

			case TAT_AddressV:
				m_AddressingModeV = mode;
				break;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_TextureSampler::SetTexture (C_Texture* texture)
		{
			m_Texture = texture;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Texture* C_TextureSampler::GetTexture ()
		{
			return m_Texture;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_TextureSampler::Parse (FILESYSTEM::C_XMLDocument::C_XMLNode* xmlFile)
		{
			// Parse child nodes
			FILESYSTEM::C_XMLDocument::C_XMLNode* node = xmlFile;

			// Parse data (independent from texture file)

			// Parse texture data
			std::string name = node->FindAttribute ("Name");
			std::wstring stName(name.begin(), name.end());

			std::string stTextureUnit = node->FindAttribute ("Sampler");
			if(!stTextureUnit.compare("") == 0)
				m_TextureStage = atoi(stTextureUnit.c_str());

			std::string sForceSet = node->FindAttribute ("BindAsTexture");
			if ((sForceSet.compare("") != 0))
			{
				if (sForceSet.compare("1") == 0 || sForceSet.compare("true") == 0)
					m_ForceSet = true;

				if (sForceSet.compare("0") == 0 || sForceSet.compare("false") == 0)
					m_ForceSet = false;
			}

			if (node->FindAttribute ("MinFilter") != "")
				SetFilterState (TFT_Min, ParseFiltering (node->FindAttribute ("MinFilter")));

			if (node->FindAttribute ("MagFilter") != "")
				SetFilterState (TFT_Mag, ParseFiltering (node->FindAttribute ("MagFilter")));

			if (node->FindAttribute ("MipFilter") != "")
				SetFilterState (TFT_Mip, ParseFiltering (node->FindAttribute ("MipFilter")));

			C_RenderDevice* device = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice();

			m_Texture = device->GetTextureManager()->GetTexture (stName);

			if (!m_Texture)
			{
				if (sForceSet == "")
					m_ForceSet = false;

				// Texture format
				std::string stFormat = node->FindAttribute ("Format");
				ERendererTextureFormat format = ParseFormat (stFormat);

				// Texture usage
				std::string stUsage = node->FindAttribute ("Usage");
				ERendererTextureUsage usage = RTU_Normal;

				if (stUsage.compare("RenderTarget") == 0)
					usage = RTU_RenderTarget;

				// Texture file data
				std::string stFile = node->FindAttribute ("FileName");
				std::wstring fileName(stFile.begin(), stFile.end());

				if (stFile.compare("") == 0 || usage  == RTU_RenderTarget)
				{
					CORE::C_Vector2D<unsigned int> dimension = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetRenderVideoMode ().ScreenResolution;

					m_Texture = device->GetTextureManager()->CreateTexture (stName, format, RTT_2D, usage, dimension.Width, dimension.Height);
				}
				else
				{
					FILESYSTEM::C_PhysicalFileANSI textureFile (fileName, FILESYSTEM::FOM_Read | FILESYSTEM::FOM_Binary);

					m_Texture = device->GetTextureManager()->CreateTexture (&textureFile, stName, format, RTT_2D, usage);
					textureFile.Close();
				}

				device->GetTextureManager()->AddTexture (stName, m_Texture);				
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		ETextureFilteringMode C_TextureSampler::ParseFiltering (const std::string& data)
		{
			if(data.compare("FILTER_NONE") == 0)
				return TFM_Point;
			else if(data.compare("FILTER_NEAREST") == 0)
				return TFM_Point;
			else if(data.compare("FILTER_TRILINEAR") == 0)
				return TFM_Linear;
			else if(data.compare("FILTER_ANISOTROPIC") == 0)
				return TFM_Anisotropic;
			else
				return TFM_Point;
		}

		//------------------------------------------------------------------------------------------------------------------------

		ERendererTextureFormat C_TextureSampler::ParseFormat(const std::string& data)
		{
			ERendererTextureFormat format = RTF_A8R8G8B8;

			if (data.compare("g16r16f") == 0)
				format = RTF_G16R16F;
			else if(data.compare("a8r8g8b8") == 0)
				format = RTF_A8R8G8B8;
			else if(data.compare("a2r10g10b10") == 0)
				format = RTF_A2R10G10B10;
			else if(data.compare("r8g8b8") == 0)
				format = RTF_R8G8B8;
			else if(data.compare("a1r5g5b5") == 0)
				format = RTF_A1R5G5B5;
			else if(data.compare("r5g5b5") == 0)
				format = RTF_R5G5B5;
			else if(data.compare("r5g6b5") == 0)
				format = RTF_R5G6B5;
			else if(data.compare("a16b16g16r16f") == 0)
				format = RTF_A16B16G16R16F;
			else if(data.compare("g32r32f") == 0)
				format = RTF_G32R32F;
			else if(data.compare("r32f") == 0)
				format = RTF_R32F;
			else if(data.compare("a132b32g32r32f") == 0)
				format = RTF_A32B32G32R32F;
			else if(data.compare("r16f") == 0)
				format = RTF_R16F;
			else if(data.compare("a8") == 0)
				format = RTF_A8;

			return format;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_TextureSampler::IsForceSet() const
		{
			return m_ForceSet;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER

