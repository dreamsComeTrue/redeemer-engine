/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_VideoMode.h
**		Definition of various video modes
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_CORE_VIDEOMODE_H_
#define _R_CORE_VIDEOMODE_H_

#include <vector>

namespace REDEEMER
{
	namespace CORE
	{
		/*!
		**	\brief A C_VideoMode class represents various video modes
		**	Video mode is described by width, height and BPP (bits per pixel).
		**	Thanks to static methods of this class, one can create appropriate desktop and fullscreen windows.
		*/
		class C_VideoMode
		{
		public:
			///	Constructor
			C_VideoMode ();

			/*!	Constructor
			**	Creates video mode with given attributes.
			**	\param width Width in pixels
			**	\param height Height in pixels
			**	\param bpp Bits Per Pixel
			*/
			C_VideoMode (unsigned int width, unsigned int height, unsigned int bpp = 32);

			///	Destructor
			virtual ~C_VideoMode ();

			/*!	Get the current desktop video mode
			**	\return Current desktop video mode
			*/
			static C_VideoMode GetDesktopMode ();

			/*!	Retrieve all the video modes supported in fullscreen mode
			**	\return Array containing all the supported fullscreen modes
			*/
			static const std::vector<C_VideoMode>& GetFullscreenModes ();

			/*!	Tell whether or not the video mode is valid
			**	Used only when considered fullscreen modes
			**	\return True if the video mode is valid for fullscreen mode
			*/
			bool IsValid () const;	 

		public:
			unsigned int m_Width;        ///< Mode width, in pixels
			unsigned int m_Height;       ///< Mode height, in pixels
			unsigned int m_BitsPerPixel; ///< Mode pixel depth, in bits per pixels
		};

		/*!	\brief Overload of == operator to compare two video modes
		** \param left  Left operand (a video mode)
		** \param right Right operand (a video mode)
		** \return True if modes are equal
		*/
		bool operator ==(const C_VideoMode& left, const C_VideoMode& right);

		/*!	\brief Overload of != operator to compare two video modes
		** \param left  Left operand (a video mode)
		** \param right Right operand (a video mode)
		** \return True if modes are different
		*/
		bool operator !=(const C_VideoMode& left, const C_VideoMode& right);

		/*!	\brief Overload of < operator to compare two video modes
		** \param left  Left operand (a video mode)
		** \param right Right operand (a video mode)
		** \return True if \a left is lesser than \a right
		*/
		bool operator <(const C_VideoMode& left, const C_VideoMode& right);

		/*!	\brief Overload of > operator to compare two video modes
		** \param left  Left operand (a video mode)
		** \param right Right operand (a video mode)
		** \return True if \a left is greater than \a right
		*/
		bool operator >(const C_VideoMode& left, const C_VideoMode& right);

		/*!	\brief Overload of <= operator to compare two video modes
		** \param left  Left operand (a video mode)
		** \param right Right operand (a video mode)
		** \return True if \a left is lesser or equal than \a right
		*/
		bool operator <=(const C_VideoMode& left, const C_VideoMode& right);

		/*!	\brief Overload of >= operator to compare two video modes
		** \param left  Left operand (a video mode)
		** \param right Right operand (a video mode)
		** \return True if \a left is greater or equal than \a right
		*/
		bool operator >=(const C_VideoMode& left, const C_VideoMode& right);

	}	//	namespace CORE
}	//	namespace REDEEMER

#endif	//	_R_CORE_VIDEOMODE_H_
