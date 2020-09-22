/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_DirectX9Shader.h
**		DirectX 9 shader implementation
*/

//------------------------------------------------------------------------------------------------------------------------
				  
#ifndef _R_RENDER_DIRECTX9SHADER_H_
#define _R_RENDER_DIRECTX9SHADER_H_

#include "..\\R_Render_Shader.h"

#include <d3d9.h>
#include <d3dx9.h>

namespace REDEEMER
{
	namespace RENDER
	{
		/*!	\brief	DirectX 9 shader implementation
		*/
		class C_DirectX9Shader : public C_Shader
		{
		public:
			//	Constructor
			C_DirectX9Shader (ERendererShaderType type);

			//	Destructor
			virtual ~C_DirectX9Shader () ;

			/*!	Use this shader in rendering process
			*/
			virtual void BindForRendering ();

			/*!	Remove shader from rendering pipeline
			*/
			virtual void UnbindFromRendering ();

			/*!	Create shader from data
			*/
			virtual bool CreateFromData (const char* shaderData, unsigned int length, const char* mainFunction);

			/*!	Gets shader constant
			*/
			virtual char* GetConstant (char* name);

			/*!	Sets variable data
			*/
			virtual void SetData (const char* handleName, float* data, unsigned int size);

			/*!	Sets variable data
			*/
			virtual void SetData (const char* handleName, float data);

			/*!	Sets variable data
			*/
			virtual void SetData (const char* handleName, int data);

			/*!	Sets variable data
			*/
			virtual void SetData (const char* handleName, bool data);

		private:
			IDirect3DVertexShader9* m_VertexShader;					///<	Vertex Shader
			ID3DXConstantTable    *	m_VertexShaderConstTable;		///<	Vertex Shader Constant Table
			IDirect3DPixelShader9 * m_PixelShader;					///<	Pixel Shader
			ID3DXConstantTable    *	m_PixelShaderConstTable;		///<	Pixel Shader Constant Table
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_DIRECTX9SHADER_H_		 
