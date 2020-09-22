/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_DirectX9Texture.h
**		DirectX 9 texture implementation class
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_RENDER_DIRECTX9TEXTURE_H_
#define _R_RENDER_DIRECTX9TEXTURE_H_

#include "..\\R_Render_Texture.h"

#include <d3d9.h>

namespace REDEEMER
{
	namespace FILESYSTEM
	{
		class IDataFile;
	}

	namespace RENDER
	{
		/*!	DirectX 9 texture implementation class
		*/
		class C_DirectX9Texture : public C_Texture
		{
		public:
			/*!	Constructor
			*/
			C_DirectX9Texture (std::wstring name = L"", ERendererTextureFormat format = RTF_A8R8G8B8, ERendererTextureType type = RTT_2D, 
				ERendererTextureUsage usage = RTU_Normal, unsigned int width = 512, unsigned int height = 512, 
				unsigned int depth = 1, float ratioWidth = 1.0f, float ratioHeight = 1.0f, unsigned int	textureStage = 0);

			/*!	Constructs texture from file
			*/
			C_DirectX9Texture (FILESYSTEM::IDataFile* file, std::wstring name = L"", ERendererTextureFormat format = RTF_A8R8G8B8, 
				ERendererTextureType type = RTT_2D, ERendererTextureUsage usage = RTU_Normal);

			/*!	Destructor
			*/
			virtual ~C_DirectX9Texture ();

			/*!	Sets texture data
			*/
			virtual void SetData (void* data, unsigned int level = 0);

			/*!	Gets texture data
			*/
			virtual void* GetData (unsigned int level = 0);

			/*!	Use texture in current rendering pass
			*/
			virtual bool BindForRendering (unsigned int stage = 0, bool forceSet = true);

			/*!	Don not use texture in current rendering pass
			*/
			virtual void UnbindFromRendering ();
													
			/*!	Returns texture bit depth.
			*/
			virtual unsigned int GetBitDepth (unsigned int level = 0) const;

		private:
			/*!	Locks texture data
			*/
			bool Lock (unsigned int level = 0) const;

			/*!	Returns DirectX texture format based on engine's format
			*/
			D3DFORMAT GetFormat (ERendererTextureFormat& format, DWORD& usage);

			/*!	Makes texture render target
			*/
			void CreateRenderTarget ();

			/*!	Binds normal texture
			*/
			void BindNormal ();

			/*!	Binds render target
			*/
			void BindRenderTarget ();

			/*!	Unbinds normal texture
			*/
			void UnbindNormal ();

			/*!	Unbinds render target
			*/
			void UnbindRenderTarget ();

		private:
			/*!	DirectX texture-specified fields
			*/
			union
			{
				IDirect3DTexture9*			m_2DTexture;
				IDirect3DVolumeTexture9*	m_3DTexture;
				IDirect3DCubeTexture9*		m_CubeTexture;
			};

			union LOCKED_STRUCT
			{
				D3DLOCKED_RECT* rect;
				D3DLOCKED_BOX*	box;
			}								m_LockedStruct;
						
			IDirect3DSurface9*				m_RenderTarget;			///<	Current render target
			IDirect3DSurface9*				m_BackBuffer;			///<	Previous render target

			bool							m_ForceSet;
			bool							m_BoundAsTexture;
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_DIRECTX9TEXTURE_H_
