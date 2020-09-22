/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_Texture.h
**		Generic Class for handling textures
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_RENDER_TEXTURE_H_
#define _R_RENDER_TEXTURE_H_

#include <string>

namespace REDEEMER
{
	namespace RENDER
	{
		/*!	Format of the texture
		*/
		enum ERendererTextureFormat
		{
			RTF_Default,					// Default format (taken from file or unknown)
			RTF_A8R8G8B8,					// Default - 32 bit. 8 bits per channel
			RTF_A2R10G10B10,				// 32 bit, 2 bit alpha and 10 bit RGB channels
			RTF_R8G8B8,						// 32 bit without alpha - alpha channel inaccessible
			RTF_A1R5G5B5,					// 16 bit - 1 per alpha and 5 per other channels
			RTF_R5G5B5,						// 16 bit - 5 per color channels. No alpha
			RTF_R5G6B5,						// 16 bit - 5 bits per red and blue and 6 per green
			RTF_G16R16F,					// 32 bit floating point. Only red and green channels
			RTF_A16B16G16R16F,				// 64 bit, 4 channel floating point texture
			RTF_R16F,						// 16 bit, 1 channel floating point texture
			RTF_G32R32F,					// 64 bit, 2 channel floating point texture
			RTF_R32F,						// 32 bit, 1 channel floating point texture
			RTF_A32B32G32R32F,				// 128 bit, 4 channel floating point texture
			RTF_A8							// 8 bit, 1-channel texture
		};

		/*!	Type of the texture.
		*/
		enum ERendererTextureType
		{
			RTT_Uunknown,
			RTT_1D,							///<	1D Texture
			RTT_2D,							///<	2D Texture - you will use it in most cases
			RTT_3D,							///<	3D Texture - memory usage expensive
			RTT_Cube						///<	Cube texture
		};

		/*!	Texture usage.
		*/
		enum ERendererTextureUsage
		{
			RTU_Unknown,
			RTU_Normal,						///<	Regular texture
			RTU_RenderTarget,				///<	Texture will be used as a render target
			RTU_Dynamic						///<	Texture is dynamic, you can write to it
		};

		/*!	Generic class used for handling image textures
		*/
		class C_Texture
		{
		public:
			/*!	Constructor
			*/
			C_Texture (std::wstring name = L"", ERendererTextureFormat format = RTF_A8R8G8B8, ERendererTextureType type = RTT_2D, 
				ERendererTextureUsage usage = RTU_Normal, unsigned int width = 512, unsigned int height = 512, 
				unsigned int depth = 1, float ratioWidth = 1.0f, float ratioHeight = 1.0f, unsigned int	textureStage = 0);

			/*!	Destructor
			*/
			virtual ~C_Texture ();

			/*!	Sets texture data
			*/
			virtual void SetData (void* data, unsigned int level = 0) = 0;

			/*!	Gets texture data
			*/
			virtual void* GetData (unsigned int level = 0) = 0;

			/*!	Gets width of the image
			*/
			unsigned int GetWidth ();

			/*!	Gets height of the image
			*/
			unsigned int GetHeight ();

			/*!	Get depth of the image
			*/
			unsigned int GetDepth ();

			/*!	Returns texture's name
			*/
			const std::wstring& GetName ();

			/*!	Sets ratio for width and height.
			*/
			void SetRatio (float x, float y);

			/*!	Returns texture's usage
			*/
			ERendererTextureUsage GetUsage ();

			/*!	Use texture in current rendering pass
			*/
			virtual bool BindForRendering (unsigned int stage = 0, bool forceSet = true) = 0;

			/*!	Don not use texture in current rendering pass
			*/
			virtual void UnbindFromRendering () = 0;
													
			/*!	Returns texture bit depth.
			*/
			virtual unsigned int GetBitDepth (unsigned int level = 0) const = 0;

		protected:
			ERendererTextureFormat		m_Format;			///<	Texture format
			ERendererTextureType		m_Type;				///<	Texture type
			ERendererTextureUsage		m_Usage;			///<	Texture usage
			unsigned int				m_Width;			///<	Image width
			unsigned int				m_Height;			///<	Image height
			unsigned int				m_Depth;			///<	Image depth
			std::wstring				m_Name;				///<	Texture name
			unsigned int				m_TextureStage;		///<	Stage, this texture is assigned to
			float						m_RatioWidth;		///<	Width scaling factor
			float						m_RatioHeight;		///<	Height scaling factor
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_TEXTURE_H_
