/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_GUI_GUICheckBox.cpp
**		Checkbox is like a button, but it can be used like a toggle switch 
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_GUI_GUICheckBox.h"
#include "R_GUI_GUISkin.h"
#include "R_GUI_GUIImage.h"
#include "R_GUI_GUIStaticText.h"
#include "R_GUI_GUIPanel.h"
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

		C_GUICheckBox::C_GUICheckBox (unsigned int ID, C_GUIControl* parent, CORE::C_Rectangle<int> dimensions, bool checked) :
			C_GUIControl (ID, GUICT_CheckBox, parent, dimensions),
			m_IsChecked (checked),
			m_IsPressed (false)
		{
			m_GlyphImage = new C_GUIImage (1, NULL, dimensions);
			m_BackgroundPanel = new C_GUIPanel (2, NULL, dimensions);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUICheckBox::~C_GUICheckBox ()
		{
			REDEEMER_SAFE_DELETE (m_BackgroundPanel);
			REDEEMER_SAFE_DELETE (m_GlyphImage);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUICheckBox::Render ()
		{
			//	Render background image
			if (m_Skin)
			{
				CORE::C_Rectangle<int> rectangle;
				RENDER::C_Material* material = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetMaterialLibrary()->GetMaterialManager(L"SimpleMaterialManager")->GetMaterial(m_Skin->GetMaterialName());

				if (material)
				{
					m_BackgroundPanel->GetInternalImageControl()->SetImage(material->GetActiveRenderTechnique()->GetRenderPass(0)->GetTextureSampler(0).GetTexture());
					m_BackgroundPanel->GetInternalImageControl()->SetTextureRectangle(m_Skin->GetControlSkinInfo(GUICT_CheckBox)[L"Surface"].States[L"Normal"]);

					m_BackgroundPanel->Render();
				}
			}
			else
			{
				m_BackgroundPanel->GetInternalImageControl()->SetTextureRectangle(CORE::C_Rectangle<int>(0,0,1,1));
				m_BackgroundPanel->GetInternalImageControl()->SetImage(NULL);
				m_BackgroundPanel->GetInternalImageControl()->Render();
			}

			m_GlyphImage->Render();

			//	Render text
			if (m_BackgroundPanel->GetInternalTextControl()->GetFont())
			{
				m_BackgroundPanel->GetInternalTextControl ()->Render();
			}

			//	Down the hierarchy
			C_GUIControl::Render();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUICheckBox::SetText (const wchar_t* caption)
		{
			m_BackgroundPanel->SetText(caption);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUICheckBox::SetChecked (bool checked)
		{
			m_IsChecked = checked;

			UpdateTransformations();
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUICheckBox::IsChecked () const
		{
			return m_IsChecked;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUIPanel* C_GUICheckBox::GetInternalPanelControl ()
		{
			return m_BackgroundPanel;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUICheckBox::UpdateTransformations ()
		{
			CORE::C_Rectangle<int> dimensions;
			int glyphScaling = 20;		//	1/5 of control's width
			int glyphSpacing = 4;

			int size = static_cast<int>(GetWidth() * glyphScaling / 100);

			dimensions.UpperLeftCorner = GetAbsolutePosition() + CORE::C_Vector2D<int>(glyphSpacing, GetHeight() / 2 - (size / 2));
			dimensions.LowerRightCorner = dimensions.UpperLeftCorner + CORE::C_Vector2D<int> (size, size);

			if (dimensions.UpperLeftCorner.Y < GetAbsolutePosition ().Y + glyphSpacing)
				dimensions.UpperLeftCorner.Y = GetAbsolutePosition ().Y + glyphSpacing;

			if (dimensions.LowerRightCorner.X - dimensions.UpperLeftCorner.X > GetWidth() - glyphSpacing)
				dimensions.LowerRightCorner.X = GetAbsolutePosition ().X + GetWidth () - glyphSpacing;

			if (dimensions.LowerRightCorner.Y - dimensions.UpperLeftCorner.Y > GetHeight() - 2 * glyphSpacing)
				dimensions.LowerRightCorner.Y = GetAbsolutePosition ().Y + GetHeight() - glyphSpacing;

			m_GlyphImage->SetRelativeDimensions(dimensions);

			dimensions.UpperLeftCorner = GetAbsolutePosition() + CORE::C_Vector2D<int>(glyphSpacing * 2 + m_GlyphImage->GetWidth(), 0);
			dimensions.LowerRightCorner = GetAbsoluteDimensions().LowerRightCorner;

			m_BackgroundPanel->GetInternalTextControl()->SetRelativeDimensions(dimensions);
			m_BackgroundPanel->GetInternalTextControl()->RecalculatePositions();
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace GUI
}	//	namespace REDEEMER
