/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_ShaderEntity.h
**		Shader entity holds an implementation of shaders and variables belonging to it
*/

//------------------------------------------------------------------------------------------------------------------------
				  
#ifndef _R_RENDER_SHADERENTITY_H_
#define _R_RENDER_SHADERENTITY_H_

#include "..\\FileSystem\\R_FileSystem_XMLDocument.h"
#include "R_Render_ShaderVariable.h"

#include <map>

namespace REDEEMER
{
	namespace RENDER
	{
		class C_RenderPass;
		class C_Shader;
		class C_ShaderVariable;

		/*!	\brief	Shader entity holds an implementation of shaders and variables belonging to it
		*/
		class C_ShaderEntity
		{
			friend class C_RenderPass;

		public:
			/*!	Constructor
			*/
			C_ShaderEntity ();

			/*!	Copy constructor
			*/
			C_ShaderEntity (const C_ShaderEntity& entity);

			/*!	Destructor
			*/
			virtual ~C_ShaderEntity () ;

			/*!	Use this shader in rendering process
			*/
			void BindForRendering ();

			/*!	Remove shader from rendering pipeline
			*/
			void UnbindFromRendering ();

			/*!	Sets shader belonging to this entity
			*/
			void SetShader (C_Shader* shader);

			/*!	Returns underlying shader instance
			*/
			C_Shader* GetShader ();

			/*!	Create shader variable
			*/
			C_ShaderVariable* CreateVariable (char* handle, EShaderUpdateFunction function = SUF_None);

			/*!	Sets shader variable
			*/
			bool AddVariable (std::wstring name, C_ShaderVariable* variable);

			/*!	Updates shader variables
			*/
			void UpdateVariables ();

		private:
			/*!	Parses XML data describing shader instance
			*/
			void Parse (FILESYSTEM::C_XMLDocument::C_XMLNode* xmlFile);

		private:
			C_Shader*										m_Shader;
			std::map<std::wstring, C_ShaderVariable*>		m_Variables;
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_SHADERENTITY_H_		 
