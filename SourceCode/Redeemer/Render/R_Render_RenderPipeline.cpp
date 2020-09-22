/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_RenderPipeline.cpp
**		Render pipeline consist of render stages
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_RenderPipeline.h"
#include "R_Render_RenderStage.h"
#include "R_Render_GeometryStage.h"
#include "R_Render_RenderDevice.h"
#include "..\\RedeemerEngine.h"

#include <algorithm>

namespace REDEEMER
{
	namespace RENDER
	{
		/*!	Class used for comparing render stages according to their priorities
		*/
		class C_RenderStageComparator
		{
		public:
			/*!	Compares between two stages using "<" operator
			*/
			bool operator () (const C_RenderStage* left, const C_RenderStage* right)
			{
				return (*left < *right);
			}
		};

		//------------------------------------------------------------------------------------------------------------------------
		C_RenderPipeline::C_RenderPipeline () :
			m_DeferredMaterial (NULL)
		{
			m_RenderStages.clear();
			m_RenderStagesVector.clear ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_RenderPipeline::~C_RenderPipeline ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_RenderPipeline::Initialize ()
		{
			C_BaseClass::Initialize();

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_RenderPipeline::Finalize ()
		{
			for (std::map<std::wstring, C_RenderStage*>::iterator it = m_RenderStages.begin(); it != m_RenderStages.end(); ++ it)
			{
				(*it).second->Finalize();

			//	REDEEMER_SAFE_DELETE ((*it).second);
			}

			m_RenderStages.clear();
			m_RenderStagesVector.clear();

			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderPipeline::AddRenderStage (std::wstring stageName, C_RenderStage* stage)
		{
			if (m_RenderStages.find(stageName) != m_RenderStages.end())
				return;

			m_RenderStages[stageName] = stage;

			m_RenderStagesVector.push_back(stage);

			SortStages();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderPipeline::RemoveRenderStage (std::wstring stageName)
		{
			for (std::map<std::wstring, C_RenderStage*>::iterator it = m_RenderStages.begin(); it != m_RenderStages.end(); ++ it)
			{
				if ((*it).first == stageName)
				{
					for (std::vector<C_RenderStage*>::iterator it2 = m_RenderStagesVector.begin(); it2 != m_RenderStagesVector.end(); ++ it2)
					{
						if ((*it2) == (*it).second)
						{
							m_RenderStagesVector.erase(it2);

							break;
						}
					}

					REDEEMER_SAFE_DELETE (m_RenderStages[stageName]);

					m_RenderStages.erase(it);

					break;
				}
			}

			SortStages();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_RenderStage* C_RenderPipeline::GetRenderStage (std::wstring stageName)
		{
			if (m_RenderStages.find(stageName) == m_RenderStages.end())
			{
				REDEEMER_LOG << LOG_WARNING << L"Render: Can not find appropriate render stage!" << LOG_ENDMESSAGE;

				return NULL;
			}

			return m_RenderStages[stageName];
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderPipeline::AddToRenderQueue (C_RenderEntity* entity, C_Material* material)
		{
			C_RenderStage* geometryStage = GetRenderStage(GEOMETRY_STAGE_NAME);

			if (!geometryStage)
				return;

			static_cast<C_GeometryStage*>(geometryStage)->AddToRenderQueue (entity, material);

			if (!m_DeferredMaterial)
				m_DeferredMaterial = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice()->GetMaterialLibrary()->GetMaterialManager(L"SimpleMaterialManager")->GetMaterial(L"DeferredMaterial");

			static_cast<C_GeometryStage*>(geometryStage)->AddToRenderQueue (entity, m_DeferredMaterial);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderPipeline::Update ()
		{
			for (unsigned int i = 0; i < m_RenderStagesVector.size(); ++ i)
			{
				C_RenderStage* stage = m_RenderStagesVector[i];

				if (stage->IsEnabled ())
					stage->Update ();
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderPipeline::SortStages()
		{
			C_RenderStageComparator sorter;

			std::sort (m_RenderStagesVector.begin(), m_RenderStagesVector.end(), sorter);
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER
