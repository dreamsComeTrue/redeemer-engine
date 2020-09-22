/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_MaterialManager.h
**		Material Manager holds a collection of materials
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_RENDER_MATERIALMANAGER_H_
#define _R_RENDER_MATERIALMANAGER_H_

#include "..\\FileSystem\\R_FileSystem_XMLDocument.h"
#include "..\\Core\\R_Core_BaseClass.h"

#include <string>
#include <map>

namespace REDEEMER
{
	namespace RENDER
	{
		class C_Material;

		/*!	Material Manager holds a collection of materials
		*/
		class C_MaterialManager : public CORE::C_BaseClass
		{
		public:
			/*!	Constructor
			*/
			C_MaterialManager ();

			/*!	Destructor
			*/
			virtual ~C_MaterialManager ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize ();

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Creates new material
			*/
			C_Material* CreateMaterial (std::wstring name = L"");

			/*!	Adds newly created material to the collection
			*/
			void AddMaterial (const std::wstring& name, C_Material* material);

			/*!	Gets material with given name
			*/
			C_Material* GetMaterial (const std::wstring& name);

			/*!	Removes material from the manager
			*/
			void RemoveMaterial (const std::wstring& name);

			/*!	Removes material from the manager
			*/
			void RemoveMaterial (C_Material* material);

			/*!	Returns material count
			*/
			unsigned int GetMaterialsCount () const;

			/*!	Parses XML data describing technique
			*/
			void Parse (FILESYSTEM::C_XMLDocument* xmlFile);

		private:
			std::map<std::wstring, C_Material*>			m_Materials;
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_MATERIALMANAGER_H_
