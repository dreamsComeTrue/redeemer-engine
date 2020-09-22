/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_GUI_GUISkin.h
**		Skin contains definitions for areas which are used to get controls coordinate when binding material for rendering
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_GUI_GUISKIN_H_
#define _R_GUI_GUISKIN_H_

#include "R_GUI_GUIControl.h"
#include "..\\Core\\Math\\R_Core_Rectangle.h"
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
		/*
			Description for XML Skin file

			<GUISkin Name="GuiSkinName" Material="NameOfOptionalMaterial">
				<Control Type="Button">
					<Part Name="GlyphIcon">
						<State Name="Normal" Offset="0 0 12 24"/>
						<State Name="Disabled" Offset="200 220 220 240"/>
						<State Name="Highlighted" Offset="20 30 50 50"/>
						<State Name="Pushed" Offset="70 90 100 120"/>
					</Part>
						...
				</Control>

				<Control Type="CheckBox">
					...
				</Control>
			</GuiSkin>
		*/

		/*!	Description of all states for control
		*/
		struct S_ControlSkinInfo
		{
			/*!	Constructor
			*/
			S_ControlSkinInfo ();

			/*!	Adds new state
			*/
			void AddState (const std::wstring& name, CORE::C_Rectangle<int> boundingRectangle);

			/*!	Clears all states
			*/
			void Clear ();
			   
		public:
			std::map<std::wstring, CORE::C_Rectangle<int> >	States;	///<	States descriptions as pairs: name - texture bounding rectangle 
		};


		/*!
		**	\brief Skin contains definitions for areas which are used to get controls coordinate when binding material for rendering
		*/
		class C_GUISkin : public CORE::C_BaseClass
		{
		public:
			/*!	Constructor
			*/
			C_GUISkin (const std::wstring& name);

			/*!	Destructor
			*/
			virtual ~C_GUISkin ();

			/*!	\brief	Initialization code - it creates new skin from XML definition
			*/
			virtual int Initialize ( FILESYSTEM::C_XMLDocument* xmlFile);

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Returns skin name
			*/
			const std::wstring GetName () const;

			/*!	Returns material name
			*/
			const std::wstring GetMaterialName () const;

			/*!	Returns coordinates on texture image specified for control with given name
			*/
			std::map <std::wstring, S_ControlSkinInfo> GetControlSkinInfo (EGUIControlType controlType);

		private:
			std::map<std::wstring, std::map <std::wstring, S_ControlSkinInfo> >		m_ControlsInfo;		///<	Information about controls' bounding rectangles
			std::wstring															m_Name;				///<	Skin name
			std::wstring															m_MaterialName;		///<	Associated material's name
		};

	}	//	namespace GUI
}	//	namespace REDEEMER

#endif	//	_R_GUI_GUISKIN_H_
