/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_RenderPipeline.h
**		Render pipeline consist of render stages
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_RENDER_RENDERPIPELINE_H_
#define _R_RENDER_RENDERPIPELINE_H_

#include "..\\Core\\R_Core_BaseClass.h"

#include <map>
#include <vector>
#include <string>

namespace REDEEMER
{
	namespace RENDER
	{
		//	RENDER STAGES
#define GEOMETRY_STAGE_PRIORITY 1
#define GEOMETRY_STAGE_NAME L"GeometryStage"

#define LIGHT_STAGE_PRIORITY 2
#define LIGHT_STAGE_NAME L"LightStage"

#define GUI_STAGE_PRIORITY 4
#define GUI_STAGE_NAME L"GUIStage"

#define POSTPROCESS_STAGE_PRIORITY 3
#define POSTPROCESS_STAGE_NAME L"PostProcessStage"

		class C_RenderStage;
		class C_RenderDevice;
		class C_RenderEntity;
		class C_Material;

		/*!
		**	\brief Render pipeline consist of render stages
		*/
		class C_RenderPipeline : public CORE::C_BaseClass
		{
			friend class C_RenderDevice;

		public:
			/*!	Constructor
			*/
			C_RenderPipeline ();

			/*!	Destructor
			*/
			virtual ~C_RenderPipeline ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize ();

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Adds new render stage to the pipeline
			*/
			void AddRenderStage (std::wstring stageName, C_RenderStage* stage);

			/*!	Removes stage from the pipeline
			*/
			void RemoveRenderStage (std::wstring stageName);

			/*!	Returns stage with given name
			*/
			C_RenderStage* GetRenderStage (std::wstring stageName);

			/*!	Adds new render entity to appropriate queue
			*/
			void AddToRenderQueue (C_RenderEntity* entity, C_Material* material);

		private:
			/*!	Updates rendering pipeline - simply updates each rendering stage
			*/
			void Update ();

			/*!	Sort stages according to their priorities
			*/
			void SortStages ();

		private:
			std::map<std::wstring, C_RenderStage*>	m_RenderStages;
			std::vector<C_RenderStage*>				m_RenderStagesVector;	///<	Used, because of priority of stages
			C_Material*								m_DeferredMaterial;		///<	Material used in deffered lighting
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_RENDERPIPELINE_H_
