/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_GUI_GUImage.cpp
**		GUI Control which uses C_Sprite to render an image
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_GUI_GUIImage.h"
#include "R_GUI_GUISystem.h"
#include "..\\Render\\R_Render_RenderDevice.h"
#include "..\\Render\\R_Render_Sprite.h"
#include "..\\Render\\R_Render_Material.h"
#include "..\\Render\\R_Render_TextureSampler.h"
#include "..\\Render\\R_Render_RenderTechnique.h"
#include "..\\Render\\R_Render_RenderPass.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace GUI
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_GUIImage::C_GUIImage (unsigned int ID, C_GUIControl* parent, CORE::C_Rectangle<int> dimensions) :
			C_GUIControl (ID, GUICT_Image, parent, dimensions),
			m_Sprite (NULL),
			m_Texture (NULL),
			m_OldTexture (NULL),
			m_GUIMaterial (NULL),
			m_ScaleImage (true)
		{
			m_Sprite = new RENDER::C_Sprite (CORE::C_Rectangle<float>((float)dimensions.UpperLeftCorner.X, (float)dimensions.UpperLeftCorner.Y,
				(float)dimensions.LowerRightCorner.X, (float)dimensions.LowerRightCorner.Y));
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUIImage::~C_GUIImage ()
		{
			REDEEMER_SAFE_DELETE (m_Sprite);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIImage::SetImage (RENDER::C_Texture* texture)
		{
			if (!m_Sprite)
				return;

			m_GUIMaterial = C_RedeemerEngine::GetSingleton().GetGUIManager()->GetGUISystem()->GetGUIMaterial();

			if (!m_GUIMaterial)
				return;

			m_Sprite->SetMaterial(m_GUIMaterial);

			m_OldTexture = m_GUIMaterial->GetActiveRenderTechnique()->GetRenderPass(0)->GetTextureSampler(0).GetTexture();

			m_Texture = texture;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIImage::SetScaleImage (bool scale)
		{
			if (!m_Texture)
				return;

			m_ScaleImage = scale;

			if (scale)
				SetTextureRectangle(CORE::C_Rectangle<int> (0, 0, m_Texture->GetWidth(), m_Texture->GetHeight()));
			else
				SetTextureRectangle(CORE::C_Rectangle<int> (0, 0, GetWidth(), GetHeight()));

		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIImage::SetTextureRectangle (CORE::C_Rectangle<int> rectangle)
		{
			m_Sprite->SetTextureRectangle (rectangle);
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_GUIImage::IsImageScaled () const
		{
			return m_ScaleImage;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIImage::Render ()
		{
			if (!m_GUIMaterial)
				return;

			m_GUIMaterial->GetActiveRenderTechnique()->GetRenderPass(0)->GetTextureSampler(0).SetTexture(m_Texture);

			m_Sprite->SetPosition(CORE::C_Vector2D<float>((float)m_AbsoluteRectangle.UpperLeftCorner.X, (float)m_AbsoluteRectangle.UpperLeftCorner.Y));
			m_Sprite->SetWidth((float)GetWidth());
			m_Sprite->SetHeight((float)GetHeight());

			C_RedeemerEngine::GetSingleton ().GetRenderManager ()->GetRenderDevice ()->SetScissorTestEnable (true);
			C_RedeemerEngine::GetSingleton ().GetRenderManager ()->GetRenderDevice ()->SetScissorRectangle (m_ClippingRectangle);

			m_Sprite->Render();

			C_RedeemerEngine::GetSingleton ().GetRenderManager ()->GetRenderDevice ()->SetScissorTestEnable (false);

			m_GUIMaterial->GetActiveRenderTechnique()->GetRenderPass(0)->GetTextureSampler(0).SetTexture(m_OldTexture);

			//	Down the hierarchy
			C_GUIControl::Render();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIImage::SetVerticesColors (RENDER::C_Color upperLeft, RENDER::C_Color upperRight, RENDER::C_Color lowerLeft, RENDER::C_Color lowerRight)
		{
			m_Sprite->SetVerticesColors(upperLeft, upperRight, lowerLeft, lowerRight);
		}

		//------------------------------------------------------------------------------------------------------------------------

		RENDER::C_Color* C_GUIImage::GetVerticesColors ()
		{
			return m_Sprite->GetVerticesColors();
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace GUI
}	//	namespace REDEEMER
