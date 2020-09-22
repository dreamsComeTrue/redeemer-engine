/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_GUI_GUIStaticText.cpp
**		GUI Control for rendering text
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_GUI_GUIStaticText.h"
#include "R_GUI_GUIFont.h"
#include "R_GUI_GUISkin.h"
#include "R_GUI_GUIImage.h"
#include "..\\Render\\R_Render_Material.h"
#include "..\\Render\\R_Render_RenderTechnique.h"
#include "..\\Render\\R_Render_RenderPass.h"
#include "..\\Render\\R_Render_TextureSampler.h"
#include "..\\Render\\R_Render_RenderDevice.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace GUI
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_GUIStaticText::C_GUIStaticText (unsigned int ID, C_GUIControl* parent, CORE::C_Rectangle<int> dimensions) :
			C_GUIControl (ID, GUICT_StaticText, parent, dimensions),
			m_Font (NULL),
			m_FontColor (RENDER::C_Color::COLOR_BLUE),
			m_WordWrap (false),
			m_VeritcalAlignment (GUICA_Center),
			m_HorizontalAlignment (GUICA_Center),
			m_BackgroundImage (NULL),
			m_TextWidth (0)
		{
			m_BackgroundImage = new C_GUIImage (1, NULL, dimensions);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUIStaticText::~C_GUIStaticText ()
		{
			REDEEMER_SAFE_DELETE (m_BackgroundImage);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIStaticText::Render ()
		{
			if (!m_Font)
				return;

			if (m_Skin)
			{
				CORE::C_Rectangle<int> rectangle;
				RENDER::C_Material* material = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetMaterialLibrary()->GetMaterialManager(L"SimpleMaterialManager")->GetMaterial(m_Skin->GetMaterialName());

				if (material)
				{
					m_BackgroundImage->SetImage(material->GetActiveRenderTechnique()->GetRenderPass(0)->GetTextureSampler(0).GetTexture());
					m_BackgroundImage->SetTextureRectangle(m_Skin->GetControlSkinInfo(GUICT_StaticText)[L"Surface"].States[L"Normal"]);

					m_BackgroundImage->Render();
				}
			}

			C_RedeemerEngine::GetSingleton ().GetRenderManager ()->GetRenderDevice ()->SetScissorTestEnable (true);
			C_RedeemerEngine::GetSingleton ().GetRenderManager ()->GetRenderDevice ()->SetScissorRectangle (m_ClippingRectangle);

			m_Font->Render(this);

			C_RedeemerEngine::GetSingleton ().GetRenderManager ()->GetRenderDevice ()->SetScissorTestEnable (false);

			//	Down the hierarchy
			C_GUIControl::Render();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIStaticText::SetText (const wchar_t* caption)
		{
			C_GUIControl::SetText(caption);

			RecalculatePositions();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIStaticText::SetFont (C_GUIFont* font)
		{
			m_Font = font;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUIFont* C_GUIStaticText::GetFont ()
		{
			return m_Font;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIStaticText::SetFontColor (RENDER::C_Color color)
		{
			m_FontColor = color;

			RecalculatePositions();
		}

		//------------------------------------------------------------------------------------------------------------------------

		RENDER::C_Color C_GUIStaticText::GetFontColor ()
		{
			return m_FontColor;
		}	

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIStaticText::SetWordWrapEnabled (bool enable)
		{
			m_WordWrap = enable;

			RecalculatePositions();
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUIStaticText::IsWordWrapEnabled () const
		{
			return m_WordWrap;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIStaticText::SetVerticalAlignment (EGUIControlAlignment alignment)
		{
			m_VeritcalAlignment = alignment;

			RecalculatePositions();
		}

		//------------------------------------------------------------------------------------------------------------------------

		EGUIControlAlignment C_GUIStaticText::GetVerticalAlignment () const
		{
			return m_VeritcalAlignment;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIStaticText::SetHorizontalAlignment (EGUIControlAlignment alignment)
		{
			m_HorizontalAlignment = alignment;

			RecalculatePositions();
		}

		//------------------------------------------------------------------------------------------------------------------------

		EGUIControlAlignment C_GUIStaticText::GetHorizontalAlignment () const
		{
			return m_HorizontalAlignment;			
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIStaticText::RecalculatePositions ()
		{
			if (!m_Font)
				return;
			
			m_Font->ClearText(this);

			int xPosition = 0;
			int yPosition = 0;
			m_TextWidth = 0;

			std::vector<unsigned char> widths = m_Font->GetWidths();

			std::vector<S_TextData> texts;
			S_TextData tempText;

			for (unsigned int i = 0; i < m_Text.length(); i ++)
			{	
				unsigned char ch ((unsigned char)m_Text[i] - C_GUIFont::SPACE);     // ch-SPACE = DisplayList offset

				// replace characters outside the valid range with undrawable
				if (ch > C_GUIFont::NUM_CHARS)
				{
					ch = C_GUIFont::NUM_CHARS - 1;   // last character is 'undrawable'
				}

				m_TextWidth += widths[ch];	
			}

			//	Word wrap enabled?
			if (m_WordWrap)
			{
				switch (m_VeritcalAlignment)
				{
				case GUICA_UpperLeft:
					yPosition = m_AbsoluteRectangle.UpperLeftCorner.Y;						  
					break;

				case GUICA_LowerRight:
					yPosition = m_AbsoluteRectangle .LowerRightCorner.Y - m_Font->GetHeight();
					break;

				case GUICA_Center:
				case GUICA_Stretch:
					yPosition = (m_AbsoluteRectangle.UpperLeftCorner.Y + (m_AbsoluteRectangle .LowerRightCorner.Y - m_AbsoluteRectangle.UpperLeftCorner.Y) / 2) - 
						(m_Font->GetHeight() / 2);
					break;
				}

				switch (m_HorizontalAlignment)
				{
				case GUICA_UpperLeft:
					xPosition = m_AbsoluteRectangle.UpperLeftCorner.X;

					if (m_TextWidth > GetWidth())
					{
						//	We need to break text into multiple lines	 
						BreakText(xPosition, yPosition);

						goto SKIP_ADDING_TEXT;	
					}
					else
					{
						goto ADD_SINGLE_LINE_TEXT;
					}
					break;

				case GUICA_LowerRight:
				case GUICA_Center:
				case GUICA_Stretch:
					xPosition = (m_AbsoluteRectangle.UpperLeftCorner.X + (m_AbsoluteRectangle .LowerRightCorner.X - m_AbsoluteRectangle.UpperLeftCorner.X) / 2);

					if (m_TextWidth > GetWidth())
					{
						//	We need to break text into multiple lines
						BreakText(xPosition, yPosition);

						goto SKIP_ADDING_TEXT;
					}
					else
					{
						goto ADD_SINGLE_LINE_TEXT;
					}
					break;
				}

ADD_SINGLE_LINE_TEXT:

				tempText.Text = m_Text;
				tempText.XPosition = xPosition;
				tempText.YPosition = yPosition;

				texts.push_back(tempText);

SKIP_ADDING_TEXT:
				;
			}
			else
			{
				//	Word wrap disabled

				switch (m_VeritcalAlignment)
				{
				case GUICA_UpperLeft:
					yPosition = m_AbsoluteRectangle.UpperLeftCorner.Y;						  
					break;

				case GUICA_LowerRight:
					yPosition = m_AbsoluteRectangle .LowerRightCorner.Y - m_Font->GetHeight();
					break;

				case GUICA_Center:
				case GUICA_Stretch:
					yPosition = (m_AbsoluteRectangle.UpperLeftCorner.Y + (m_AbsoluteRectangle .LowerRightCorner.Y - m_AbsoluteRectangle.UpperLeftCorner.Y) / 2) - 
						(m_Font->GetHeight() / 2);
					break;
				}

				switch (m_HorizontalAlignment)
				{
				case GUICA_UpperLeft:
					xPosition = m_AbsoluteRectangle.UpperLeftCorner.X;						  
					break;

				case GUICA_LowerRight:

					xPosition = m_AbsoluteRectangle .LowerRightCorner.X - m_TextWidth;

					if (xPosition < m_AbsoluteRectangle.UpperLeftCorner.X)
						xPosition = m_AbsoluteRectangle.UpperLeftCorner.X;
					break;

				case GUICA_Center:
				case GUICA_Stretch:
					xPosition = (m_AbsoluteRectangle.UpperLeftCorner.X + (m_AbsoluteRectangle .LowerRightCorner.X - m_AbsoluteRectangle.UpperLeftCorner.X) / 2) - 
						(m_TextWidth / 2);

					if (xPosition < m_AbsoluteRectangle.UpperLeftCorner.X)
						xPosition = m_AbsoluteRectangle.UpperLeftCorner.X;
					break;
				}

				tempText.Text = m_Text;
				tempText.XPosition = xPosition;
				tempText.YPosition = yPosition;

				texts.push_back(tempText);
			}

			for (unsigned int i = 0; i < texts.size(); i ++)
				m_Font->AddText(texts[i].Text, texts[i].XPosition, texts[i].YPosition, m_FontColor, this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIStaticText::BreakText (int XPos, int yPos)
		{
			int xPosition = XPos;
			int xDiff = XPos - m_AbsoluteRectangle.UpperLeftCorner.X;
			int yPosition = yPos;
			int textWidth = 0;

			std::vector<unsigned char> widths = m_Font->GetWidths();

			std::vector<S_TextData> texts;
			S_TextData tempText;

			int controlWidth = GetWidth();
			std::wstring text;
			unsigned int i = 0;
			int tempWidth = 0;

			while (true)
			{
				unsigned char ch ((unsigned char)m_Text[i] - C_GUIFont::SPACE);     // ch-SPACE = DisplayList offset

				// replace characters outside the valid range with undrawable
				if (ch > C_GUIFont::NUM_CHARS)
				{
					ch = C_GUIFont::NUM_CHARS - 1;   // last character is 'undrawable'
				}

				if (i >= m_Text.length())
				{
					tempText.Text = text;
					tempText.XPosition = xPosition;
					tempText.YPosition = yPosition;

					texts.push_back(tempText);

					break;
				}

				tempWidth += widths[ch];
				text += m_Text[i];

				if (((tempWidth + xDiff) >= controlWidth) || (m_Text[i] == L'\n'))
				{
					text.resize(text.size() - 1);

					if (m_Text[i] != L'\n')
						--i;

					tempText.Text = text;
					tempText.XPosition = xPosition;
					tempText.YPosition = yPosition;

					texts.push_back(tempText);

					yPosition += m_Font->GetHeight();

					text.clear();
					tempWidth = 0;
				}

				++i;
			}

			for (unsigned int i = 0; i < texts.size(); i ++)
				m_Font->AddText(texts[i].Text, texts[i].XPosition, texts[i].YPosition, m_FontColor, this);
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_GUIStaticText::GetTextWidth() const
		{
			return m_TextWidth;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace GUI
}	//	namespace REDEEMER
