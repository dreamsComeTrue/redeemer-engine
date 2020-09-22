/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_RenderQueue.h
**		Queue holds a collection of RenderEntities ready to be processed
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_RENDER_RENDERQUEUE_H_
#define _R_RENDER_RENDERQUEUE_H_

#include <string>
#include <map>
#include <vector>

namespace REDEEMER
{
	namespace RENDER
	{
		class C_RenderEntity;
		class C_Material;
		class C_RenderDevice;


		/*!	\brief	Queue holds a collection of RenderEntities ready to be processed
		*/
		class C_RenderQueue
		{
		public:
			/*!	Constructor
			*/
			C_RenderQueue ();

			/*!	Destructor
			*/
			virtual ~C_RenderQueue ();

			/*!	Adds render entity to queue
			*/
			void AddRenderEntity (C_RenderEntity* renderEntity);

			/*!	Gets render entities matching given material
			*/
			std::vector<C_RenderEntity*>* GetRenderEntities ();

			/*!	Process queue - renders all entities. All entities are sorted using materials as keys before rendering,
			**	so the overhead of switching materials is minimized
			*/
			void ProcessQueue (C_Material* material);

		private:
			std::vector<C_RenderEntity*>	m_Entities;
			C_RenderDevice*					m_RenderDevice;			///<	For speeding things up
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_RENDERQUEUE_H_
