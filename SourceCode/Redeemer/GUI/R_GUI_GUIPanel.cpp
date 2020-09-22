/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_GUI_GUIPanel.cpp
**		Panel is a simple area for placing other controls
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_GUI_GUIPanel.h"	
#include "R_GUI_GUIStaticText.h"
#include "R_GUI_GUIImage.h"
#include "R_GUI_GUISkin.h"
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
		C_GUIPanel::C_GUIPanel (unsigned int ID, C_GUIControl* parent, CORE::C_Rectangle<int> dimensions) :
			C_GUIControl (ID, GUICT_Panel, parent, dimensions)
		{
			m_BackgroundText = new C_GUIStaticText (1, NULL, dimensions);
			m_BackgroundImage = new C_GUIImage (1, NULL, dimensions);

			m_BackgroundText->SetVerticalAlignment(GUICA_Center);
			m_BackgroundText->SetHorizontalAlignment(GUICA_Center);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUIPanel::~C_GUIPanel ()
		{
			REDEEMER_SAFE_DELETE (m_BackgroundImage);
			REDEEMER_SAFE_DELETE (m_BackgroundText);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIPanel::Render ()
		{
			if (m_Skin)
			{
				CORE::C_Rectangle<int> rectangle;
				RENDER::C_Material* material = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetMaterialLibrary()->GetMaterialManager(L"SimpleMaterialManager")->GetMaterial(m_Skin->GetMaterialName());

				if (material)
				{
					m_BackgroundImage->SetImage(material->GetActiveRenderTechnique()->GetRenderPass(0)->GetTextureSampler(0).GetTexture());
					m_BackgroundImage->SetTextureRectangle(m_Skin->GetControlSkinInfo(GUICT_Button)[L"Surface"].States[L"Normal"]);

					m_BackgroundImage->Render();
				}
			}
			else
			{
				m_BackgroundImage->SetTextureRectangle(CORE::C_Rectangle<int>(0,0,1,1));
				m_BackgroundImage->SetImage(NULL);
				m_BackgroundImage->Render();
			}

			m_BackgroundText->Render();

			//	Down the hierarchy
			C_GUIControl::Render();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIPanel::SetText (const wchar_t* caption)
		{
			C_GUIControl::SetText(caption);

			m_BackgroundText->SetText(caption);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIPanel::SetCaptionFont (C_GUIFont* font)
		{
			m_BackgroundText->SetFont(font);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUIFont* C_GUIPanel::GetCaptionFont ()
		{
			return m_BackgroundText->GetFont();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIPanel::SetColor (RENDER::C_Color color)
		{
			m_Color = color;

			m_BackgroundImage->SetVerticesColors(color, color, color, color);
		}

		//------------------------------------------------------------------------------------------------------------------------

		RENDER::C_Color C_GUIPanel::GetColor ()
		{
			return m_Color;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIPanel::SetCaptionColor (RENDER::C_Color color)
		{
			m_BackgroundText->SetFontColor(color);
		}

		//------------------------------------------------------------------------------------------------------------------------

		RENDER::C_Color C_GUIPanel::GetCaptionColor ()
		{
			return m_BackgroundText->GetFontColor();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIPanel::SetCaptionVerticalAlignment (EGUIControlAlignment alignment)
		{
			m_BackgroundText->SetVerticalAlignment(alignment);
		}

		//------------------------------------------------------------------------------------------------------------------------

		EGUIControlAlignment C_GUIPanel::GetCaptionVerticalAlignment () const
		{
			return m_BackgroundText->GetVerticalAlignment();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIPanel::SetCaptionHorizontalAlignment (EGUIControlAlignment alignment)
		{
			m_BackgroundText->SetHorizontalAlignment(alignment);
		}

		//------------------------------------------------------------------------------------------------------------------------

		EGUIControlAlignment C_GUIPanel::GetCaptionHorizontalAlignment () const
		{
			return m_BackgroundText->GetHorizontalAlignment();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUIStaticText* C_GUIPanel::GetInternalTextControl ()
		{
			return m_BackgroundText;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUIImage* C_GUIPanel::GetInternalImageControl ()
		{
			return m_BackgroundImage;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace GUI
}	//	namespace REDEEMER
