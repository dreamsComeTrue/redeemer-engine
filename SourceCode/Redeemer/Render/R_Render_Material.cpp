/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_Material.cpp
**		Material controls how render techniques are processed
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_Material.h"
#include "R_Render_RenderTechnique.h"
#include "..\\Core\\R_Core_Utility.h"
#include "..\\FileSystem\\R_FileSystem_XMLDocument.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{	
		//------------------------------------------------------------------------------------------------------------------------
		C_Material::C_Material (std::wstring name) :
			m_Name (name),
			m_AmbientColor (C_Color::COLOR_WHITE),
			m_DiffuseColor (C_Color::COLOR_WHITE),
			m_SpecularColor (C_Color::COLOR_WHITE),
			m_EmissiveColor (C_Color::COLOR_WHITE),
			m_Shininess (0.0f),
			m_AlphaBlending (false),
			m_AlphaTest (false),
			m_AlphaTestReference (1),
			m_ZWrite (true),
			m_SourceBlend (BM_SrcColor),
			m_DestinationBlend (BM_Zero),
			m_BlendOperation (BO_Add),
			m_ActiveRenderTechnique (NULL)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Material::~C_Material ()
		{
			for (std::vector<C_RenderTechnique*>::iterator it = m_RenderTechniques.begin(); it != m_RenderTechniques.end(); ++it)
			{
				REDEEMER_SAFE_DELETE ((*it));
			}

			m_RenderTechniques.clear();
			
			m_ActiveRenderTechnique = NULL;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Material::SetName (const std::wstring& name)
		{
			m_Name = name;
		}

		//------------------------------------------------------------------------------------------------------------------------

		const std::wstring& C_Material::GetName ()
		{
			return m_Name;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Material::SetAmbientColor (C_Color color)
		{
			m_AmbientColor = color;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Color C_Material::GetAmbientColor ()
		{
			return m_AmbientColor;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Material::SetDiffuseColor (C_Color color)
		{
			m_DiffuseColor = color;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Color C_Material::GetDiffuseColor ()
		{
			return m_DiffuseColor;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Material::SetSpecularColor (C_Color color)
		{
			m_SpecularColor = color;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Color C_Material::GetSpecularColor ()
		{
			return m_SpecularColor;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Material::SetEmissiveColor (C_Color color)
		{
			m_EmissiveColor = color;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Color C_Material::GetEmissiveColor ()
		{
			return m_EmissiveColor;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Material::SetShininess (float shine)
		{
			m_Shininess = shine;
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_Material::GetShininess ()
		{
			return m_Shininess;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Material::SetAlphaBlending (bool alpha)
		{
			m_AlphaBlending= alpha;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_Material::GetAlphaBlending ()
		{
			return m_AlphaBlending;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Material::BindForRendering ()
		{
			C_RenderDevice* device = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice();

			m_PreviousState = device->GetRenderState();

			if (device->IsAlphaTestEnabled() != m_AlphaTest)
				device->SetAlphaTestEnabled(m_AlphaTest);

			if (device->GetAlphaTestReference() != m_AlphaTestReference)
				device->SetAlphaTestReference(m_AlphaTestReference);

			if (device->IsAlphaBlendEnabled() != m_AlphaBlending)
				device->SetAlphaBlendEnabled(m_AlphaBlending);

			if (device->GetBlendOperation() != m_BlendOperation)
				device->SetBlendOperation (m_BlendOperation);

			if (device->GetSourceBlendingMode() != m_SourceBlend)
				device->SetSourceBlendingMode (m_SourceBlend);

			if (device->GetDestinationBlendingMode () != m_DestinationBlend)
				device->SetDestinationBlendingMode (m_DestinationBlend);
		
			if (device->IsZWriteEnabled() != m_ZWrite)
				device->SetZWriteEnabled(m_ZWrite);

			m_ActiveRenderTechnique->BindForRendering();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Material::UnbindFromRendering ()
		{
			C_RenderDevice* device = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice();

			if (m_PreviousState != device->GetRenderState())
			{
				device->SetAlphaTestEnabled(m_PreviousState.AlphaTest);
				device->SetAlphaTestReference(m_PreviousState.AlphaTestReference);
				device->SetAlphaBlendEnabled(m_PreviousState.AlphaBlend);
				device->SetBlendOperation (m_PreviousState.BlendOperation);
				device->SetSourceBlendingMode (m_PreviousState.SourceBlending);
				device->SetDestinationBlendingMode (m_PreviousState.DestinationBlending);
				device->SetZWriteEnabled(m_PreviousState.ZWrite);
			}

			m_ActiveRenderTechnique->UnbindFromRendering();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Material::SetSourceBlendingMode (EBlendMode mode)
		{
			m_SourceBlend = mode;
		}

		//------------------------------------------------------------------------------------------------------------------------

		EBlendMode C_Material::GetSourceBlendingMode () const
		{
			return m_SourceBlend;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Material::SetDestinationBlendingMode (EBlendMode mode)
		{
			m_DestinationBlend = mode;
		}

		//------------------------------------------------------------------------------------------------------------------------

		EBlendMode C_Material::GetDestinationBlendingMode () const
		{
			return m_DestinationBlend;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Material::SetBlendOperation (EBlendOperation operation)
		{
			m_BlendOperation = operation;
		}

		//------------------------------------------------------------------------------------------------------------------------

		EBlendOperation C_Material::GetBlendOperation () const
		{
			return m_BlendOperation;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Material::SetAlphaTestEnabled (bool enable)
		{
			m_AlphaTest = enable;			
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_Material::GetAlphaTestEnabled () const
		{
			return m_AlphaTest;

		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Material::SetAlphaTestReference (DWORD value)
		{
			m_AlphaTestReference = value;
		}

		//------------------------------------------------------------------------------------------------------------------------

		DWORD C_Material::GetAlphaTestReference () const
		{
			return m_AlphaTestReference;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Material::SetZWriteEnable (bool enable)
		{
			m_ZWrite = enable;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_Material::GetZWriteEnable () const
		{
			return m_ZWrite;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Material::AddRenderTechnique (C_RenderTechnique* technique)
		{
			m_RenderTechniques.push_back (technique);

			// Set first technique as active
			m_ActiveRenderTechnique = m_RenderTechniques[0];
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_RenderTechnique* C_Material::GetRenderTechnique (unsigned int index)
		{
			if (index >= 0 && index < m_RenderTechniques.size())
				return m_RenderTechniques[index];
			else
				return NULL;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_RenderTechnique* C_Material::GetRenderTechnique (const std::wstring& name)
		{
			for (unsigned int i = 0; i < m_RenderTechniques.size(); ++i)
			{
				if (m_RenderTechniques[i]->GetName() == name)
					return m_RenderTechniques[i];
			}

			return NULL;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Material::SetActiveRenderTechnique (unsigned int index)
		{
			if (index >= m_RenderTechniques.size())
				return;

			m_ActiveRenderTechnique = m_RenderTechniques[index];
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Material::SetActiveRenderTechnique (C_RenderTechnique* technique)
		{
			if (technique)
				m_ActiveRenderTechnique = technique;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_RenderTechnique* C_Material::GetActiveRenderTechnique() const
		{
			return m_ActiveRenderTechnique;
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned int C_Material::GetRenderTechniquesCount() const
		{
			return m_RenderTechniques.size();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Material::Parse (FILESYSTEM::C_XMLDocument::C_XMLNode* xmlFile)
		{
			FILESYSTEM::C_XMLDocument::C_XMLNode* root = xmlFile;

			FILESYSTEM::C_XMLDocument::VectorAttributes attributes = root->GetAttributes();

			//	We check each attribute
			for (unsigned int i = 0; i < attributes.size(); i ++)
			{
				if (attributes[i].first == "Name")
				{
					m_Name.assign(attributes[i].second.begin(), attributes[i].second.end());

					continue;
				}

				if (attributes[i].first == "Transparent")
				{
					if (attributes[i].second.compare ("true") == 0 || attributes[i].second.compare ("1") == 0)
						m_AlphaBlending = true;

					if (attributes[i].second.compare ("false") == 0 || attributes[i].second.compare ("0") == 0)
						m_AlphaBlending = false;

					continue;
				}

				if (attributes[i].first == "AlphaTest")
				{
					if (attributes[i].second.compare ("true") == 0 || attributes[i].second.compare ("1") == 0)
						m_AlphaTest = true;

					if (attributes[i].second.compare ("false") == 0 || attributes[i].second.compare ("0") == 0)
						m_AlphaTest = false;

					continue;
				}

				if (attributes[i].first == "AlphaTestReference")
				{
					m_AlphaTestReference = CORE::C_Utility::ParseInt(attributes[i].second);

					continue;
				}

				if (attributes[i].first == "ZWrite")
				{
					if (attributes[i].second.compare ("true") == 0 || attributes[i].second.compare ("1") == 0)
						m_ZWrite = true;

					if (attributes[i].second.compare ("false") == 0 || attributes[i].second.compare ("0") == 0)
						m_ZWrite = false;

					continue;
				}

				if (attributes[i].first == "SourceBlend")
				{
					m_SourceBlend = static_cast<EBlendMode>(CORE::C_Utility::ParseInt(attributes[i].second));

					continue;
				}

				if (attributes[i].first == "DestinationBlend")
				{
					m_DestinationBlend = static_cast<EBlendMode>(CORE::C_Utility::ParseInt(attributes[i].second));

					continue;
				}
			}

			m_RenderTechniques.clear();

			// Parse child nodes
			FILESYSTEM::C_XMLDocument::C_XMLNodeEnumerator node = root->GetNodeEnumerator();

			while (node.Next ())
			{
				// Parse render techniques
				if (node->GetName () == "Technique")
				{
					std::string name = node->FindAttribute ("Name");

					std::wstring techniqueName;
					techniqueName.assign(name.begin(), name.end());

					C_RenderTechnique* technique = new C_RenderTechnique ();

					technique->SetMaterial(this);
					technique->SetName(techniqueName);
					technique->Parse (node.Current ());

					/// Check if there are no techniques with the same name
					std::vector <C_RenderTechnique*>::iterator iter, iter2;

					for (iter = m_RenderTechniques.begin(); iter != m_RenderTechniques.end(); ++iter)
					{
						std::wstring name = (*iter)->GetName();

						if(name == techniqueName)
							break;
					}

					if (iter == m_RenderTechniques.end())
					{
						m_RenderTechniques.push_back (technique);
					}
					else
					{
						REDEEMER_LOG<< LOG_WARNING << L"RENDER: Material: '" <<  m_Name << "' have more than one render technique with a name: " <<
							techniqueName << LOG_ENDMESSAGE;

						return;
					}
				}
			}

			if (m_RenderTechniques.empty())
			{
				C_RenderTechnique* technique = new C_RenderTechnique ();

				technique->SetName (L"DefaultTechnique");
				AddRenderTechnique(technique);
			}

			// Set first technique as active
			m_ActiveRenderTechnique = m_RenderTechniques[0];
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER

