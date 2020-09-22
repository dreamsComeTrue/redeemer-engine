/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_DirectX9Texture.cpp
**		DirectX 9 texture implementation class
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_DirectX9Texture.h"
#include "R_Render_DirectX9RenderDevice.h"
#include "..\\..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_DirectX9Texture::C_DirectX9Texture (std::wstring name, ERendererTextureFormat format, ERendererTextureType type, 
			ERendererTextureUsage usage, unsigned int width, unsigned int height, unsigned int depth, float ratioWidth, float ratioHeight, 
			unsigned int textureStage) :
			C_Texture (name, format, type, usage, width, height, depth, ratioWidth, ratioHeight, textureStage)
		{
			C_DirectX9RenderDevice* device = static_cast<C_DirectX9RenderDevice*>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice());

			m_2DTexture = NULL;
			m_3DTexture = NULL;
			m_CubeTexture = NULL;
			m_RenderTarget = NULL;
			m_BackBuffer = NULL;
			m_LockedStruct.rect = NULL;
			m_LockedStruct.box = NULL;
			m_BoundAsTexture = false;
			m_TextureStage = 0;

			// Specify DirectX settings
			DWORD dwUsage = 0;

			if (usage == RTU_RenderTarget)
				dwUsage = D3DUSAGE_RENDERTARGET;

			if (usage == RTU_Dynamic)
				dwUsage = D3DUSAGE_DYNAMIC;

			D3DPOOL pool = static_cast<D3DPOOL>(0);
			unsigned int nMipMaps = 0;

			if (usage == RTU_RenderTarget || usage == RTU_Dynamic)
				nMipMaps = 1;

			D3DFORMAT textureFormat = GetFormat (format, dwUsage);

			// Create appropriate texture
			HRESULT hr = 0;

			switch (m_Type)
			{
				// 1D texture
			case RTT_1D: hr = D3DXCreateTexture (device->GetDirectX9Device(), m_Width, 1, nMipMaps, dwUsage, textureFormat, pool, &m_2DTexture);

				if(hr != D3D_OK)
					m_2DTexture = NULL;
				else
					m_LockedStruct.rect = new D3DLOCKED_RECT;
				break;

				// 2D texture
			case RTT_2D: hr = D3DXCreateTexture (device->GetDirectX9Device(), m_Width, m_Height, nMipMaps, dwUsage, textureFormat, pool, &m_2DTexture);

				if(hr != D3D_OK)
					m_2DTexture = NULL;
				else
					m_LockedStruct.rect = new D3DLOCKED_RECT;
				break;

				// 3D texture
			case RTT_3D: hr = D3DXCreateVolumeTexture (device->GetDirectX9Device(), m_Width, m_Height, m_Depth, nMipMaps, dwUsage, textureFormat, pool, &m_3DTexture);

				if(hr != D3D_OK)
					m_3DTexture = NULL;
				else
					m_LockedStruct.box = new D3DLOCKED_BOX;
				break;

				// Cube texture
			case RTT_Cube: hr = D3DXCreateCubeTexture (device->GetDirectX9Device(), m_Width, nMipMaps, dwUsage, textureFormat, pool, &m_CubeTexture);

				if (hr != D3D_OK)
					m_CubeTexture = NULL;
				else
					m_LockedStruct.rect = new D3DLOCKED_RECT;
				break;
			}

			// Something went wrong - find out what
			if(hr != D3D_OK)
			{
				REDEEMER_LOG << LOG_ERROR << L"RENDER: Failed to create DirectX Texture!" << LOG_ENDMESSAGE;
			}
			else
			{
				REDEEMER_LOG << LOG_INFO << L"RENDER: Created texture: '" << m_Name << "'." << LOG_ENDMESSAGE;
			}

			if (m_Usage == RTU_RenderTarget)
				CreateRenderTarget();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DirectX9Texture::C_DirectX9Texture (FILESYSTEM::IDataFile* file, std::wstring name, ERendererTextureFormat format, ERendererTextureType type,
			ERendererTextureUsage usage) :
			C_Texture (name, format, type, usage)
		{
			C_DirectX9RenderDevice* device = static_cast<C_DirectX9RenderDevice*>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice());

			m_2DTexture = NULL;
			m_3DTexture = NULL;
			m_CubeTexture = NULL;
			m_RenderTarget = NULL;
			m_BackBuffer = NULL;
			m_LockedStruct.rect = NULL;
			m_LockedStruct.box = NULL;
			m_BoundAsTexture = false;
			m_TextureStage = 0;

			// Specify DirectX settings
			DWORD dwUsage = 0;

			if (usage == RTU_RenderTarget)
				dwUsage = D3DUSAGE_RENDERTARGET;

			if (usage == RTU_Dynamic)
				dwUsage = D3DUSAGE_DYNAMIC;

			D3DPOOL pool = static_cast<D3DPOOL>(0);
			unsigned int nMipMaps = 0;

			if (usage == RTU_RenderTarget || usage == RTU_Dynamic)
				nMipMaps = 1;

			D3DFORMAT textureFormat = GetFormat (format, dwUsage);

			// Create appropriate texture
			HRESULT hr = 0;

			unsigned int size = file->GetSize();
			void* buffer = new char[size];
			file->Read((char*)buffer, size);

			D3DXIMAGE_INFO srcInfo;

			switch (m_Type)
			{
				// 1D texture
			case RTT_1D: hr = D3DXCreateTextureFromFileInMemoryEx (device->GetDirectX9Device(), (void*)buffer, size, 0, 1, nMipMaps,
							 dwUsage, textureFormat, pool, D3DX_DEFAULT, D3DX_DEFAULT, 0, &srcInfo, NULL, &m_2DTexture);

				if (hr != D3D_OK)
					m_2DTexture = NULL;
				break;

				// 2D texture
			case RTT_2D: hr = D3DXCreateTextureFromFileInMemoryEx(device->GetDirectX9Device(), (void*)buffer, size, 0, 0, nMipMaps,
							 dwUsage, textureFormat, pool, D3DX_DEFAULT, D3DX_DEFAULT, 0, &srcInfo, NULL, &m_2DTexture);

				if (hr != D3D_OK)
					m_2DTexture = NULL;
				break;

				// 3D texture
			case RTT_3D: hr = D3DXCreateVolumeTextureFromFileInMemoryEx (device->GetDirectX9Device(), (void*)buffer, size,	0, 0, 0, nMipMaps,
							 dwUsage, textureFormat, pool, D3DX_DEFAULT, 0, 0, &srcInfo, NULL, &m_3DTexture);

				if (hr != D3D_OK)
					m_3DTexture = NULL;
				break;

				// Cube texture
			case RTT_Cube: hr = D3DXCreateCubeTextureFromFileInMemoryEx (device->GetDirectX9Device(),  (void*)buffer, size, 0, nMipMaps,
							   dwUsage, textureFormat, pool, D3DX_DEFAULT, 0, 0, &srcInfo, NULL, &m_CubeTexture);

				if (hr != D3D_OK)
					m_CubeTexture = NULL;
				break;
			}

			// Something went wrong
			if (hr != D3D_OK)
			{
				REDEEMER_LOG << LOG_ERROR << L"RENDER: Failed to create DirectX Texture!" << LOG_ENDMESSAGE;
			}
			else
			{
				if (m_Type == RTT_1D || m_Type == RTT_2D)
				{
					m_Width = srcInfo.Width;
					m_Height = srcInfo.Height;

					m_LockedStruct.rect = new D3DLOCKED_RECT;
				}
				else if(m_Type == RTT_3D)
				{
					m_Width = srcInfo.Width;
					m_Height = srcInfo.Height;
					m_Depth = srcInfo.Depth;

					m_LockedStruct.box = new D3DLOCKED_BOX;
				}
				else if(m_Type == RTT_Cube)
				{
					m_Width = srcInfo.Width;
					m_Height = srcInfo.Height;

					m_LockedStruct.rect = new D3DLOCKED_RECT;
				}

				if (m_Usage == RTU_RenderTarget)
					CreateRenderTarget();

				REDEEMER_LOG << LOG_INFO << L"RENDER: Created texture: '" << m_Name << "' from file: " << file->GetFileName() << LOG_ENDMESSAGE;
			}

			REDEEMER_SAFE_DELETE_ARRAY (buffer);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DirectX9Texture::~C_DirectX9Texture ()
		{
			REDEEMER_SAFE_RELEASE (m_CubeTexture);
			REDEEMER_SAFE_RELEASE (m_3DTexture);
			REDEEMER_SAFE_RELEASE (m_2DTexture);
			REDEEMER_SAFE_RELEASE (m_RenderTarget);
			REDEEMER_SAFE_RELEASE (m_BackBuffer);

			REDEEMER_SAFE_DELETE (m_LockedStruct.rect);
			REDEEMER_SAFE_DELETE (m_LockedStruct.box);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9Texture::SetData (void* data, unsigned int level)
		{
			if (!Lock (level))
				return;

			if (m_Type == RTT_3D)
			{
				DWORD dwBytes = m_LockedStruct.box->RowPitch * m_LockedStruct.box->SlicePitch;

				memcpy (m_LockedStruct.box->pBits, data, dwBytes);
			}
			else
			{
				DWORD dwBytes = m_LockedStruct.rect->Pitch * m_Height;

				memcpy (m_LockedStruct.rect->pBits, data, dwBytes);
			}

			switch(m_Type)
			{
			case RTT_1D: m_2DTexture->UnlockRect (level);
				break;
			case RTT_2D: m_2DTexture->UnlockRect (level);
				break;
			case RTT_3D: m_3DTexture->UnlockBox (level);
				break;
			case RTT_Cube: m_CubeTexture->UnlockRect(static_cast<D3DCUBEMAP_FACES>(0), level);
				break;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void* C_DirectX9Texture::GetData (unsigned int level)
		{
			if (!Lock (level))
				return NULL;

			void* temp = NULL;

			if (m_Type == RTT_3D)
			{
				DWORD dwBytes = m_LockedStruct.box->RowPitch * m_LockedStruct.box->SlicePitch;
				temp = new unsigned char[dwBytes];

				memcpy(temp, m_LockedStruct.box->pBits, dwBytes);
			}
			else
			{
				DWORD dwBytes = m_LockedStruct.rect->Pitch * m_Height;
				temp = new unsigned char[dwBytes];

				memcpy(temp, m_LockedStruct.rect->pBits, dwBytes);
			}

			switch(m_Type)
			{
			case RTT_1D: m_2DTexture->UnlockRect(level);
				break;
			case RTT_2D: m_2DTexture->UnlockRect(level);
				break;
			case RTT_3D: m_3DTexture->UnlockBox(level);
				break;
			case RTT_Cube: m_CubeTexture->UnlockRect(static_cast<D3DCUBEMAP_FACES>(0), level);
				break;
			}

			return temp;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_DirectX9Texture::BindForRendering (unsigned int stage, bool forceSet)
		{
			// If is currently bound as texture
			if (m_BoundAsTexture && !forceSet)
			{
				IDirect3DBaseTexture9* pTexture = NULL;
				C_DirectX9RenderDevice* device = static_cast<C_DirectX9RenderDevice*>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice());

				device->GetDirectX9Device ()->GetTexture (m_TextureStage, &pTexture);
				
				if (pTexture == m_2DTexture)
					UnbindNormal ();
				
				REDEEMER_SAFE_RELEASE (pTexture);

				m_BoundAsTexture = false;
			}

			m_TextureStage = stage;
			m_ForceSet = forceSet;

			if (m_ForceSet)
				BindNormal();
			else
			{
				switch (m_Usage)
				{
				case RTU_Normal:
				case RTU_Dynamic:
					BindNormal ();
					break;

				case RTU_RenderTarget:
					BindRenderTarget ();
					break;
				}
			}

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9Texture::UnbindFromRendering ()
		{
			switch (m_Usage)
			{
			case RTU_RenderTarget:
				UnbindRenderTarget ();
				break;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned int C_DirectX9Texture::GetBitDepth (unsigned int level) const
		{
			if (!Lock (level))
				return 0;

			DWORD bitDepth = 0;

			if(m_Type == RTT_3D)
			{
				bitDepth = (m_LockedStruct.box->RowPitch * m_LockedStruct.box->SlicePitch) / (m_Width * m_Height);
			}
			else
			{
				bitDepth = m_LockedStruct.rect->Pitch / m_Width;
			}

			switch (m_Type)
			{
			case RTT_1D: m_2DTexture->UnlockRect(level);
				break;
			case RTT_2D: m_2DTexture->UnlockRect(level);
				break;
			case RTT_3D: m_3DTexture->UnlockBox(level);
				break;
			case RTT_Cube: m_CubeTexture->UnlockRect(static_cast<D3DCUBEMAP_FACES>(0), level);
				break;
			}

			return bitDepth;
		}

		//------------------------------------------------------------------------------------------------------------------------

		D3DFORMAT C_DirectX9Texture::GetFormat (ERendererTextureFormat& format, DWORD& usage)
		{
			D3DFORMAT textureFormat;

			switch (format)
			{
			case RTF_Default: textureFormat = D3DFMT_UNKNOWN; break;
			case RTF_A8R8G8B8: textureFormat = D3DFMT_A8R8G8B8; break;
			case RTF_A2R10G10B10: textureFormat = D3DFMT_A2R10G10B10; break;
			case RTF_R8G8B8: textureFormat = D3DFMT_X8R8G8B8; break;
			case RTF_A1R5G5B5: textureFormat = D3DFMT_A1R5G5B5; break;
			case RTF_R5G5B5: textureFormat = D3DFMT_X1R5G5B5; break;
			case RTF_R5G6B5: textureFormat = D3DFMT_R5G6B5; break;
			case RTF_G16R16F: textureFormat = D3DFMT_G16R16F; break;
			case RTF_A16B16G16R16F: textureFormat = D3DFMT_A16B16G16R16F; break;
			case RTF_R16F: textureFormat = D3DFMT_R16F; break;
			case RTF_G32R32F: textureFormat = D3DFMT_G32R32F; break;
			case RTF_R32F: textureFormat = D3DFMT_R32F; break;
			case RTF_A32B32G32R32F: textureFormat = D3DFMT_A32B32G32R32F; break;
			case RTF_A8: textureFormat = D3DFMT_A8; break;
			default: textureFormat = D3DFMT_UNKNOWN; break;
			}

			if (format == RTF_Default)
				return textureFormat;

			D3DRESOURCETYPE resType = D3DRTYPE_TEXTURE;

			switch (m_Type)
			{
			case RTT_1D: resType = D3DRTYPE_TEXTURE;
				break;
			case RTT_2D: resType = D3DRTYPE_TEXTURE;
				break;
			case RTT_3D: resType = D3DRTYPE_VOLUMETEXTURE;
				break;
			case RTT_Cube: resType = D3DRTYPE_CUBETEXTURE;
				break;
			}

			// Check for device support
			D3DFORMAT adapterFormat = D3DFMT_X8R8G8B8;

			C_DirectX9RenderDevice* device = static_cast<C_DirectX9RenderDevice*>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice());

			if(FAILED (device->GetDirectX9Object()->CheckDeviceFormat (D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, adapterFormat, usage, resType, textureFormat)))
			{
				REDEEMER_LOG << LOG_ERROR << L"RENDER: Texture format is not supported by this adapter!" << LOG_ENDMESSAGE;

				textureFormat = D3DFMT_A8R8G8B8;
			}

			return textureFormat;		   
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_DirectX9Texture::Lock(unsigned int level) const
		{
			// Render targets are not lockable!
			if (m_Usage == RTU_RenderTarget)
			{
				REDEEMER_LOG << LOG_ERROR << L"RENDER: Locking texture failed. Render target cannot be locked!" << LOG_ENDMESSAGE;

				return false;
			}

			if (m_Type == RTT_1D || m_Type == RTT_2D)
			{
				if (FAILED (m_2DTexture->LockRect(level, m_LockedStruct.rect, NULL, D3DLOCK_DISCARD)))
				{
					REDEEMER_LOG << LOG_ERROR << L"RENDER: Locking 2D texture failed!" << LOG_ENDMESSAGE;

					return false;
				}
			}
			else if(m_Type == RTT_3D)
			{
				if(FAILED (m_3DTexture->LockBox (level, m_LockedStruct.box, NULL, D3DLOCK_DISCARD)))
				{
					REDEEMER_LOG << LOG_ERROR << L"RENDER: Locking 3D texture failed!" << LOG_ENDMESSAGE;

					return false;
				}
			}
			else if(m_Type == RTT_Cube)
			{
				if(FAILED (m_CubeTexture->LockRect(static_cast<D3DCUBEMAP_FACES>(0), level, m_LockedStruct.rect, NULL, D3DLOCK_DISCARD)))
				{
					REDEEMER_LOG << LOG_ERROR << L"RENDER: Locking cube texture failed!" << LOG_ENDMESSAGE;

					return false;
				}
			}

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9Texture::CreateRenderTarget ()
		{
//			C_DirectX9RenderDevice* device = static_cast<C_DirectX9RenderDevice*>(C_RedeemerEngine::GetSingleton().GetRenderManager().GetRenderDevice());

//			device->GetDirectX9Device()->CreateDepthStencilSurface (m_Width, m_Height, D3DFMT_D16, D3DMULTISAMPLE_NONE,	0, TRUE, &m_RenderTarget, NULL);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9Texture::BindNormal ()
		{
			C_DirectX9RenderDevice* device = static_cast<C_DirectX9RenderDevice*>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice());
				
			switch (m_Type)
			{
			case RTT_1D: 
				device->GetDirectX9Device()->SetTexture(m_TextureStage, m_2DTexture);
				break;

			case RTT_2D: 
				 device->GetDirectX9Device()->SetTexture(m_TextureStage, m_2DTexture);
				break;

			case RTT_3D:
				device->GetDirectX9Device()->SetTexture(m_TextureStage, m_3DTexture);
				break;

			case RTT_Cube: 
				device->GetDirectX9Device()->SetTexture(m_TextureStage, m_CubeTexture);
				break;
			}

			if (m_Usage == RTU_RenderTarget)
				m_BoundAsTexture = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9Texture::BindRenderTarget ()
		{
			C_DirectX9RenderDevice* device = static_cast<C_DirectX9RenderDevice*>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice());

			if (m_TextureStage == 0)
				device->GetDirectX9Device ()->GetRenderTarget (m_TextureStage, &m_BackBuffer);

			m_2DTexture->GetSurfaceLevel(0, &m_RenderTarget);

			// Check if the render target index is valid for this device
			unsigned int nTarget = m_TextureStage;
			unsigned int nMaxRTs = 0;

			C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->TestRendererCapabilities(RTC_MaxRTS, &nMaxRTs);

			nTarget = CORE::C_MathUtils::Clamp<unsigned int> (nTarget, 0, nMaxRTs);

			device->GetDirectX9Device()->SetRenderTarget(nTarget, m_RenderTarget);		
			device->ClearBuffers (CF_BackBuffer);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9Texture::UnbindNormal ()
		{
			C_DirectX9RenderDevice* device = static_cast<C_DirectX9RenderDevice*>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice());

			device->GetDirectX9Device()->SetTexture (m_TextureStage, NULL);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9Texture::UnbindRenderTarget ()
		{
			if (m_TextureStage == 0 && !m_BackBuffer)
				return;

			C_DirectX9RenderDevice* device = static_cast<C_DirectX9RenderDevice*>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice());

			// Check if render target index is valid for this device
			unsigned int nTarget = m_TextureStage;
			unsigned int nMaxRTs = 0;

			C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->TestRendererCapabilities(RTC_MaxRTS, &nMaxRTs);
			nTarget = CORE::C_MathUtils::Clamp<unsigned int> (nTarget, 0, nMaxRTs);

			device->GetDirectX9Device()->SetRenderTarget (nTarget, m_BackBuffer);
			
			REDEEMER_SAFE_RELEASE (m_RenderTarget);
			REDEEMER_SAFE_RELEASE (m_BackBuffer);
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER

