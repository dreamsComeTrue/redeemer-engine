/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_GUI_GUISkinManager.cpp
**		Skin manager holds a collection of skins used in drawing GUI controls.
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_GUI_GUISkinManager.h"
#include "R_GUI_GUISkin.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace GUI
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_GUISkinManager::C_GUISkinManager ()
		{
			m_Skins.clear();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUISkinManager::~C_GUISkinManager ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_GUISkinManager::Initialize ()
		{
			C_BaseClass::Initialize();

			m_Skins.clear();

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_GUISkinManager::Finalize ()
		{
			for (std::map<std::wstring, C_GUISkin*>::iterator it = m_Skins.begin(); it != m_Skins.end(); ++ it)
			{
				it->second->Finalize();

				REDEEMER_SAFE_DELETE (it->second);
			}

			m_Skins.clear();

			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUISkin* C_GUISkinManager::CreateSkinDefinition (const std::wstring& name, FILESYSTEM::C_XMLDocument* xmlFile)
		{
			C_GUISkin* skin = new C_GUISkin (name);	 

			if (skin != NULL)
			{
				if (skin->Initialize (xmlFile) != RESULT_OK)
				{
					REDEEMER_SAFE_DELETE (skin);

					REDEEMER_LOG << LOG_ERROR << L"C_GUISkinManager: Can not create skin definition: '" << name << "'." << LOG_ENDMESSAGE;

					return NULL;
				}

				m_Skins[name] = skin;
			}

			return skin;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUISkinManager::AddSkinDefintion (const std::wstring& name, C_GUISkin* skin)
		{
			// The material already exists, so return
			if (m_Skins.find (name) != m_Skins.end())
				return;

			m_Skins [name] = skin;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUISkin* C_GUISkinManager::GetSkinDefinition (const std::wstring& name)
		{
			if (m_Skins.find(name) == m_Skins.end())
				return NULL;

			return m_Skins[name];
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUISkinManager::RemoveSkinDefintion (const std::wstring& name)
		{
			for (std::map<std::wstring, C_GUISkin*>::iterator it = m_Skins.begin(); it != m_Skins.end(); ++ it)
			{
				if (it->first == name)
				{
					it->second->Finalize();

					REDEEMER_SAFE_DELETE (it->second);
					m_Skins.erase (name);

					break;
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUISkinManager::RemoveSkinDefinition (C_GUISkin* skin)
		{
			for (std::map<std::wstring, C_GUISkin*>::iterator it = m_Skins.begin(); it != m_Skins.end(); ++ it)
			{
				if (it->second == skin)
				{
					it->second->Finalize();
					REDEEMER_SAFE_DELETE (it->second);
					m_Skins.erase (it);

					break;
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned int C_GUISkinManager::GetSkinDefinitionsCount () const
		{
			 return m_Skins.size();
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace GUI
}	//	namespace REDEEMER
