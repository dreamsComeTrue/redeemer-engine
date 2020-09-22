/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_Color.h
**		Colo(u)r class implementation
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_RENDER_COLOR_H_
#define _R_RENDER_COLOR_H_

#include <windows.h>

namespace REDEEMER
{
	namespace RENDER
	{
		/*!
		**	\brief Colo(u)r class implementation
		*/
		class C_Color
		{
		public:
			/*!	Constructor
			*/
			C_Color (unsigned char red = 255, unsigned char green = 255, unsigned char blue = 255, unsigned char alpha = 255);

			/*!	Constructor, takes DWORD value
			*/
			C_Color (const DWORD color);

			/*!	Destructor
			*/
			~C_Color ();

			/*!	Returns Red component of the color
			*/
			unsigned char GetRed () const;

			/*!	Returns Green component of the color
			*/
			unsigned char GetGreen () const;

			/*!	Returns Blue component of the color
			*/
			unsigned char GetBlue () const;

			/*!	Returns Alpha component of the color
			*/
			unsigned char GetAlpha () const;

			/*!	Returns Red component of the color (float version)
			*/
			float GetRedFloat () const;

			/*!	Returns Green component of the color (float version)
			*/
			float GetGreenFloat () const;

			/*!	Returns Blue component of the color (float version)
			*/
			float GetBlueFloat () const;

			/*!	Returns Alpha component of the color (float version)
			*/
			float GetAlphaFloat () const;

			/*!	Sets Red component of the color
			*/
			void SetRed (unsigned char red);

			/*!	Sets Green component of the color
			*/
			void SetGreen (unsigned char green);

			/*!	Sets Blue component of the color
			*/
			void SetBlue (unsigned char blue);

			/*!	Sets Alpha component of the color
			*/
			void SetAlpha (unsigned char alpha);

			/*!	Sets color, value should contains components in order: red, green, blue, alpha
			*/
			void SetColorRGBA (const DWORD color);

			/*!	Returns color in ARGB format
			*/
			DWORD GetColorARGB () const;

			/*!	Sets color, value should contains components in order: red, green, blue, alpha
			*/
			void SetColorRGBA (unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);

			/*!	Sets color using float values (each one is from range 0.0f..1.0f)
			*/
			void SetColorRGBAFloat (float red, float green, float blue, float alpha = 1.0f);

			//	Predefined colors

			static const C_Color COLOR_BLACK;			///< Black
			static const C_Color COLOR_GRAY;			///< Gray
			static const C_Color COLOR_SILVER;			///< Silver
			static const C_Color COLOR_RED;				///< Red
			static const C_Color COLOR_ORANGE;			///< Orange
			static const C_Color COLOR_GREEN;			///< Green
			static const C_Color COLOR_BLUE;			///< Blue
			static const C_Color COLOR_CYAN;			///< Cyan
			static const C_Color COLOR_MAGENTA;			///< Magenta
			static const C_Color COLOR_YELLOW;			///< Yellow
			static const C_Color COLOR_WHITE;			///< White
			static const C_Color COLOR_PURPLE;			///< Purple
			static const C_Color COLOR_MAROON;			///< Maroon
			static const C_Color COLOR_OLIVE;			///< Olive
			static const C_Color COLOR_TRANSPARENT;		///< Transparent

		private:
			/*!	Internal 
			*/
			void UpdateColor ();	 

		private:
			unsigned char	m_Red;						///<	Red component
			unsigned char	m_Green;					///<	Green component
			unsigned char	m_Blue;						///<	Blue component
			unsigned char	m_Alpha;					///<	Alpha component
			DWORD			m_Color;					///<	Color value
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_COLOR_H_
