/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_Sprite.cpp
**		Sprites are used to draw 2D surfaces on the screen, mostly used in GUI
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_Sprite.h"
#include "R_Render_RenderDevice.h"
#include "R_Render_VertexBuffer.h"
#include "R_Render_TextureSampler.h"
#include "R_Render_RenderTechnique.h"
#include "R_Render_RenderPass.h"
#include "R_Render_Material.h"
#include "..\\Render\\R_Render_ShaderEntity.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_Sprite::C_Sprite (CORE::C_Rectangle<float> dimension) :
			m_Dimension (dimension),
			m_VertexBuffer (NULL),
			m_Material (NULL)
		{
			m_VertexBuffer = C_RedeemerEngine::GetSingleton ().GetRenderManager ()->GetRenderDevice ()->GetVertexBufferManager()->CreateVertexBuffer(4, RENDER::FVF_VERTEX3TC, false);
			m_VertexBuffer->SetPrimitivesCount(2);
			m_VertexBuffer->SetPrimitiveType (RENDER::RPT_TriangleFan);

			m_Colors[0].SetColorRGBA(0xffffffff);
			m_Colors[1].SetColorRGBA(0xffffffff);
			m_Colors[2].SetColorRGBA(0xffffffff);
			m_Colors[3].SetColorRGBA(0xffffffff);
							
			SetVertexData();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Sprite::~C_Sprite ()
		{
			C_RedeemerEngine::GetSingleton ().GetRenderManager ()->GetRenderDevice ()->GetVertexBufferManager()->RemoveVertexBuffer (m_VertexBuffer);

			m_Material = NULL;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Sprite::SetPosition (CORE::C_Vector2D<float> position)
		{
			m_Dimension.UpperLeftCorner = position;
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Vector2D<float> C_Sprite::GetPosition () const
		{
			return m_Dimension.UpperLeftCorner;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Sprite::SetWidth (float width)
		{
			m_Dimension.LowerRightCorner.X = width;
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_Sprite::GetWidth () const
		{
			return m_Dimension.LowerRightCorner.X;			
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Sprite::SetHeight (float height)
		{
			m_Dimension.LowerRightCorner.Y = height;
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_Sprite::GetHeight () const
		{
			return m_Dimension.LowerRightCorner.Y;
		}	

		//------------------------------------------------------------------------------------------------------------------------

		void C_Sprite::SetVerticesColors (C_Color upperLeft, C_Color upperRight, C_Color lowerLeft, C_Color lowerRight)
		{
			m_Colors[0] = upperLeft;
			m_Colors[1] = upperRight;
			m_Colors[2] = lowerLeft;
			m_Colors[3] = lowerRight;

			SetVertexData();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Color* C_Sprite::GetVerticesColors ()
		{
			return m_Colors;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Sprite::SetTextureRectangle (CORE::C_Rectangle<int> rectangle)
		{
			if (!m_Material)
				return;

			//	We use only first render pass, first texture sampler

			int textureWidth = m_Material->GetActiveRenderTechnique()->GetRenderPass(0)->GetTextureSampler(0).GetTexture()->GetWidth();
			int textureHeight = m_Material->GetActiveRenderTechnique()->GetRenderPass(0)->GetTextureSampler(0).GetTexture()->GetHeight();

			RENDER::S_VertexColorTexture *vertices;

			//	Set vertices
			m_VertexBuffer->Lock(4, (void**)&vertices);

			vertices[0].TexCoord = CORE::C_Vector2D<float>(((float)rectangle.UpperLeftCorner.X / textureWidth), ((float)rectangle.UpperLeftCorner.Y / textureHeight));
			vertices[1].TexCoord = CORE::C_Vector2D<float>(((float)rectangle.LowerRightCorner.X / textureWidth), ((float)rectangle.UpperLeftCorner.Y / textureHeight));
			vertices[2].TexCoord = CORE::C_Vector2D<float>(((float)rectangle.LowerRightCorner.X / textureWidth), ((float)rectangle.LowerRightCorner.Y / textureHeight));
			vertices[3].TexCoord = CORE::C_Vector2D<float>(((float)rectangle.UpperLeftCorner.X / textureWidth), ((float)rectangle.LowerRightCorner.Y / textureHeight));

			m_VertexBuffer->Unlock();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Sprite::SetMaterial (C_Material* material)
		{
			m_Material = material;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Material* C_Sprite::GetMaterial ()
		{
			return m_Material;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Sprite::Render ()
		{
			if (!m_Material)
				return;

			CORE::C_Matrix<float> newWorld, translateMat;

			newWorld.SetScale(CORE::C_Vector3D<float> (m_Dimension.LowerRightCorner.X, m_Dimension.LowerRightCorner.Y, 1.0f));
			translateMat.SetTranslation(CORE::C_Vector3D<float> (m_Dimension.UpperLeftCorner.X, m_Dimension.UpperLeftCorner.Y, 0.0f));

			newWorld *= translateMat;

			C_RedeemerEngine::GetSingleton ().GetRenderManager ()->GetRenderDevice ()->SetRenderMatrix (RENDER::RMM_World, newWorld);

			m_Material->BindForRendering();

			C_RenderTechnique* pTechnique = m_Material->GetActiveRenderTechnique();

			for (unsigned int i = 0; i < pTechnique->GetRenderPassesCount(); ++i)
			{
				C_RenderPass* pass = pTechnique->GetRenderPass(i);

				pass->BindForRendering();
				pass->UpdateShaders();

				if (!pass->GetTextureSampler(0).GetTexture())
					pass->GetPixelShader()->GetShader()->SetData("UseTexture", false);
				else
					pass->GetPixelShader()->GetShader()->SetData("UseTexture", true);

				C_RedeemerEngine::GetSingleton ().GetRenderManager ()->GetRenderDevice ()->DrawPrimitives (m_VertexBuffer);

				pass->UnbindFromRendering();
			}		

			m_Material->UnbindFromRendering();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Sprite::SetVertexData()
		{
			S_VertexColorTexture quad[] =
			{
				{CORE::C_Vector3D<float>(0.0f, 0.0f, 0.5f), CORE::C_Vector2D<float>(0.0f, 0.0f), m_Colors[0].GetColorARGB()},
				{CORE::C_Vector3D<float>(1.0f, 0.0f, 0.5f), CORE::C_Vector2D<float>(1.0f, 0.0f), m_Colors[1].GetColorARGB()},
				{CORE::C_Vector3D<float>(1.0f, 1.0f, 0.5f), CORE::C_Vector2D<float>(1.0f, 1.0f), m_Colors[3].GetColorARGB()},
				{CORE::C_Vector3D<float>(0.0f, 1.0f, 0.5f), CORE::C_Vector2D<float>(0.0f, 1.0f), m_Colors[2].GetColorARGB()}
			};

			m_VertexBuffer->SetVertexData (quad, 4);
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER

