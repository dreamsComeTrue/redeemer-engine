/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_IndexBuffer.h
**		Generic implementation of Index Buffer - an ordered collection of vertices' indexes
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_RENDER_INDEXBUFFER_H_
#define _R_RENDER_INDEXBUFFER_H_

namespace REDEEMER
{
	namespace RENDER
	{	
		/*!
		**	\brief Generic implementation of Index Buffer - an ordered collection of vertices' indexes
		*/
		class C_IndexBuffer
		{
		public:
			/*!	Virtual destructor
			*/
			virtual ~C_IndexBuffer ();

			/*!	Lock the IB (for writing operations), returns true on success
			*/
			virtual bool Lock (unsigned int count, void** indices = 0) = 0;

			/*!	Unlock the IB, returns true on success
			*/
			virtual bool Unlock () = 0;

			/*!	Check, if VB is locked, returns true on success
			*/
			virtual bool IsLocked () = 0;

			/*!	Use given IB for rendering process
			*/
			virtual bool BindForRendering () = 0;

			/*!	Fill in index data
			*/
			virtual void SetIndexData (void* data, unsigned int count) = 0;

			/*!	Return an array of indices in the buffer
			*/
			virtual void GetIndexData (void* data, unsigned int count) = 0;

			/*!	Returns indices count
			*/
			virtual long GetIndicesCount () const = 0;

			/*!	Returns indices size
			*/
			virtual unsigned int GetIndexSize () const = 0;
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_INDEXBUFFER_H_
