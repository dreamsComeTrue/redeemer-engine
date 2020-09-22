						 /*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_Color.cpp
**		Colo(u)r class implementation
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_Color.h"
#include "..\\Core\\Math\\R_Core_MathUtils.h"

namespace REDEEMER
{
	namespace RENDER
	{
		const C_Color C_Color::COLOR_BLACK (0, 0, 0, 255);
		const C_Color C_Color::COLOR_GRAY (128, 128, 128, 255);
		const C_Color C_Color::COLOR_SILVER (192, 192, 192, 255);
		const C_Color C_Color::COLOR_RED (255, 0, 0, 255);
		const C_Color C_Color::COLOR_ORANGE (255, 164, 0, 255);
		const C_Color C_Color::COLOR_GREEN (0, 255, 0, 255);
		const C_Color C_Color::COLOR_BLUE (0, 0, 255, 255);
		const C_Color C_Color::COLOR_CYAN (0, 255, 255, 255);
		const C_Color C_Color::COLOR_MAGENTA (255, 0, 255, 255);
		const C_Color C_Color::COLOR_YELLOW (255, 255, 0, 255);
		const C_Color C_Color::COLOR_WHITE (255, 255, 255, 255);
		const C_Color C_Color::COLOR_PURPLE (128, 0, 128, 255);
		const C_Color C_Color::COLOR_MAROON (128, 0, 0, 255);
		const C_Color C_Color::COLOR_OLIVE (128, 128, 0, 255);
		const C_Color C_Color::COLOR_TRANSPARENT (0, 0, 0, 0);

		//------------------------------------------------------------------------------------------------------------------------
		C_Color::C_Color (unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) :
			m_Red (red),
			m_Green (green),
			m_Blue (blue),
			m_Alpha (alpha)
		{
			UpdateColor();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Color::C_Color (const DWORD color) :
			m_Color (color)
		{
			// Convert value to 0-255 values
			m_Red = static_cast<unsigned char>((color >> 16) & 0xff);
			m_Green = static_cast<unsigned char>((color >> 8)  & 0xff);
			m_Blue = static_cast<unsigned char>((color) & 0xff);
			m_Alpha = static_cast<unsigned char>((color >> 24) & 0xff);

			UpdateColor();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Color::~C_Color ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned char C_Color::GetRed () const
		{
			return m_Red;
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned char C_Color::GetGreen () const
		{
			return m_Green;
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned char C_Color::GetBlue () const
		{
			return m_Blue;
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned char C_Color::GetAlpha () const
		{
			return m_Alpha;
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_Color::GetRedFloat () const
		{
			return (float)m_Red / 255.0f;
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_Color::GetGreenFloat () const
		{
			return (float)m_Green / 255.0f;
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_Color::GetBlueFloat () const
		{
			return (float)m_Blue / 255.0f;
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_Color::GetAlphaFloat () const
		{
			return (float)m_Alpha / 255.0f;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Color::SetRed (unsigned char red)
		{
			m_Red = red;

			UpdateColor();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Color::SetGreen (unsigned char green)
		{
			m_Green = green;

			UpdateColor();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Color::SetBlue (unsigned char blue)
		{
			m_Blue = blue;

			UpdateColor();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Color::SetAlpha (unsigned char alpha)
		{
			m_Alpha = alpha;

			UpdateColor();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Color::SetColorRGBA (const DWORD color)
		{
			m_Color = color;

			// Convert value to 0-255 values
			m_Red   = static_cast<unsigned char>((color >> 24) & 0xff);
			m_Green = static_cast<unsigned char>((color >> 16) & 0xff);
			m_Blue  = static_cast<unsigned char>((color >> 8)  & 0xff);
			m_Alpha = static_cast<unsigned char>((color) & 0xff);
		}

		//------------------------------------------------------------------------------------------------------------------------

		DWORD C_Color::GetColorARGB () const
		{
			return m_Color;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Color::SetColorRGBA (unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
		{
			m_Red  = red;
			m_Green= green;
			m_Blue = blue;
			m_Alpha= alpha;
						
			UpdateColor ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Color::SetColorRGBAFloat (float red, float green, float blue, float alpha)
		{
			m_Red  = CORE::C_MathUtils::FloatToInt (red * 255.0f);
			m_Green= CORE::C_MathUtils::FloatToInt (green * 255.0f);
			m_Blue = CORE::C_MathUtils::FloatToInt (blue * 255.0f);
			m_Alpha= CORE::C_MathUtils::FloatToInt (alpha * 255.0f);

			UpdateColor();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Color::UpdateColor ()
		{
			m_Color = (((m_Alpha) & 0xff) << 24)| (((m_Red) & 0xff) << 16)  | (((m_Green) & 0xff) << 8) | ((m_Blue) & 0xff);
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER

