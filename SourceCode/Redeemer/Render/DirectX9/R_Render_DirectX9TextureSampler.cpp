/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_DirectX9TextureSampler.cpp
**		Class used for handling various parameters for textures in DirectX 9, such as addressing modes, etc.
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_DirectX9TextureSampler.h"
#include "R_Render_DirectX9RenderDevice.h"
#include "..\\..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_DirectX9TextureSampler::C_DirectX9TextureSampler ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DirectX9TextureSampler::~C_DirectX9TextureSampler ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9TextureSampler::SetFilterState (ETextureFilteringType type, ETextureFilteringMode mode)
		{
			C_TextureSampler::SetFilterState(type, mode);

			DWORD value;
			D3DSAMPLERSTATETYPE sampler;

			switch (type)
			{
			case TFT_Min:
				sampler = D3DSAMP_MINFILTER;
				break;

			case TFT_Mag:
				sampler = D3DSAMP_MAGFILTER;
				break;

			case TFT_Mip:
				sampler = D3DSAMP_MIPFILTER;
				break;
			}

			switch (mode)
			{
			case TFM_Point:
				value = D3DTEXF_POINT;
				break;

			case TFM_Linear:
				value = D3DTEXF_LINEAR;
				break;

			case TFM_Anisotropic:
				value = D3DTEXF_ANISOTROPIC;
				break;
			}

			C_DirectX9RenderDevice* device = static_cast<C_DirectX9RenderDevice*>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice());

			device->GetDirectX9Device()->SetSamplerState (m_TextureStage, sampler, value);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9TextureSampler::SetAddressingState (ETextureAddressingType type, ETextureAddressingMode mode)
		{
			C_TextureSampler::SetAddressingState (type, mode);

			C_DirectX9RenderDevice* device = static_cast<C_DirectX9RenderDevice*>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice());
			DWORD value;
			D3DSAMPLERSTATETYPE sampler;

			switch (type)
			{
			case TAT_AddressU:
				sampler = D3DSAMP_ADDRESSU;
				break;

			case TAT_AddressV:
				sampler = D3DSAMP_ADDRESSV;
				break;
			}

			switch (mode)
			{
			case TAM_Wrap:
				value = D3DTADDRESS_WRAP;
				break;

			case TAM_Border:
				value = D3DTADDRESS_BORDER;
				device->GetDirectX9Device()->SetSamplerState (m_TextureStage, D3DSAMP_BORDERCOLOR, 0x00000000);
				break;

			case TAM_Clamp:
				value = D3DTADDRESS_CLAMP;
				break;

			case TAM_Mirror:
				value = D3DTADDRESS_MIRROR;
				break;
			}

			device->GetDirectX9Device()->SetSamplerState (m_TextureStage, sampler, value);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9TextureSampler::BindForRendering ()
		{
			SetFilterState (TFT_Min, m_MinFilteringMode);
			SetFilterState (TFT_Mag, m_MagFilteringMode);
			SetFilterState (TFT_Mip, m_MipFilteringMode);

			SetAddressingState (TAT_AddressU, m_AddressingModeU);
			SetAddressingState (TAT_AddressV, m_AddressingModeV);

			if (m_Texture)
				m_Texture->BindForRendering (m_TextureStage, m_ForceSet);
			else
				static_cast<C_DirectX9RenderDevice*>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice())->GetDirectX9Device()->SetTexture(m_TextureStage, NULL);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9TextureSampler::UnbindFromRendering ()
		{
			if (m_Texture)
				m_Texture->UnbindFromRendering ();
			else
				static_cast<C_DirectX9RenderDevice*>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice())->GetDirectX9Device()->SetTexture(m_TextureStage, NULL);
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER

