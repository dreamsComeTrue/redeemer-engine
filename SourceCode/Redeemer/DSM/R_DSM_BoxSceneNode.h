/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_DSM_BoxSceneNode.h
**		Represents standard cube
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_DSM_BOXSCENENODE_H_
#define _R_DSM_BOXSCENENODE_H_

#include "R_DSM_SceneNodeEntity.h"
#include "..\\Core\\Math\\R_Core_AABBox3D.h"

namespace REDEEMER
{
	namespace RENDER
	{
		class C_VertexBuffer;
	}

	namespace DSM
	{
		/*!	\brief	Represents standard cube
		*/
		class C_DSMBoxSceneNode : public C_DSMSceneNodeEntity
		{
		public:
			/*!	Constructor
			*/
			C_DSMBoxSceneNode (CORE::C_AABBox3D<float> box);

			/*!	Destructor
			*/
			virtual ~C_DSMBoxSceneNode ();

		private:
			/*!	Creates cube data
			*/
			void CreateData ();

		private:
			CORE::C_AABBox3D<float>		m_Box;				///<	Bounding volume
			RENDER::C_VertexBuffer*		m_VertexBuffer;
		};

	}	//	namespace DSM
}	//	namespace REDEEMER

#endif	//	_R_DSM_BOXSCENENODE_H_
