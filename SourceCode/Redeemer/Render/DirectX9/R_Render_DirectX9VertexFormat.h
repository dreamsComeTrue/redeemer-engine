/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_DirectX9VertexFormat.h
**		DirectX 9 FVF implementation
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_RENDER_DIRECTX9VERTEXFORMAT_H_
#define _R_RENDER_DIRECTX9VERTEXFORMAT_H_

#include "..\\R_Render_VertexFormat.h"

#include <d3d9.h>

namespace REDEEMER
{
	namespace RENDER
	{	
		/*!
		**	\brief DirectX 9 FVF implementation
		*/
		class C_DirectX9VertexFormat : public C_VertexFormat
		{
		public:
			/*!	Constructor
			*/
			C_DirectX9VertexFormat ();

			/*!	Destructor
			*/
			virtual ~C_DirectX9VertexFormat ();

			/*!	Adds new description to the format
			*/
			virtual void AddVertexFormatDescription (S_VertexFormatDescription& description);

			/*!	Adds variable size descriptions to the format
			*/
			virtual void AddVertexFormatDescriptions (S_VertexFormatDescription* descriptions, unsigned int count);

			/*!	Uses this format for rendering purposes
			*/
			virtual void BindForRendering ();

		private:
			/*!	Internal use
			*/
			void CreateFormatDeclaration ();

			IDirect3DVertexDeclaration9*	m_VertexDeclaration;	///<	DirectX Vertex Declaration
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_DIRECTX9VERTEXFORMAT_H_
