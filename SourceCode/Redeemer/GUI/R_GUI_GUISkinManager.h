/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_GUI_GUISkinManager.h
**		Skin manager holds a collection of skins used in drawing GUI controls.
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_GUI_GUISKINMANAGER_H_
#define _R_GUI_GUISKINMANAGER_H_

#include "..\\Core\\R_Core_BaseClass.h"

#include <string>
#include <map>

namespace REDEEMER
{
	namespace FILESYSTEM
	{
		class C_XMLDocument;
	}

	namespace GUI
	{
		class C_GUISkin;

		/*!
		**	\brief Skin manager holds a collection of skins used in drawing GUI controls.
		*/
		class C_GUISkinManager : public CORE::C_BaseClass
		{
		public:
			/*!	Constructor
			*/
			C_GUISkinManager ();

			/*!	Destructor
			*/
			virtual ~C_GUISkinManager ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize ();

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Creates new skin from XML definition
			*/
			C_GUISkin* CreateSkinDefinition (const std::wstring& name, FILESYSTEM::C_XMLDocument* xmlFile);

			/*!	Adds newly created skin to the collection
			*/
			void AddSkinDefintion (const std::wstring& name, C_GUISkin* skin);

			/*!	Gets skin with given name
			*/
			C_GUISkin* GetSkinDefinition (const std::wstring& name);

			/*!	Removes skin from the manager
			*/
			void RemoveSkinDefintion (const std::wstring& name);

			/*!	Removes skin from the manager
			*/
			void RemoveSkinDefinition (C_GUISkin* skin);

			/*!	Returns skins count
			*/
			unsigned int GetSkinDefinitionsCount () const;

		private:
			std::map<std::wstring, C_GUISkin*>			m_Skins;	///<	A collection of skins
		};

	}	//	namespace GUI
}	//	namespace REDEEMER

#endif	//	_R_GUI_GUISKINMANAGER_H_
