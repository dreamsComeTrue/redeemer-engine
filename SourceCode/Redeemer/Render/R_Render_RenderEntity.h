/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_RenderEntity.h
**		Render entities holds material used for rendering process as well as vertex buffer containing geometrical data
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_RENDER_RENDERENTITY_H_
#define _R_RENDER_RENDERENTITY_H_

#include "..\\..\\ExternalDependencies\\FastDelegate_Library\\FastDelegate.h"

#include <string>

namespace REDEEMER
{
	namespace DSM
	{
		class C_DSMSceneNodeEntity;
	}

	namespace RENDER
	{	
		class C_Material;
		class C_VertexBuffer;
		class C_RenderDevice;
		class C_RenderEntity;

		typedef fastdelegate::FastDelegate1<C_RenderEntity*, fastdelegate::detail::DefaultVoid> RENDER_EVENT;

		/*!	 \brief Render entities holds material used for rendering process as well as vertex buffer containing geometrical data
		*/
		class C_RenderEntity
		{
		public:
			/*!	Constructor
			*/
			C_RenderEntity (std::wstring name);

			/*!	Destructor
			*/
			virtual ~C_RenderEntity ();

			/*!	Sets render entity name
			*/
			void SetName (const std::wstring& name);

			/*!	Returns render entity's name
			*/
			const std::wstring& GetName ();

			/*!	Sets render entity material
			*/
			void SetMaterial (C_Material* material);

			/*!	Returns render entity's material
			*/
			C_Material* GetMaterial ();

			/*!	Sets render entity vertex buffer
			*/
			void SetVertexBuffer (C_VertexBuffer* vertexBuffer);

			/*!	Returns render entity's vertex buffer
			*/
			C_VertexBuffer* GetVertexBuffer ();

			/*!	Sets enabled for rendering
			*/
			void SetEnabled (bool enable = true);

			/*!	Checks, if this entity is ready for rendering
			*/
			bool IsEnabled() const;

			/*!	Renders geometrical data
			*/
			void Render ();

			/*!	Sets scene node entity, to which this render entity belongs
			*/
			void SetSceneNodeEntity (DSM::C_DSMSceneNodeEntity* entity);

			/*!	Gets scene node entity, to which this render entity belongs
			*/
			DSM::C_DSMSceneNodeEntity* GetSceneNodeEntity ();

		public:
			/*!	Method used before shaders update and geometry rendering
			*/
			RENDER_EVENT* OnBeforeRender;

			/*!	Method used before after geometry rendering
			*/
			RENDER_EVENT* OnAfterRender;

		private:
			std::wstring				m_Name;					///<	Entity name
			C_Material*					m_Material;				///<	Underlying material
			C_VertexBuffer*				m_VertexBuffer;			///<	Geometrical data
			bool						m_IsEnabled;			///<	Is this entity used for rendering			
			C_RenderDevice*				m_RenderDevice;			///<	For speeding things up
			DSM::C_DSMSceneNodeEntity*	m_SceneNodeEntity;		///<	Associated scene node
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_RENDERENTITY_H_
