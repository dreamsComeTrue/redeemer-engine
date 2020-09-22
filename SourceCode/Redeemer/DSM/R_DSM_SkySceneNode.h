/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_DSM_SkySceneNode.h
**		Abstract class to represent sky node
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_DMS_SKYSCENENODE_H_
#define _R_DMS_SKYSCENENODE_H_

#include "R_DSM_SceneNodeEntity.h"

namespace REDEEMER
{
	namespace DSM
	{
		/*!	\brief	Abstract class to represent sky node
		*/
		class C_DSMSkySceneNode : public C_DSMSceneNodeEntity
		{
		public:
			/*!	Constructor
			*/
			C_DSMSkySceneNode ();

			/*!	Destructor
			*/
			virtual ~C_DSMSkySceneNode ();
		};

	}	//	namespace DMS
}	//	namespace REDEEMER

#endif	//	_R_DMS_SKYSCENENODE_H_

