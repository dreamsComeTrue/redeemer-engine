/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_Texture.cpp
**		Generic Class for handling textures
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_Texture.h"

namespace REDEEMER
{
	namespace RENDER
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_Texture::C_Texture (std::wstring name, ERendererTextureFormat format, ERendererTextureType type, ERendererTextureUsage usage, 
			unsigned int width, unsigned int height, unsigned int depth, float ratioWidth, float ratioHeight, unsigned int	textureStage) :
			m_Name (name),
			m_Format (format),
			m_Type (type),
			m_Usage (usage),
			m_Width (width),
			m_Height (height),
			m_Depth (depth),
			m_RatioWidth (ratioWidth),
			m_RatioHeight (ratioHeight),
			m_TextureStage (textureStage)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Texture::~C_Texture ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned int C_Texture::GetWidth ()
		{
			return m_Width;
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned int C_Texture::GetHeight ()
		{
			return m_Height;
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned int C_Texture::GetDepth ()
		{
			return m_Depth;
		}

		//------------------------------------------------------------------------------------------------------------------------

		const std::wstring& C_Texture::GetName ()
		{
			return m_Name;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Texture::SetRatio (float x, float y)
		{
			m_RatioWidth = x;
			m_RatioHeight = y;
		}

		//------------------------------------------------------------------------------------------------------------------------

		ERendererTextureUsage C_Texture::GetUsage ()
		{
			return m_Usage;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER
