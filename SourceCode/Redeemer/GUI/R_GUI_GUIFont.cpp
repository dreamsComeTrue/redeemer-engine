/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_GUI_GUIFont.cpp
**		GUI Font uses FreeType library for reading font files
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_GUI_GUIFont.h"
#include "R_GUI_GUIManager.h"
#include "R_GUI_GUIFontManager.h"
#include "R_GUI_GUISystem.h"
#include "R_GUI_GUIControl.h"
#include "..\\Core\\R_Core_Convert.h"
#include "..\\Core\\R_Core_Utility.h"
#include "..\\Core\\Math\\R_Core_Vector4D.h"
#include "..\\Render\\R_Render_RenderDevice.h"
#include "..\\Render\\R_Render_VertexBuffer.h"
#include "..\\Render\\R_Render_IndexBuffer.h"
#include "..\\Render\\R_Render_VertexBufferManager.h"
#include "..\\Render\\R_Render_Texture.h"
#include "..\\Render\\R_Render_TextureManager.h"
#include "..\\Render\\R_Render_TextureSampler.h"
#include "..\\Render\\R_Render_Material.h"
#include "..\\Render\\R_Render_RenderPass.h"
#include "..\\Render\\R_Render_RenderTechnique.h"
#include "..\\Render\\R_Render_ShaderEntity.h"
#include "..\\Render\\DirectX9\\R_Render_DirectX9RenderDevice.h"
#include "..\\RedeemerEngine.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace REDEEMER
{
	namespace GUI
	{

#define GUI_FONT_POINT_SIZE 64

		//------------------------------------------------------------------------------------------------------------------------
		unsigned long C_GUIFont::s_TextureID = 0;

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_GUIFont::C_GUIFont (unsigned int maxCharacters) :
			m_FontFace (NULL),		   			
			m_MaxCharacters (maxCharacters),
			m_VertexBuffer (NULL),
			m_FontBuffer (NULL),
			m_Texture (NULL),
			m_Widths (NUM_CHARS),
			m_BackgroundColor (RENDER::C_Color::COLOR_BLACK),
			m_FontName (L""),
			m_IsRenderable (true),
			m_InternalControl(NULL)
		{
			m_VertexBuffer = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetVertexBufferManager()->CreateVertexBuffer(maxCharacters * 4, RENDER::FVF_VERTEX3TC, true);
			m_VertexBuffer->AttachIndexBuffer (maxCharacters * 6, true);
			m_VertexBuffer->SetPrimitivesCount (maxCharacters * 2);
			m_VertexBuffer->SetPrimitiveType (RENDER::RPT_TriangleList);

			m_InternalControl = new C_GUIControl (1, GUICT_Control, NULL, CORE::C_Rectangle<int>(0,0,0,0));
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUIFont::~C_GUIFont ()
		{
			m_Texts.clear();

			if (m_FontFace)
				FT_Done_Face ((FT_Face)m_FontFace);

			if (m_VertexBuffer)
				C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetVertexBufferManager()->RemoveVertexBuffer(m_VertexBuffer);

			REDEEMER_SAFE_DELETE (m_FontBuffer);
			REDEEMER_SAFE_DELETE (m_InternalControl);

			if (m_Texture)
				C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetTextureManager()->RemoveTexture(m_Texture);
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUIFont::LoadFromFile (FILESYSTEM::IDataFile* fontFile)
		{
			FT_Library freeTypeLibrary = (FT_Library)C_RedeemerEngine::GetSingleton().GetGUIManager()->GetGUISystem()->GetFreeTypeLibrary();

			unsigned int size = fontFile->GetSize();
			m_FontBuffer = new char[size];
			fontFile->Read((char*)m_FontBuffer, size);
																   
			//	Try to load TrueType font
			int error = FT_New_Memory_Face (freeTypeLibrary, (const FT_Byte*)m_FontBuffer, size, 0, (FT_Face*)&m_FontFace);

			if (error == FT_Err_Unknown_File_Format)
			{
				REDEEMER_LOG << LOG_ERROR << "C_GUIFont: Font format unsupported!" << LOG_ENDMESSAGE;

				return false;
			}
			else 
				if (error)											
				{
					REDEEMER_LOG << LOG_ERROR << "C_GUIFont: Font file could not be opened or read, it may be broken!" << LOG_ENDMESSAGE;

					return false;
				}

			REDEEMER_LOG << LOG_INFO << "GUI: Loaded font: '" << m_FontFace->family_name << "' from '" << fontFile->GetFileName() << "' file." << LOG_ENDMESSAGE;

			CORE::C_Convert::ConvertCharsToUnicode(&m_FontName, m_FontFace->family_name);

			SetPointSize(15, 15);
										
			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIFont::SetPointSize (unsigned int characterWidth, unsigned int characterHeight)
		{
			int error = FT_Set_Char_Size(
											(FT_Face)m_FontFace,					/* handle to face object           */
											characterWidth * GUI_FONT_POINT_SIZE,   /* char_width in 1/64th of points  */
											characterHeight * GUI_FONT_POINT_SIZE,	/* char_height in 1/64th of points */
											0,										/* horizontal device resolution    */
											0);										/* vertical device resolution      */

			CreateTexture();
			CreateVertexData();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIFont::SetPixelSize (unsigned int characterWidth, unsigned int characterHeight)
		{
			int error = FT_Set_Pixel_Sizes(
											(FT_Face)m_FontFace,	/* handle to face object */
											characterWidth,			/* pixel_width           */
											characterHeight);		/* pixel_height          */

			CreateTexture();
			CreateVertexData();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIFont::AddText (const std::wstring& text, unsigned int xPosition, unsigned int yPosition, RENDER::C_Color color, C_GUIControl* control)
		{
			S_TextData data;

			data.Text = text;
			data.XPosition = xPosition;
			data.YPosition = yPosition;
			data.ScreenLength = 0;
			data.RealLength = 0;
			data.Color = color;

			if (control == NULL)
				m_Texts[m_InternalControl].push_back(data);
			else
				m_Texts[control].push_back(data);

			CreateVertexData();
		}

		//------------------------------------------------------------------------------------------------------------------------

		const std::vector<S_TextData> C_GUIFont::GetTexts (C_GUIControl* control) 
		{
			if (control == NULL)
				return m_Texts[m_InternalControl];
			else
				return m_Texts[control];
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIFont::ClearText(C_GUIControl* control)
		{
			if (control == NULL)
				m_Texts[m_InternalControl].clear();
			else
				m_Texts[control].clear();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIFont::CreateTexture()
		{
			if (!m_FontFace)
				return;	

			if (m_Texture)
				C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetTextureManager()->RemoveTexture(m_Texture);

			m_Height = 0;

			m_Widths.clear();
			m_Widths.resize(NUM_CHARS);

			m_CharactersPositions.clear();

			const size_t MARGIN = 3;

			// Step 1: Find maxAscent/Descent to calculate imageHeight 
			size_t imageHeight = 0;
			size_t imageWidth = 256;
			int maxDescent = 0;
			int maxAscent = 0;
			size_t lineSpace = imageWidth - MARGIN;
			size_t lines = 1;
			size_t charIndex;

			for (unsigned int ch = 0; ch < NUM_CHARS; ++ch)
			{
				// Look up the character in the font file.
				charIndex = FT_Get_Char_Index (m_FontFace, ch + SPACE);

				// Render the current glyph.
				FT_Load_Glyph (m_FontFace, charIndex, FT_LOAD_RENDER);

				m_Widths[ch] = (unsigned char)(m_FontFace->glyph->metrics.horiAdvance >> 6) + MARGIN;

				// If the line is full go to the next line
				if (m_Widths[ch] > lineSpace)
				{
					lineSpace = imageWidth - MARGIN;
					++lines;
				}

				lineSpace -= m_Widths[ch];

				maxAscent = CORE::C_MathUtils::Max<int> (m_FontFace->glyph->bitmap_top, maxAscent);
				maxDescent = CORE::C_MathUtils::Max<int> (m_FontFace->glyph->bitmap.rows - m_FontFace->glyph->bitmap_top, maxDescent);
			}

			m_Height = maxAscent + maxDescent;   // calculate height for text

			// Compute how high the texture has to be.
			size_t neededHeight = (maxAscent + maxDescent + MARGIN) * lines + MARGIN;

			// Get the first power of two in which it will fit
			imageHeight = 16;

			while (imageHeight < neededHeight)
				imageHeight <<= 1;

			// Step 2: Generation of the actual texture 

			// create and zero the memory
			unsigned char* image = new unsigned char[imageHeight * imageWidth];

			std::memset(image, 0, imageHeight * imageWidth);

			// These are the position at which to draw the next glyph
			size_t x = MARGIN;
			size_t y = MARGIN + maxAscent;
			float texX1, texX2, texY1, texY2;   // used for indexing

			// Drawing loop
			for (unsigned int ch = 0; ch < NUM_CHARS; ++ch)
			{
				size_t charIndex = FT_Get_Char_Index (m_FontFace, ch + SPACE);

				// Render the glyph
				FT_Load_Glyph (m_FontFace, charIndex, FT_LOAD_DEFAULT);
				FT_Render_Glyph (m_FontFace->glyph, FT_RENDER_MODE_NORMAL);

				// See whether the character fits on the current line
				if (m_Widths[ch] > imageWidth - x)
				{
					x = MARGIN;
					y += (maxAscent + maxDescent + MARGIN);
				}

				// calculate texture coordinates of the character
				texX1 = static_cast<float>(x) / imageWidth;
				texX2 = static_cast<float>(x + m_Widths[ch]) / imageWidth;
				texY1 = static_cast<float>(y - maxAscent) / imageHeight;
				texY2 = texY1 + static_cast<float>(m_Height) / imageHeight;

				//	Add character specification to the collection
				S_CharacterPosition pos;

				pos.XPosition = m_Widths[ch];
				pos.YPosition = m_Height;
				pos.TexCoordinates = CORE::C_Rectangle<float> (texX1, texY1, texX2, texY2);

				m_CharactersPositions.push_back(pos);
	 
				// copy image generated by FreeType to the texture
				for (int row = 0; row < m_FontFace->glyph->bitmap.rows; ++row)
				{
					for (int pixel = 0; pixel < m_FontFace->glyph->bitmap.width; ++pixel)
					{
						// set pixel at position to intensity (0-255) at the position
						int coord = (x + m_FontFace->glyph->bitmap_left + pixel) + (y - m_FontFace->glyph->bitmap_top + row) * imageWidth;
						unsigned char pixelColor = m_FontFace->glyph->bitmap.buffer[pixel + row * m_FontFace->glyph->bitmap.pitch];

						image[coord] = pixelColor;
					}
				}

				x += m_Widths[ch];
			}

			// Generate the texture from the byte array
			std::string fontNameString;
			std::wstring fontName;

			fontNameString = CORE::C_Utility::ToString("FontSurface_", s_TextureID);
			fontName = std::wstring (fontNameString.begin(), fontNameString.end());

			m_Texture = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetTextureManager()->CreateTexture(fontName.c_str(), RENDER::RTF_A8, 
				RENDER::RTT_2D, RENDER::RTU_Dynamic, imageWidth, imageHeight, 1);

			m_Texture->SetData(image);

			delete[] image;     // now done with the image memory

			s_TextureID ++;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIFont::CreateVertexData ()
		{
			RENDER::S_VertexColorTexture*	vertices;
			unsigned short*					indices;

			m_VertexBuffer->Lock (m_VertexBuffer->GetVerticesCount(), (void**)&vertices);
			m_VertexBuffer->GetIndexBuffer()->Lock(m_VertexBuffer->GetIndexBuffer()->GetIndicesCount(), (void**)&indices);	

			int index1 = 0, index2 = 0;
			unsigned char characterCounter = 0;
			unsigned int screenLength = 0, maxScreenLength = 0;

			for (std::map<C_GUIControl*, std::vector<S_TextData> >::iterator it = m_Texts.begin(); it != m_Texts.end(); ++ it)
				for (unsigned int i = 0; i < m_Texts[(*it).first].size(); ++ i)
				{	
					m_Texts[(*it).first][i].RealLength = m_Texts[(*it).first][i].Text.length();

					float currentX = 0.0f;
					float currentY = 0.0f;   

					//	For each contained text
					for (unsigned int j = 0; j < m_Texts[(*it).first][i].Text.size(); j ++)
					{		 
						if (characterCounter == m_MaxCharacters)
							break;

						unsigned char ch ((unsigned char)m_Texts[(*it).first][i].Text[j] - SPACE);     // ch-SPACE = DisplayList offset

						// replace characters outside the valid range with undrawable
						if (ch > NUM_CHARS)
						{
							ch = NUM_CHARS - 1;   // last character is 'undrawable'
						}

						characterCounter ++;

						S_CharacterPosition charPosition = m_CharactersPositions[ch];

						if (m_Texts[(*it).first][i].Text[j] == L'\n')
						{
							currentY += m_Height;
							currentX = 0.0f;

							maxScreenLength = screenLength;
							screenLength = 0;

							m_Texts[(*it).first][i].RealLength --;

							continue;
						}

						float x1 = currentX;
						float x2 = currentX + charPosition.XPosition;
						float y1 = currentY;
						float y2 = currentY + charPosition.YPosition;

						int base = index1;

						vertices[index1].Position = CORE::C_Vector3D<float>(x1, y1, 0.5f);
						vertices[index1].Color = m_Texts[(*it).first][i].Color.GetColorARGB();
						vertices[index1++].TexCoord = CORE::C_Vector2D<float>(charPosition.TexCoordinates.UpperLeftCorner.X, charPosition.TexCoordinates.UpperLeftCorner.Y);

						vertices[index1].Position = CORE::C_Vector3D<float>(x2, y1, 0.5f);
						vertices[index1].Color = m_Texts[(*it).first][i].Color.GetColorARGB();
						vertices[index1++].TexCoord = CORE::C_Vector2D<float>(charPosition.TexCoordinates.LowerRightCorner.X, charPosition.TexCoordinates.UpperLeftCorner.Y);

						vertices[index1].Position = CORE::C_Vector3D<float>(x2, y2, 0.5f);
						vertices[index1].Color = m_Texts[(*it).first][i].Color.GetColorARGB();
						vertices[index1++].TexCoord = CORE::C_Vector2D<float>(charPosition.TexCoordinates.LowerRightCorner.X, charPosition.TexCoordinates.LowerRightCorner.Y);

						vertices[index1].Position = CORE::C_Vector3D<float>(x1, y2, 0.5f);
						vertices[index1].Color = m_Texts[(*it).first][i].Color.GetColorARGB();
						vertices[index1++].TexCoord = CORE::C_Vector2D<float>(charPosition.TexCoordinates.UpperLeftCorner.X, charPosition.TexCoordinates.LowerRightCorner.Y);

						indices[index2++] = base;
						indices[index2++] = base + 1;
						indices[index2++] = base + 2;
						indices[index2++] = base;
						indices[index2++] = base + 2;
						indices[index2++] = base + 3;

						//	Move the cursor
						currentX += m_Widths[ch];	
						screenLength += m_Widths[ch];
					}

					if (screenLength > maxScreenLength)
						m_Texts[(*it).first][i].ScreenLength = screenLength;
					else
						m_Texts[(*it).first][i].ScreenLength = maxScreenLength;
				}

			m_VertexBuffer->GetIndexBuffer()->Unlock();
			m_VertexBuffer->Unlock();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIFont::Render(C_GUIControl* control) 
		{
			if (!m_FontFace)
				return;	

			RENDER::C_RenderTechnique* pTechnique = C_RedeemerEngine::GetSingleton().GetGUIManager()->GetGUISystem()->GetFontManager()->GetFontMaterialTechnique();

			if (!pTechnique)
				return;

			m_Texture->BindForRendering();

			CORE::C_Matrix<float> translateMat;
			unsigned long textsLength = 0;
			std::vector<S_TextData>* texts = NULL;

			if (control == NULL)
			{
				texts = &m_Texts[m_InternalControl];
				control = m_InternalControl;
			}
			else
				texts = &m_Texts[control];

			for (std::map<C_GUIControl*, std::vector<S_TextData> >::iterator it = m_Texts.begin(); it != m_Texts.end(); ++ it)
				if ((*it).first == control)
					break;
				else
				{
					for (unsigned int i = 0; i < (*it).second.size(); ++ i)
						textsLength += (*it).second[i].RealLength;
				}

										
			for (unsigned int i = 0; i < texts->size(); ++ i)
			{	
				for (unsigned int j = 0; j < pTechnique->GetRenderPassesCount(); ++j)
				{
					RENDER::C_RenderPass* pass = pTechnique->GetRenderPass(j);

					translateMat.SetTranslation(CORE::C_Vector3D<float> ((float)(*texts)[i].XPosition, (float)(*texts)[i].YPosition, 0.0f));

					C_RedeemerEngine::GetSingleton ().GetRenderManager ()->GetRenderDevice ()->SetRenderMatrix (RENDER::RMM_World, translateMat);

					pass->BindForRendering();
					pass->UpdateShaders();

					CORE::C_Vector4D<float> updateColor (m_BackgroundColor.GetRedFloat(), m_BackgroundColor.GetGreenFloat(), m_BackgroundColor.GetBlueFloat(), m_BackgroundColor.GetAlphaFloat());
										 
					pass->GetPixelShader()->GetShader()->SetData("BackGroundColor", &updateColor.X, 4);

					unsigned int length = (*texts)[i].RealLength;

					C_RedeemerEngine::GetSingleton ().GetRenderManager ()->GetRenderDevice ()->DrawPrimitives (m_VertexBuffer, 0, length * 4, textsLength * 6, length * 2);

					pass->UnbindFromRendering();

					textsLength += length;
				}
			}

			m_Texture->UnbindFromRendering();
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::wstring C_GUIFont::GetFontName () const
		{
			return m_FontName;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIFont::SetBackgroundColor (RENDER::C_Color color)
		{
			m_BackgroundColor = color;
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned char C_GUIFont::GetHeight () const
		{
			return m_Height;
		}

		//------------------------------------------------------------------------------------------------------------------------

		const std::vector<unsigned char>& C_GUIFont::GetWidths () const
		{
			return m_Widths;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace GUI
}	//	namespace REDEEMER
