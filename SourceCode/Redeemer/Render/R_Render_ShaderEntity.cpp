/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_ShaderEntity.cpp
**		Shader entity holds an implementation of shaders and variables belonging to it
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_ShaderEntity.h"
#include "R_Render_Shader.h"
#include "..\\Core\\R_Core_Convert.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_ShaderEntity::C_ShaderEntity ()
		{
			m_Variables.clear();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_ShaderEntity::C_ShaderEntity (const C_ShaderEntity& entity)
		{
			m_Shader = entity.m_Shader;

			if (!entity.m_Variables.empty())
			{
				for (std::map<std::wstring, C_ShaderVariable*>::const_iterator it = entity.m_Variables.begin(); it != entity.m_Variables.end(); ++it)
				{			
					C_ShaderVariable* variable = new C_ShaderVariable (*(it->second));

					std::wstring tmp;
					CORE::C_Convert::ConvertCharsToUnicode (&tmp, variable->GetHandle());

					AddVariable(tmp, variable);
				}												 
			}	
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_ShaderEntity::~C_ShaderEntity ()
		{
			if (!m_Variables.empty())
			{
				for (std::map <std::wstring, C_ShaderVariable*>::iterator it = m_Variables.begin(); it != m_Variables.end(); ++it)
					REDEEMER_SAFE_DELETE ((*it).second);

				m_Variables.clear();
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_ShaderEntity::BindForRendering ()
		{
			if (m_Shader)
				m_Shader->BindForRendering();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_ShaderEntity::UnbindFromRendering ()
		{
			if (m_Shader)
				m_Shader->UnbindFromRendering();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_ShaderEntity::SetShader (C_Shader* shader)
		{
			m_Shader = shader;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Shader* C_ShaderEntity::GetShader ()
		{
			return m_Shader;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_ShaderVariable* C_ShaderEntity::CreateVariable (char* handle, EShaderUpdateFunction function)
		{
			return new C_ShaderVariable (handle, function);
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_ShaderEntity::AddVariable (std::wstring name, C_ShaderVariable* variable)
		{
			// The variables already exists, so return
			if (m_Variables.find (name) != m_Variables.end())
				return false;

			m_Variables[name] = variable;

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_ShaderEntity::UpdateVariables ()
		{
			for (std::map<std::wstring, C_ShaderVariable*>::iterator it = m_Variables.begin(); it != m_Variables.end (); it ++)
			{
				(*it).second->UpdateValues ();

				m_Shader->SetData ((*it).second->GetHandle (), (*it).second->GetValues (), (*it).second->GetCount ());
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_ShaderEntity::Parse (FILESYSTEM::C_XMLDocument::C_XMLNode* xmlFile)
		{
			// Parse child nodes
			FILESYSTEM::C_XMLDocument::C_XMLNodeEnumerator node = xmlFile->GetNodeEnumerator();

			while (node.Next ())
			{
				// Set constants
				if (node->GetName() == "Constant")
				{
					C_ShaderVariable* constant = new C_ShaderVariable ();

					constant->Parse (node.Current ());

					std::wstring tmp;
					CORE::C_Convert::ConvertCharsToUnicode (&tmp, constant->GetHandle());

					AddVariable (tmp, constant);
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER

