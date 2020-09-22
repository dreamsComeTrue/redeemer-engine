/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_DirectX9VertexBuffer.h
**		DirectX 9 implementation of Vertex Buffer - a source of vertex data that is sent to the GPU.
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_RENDER_DIRECTX9VERTEXBUFFER_H_
#define _R_RENDER_DIRECTX9VERTEXBUFFER_H_

#include "..\\R_Render_VertexBuffer.h"

#include <d3d9.h>

namespace REDEEMER
{
	namespace RENDER
	{	
		/*!
		**	\brief DirectX 9 implementation of Vertex Buffer - a source of vertex data that is sent to the GPU.
		*/
		class C_DirectX9VertexBuffer : public C_VertexBuffer
		{
		public:
			/*!	Constructor
			*/							
			C_DirectX9VertexBuffer (unsigned long verticesCount, ERendererFVFFormat vertexFormat, bool isDynamic);

			/*!	Virtual destructor
			*/
			virtual ~C_DirectX9VertexBuffer ();

			/*!	Lock the VB (for writing operations), returns true on success
			*/
			virtual bool Lock (unsigned int count = 0, void** vertices = NULL);

			/*!	Unlock the VB, returns true on success
			*/
			virtual bool Unlock ();

			/*!	Check, if VB is locked, returns true on success
			*/
			virtual bool IsLocked ();

			/*!	Use given VB for rendering process
			*/
			virtual bool BindForRendering ();

			/*!	Fill in vertex data
			*/
			virtual void SetVertexData (void* data, unsigned int count);

			/*!	Return an array of vertices in the buffer
			*/
			virtual void GetVertexData (void* data, unsigned int count);

		private:
			LPDIRECT3DVERTEXBUFFER9 m_D3DVertexBuffer; ///<		Direct3D vertex buffer
			LPDIRECT3DDEVICE9       m_D3DDevice;       ///<		Direct3D device
			unsigned long			m_FVFSize;
			ERendererFVFFormat		m_VertexFormat;

			bool					m_IsLocked;
			bool					m_IsDynamic;
			void*					m_Data;				///<	Pointer to data
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_DIRECTX9VERTEXBUFFER_H_
