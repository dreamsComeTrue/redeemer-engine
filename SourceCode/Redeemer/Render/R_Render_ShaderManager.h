/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_ShaderManager.h
**		Shader Manager holds a collection of shaders
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_RENDER_SHADERMANAGER_H_
#define _R_RENDER_SHADERMANAGER_H_

#include "R_Render_Shader.h"
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
		/*!	Shader Manager holds a collection of shaders
		*/
		class C_ShaderManager : public CORE::C_BaseClass
		{
		public:
			/*!	Constructor
			*/
			C_ShaderManager ();

			/*!	Destructor
			*/
			virtual ~C_ShaderManager ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize ();

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Creates new shader
			*/
			C_Shader* CreateShader (std::wstring name, FILESYSTEM::IDataFile* file, const char* mainFunction, ERendererShaderType type);

			/*!	Adds newly created shader to the collection
			*/
			void AddShader (const std::wstring& name, C_Shader* shader);

			/*!	Gets shader with given name
			*/
			C_Shader* GetShader (const std::wstring& name);

			/*!	Removes shader from the manager
			*/
			void RemoveShader (const std::wstring& name);

			/*!	Removes shader from the manager
			*/
			void RemoveShader (C_Shader* shader);

			/*!	Returns shader count
			*/
			unsigned int GetShadersCount () const;

		private:
			std::map<std::wstring, C_Shader*>			m_Shaders;
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_SHADERMANAGER_H_
