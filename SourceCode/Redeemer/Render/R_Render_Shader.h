/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_Shader.h
**		Shaders control how object looks by doing some computation on GPU
*/

//------------------------------------------------------------------------------------------------------------------------
				  
#ifndef _R_RENDER_SHADER_H_
#define _R_RENDER_SHADER_H_

#include "..\\FileSystem\\R_FileSystem_XMLDocument.h"

#include <map>

namespace REDEEMER
{
	namespace RENDER
	{
		/*!	Valid shader types
		*/
		enum ERendererShaderType
		{
			RST_PixelShader,
			RST_VertexShader
		};

		/*!	\brief	Shaders control how object looks by doing some computation on GPU
		*/
		class C_Shader
		{
		public:
			/*!	Constructor
			*/
			C_Shader (ERendererShaderType type);

			/*!	Destructor
			*/
			virtual ~C_Shader () ;

			/*!	Returns shader type
			*/
			ERendererShaderType GetType ();

			/*!	Use this shader in rendering process
			*/
			virtual void BindForRendering () = 0;

			/*!	Remove shader from rendering pipeline
			*/
			virtual void UnbindFromRendering () = 0;

			/*!	Create shader from data
			*/
			virtual bool CreateFromData (const char* shaderData, unsigned int length, const char* mainFunction) = 0;

			/*!	Gets shader constant
			*/
			virtual char* GetConstant (char* name) = 0;

			/*!	Sets variable data
			*/
			virtual void SetData (const char* handleName, float* data, unsigned int size) = 0;

			/*!	Sets variable data
			*/
			virtual void SetData (const char* handleName, float data) = 0;

			/*!	Sets variable data
			*/
			virtual void SetData (const char* handleName, int data) = 0;

			/*!	Sets variable data
			*/
			virtual void SetData (const char* handleName, bool data) = 0;

		protected:
			ERendererShaderType								m_Type;
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_SHADER_H_		 
