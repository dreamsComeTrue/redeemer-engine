/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_DSM_LightSceneNode.h
**		Light node is used for shading surfaces
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_DMS_LIGHTSCENENODE_H_
#define _R_DMS_LIGHTSCENENODE_H_

#include "R_DSM_SceneNodeEntity.h"

namespace REDEEMER
{
	namespace RENDER
	{
		class C_Material;
	}

	enum ELightType
	{
		LT_Spot,
		LT_Directional,
		LT_Point
	};

	namespace DSM
	{
		/*!	\brief	Light node is used for shading surfaces
		*/
		class C_LightSceneNode : public C_DSMSceneNodeEntity
		{
		public:
			/*!	Constructor
			*/
			C_LightSceneNode ();

			/*!	Constructor
			**	\param type Type (from ELightType typedef) of light to create
			*/
			C_LightSceneNode (ELightType lightType);

			/*!	Destructor
			*/
			virtual ~C_LightSceneNode ();

			/*!	Sets spot light direction
			*/
			void SetDirection (CORE::C_Vector3D<float> direction);

			/*!	Returns light direction
			*/
			CORE::C_Vector3D<float> GetDirection () const;

			/*!	Sets light type
			*/
			void SetType (ELightType lightType);

			/*!	Returns light type
			*/
			ELightType GetType () const;

			/*!	Returns light's material
			*/
			RENDER::C_Material* GetMaterial ();

		private:
			ELightType					m_Type;			///<	Type of light
			CORE::C_Vector3D<float>		m_Direction;	///<	Direction of spot light cone
			RENDER::C_Material*			m_Material;		///<	Material used for rendering lights
		};

	}	//	namespace DMS
}	//	namespace REDEEMER

#endif	//	_R_DMS_LIGHTSCENENODE_H_

