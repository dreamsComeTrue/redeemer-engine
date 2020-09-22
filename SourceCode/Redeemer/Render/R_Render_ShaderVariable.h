/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_ShaderVariable.h
**		 Variables are used for communication between shader and engine
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_RENDER_SHADERVARIABLE_H_
#define _R_RENDER_SHADERVARIABLE_H_

#include "..\\FileSystem\\R_FileSystem_XMLDocument.h"

namespace REDEEMER
{
	namespace RENDER
	{
		class C_ShaderEntity;

		/*!	Valid update functions
		*/
		enum EShaderUpdateFunction
		{
			SUF_None,
			SUF_World,
			SUF_View,
			SUF_Projection,
			SUF_WorldView,
			SUF_WorldViewInv,
			SUF_WorldViewProjection,
			SUF_ViewInverseTranspose,
			SUF_EyePosition,
			SUF_CameraView
		};

		/*!	\brief Variables are used for communication between shader and engine
		*/
		class C_ShaderVariable
		{
			friend class C_ShaderEntity;

		public:
			/*!	Constructor
			*/
			C_ShaderVariable ();

			/*!	Copy constructor
			*/
			C_ShaderVariable (const C_ShaderVariable& variable);

			/*!	Constructor
			*/
			C_ShaderVariable (char* handle, EShaderUpdateFunction function = SUF_None);

			/*!	Destructor
			*/
			virtual ~C_ShaderVariable ();

			/*!	Sets shader data
			*/
			void SetData (float* values, unsigned int count);

			/*!	Gets handle
			*/
			const char* GetHandle () const;

			/*!	Gets values
			*/
			float* GetValues ();

			/*!	Gets count
			*/
			unsigned int GetCount ();

			/*!	Updates variables (if function is active)
			*/
			void UpdateValues ();

		private:
			/*!	Parses XML data describing technique
			*/
			void Parse (FILESYSTEM::C_XMLDocument::C_XMLNode* xmlFile);

			/*!	Returns shader update function based on its name
			*/
			EShaderUpdateFunction GetShaderUpdateFunction (std::string function);

		private:
			char*						m_Handle;
			float*						m_Values;
			unsigned int				m_Count;
			EShaderUpdateFunction		m_Function;
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_SHADERVARIABLE_H_
