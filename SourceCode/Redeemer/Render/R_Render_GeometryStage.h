/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_GeometryStage.h
**		Stage for processing geometry data
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_RENDER_GEOMETRYSTAGE_H_
#define _R_RENDER_GEOMETRYSTAGE_H_

#include "R_Render_RenderStage.h"
#include "R_Render_RenderQueue.h"

#include <map>
#include <string>

namespace REDEEMER
{
	namespace RENDER
	{
		class C_RenderEntity;
		class C_Material;

		/*!
		**	\brief Stage for processing geometry data
		*/
		class C_GeometryStage : public C_RenderStage
		{
		public:
			/*!	Constructor
			*/
			C_GeometryStage ();

			/*!	Destructor
			*/
			virtual ~C_GeometryStage ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize (int priority);

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Adds new render entity to queue
			*/
			void AddToRenderQueue (C_RenderEntity* entity, C_Material* material);

		private:
			/*!	Updates rendering stage
			*/
			virtual void Update ();

		private:
			std::map<std::wstring, C_RenderQueue>		m_RenderQueue;		///<	Contains all render entities
			std::map<std::wstring, C_RenderQueue>		m_PreRenderQueue;	///<	Contains all render entities processed before actual rendering
			std::map<std::wstring, C_Material*>			m_Materials;
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_GEOMETRYSTAGE_H_
