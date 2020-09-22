/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_DSM_SkyBoxSceneNode.h
**		SkyBox is a cube with 6 textures rendered inside to get a feeling of watching at sky
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_DMS_SKYBOXSCENENODE_H_
#define _R_DMS_SKYBOXSCENENODE_H_

#include "R_DSM_SkySceneNode.h"

namespace REDEEMER
{
	namespace RENDER
	{
		class C_Material;
		class C_VertexBuffer;
	}

	namespace DSM
	{
		/*!	\brief	SkyBox is a cube with 6 textures rendered inside to get a feeling of watching at sky
		*/
		class C_DSMSkyBoxSceneNode : public C_DSMSkySceneNode
		{
		public:
			/*!	Constructor
			**	\param distance Distance from observer to sky box walls
			*/
			C_DSMSkyBoxSceneNode (float distance = 10.0f);

			/*!	Destructor
			*/
			virtual ~C_DSMSkyBoxSceneNode ();

			/*!	Sets sky material - it should contain 6 valid texture samplers in this order:
			**	up, bottom, front, back, left, right
			*/
			void SetSkyMaterial (RENDER::C_Material* skyBoxMaterial);

			/*!	Use internally with shaders to change texture sampler index for each sky box side
			*/
			void ChangeSkyBoxTexture (RENDER::C_RenderEntity* entity);

		private:
			void InitData ();

			float					m_Distance;
			RENDER::C_VertexBuffer*	m_VertexBuffers[6];
		};

	}	//	namespace DMS
}	//	namespace REDEEMER

#endif	//	_R_DMS_SKYBOXSCENENODE_H_

