/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_GUI_GUISkin.cpp
**		Skin contains definitions for areas which are used to get controls coordinate when binding material for rendering
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_GUI_GUISkin.h"
#include "..\\Core\\R_Core_Utility.h"
#include "..\\Core\\R_Core_Convert.h"
#include "..\\FileSystem\\R_FileSystem_XMLDocument.h"

#include <vector>

namespace REDEEMER
{
	namespace GUI
	{
		//------------------------------------------------------------------------------------------------------------------------
		S_ControlSkinInfo::S_ControlSkinInfo ()
		{
			States.clear();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void S_ControlSkinInfo::AddState (const std::wstring& name, CORE::C_Rectangle<int> boundingRectangle)
		{
			States[name] = boundingRectangle;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void S_ControlSkinInfo::Clear ()
		{
			States.clear();
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_GUISkin::C_GUISkin (const std::wstring& name) :
			m_Name (name),
			m_MaterialName (L"")
		{
			m_ControlsInfo.clear();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_GUISkin::~C_GUISkin ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_GUISkin::Initialize ( FILESYSTEM::C_XMLDocument* xmlFile)
		{
			C_BaseClass::Initialize();

			//	Try to read XML file with controls definition
			FILESYSTEM::C_XMLDocument::C_XMLNode* root = xmlFile->GetRootNode();

			FILESYSTEM::C_XMLDocument::VectorAttributes attributes = root->GetAttributes();

			for (unsigned int i = 0; i < attributes.size(); i ++)
			{
				if (attributes[i].first == "Name")
				{
					m_Name.assign(attributes[i].second.begin(), attributes[i].second.end());

					continue;
				}

				if (attributes[i].first == "Material")
				{
					m_MaterialName.assign(attributes[i].second.begin(), attributes[i].second.end());

					continue;
				}
			}

			// Parse child nodes
			FILESYSTEM::C_XMLDocument::C_XMLNodeEnumerator node = root->GetNodeEnumerator();

			while (node.Next ())
			{
				std::string controlNameStr;
				std::wstring controlName;
				S_ControlSkinInfo states;

				// Parse controls definitions
				if (node->GetName () == "Control")
				{
					controlNameStr = node->FindAttribute ("Type");

					CORE::C_Convert::ConvertCharsToUnicode(&controlName, controlNameStr.c_str());

					for (unsigned int i = 0; i < GUICT_Count; ++ i)
					{
						if (controlName == GUIControlTypeNames[i])
						{
							FILESYSTEM::C_XMLDocument::C_XMLNodeEnumerator controlNode = node->GetNodeEnumerator();
							std::string partNameStr;
							std::wstring partName;

							if (controlNode->GetName() == "Part")
							{
								partNameStr = controlNode->FindAttribute("Name");

								CORE::C_Convert::ConvertCharsToUnicode(&partName, partNameStr.c_str());

								states.Clear();

								FILESYSTEM::C_XMLDocument::C_XMLNodeEnumerator partNode = controlNode->GetNodeEnumerator();

								while (partNode.Next ())
								{						 
									if (partNode->GetName () == "State")
									{
										std::wstring stateName;
										CORE::C_Rectangle<int> stateOffset;
										bool ok1 = false, ok2 = false;
										FILESYSTEM::C_XMLDocument::VectorAttributes stateAttributes = partNode->GetAttributes();

										for (unsigned int i = 0; i < attributes.size(); i ++)
										{
											if (stateAttributes[i].first == "Name")
											{
												stateName.assign(stateAttributes[i].second.begin(), stateAttributes[i].second.end());

												ok1 = true;

												continue;
											}

											if (stateAttributes[i].first == "Offset")
											{
												std::string offsetString = stateAttributes[i].second;

												std::vector<std::string> offsetParts = CORE::C_Utility::TokenizeString(const_cast<char*>(offsetString.c_str()), " ");

												if (offsetParts.size() != 4)
													continue;

												stateOffset.UpperLeftCorner.X = CORE::C_Utility::ParseInt(offsetParts[0]);
												stateOffset.UpperLeftCorner.Y =	CORE::C_Utility::ParseInt(offsetParts[1]);
												stateOffset.LowerRightCorner.X = CORE::C_Utility::ParseInt(offsetParts[2]);
												stateOffset.LowerRightCorner.Y = CORE::C_Utility::ParseInt(offsetParts[3]);

												ok2 = true;
											}
										}

										if ((ok1 == true) && (ok2 == true))
										{
											//	We have name and offset, we can add to the collection
											states.AddState(stateName, stateOffset);
										}	
									}		
								}
								std::map <std::wstring, S_ControlSkinInfo> tmpPartInfo;

								tmpPartInfo[partName] = states;

								m_ControlsInfo[controlName] = tmpPartInfo;
							}
						}
					}
				}
			}

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_GUISkin::Finalize ()
		{
			m_ControlsInfo.clear();

			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		const std::wstring C_GUISkin::GetName () const
		{
			return m_Name;
		}

		//------------------------------------------------------------------------------------------------------------------------

		const std::wstring C_GUISkin::GetMaterialName () const
		{
			return m_MaterialName;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::map <std::wstring, S_ControlSkinInfo> C_GUISkin::GetControlSkinInfo (EGUIControlType controlType)
		{
			return m_ControlsInfo[GUIControlTypeNames[controlType]];
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace GUI
}	//	namespace REDEEMER
