/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_DirectX9IndexBuffer.h
**		DirectX 9 implementation of Index Buffer - an ordered collection of vertices' indexes
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_RENDER_DIRECTX9INDEXBUFFER_H_
#define _R_RENDER_DIRECTX9INDEXBUFFER_H_

#include "..\\R_Render_IndexBuffer.h"

#include <d3d9.h>

namespace REDEEMER
{
	namespace RENDER
	{	
		/*!
		**	\brief DirectX 9 implementation of Index Buffer - an ordered collection of vertices' indexes
		*/
		class C_DirectX9IndexBuffer : public C_IndexBuffer
		{
		public:
			/*!	Constructor
			*/							
			C_DirectX9IndexBuffer (unsigned long indicesCount, DWORD indexSize, bool isDynamic);

			/*!	Virtual destructor
			*/
			virtual ~C_DirectX9IndexBuffer ();
			
			/*!	Lock the IB (for writing operations), returns true on success
			*/
			virtual bool Lock (unsigned int count, void** indices = 0);

			/*!	Unlock the IB, returns true on success
			*/
			virtual bool Unlock ();

			/*!	Check, if VB is locked, returns true on success
			*/
			virtual bool IsLocked ();

			/*!	Use given IB for rendering process
			*/
			virtual bool BindForRendering ();

			/*!	Fill in index data
			*/
			virtual void SetIndexData (void* data, unsigned int count);

			/*!	Return an array of indices in the buffer
			*/
			virtual void GetIndexData (void* data, unsigned int count);

			/*!	Returns indices count
			*/
			virtual long GetIndicesCount () const;

			/*!	Returns indices size
			*/
			virtual unsigned int GetIndexSize () const;

		private:
			IDirect3DIndexBuffer9*	m_D3DIndexBuffer;		///<	Direct3D index buffer
			unsigned int			m_Size;					///<	Size of a single index item. Typically it is 16-bit or 32-bit unsigned integer 
															///		meaning it is 2 or 4 bytes in size.
			bool					m_IsLocked;
			bool					m_IsDynamic;
			void*					m_Data;					///<	Pointer to data
			unsigned long			m_IndicesCount;			///<	Number of indices
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_DIRECTX9INDEXBUFFER_H_
