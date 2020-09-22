/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_GUI_GUIButton.cpp
**		Button is a widget that can respond to the user events
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_GUI_GUIButton.h"	
#include "R_GUI_GUISkin.h"
#include "R_GUI_GUIPanel.h"
#include "R_GUI_GUIImage.h"
#include "R_GUI_GUIStaticText.h"
#include "R_GUI_GUIFont.h"
#include "..\\Render\\R_Render_Material.h"
#include "..\\Render\\R_Render_RenderTechnique.h"
#include "..\\Render\\R_Render_RenderPass.h"
#include "..\\Render\\R_Render_TextureSampler.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace GUI
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_GUIButton::C_GUIButton (unsigned int ID, C_GUIControl* parent, CORE::C_Rectangle<int> dimensions) :
			C_GUIControl (ID, GUICT_Button, parent, dimensions),
			m_IsPushable (false),
			m_IsPressed (false),
			m_RenderGlyphImage (false),
			m_GlyphAlignment (GA_Left),
			m_GlyphSpacing (3),
			m_GlyphScaling (30)
		{
			m_GlyphImage = new C_GUIImage (1, NULL, dimensions);
			m_BackgroundPanel = new C_GUIPanel (2, NULL, dimensions);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUIButton::~C_GUIButton ()
		{
			REDEEMER_SAFE_DELETE (m_BackgroundPanel);
			REDEEMER_SAFE_DELETE (m_GlyphImage);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIButton::Render ()
		{
			//	Render background image
			if (m_Skin)
			{
				CORE::C_Rectangle<int> rectangle;
				RENDER::C_Material* material = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetMaterialLibrary()->GetMaterialManager(L"SimpleMaterialManager")->GetMaterial(m_Skin->GetMaterialName());

				if (material)
				{
					m_BackgroundPanel->GetInternalImageControl()->SetImage(material->GetActiveRenderTechnique()->GetRenderPass(0)->GetTextureSampler(0).GetTexture());
					m_BackgroundPanel->GetInternalImageControl()->SetTextureRectangle(m_Skin->GetControlSkinInfo(GUICT_Button)[L"Surface"].States[L"Normal"]);

					m_BackgroundPanel->Render();
				}
			}
			else
			{
				m_BackgroundPanel->GetInternalImageControl()->SetTextureRectangle(CORE::C_Rectangle<int>(0,0,1,1));
				m_BackgroundPanel->GetInternalImageControl()->SetImage(NULL);
				m_BackgroundPanel->GetInternalImageControl()->Render();
			}

			//	Render text
			if (m_BackgroundPanel->GetInternalTextControl()->GetFont())
			{
				if (m_RenderGlyphImage && m_GlyphImage)
					m_GlyphImage->Render();

				m_BackgroundPanel->GetInternalTextControl ()->Render();
			}

			//	Down the hierarchy
			C_GUIControl::Render();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIButton::SetText(const wchar_t* caption)
		{
			m_BackgroundPanel->SetText(caption);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIButton::SetPushable (bool pushable)
		{
			m_IsPushable = pushable;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUIButton::IsPushable () const
		{
			return m_IsPushable;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIButton::SetPressed ()
		{
			m_IsPressed = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIButton::SetReleased ()
		{
			m_IsPressed = false;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIButton::ChangePressedState ()
		{
			m_IsPressed = !m_IsPressed;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUIButton::IsPressed () const
		{
			return m_IsPressed;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIButton::SetGlyphImage (RENDER::C_Texture* texture)
		{	  
			m_GlyphImage->SetImage(texture);
			m_GlyphImage->SetScaleImage(true);

			UpdateTransformations();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIButton::SetGlyphImageRendered (bool enable)
		{
			m_RenderGlyphImage = enable;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUIButton::IsGlyphImageRendered () const
		{
			return m_RenderGlyphImage;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIButton::SetGlyphAlignment (EGlyphAlignment alignment)
		{
			m_GlyphAlignment = alignment;

			UpdateTransformations();
		}

		//------------------------------------------------------------------------------------------------------------------------

		EGlyphAlignment C_GUIButton::GetGlyphAlignment () const
		{
			return m_GlyphAlignment;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIButton::SetGlyphSpacing (int spacing)
		{
			m_GlyphSpacing = spacing;

			UpdateTransformations();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_GUIButton::GetGlyphSpacing () const
		{
			return m_GlyphSpacing;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIButton::UpdateTransformations ()
		{
			//	Glyph image, should be 0.20 of button's width
			int size, height, tmp;
			int textHeight;
			CORE::C_Rectangle<int> dimensions;

			if (m_BackgroundPanel->GetInternalTextControl()->GetText().empty())
				textHeight = 0;
			else
				textHeight = m_BackgroundPanel->GetInternalTextControl()->GetFont()->GetHeight();

			//	Calculate new positions for glyph image and text
			switch (m_GlyphAlignment)
			{
			case GA_Left:
				size = static_cast<int>(GetWidth() * m_GlyphScaling / 100);
				dimensions.UpperLeftCorner = GetAbsolutePosition() + CORE::C_Vector2D<int>(m_GlyphSpacing, GetHeight() / 2 - (size / 2));
				dimensions.LowerRightCorner = dimensions.UpperLeftCorner + CORE::C_Vector2D<int> (size, size);

				if (dimensions.UpperLeftCorner.Y < GetAbsolutePosition ().Y + m_GlyphSpacing)
					dimensions.UpperLeftCorner.Y = GetAbsolutePosition ().Y + m_GlyphSpacing;

				if (dimensions.LowerRightCorner.X - dimensions.UpperLeftCorner.X > GetWidth() - m_GlyphSpacing)
					dimensions.LowerRightCorner.X = GetAbsolutePosition ().X + GetWidth () - m_GlyphSpacing;

				if (dimensions.LowerRightCorner.Y - dimensions.UpperLeftCorner.Y > GetHeight() - 2 * m_GlyphSpacing)
					dimensions.LowerRightCorner.Y = GetAbsolutePosition ().Y + GetHeight() - m_GlyphSpacing;

				m_GlyphImage->SetRelativeDimensions(dimensions);

				dimensions.UpperLeftCorner = GetAbsolutePosition() + CORE::C_Vector2D<int>(m_GlyphSpacing * 2 + m_GlyphImage->GetWidth(), 0);
				dimensions.LowerRightCorner = GetAbsoluteDimensions().LowerRightCorner;

				m_BackgroundPanel->GetInternalTextControl()->SetRelativeDimensions(dimensions);
				m_BackgroundPanel->GetInternalTextControl()->RecalculatePositions();

				break;

			case GA_Right:
				size = static_cast<int>(GetWidth() * m_GlyphScaling / 100);
				dimensions.UpperLeftCorner = GetAbsolutePosition() + CORE::C_Vector2D<int>(GetWidth() - size - m_GlyphSpacing, GetHeight() / 2 - (size / 2));
				dimensions.LowerRightCorner = dimensions.UpperLeftCorner + CORE::C_Vector2D<int> (size, size);

				if (dimensions.LowerRightCorner.Y > GetAbsolutePosition ().Y + GetHeight () - 2 * m_GlyphSpacing)
					dimensions.LowerRightCorner.Y = GetAbsolutePosition ().Y + GetHeight () - m_GlyphSpacing;

				if (dimensions.UpperLeftCorner.X < GetAbsolutePosition ().X + m_GlyphSpacing)
					dimensions.UpperLeftCorner.X = GetAbsolutePosition ().X + m_GlyphSpacing;

				if (dimensions.UpperLeftCorner.Y < GetAbsolutePosition().Y + m_GlyphSpacing)
					dimensions.UpperLeftCorner.Y = GetAbsolutePosition ().Y + m_GlyphSpacing;

				m_GlyphImage->SetRelativeDimensions(dimensions);

				dimensions.UpperLeftCorner = GetAbsolutePosition() + CORE::C_Vector2D<int>(m_GlyphSpacing, 0);

				size = m_GlyphImage->GetWidth();

				dimensions.LowerRightCorner =  dimensions.UpperLeftCorner + CORE::C_Vector2D<int>(GetWidth() - size - 3 * m_GlyphSpacing, GetHeight());

				m_BackgroundPanel->GetInternalTextControl()->SetRelativeDimensions(dimensions);
				m_BackgroundPanel->GetInternalTextControl()->RecalculatePositions();

				break;

			case GA_Top:
				size = static_cast<int>(GetHeight() * m_GlyphScaling / 100);
				height = size + m_GlyphSpacing + textHeight;
				dimensions.UpperLeftCorner = GetAbsolutePosition() + CORE::C_Vector2D<int>(GetWidth() / 2 - (size / 2), GetHeight() / 2 - (height / 2));
				dimensions.LowerRightCorner = dimensions.UpperLeftCorner + CORE::C_Vector2D<int> (size, size);

				if (dimensions.LowerRightCorner.X > GetAbsolutePosition ().X + GetWidth () - m_GlyphSpacing)
					dimensions.LowerRightCorner.X = GetAbsolutePosition ().X + GetWidth () - m_GlyphSpacing;

				if (dimensions.UpperLeftCorner.X < GetAbsolutePosition ().X + m_GlyphSpacing)
					dimensions.UpperLeftCorner.X = GetAbsolutePosition ().X + m_GlyphSpacing;

				if (dimensions.UpperLeftCorner.Y < GetAbsolutePosition().Y + m_GlyphSpacing)
					dimensions.UpperLeftCorner.Y = GetAbsolutePosition ().Y + m_GlyphSpacing;

				m_GlyphImage->SetRelativeDimensions(dimensions);

				height = dimensions.UpperLeftCorner.Y - GetAbsolutePosition().Y;

				dimensions.UpperLeftCorner = GetAbsolutePosition() + CORE::C_Vector2D<int>(m_GlyphSpacing, height + m_GlyphSpacing + m_GlyphImage->GetHeight());
				dimensions.LowerRightCorner =  dimensions.UpperLeftCorner + CORE::C_Vector2D<int>(GetWidth() - 2 * m_GlyphSpacing, textHeight);

				if (dimensions.LowerRightCorner.Y > GetAbsolutePosition().Y + GetHeight() - 2 * m_GlyphSpacing)
					dimensions.LowerRightCorner.Y = GetAbsolutePosition().Y + GetHeight() - m_GlyphSpacing;

				m_BackgroundPanel->GetInternalTextControl()->SetRelativeDimensions(dimensions);
				m_BackgroundPanel->GetInternalTextControl()->RecalculatePositions();

				break;

			case GA_Bottom:
				size = static_cast<int>(GetHeight() * m_GlyphScaling / 100);
				height = size + m_GlyphSpacing + textHeight;

				dimensions.UpperLeftCorner = GetAbsolutePosition() + CORE::C_Vector2D<int>(GetWidth() / 2 - (size / 2), GetHeight() / 2 - (height / 2) + textHeight + m_GlyphSpacing);
				dimensions.LowerRightCorner = dimensions.UpperLeftCorner + CORE::C_Vector2D<int> (size, size);

				if (dimensions.LowerRightCorner.X > GetAbsolutePosition ().X + GetWidth () - m_GlyphSpacing)
					dimensions.LowerRightCorner.X = GetAbsolutePosition ().X + GetWidth () - m_GlyphSpacing;

				if (dimensions.LowerRightCorner.Y > GetAbsolutePosition ().Y + GetHeight () - m_GlyphSpacing)
					dimensions.LowerRightCorner.Y = GetAbsolutePosition ().Y + GetHeight () - m_GlyphSpacing;

				if (dimensions.UpperLeftCorner.X < GetAbsolutePosition ().X + m_GlyphSpacing)
					dimensions.UpperLeftCorner.X = GetAbsolutePosition ().X + m_GlyphSpacing;

				if (dimensions.UpperLeftCorner.Y < GetAbsolutePosition().Y + m_GlyphSpacing)
					dimensions.UpperLeftCorner.Y = GetAbsolutePosition ().Y + m_GlyphSpacing;

				m_GlyphImage->SetRelativeDimensions(dimensions);

				height = m_GlyphImage->GetHeight() + m_GlyphSpacing + textHeight;
				tmp = dimensions.UpperLeftCorner.Y - m_AbsoluteRectangle.UpperLeftCorner.Y;

				dimensions.UpperLeftCorner = GetAbsolutePosition() + CORE::C_Vector2D<int>(m_GlyphSpacing, GetHeight() / 2 - (height / 2));
				dimensions.LowerRightCorner =  dimensions.UpperLeftCorner + CORE::C_Vector2D<int>(GetWidth() - 2 * m_GlyphSpacing, tmp - 2 * m_GlyphSpacing);
																										
				if (dimensions.UpperLeftCorner.Y < GetAbsolutePosition().Y + m_GlyphSpacing)
				{
					dimensions.UpperLeftCorner.Y = GetAbsolutePosition().Y + m_GlyphSpacing;
					dimensions.LowerRightCorner =  dimensions.UpperLeftCorner + CORE::C_Vector2D<int>(GetWidth() - 2 * m_GlyphSpacing, tmp - 2 * m_GlyphSpacing);

					if (dimensions.LowerRightCorner.Y < dimensions.UpperLeftCorner.Y)
						dimensions.LowerRightCorner.Y = dimensions.UpperLeftCorner.Y;
				}

				m_BackgroundPanel->GetInternalTextControl()->SetRelativeDimensions(dimensions);
				m_BackgroundPanel->GetInternalTextControl()->RecalculatePositions();

				break;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIButton::SetGlyphScalingFactor (int scaling)
		{
			m_GlyphScaling = CORE::C_MathUtils::Clamp(scaling, 0, 100);

			UpdateTransformations();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_GUIButton::GetGlyphScalingFactor () const
		{
			return m_GlyphScaling;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUIPanel* C_GUIButton::GetInternalPanelControl()
		{
			return m_BackgroundPanel;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace GUI
}	//	namespace REDEEMER
