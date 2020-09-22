/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_DSM_LightSceneNode.cpp
**		Light node is used for shading surfaces
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_DSM_LightSceneNode.h"

namespace REDEEMER
{
	namespace DSM
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_LightSceneNode::C_LightSceneNode () :
			m_Type (LT_Point),
			m_Material (NULL)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------
		C_LightSceneNode::C_LightSceneNode (ELightType lightType) :
			m_Type (LT_Point),
			m_Material (NULL)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_LightSceneNode::~C_LightSceneNode ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_LightSceneNode::SetDirection (CORE::C_Vector3D<float> direction)
		{
			m_Direction = direction;
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Vector3D<float> C_LightSceneNode::GetDirection () const
		{
			return m_Direction;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_LightSceneNode::SetType(ELightType lightType)
		{
			m_Type = lightType;

			switch (lightType)
			{
			case LT_Directional:

				break;

			case LT_Point:

				break;

			case LT_Spot:

				break;
			}	   
		}

		//------------------------------------------------------------------------------------------------------------------------

		ELightType C_LightSceneNode::GetType () const
		{
			return m_Type;
		}

		//------------------------------------------------------------------------------------------------------------------------

		RENDER::C_Material* C_LightSceneNode::GetMaterial ()
		{
			return m_Material;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace DMS
}	//	namespace REDEEMER
