/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Physics_PhysXAllcator.h
**		Custom memory allocator for PhysX
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_PHYSICS_PHYSXALLOCATOR_H_
#define _R_PHYSICS_PHYSXALLOCATOR_H_

#pragma warning(push)
#pragma warning(disable:4512)
#include <NxUserAllocator.h>
#pragma warning(pop)

namespace REDEEMER
{
	namespace PHYSICS
	{
		/// Memory allocator for PhysX by Ageia Corp.
		class C_PhysXAllocator: public NxUserAllocator
		{
		public:
			C_PhysXAllocator();
			virtual ~C_PhysXAllocator();

			void reset();

			void* malloc(NxU32 size);
			void* malloc(NxU32 size, NxMemoryType type);
			void* mallocDEBUG(NxU32 size, const char* file, int line);
			void* mallocDEBUG(NxU32 size, const char* file, int line, const char* className, NxMemoryType type);
			void* realloc(void* memory, NxU32 size);
			void free(void* memory);

			NxU32* mMemBlockList;
			NxU32 mMemBlockListSize;
			NxU32 mMemBlockFirstFree;
			NxU32 mMemBlockUsed;

			NxI32 mNbAllocatedBytes;
			NxI32 mHighWaterMark;
			NxI32 mTotalNbAllocs;
			NxI32 mNbAllocs;
			NxI32 mNbReallocs;
		};

	}	//	namespace PHYSICS
}	//	namespace REDEEMER

#endif	//	_R_PHYSICS_PHYSXALLOCATOR_H_
