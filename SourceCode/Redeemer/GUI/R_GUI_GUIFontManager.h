/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_GUI_GUIFontManager.h
**		Font manager holds a collection of texts used for rendering
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_GUI_GUIFONTMANAGER_H_
#define _R_GUI_GUIFONTMANAGER_H_

#include "..\\Core\\R_Core_BaseClass.h"

#include <string>
#include <map>

namespace REDEEMER
{
	namespace FILESYSTEM
	{
		class IDataFile;
	}

	namespace RENDER
	{
		class C_RenderTechnique;
	}

	namespace GUI
	{
		class C_GUIFont;

		/*!
		**	\brief Font manager holds a collection of texts used for rendering
		*/
		class C_GUIFontManager : public CORE::C_BaseClass
		{
		public:
			/*!	Constructor
			*/
			C_GUIFontManager ();

			/*!	Destructor
			*/
			virtual ~C_GUIFontManager ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize ();

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Creates new font from file
			*/
			C_GUIFont* CreateFontDefinition (const std::wstring& fontName, FILESYSTEM::IDataFile* fontFile, unsigned int maxCharacters, bool isRenderable = true);

			/*!	Adds newly created font to the collection
			*/
			void AddFont (const std::wstring& name, C_GUIFont* font);

			/*!	Gets font with given name
			*/
			C_GUIFont* GetFont (const std::wstring& name);

			/*!	Removes font from the manager
			*/
			void RemoveFont (const std::wstring& name);

			/*!	Removes font from the manager
			*/
			void RemoveFont (C_GUIFont* font);

			/*!	Returns fonts count
			*/
			unsigned int GetFontsCount () const;

			/*!	Renders all contained fonts to the screen
			*/
			void RenderFonts ();

			/*!	Sets material (shaders)technique  used for rendering fonts
			*/
			void SetFontMaterialTechnique (RENDER::C_RenderTechnique* technique);

			/*!	Gets material (shaders) technique used for rendering fonts
			*/
			RENDER::C_RenderTechnique* GetFontMaterialTechnique ();

		private:
			std::map<std::wstring, C_GUIFont*>			m_Fonts;		///<	A collection of fonts
			RENDER::C_RenderTechnique*					m_Technique;	///<	Technique used for rendering fonts
		};

	}	//	namespace GUI
}	//	namespace REDEEMER

#endif	//	_R_GUI_GUIFONTMANAGER_H_
