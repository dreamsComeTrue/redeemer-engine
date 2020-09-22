/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_DSM_PlaneSceneNode.h
**		Represents textured plane
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_DSM_PLANESCENENODE_H_
#define _R_DSM_PLANESCENENODE_H_

#include "R_DSM_SceneNodeEntity.h"
#include "..\\Core\\Math\\R_Core_Vector2D.h"
#include "..\\Core\\Math\\R_Core_Plane3D.h"

namespace REDEEMER
{
	namespace RENDER
	{
		class C_VertexBuffer;
	}

	namespace DSM
	{
		/*!	\brief	Represents textured plane
		*/
		class C_DSMPlaneSceneNode : public C_DSMSceneNodeEntity
		{
		public:
			/*!	Constructor
			*/
			C_DSMPlaneSceneNode (CORE::C_Plane3D<float> plane, float size, CORE::C_Vector2D<float> textureMultipier);

			/*!	Destructor
			*/
			virtual ~C_DSMPlaneSceneNode ();

		private:
			/*!	Creates plane data
			*/
			void CreateData ();

		private:
			CORE::C_Plane3D<float>		m_Plane;			///<	Plane description
			float						m_Size;				///<	Size multipier
			CORE::C_Vector2D<float>		m_TextureMultipier;
			RENDER::C_VertexBuffer*		m_VertexBuffer;
		};

	}	//	namespace DSM
}	//	namespace REDEEMER

#endif	//	_R_DSM_PLANESCENENODE_H_
