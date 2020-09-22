/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_GUI_GUIFont.h
**		GUI Font uses FreeType library for reading font files
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_GUI_GUIFONT_H_
#define _R_GUI_GUIFONT_H_

#include "..\\Render\\R_Render_Color.h"
#include "..\\Core\\\Math\\R_Core_Rectangle.h"

#include <string>
#include <vector>
#include <map>

//	FreeType2
struct FT_FaceRec_;
	
namespace REDEEMER
{
	namespace RENDER
	{
		class C_VertexBuffer;
		class C_Texture;
	}

	namespace FILESYSTEM
	{
		class IDataFile;		
	}

	namespace GUI
	{
		class C_GUIFontManager;
		class C_GUIControl;

		/*!	Single line of text
		*/
		struct S_TextData
		{
			std::wstring	Text;
			RENDER::C_Color	Color;
			unsigned int	RealLength;
			unsigned int	ScreenLength;
			unsigned int	XPosition;
			unsigned int	YPosition;			
		};

		/*!
		**	\brief GUI Font uses FreeType library for reading font files
		*/
		class C_GUIFont
		{
			friend class C_GUIFontManager;

		public:
			/*!	Constructor
			**	It is VERY important to not overrun maximum number of declared characters
			*/
			C_GUIFont (unsigned int maxCharacters);

			/*!	Destructor
			*/
			virtual ~C_GUIFont ();

			/*!	Loads font from file
			*/
			bool LoadFromFile (FILESYSTEM::IDataFile* fontFile);

			/*!	Sets point font size.
			**	The character widths and heights are specified in 1/64th of points. A point is a physical distance, equaling 1/72th of an inch.
			**	Normally, it is not equivalent to a pixel. A value of 0 for the character width means ‘same as character height’, a value of 0 for 
			**	the character height means ‘same as character width’. Otherwise, it is possible to specify different character widths and heights.
			*/
			void SetPointSize (unsigned int characterWidth, unsigned int characterHeight);

			/*!	Sets pixel font size.
			**	If you want to specify the pixel sizes for font chars yourself, use this method. A value of 0 for the character width means 
			**	‘same as character height’, a value of 0 for the character height means ‘same as character width’.
			*/
			void SetPixelSize (unsigned int characterWidth, unsigned int characterHeight);

			/*!	Sets text used for rendering
			*/
			void AddText (const std::wstring& text, unsigned int xPosition = 0, unsigned int yPosition = 0, RENDER::C_Color color = RENDER::C_Color::COLOR_WHITE, C_GUIControl* control = NULL);

			/*!	Returns all texts
			*/
			const std::vector<S_TextData> GetTexts (C_GUIControl* control = NULL);

			/*!	Clears all texts
			*/
			void ClearText (C_GUIControl* control = NULL);

			/*!	Renders text with font
			*/
			void Render (C_GUIControl* control = NULL);

			/*!	Returns font name
			*/
			std::wstring GetFontName () const;

			/*!	Sets an alternative background color, used to get effect of transparency, default is C_Color::COLOR_BLACK
			*/
			void SetBackgroundColor (RENDER::C_Color color);

			/*!	Returns font height
			*/
			unsigned char GetHeight () const; 

			/*!	Returns characters widths
			*/
			const std::vector<unsigned char>& GetWidths () const;

			static const unsigned int			SPACE = 32;
			static const unsigned int			NUM_CHARS = 96;

		private:
			/*!	Creates texture surface with font data
			*/
			void CreateTexture ();

			/*!	Creates vertex data describing texts
			*/
			void CreateVertexData ();

		private:
			struct S_CharacterPosition
			{
				float						XPosition;
				float						YPosition;
				CORE::C_Rectangle<float>	TexCoordinates;
			};

			FT_FaceRec_*										m_FontFace;				///<	FreeType2 font face
			std::map<C_GUIControl*, std::vector<S_TextData>	>	m_Texts;				///<	Texts used for rendering
			unsigned int										m_MaxCharacters;		///<	Number of allowed characters
			RENDER::C_VertexBuffer*								m_VertexBuffer;			///<	Holds primitive used as a surface to render
			RENDER::C_Texture*									m_Texture;				///<	Texture to hold font data
			void*												m_FontBuffer;			///<	Internal buffer to hold font data
			std::wstring										m_FontName;
			std::vector<unsigned char>							m_Widths;				///<	Characters widths
			unsigned char										m_Height;
			bool												m_IsRenderable;		

			std::vector<S_CharacterPosition>					m_CharactersPositions;

			RENDER::C_Color										m_BackgroundColor;	
								   
			static unsigned long								s_TextureID;

			C_GUIControl*										m_InternalControl;		///<	Control used for storing texts internally belonging to the font
		};

	}	//	namespace GUI
}	//	namespace REDEEMER

#endif	//	_R_GUI_GUIFONT_H_
