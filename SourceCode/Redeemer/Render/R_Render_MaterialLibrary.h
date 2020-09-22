/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_MaterialLibrary.h
**		Material library holds a collection of material managers
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_RENDER_MATERIALLIBRARY_H_
#define _R_RENDER_MATERIALLIBRARY_H_

#include "R_Render_MaterialManager.h"
#include "..\\Core\\R_Core_BaseClass.h"

#include <string>
#include <map>

namespace REDEEMER
{
	namespace RENDER
	{
		/*!	Material library holds a collection of material managers
		*/
		class C_MaterialLibrary : public CORE::C_BaseClass
		{
		public:
			/*!	Constructor
			*/
			C_MaterialLibrary ();

			/*!	Destructor
			*/
			virtual ~C_MaterialLibrary ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize ();

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Creates new material manager
			*/
			C_MaterialManager* CreateMaterialManager (std::wstring name = L"");

			/*!	Adds newly created material manager to the collection
			*/
			void AddMaterialManager (const std::wstring& name, C_MaterialManager* materialManager);

			/*!	Gets material manager with given name
			*/
			C_MaterialManager* GetMaterialManager (const std::wstring& name);

			/*!	Removes material manager from the manager
			*/
			void RemoveMaterialManager (const std::wstring& name);

			/*!	Removes material manager from the manager
			*/
			void RemoveMaterialManager (C_MaterialManager* materialManager);

			/*!	Returns material manager count
			*/
			unsigned int GetMaterialManagersCount () const;

		private:
			std::map<std::wstring, C_MaterialManager*>			m_MaterialManagers;
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_MATERIALLIBRARY_H_
