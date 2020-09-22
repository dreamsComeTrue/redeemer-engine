/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_GUI_GUIFontManager.cpp
**		Font manager holds a collection of texts used for rendering
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_GUI_GUIFontManager.h"
#include "R_GUI_GUIFont.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace GUI
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_GUIFontManager::C_GUIFontManager ()  :
			m_Technique (NULL)
		{
			m_Fonts.clear();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUIFontManager::~C_GUIFontManager ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_GUIFontManager::Initialize ()
		{
			C_BaseClass::Initialize();

			m_Fonts.clear();

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_GUIFontManager::Finalize ()
		{
			for (std::map<std::wstring, C_GUIFont*>::iterator it = m_Fonts.begin(); it != m_Fonts.end(); ++ it)
			{
				REDEEMER_SAFE_DELETE (it->second);
			}

			m_Fonts.clear();

			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUIFont* C_GUIFontManager::CreateFontDefinition (const std::wstring& fontName, FILESYSTEM::IDataFile* fontFile, unsigned int maxCharacters, bool isRenderable)
		{
			C_GUIFont* font = new C_GUIFont (maxCharacters);	 

			if (font != NULL)
			{
				if (font->LoadFromFile(fontFile) != true)
				{
					REDEEMER_SAFE_DELETE (font);

					REDEEMER_LOG << LOG_ERROR << L"C_GUIFontManager: Can not create font: '" << fontFile->GetFileName() << "'." << LOG_ENDMESSAGE;

					return NULL;
				}

				if (!isRenderable)
					font->m_IsRenderable = false;

				m_Fonts[fontName] = font;
			}

			return font;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIFontManager::AddFont (const std::wstring& name, C_GUIFont* font)
		{
			// The material already exists, so return
			if (m_Fonts.find (name) != m_Fonts.end())
				return;

			m_Fonts [name] = font;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUIFont* C_GUIFontManager::GetFont (const std::wstring& name)
		{
			if (m_Fonts.find(name) == m_Fonts.end())
				return NULL;

			return m_Fonts[name];
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIFontManager::RemoveFont (const std::wstring& name)
		{
			for (std::map<std::wstring, C_GUIFont*>::iterator it = m_Fonts.begin(); it != m_Fonts.end(); ++ it)
			{
				if (it->first == name)
				{
					REDEEMER_SAFE_DELETE (it->second);
					m_Fonts.erase (name);

					break;
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIFontManager::RemoveFont (C_GUIFont* font)
		{
			for (std::map<std::wstring, C_GUIFont*>::iterator it = m_Fonts.begin(); it != m_Fonts.end(); ++ it)
			{
				if (it->second == font)
				{
					REDEEMER_SAFE_DELETE (it->second);
					m_Fonts.erase (it);

					break;
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned int C_GUIFontManager::GetFontsCount () const
		{
			return m_Fonts.size();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIFontManager::RenderFonts ()
		{
			for (std::map<std::wstring, C_GUIFont*>::iterator it = m_Fonts.begin(); it != m_Fonts.end(); ++ it)
				if ((*it).second->m_IsRenderable)
					(*it).second->Render();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_GUIFontManager::SetFontMaterialTechnique (RENDER::C_RenderTechnique* technique)
		{
			m_Technique = technique;
		}

		//------------------------------------------------------------------------------------------------------------------------

		RENDER::C_RenderTechnique* C_GUIFontManager::GetFontMaterialTechnique ()
		{
			return m_Technique;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace GUI
}	//	namespace REDEEMER
