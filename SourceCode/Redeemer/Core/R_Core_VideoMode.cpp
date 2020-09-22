/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_VideoMode.cpp
**		Definition of various video modes
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Core_VideoMode.h"

#include <Windows.h>
#include <algorithm>
#include <functional>

namespace REDEEMER
{
	namespace CORE
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_VideoMode::C_VideoMode () :
			m_Width (0),
			m_Height (0),
			m_BitsPerPixel (0)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_VideoMode::C_VideoMode (unsigned int width, unsigned int height, unsigned int bpp) :
			m_Width (width),
			m_Height (height),
			m_BitsPerPixel (bpp)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_VideoMode::~C_VideoMode ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_VideoMode C_VideoMode::GetDesktopMode ()
		{
			DEVMODE win32Mode;
			win32Mode.dmSize = sizeof(win32Mode);
			EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &win32Mode);

			return C_VideoMode (win32Mode.dmPelsWidth, win32Mode.dmPelsHeight, win32Mode.dmBitsPerPel);
		}

		//------------------------------------------------------------------------------------------------------------------------

		const std::vector<C_VideoMode>& C_VideoMode::GetFullscreenModes ()
		{
			static std::vector<C_VideoMode> modes;

			// Populate the array on first call
			if (modes.empty())
			{
				// Enumerate all available video modes for the primary display adapter
				DEVMODE win32Mode;
				win32Mode.dmSize = sizeof(win32Mode);

				for (int count = 0; EnumDisplaySettings (NULL, count, &win32Mode); ++count)
				{
					// Convert to sf::VideoMode
					C_VideoMode mode (win32Mode.dmPelsWidth, win32Mode.dmPelsHeight, win32Mode.dmBitsPerPel);

					// Add it only if it is not already in the array
					if (std::find(modes.begin(), modes.end(), mode) == modes.end())
						modes.push_back(mode);
				}							  

				std::sort(modes.begin (), modes.end (), std::greater<C_VideoMode>());
			}

			return modes;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_VideoMode::IsValid () const
		{
			const std::vector<C_VideoMode>& modes = GetFullscreenModes ();

			return std::find (modes.begin (), modes.end (), *this) != modes.end();
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool operator ==(const C_VideoMode& left, const C_VideoMode& right)
		{
			return (left.m_Width == right.m_Width) && (left.m_Height == right.m_Height) && (left.m_BitsPerPixel == right.m_BitsPerPixel);
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool operator !=(const C_VideoMode& left, const C_VideoMode& right)
		{
			return !(left == right);
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool operator <(const C_VideoMode& left, const C_VideoMode& right)
		{
			if (left.m_BitsPerPixel == right.m_BitsPerPixel)
			{
				if (left.m_Width == right.m_Width)
				{
					return left.m_Height < right.m_Height;
				}
				else
				{
					return left.m_Width < right.m_Width;
				}
			}
			else
			{
				return left.m_BitsPerPixel < right.m_BitsPerPixel;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool operator >(const C_VideoMode& left, const C_VideoMode& right)
		{
			return right < left;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool operator <=(const C_VideoMode& left, const C_VideoMode& right)
		{
			return !(right < left);
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool operator >=(const C_VideoMode& left, const C_VideoMode& right)
		{
			return !(left < right);
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace CORE
}	//	namespace REDEEMER

