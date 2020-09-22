/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_VertexBuffer.h
**		Generic implementation of Vertex Buffer - a source of vertex data that is sent to the GPU.
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_RENDER_VERTEXBUFFER_H_
#define _R_RENDER_VERTEXBUFFER_H_

#include "R_Render_RenderDevice.h"
#include "R_Render_IndexBuffer.h"

namespace REDEEMER
{
	namespace RENDER
	{	
		/*!
		**	\brief Generic implementation of Vertex Buffer - a source of vertex data that is sent to the GPU.
		*/
		class C_VertexBuffer
		{
		public:
			/*!	Constructor
			*/
			C_VertexBuffer ();

			/*!	Virtual destructor
			*/
			virtual ~C_VertexBuffer ();

			/*!	Lock the VB (for writing operations), returns true on success
			*/
			virtual bool Lock (unsigned int count = 0, void** vertices = NULL) = 0;

			/*!	Unlock the VB, returns true on success
			*/
			virtual bool Unlock () = 0;

			/*!	Check, if VB is locked, returns true on success
			*/
			virtual bool IsLocked () = 0;

			/*!	Use given VB for rendering process
			*/
			virtual bool BindForRendering () = 0;

			/*!	Fill in vertex data
			*/
			virtual void SetVertexData (void* data, unsigned int count) = 0;

			/*!	Return an array of vertices in the buffer
			*/
			virtual void GetVertexData (void* data, unsigned int count) = 0;

			/*!	Sets primitive count
			*/
			void SetPrimitivesCount (unsigned int count);

			/*!	Return number of primitives
			*/
			unsigned int GetPrimitivesCount () const;

			/*!	Return number of vertices
			*/
			long GetVerticesCount () const;

			/*!	Sets new primitive type
			*/
			void SetPrimitiveType (ERendererPrimitiveType type);

			/*!	Returns primitive type
			*/
			ERendererPrimitiveType GetPrimitiveType () const;

			/*!	Returns internal index buffer
			*/
			C_IndexBuffer* GetIndexBuffer ();

			/*!	Attaches new Index Buffer
			*/
			void AttachIndexBuffer (unsigned long indicesCount, DWORD indexSize = 2, bool isDynamic = false);

		protected:
			unsigned int				m_PrimitivesCount;	///<	Number of primitives
			unsigned long				m_VerticesCount;	///<	Number of vertices
			ERendererPrimitiveType		m_PrimitiveType;	///<	Type of primitive to render
			C_IndexBuffer*				m_IndexBuffer;		///<	VB's have an Index Buffers attached too
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_VERTEXBUFFER_H_
