/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_DSM_SceneNodeEntity.h
**		Scene nodes entities are derived from regular scene nodes and also contain RenderEntities data
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_DSM_SCENENODEENTITY_H_
#define _R_DSM_SCENENODEENTITY_H_

#include "R_DSM_SceneNode.h"

namespace REDEEMER
{
	namespace RENDER
	{
		class C_RenderEntity;
		class C_RenderDevice;
	}

	namespace DSM
	{
		/*!	\brief	Scene nodes entities are derived from regular scene nodes and also contain RenderEntities data
		*/
		class C_DSMSceneNodeEntity : public C_DSMSceneNode
		{
		public:
			/*!	Constructor
			*/
			C_DSMSceneNodeEntity ();

			/*!	Destructor
			*/
			virtual ~C_DSMSceneNodeEntity ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize ();

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Adds new entity
			*/
			void AddRenderEntity (const std::wstring& name, RENDER::C_RenderEntity* entity);
			
			/*!	Returns render entity with given name
			*/
			RENDER::C_RenderEntity* GetRenderEntity (const std::wstring& name);					  

			/*!	Returns render entity with given index
			*/
			RENDER::C_RenderEntity* GetRenderEntity (unsigned int index);		

			/*!	Returns render entities count
			*/
			unsigned int GetRenderEntityCount () const;

		protected:
			/*!	Updates scene node data
			*/
			virtual void BeforeUpdate ();

		private:	
			std::map<std::wstring, RENDER::C_RenderEntity*>			m_RenderEntities;
			RENDER::C_RenderDevice*									m_RenderDevice;		///<	For speeding things up
		};

	}	//	namespace DSM
}	//	namespace REDEEMER

#endif	//	_R_DSM_SCENENODEENTITY_H_
