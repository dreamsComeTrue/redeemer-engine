/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_DirectX9Material.cpp
**		DirectX9 Material implementation
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_DirectX9Material.h"
#include "R_Render_DirectX9RenderDevice.h"
#include "..\\..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{	
		//------------------------------------------------------------------------------------------------------------------------
		C_DirectX9Material::C_DirectX9Material (std::wstring name) :
			C_Material (name)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DirectX9Material::~C_DirectX9Material ()
		{
			//	EMPTY			
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9Material::BindForRendering ()
		{
			C_DirectX9RenderDevice* device = static_cast<C_DirectX9RenderDevice*>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice());

			ZeroMemory (&m_Material, sizeof (m_Material));

			memcpy (&m_Material.Ambient, &m_AmbientColor, sizeof (float) * 4);
			memcpy (&m_Material.Diffuse, &m_DiffuseColor, sizeof (float) * 4);
			memcpy (&m_Material.Specular, &m_SpecularColor, sizeof (float) * 4);
			memcpy (&m_Material.Emissive, &m_EmissiveColor, sizeof (float) * 4);

			m_Material.Power = m_Shininess;

			device->GetDirectX9Device() ->SetMaterial (&m_Material);

			C_Material::BindForRendering();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9Material::UnbindFromRendering ()
		{
			C_Material::UnbindFromRendering();
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER
