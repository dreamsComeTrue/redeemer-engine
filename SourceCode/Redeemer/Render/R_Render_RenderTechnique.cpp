/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_RenderTechnique.cpp
**		Render technique contains multiple passes for material
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_RenderTechnique.h"
#include "R_Render_RenderPass.h"
#include "R_Render_RenderQueue.h"
#include "R_Render_RenderEntity.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_RenderTechnique::C_RenderTechnique ()	:
			m_Name (L""),
			m_Material (NULL)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_RenderTechnique::~C_RenderTechnique ()
		{
			for (std::vector<C_RenderPass*>::iterator it = m_RenderPasses.begin(); it != m_RenderPasses.end(); ++ it)
			{
				REDEEMER_SAFE_DELETE (*it);
			}

			m_RenderPasses.clear();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderTechnique::SetName (const std::wstring& name)
		{
			m_Name = name;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::wstring C_RenderTechnique::GetName () const
		{				  
			return m_Name;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderTechnique::AddRenderPass (C_RenderPass* pass)
		{
			m_RenderPasses.push_back(pass);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_RenderPass* C_RenderTechnique::GetRenderPass (unsigned int index)
		{
			return m_RenderPasses[index];
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned int C_RenderTechnique::GetRenderPassesCount () const
		{
			return m_RenderPasses.size();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderTechnique::SetMaterial (C_Material* material)
		{
			m_Material = material;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Material* C_RenderTechnique::GetMaterial () const
		{
			return m_Material;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderTechnique::Update ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderTechnique::BindForRendering ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderTechnique::UnbindFromRendering ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderTechnique::Parse (FILESYSTEM::C_XMLDocument::C_XMLNode* xmlFile)
		{
			// Parse child nodes
			FILESYSTEM::C_XMLDocument::C_XMLNodeEnumerator node = xmlFile->GetNodeEnumerator();

			while (node.Next ())
			{
				// Parse render pass
				if (node->GetName () == "Pass")
				{
					std::string name = node->FindAttribute("Name");
					std::wstring passName;

					passName.assign(name.begin(), name.end());

					C_RenderPass* pass = new C_RenderPass (passName);

					pass->Parse (node.Current ());

					std::vector <C_RenderPass*>::iterator it;

					for (it = m_RenderPasses.begin(); it != m_RenderPasses.end(); ++it)
					{
						std::wstring name = (*it)->GetName();

						if(name == passName)
						{
							break;
						}
					}

					if (it == m_RenderPasses.end())
						m_RenderPasses.push_back(pass);
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER

