/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_RenderDevice.cpp
**		Generic rendering device, act as a base class for all renderers
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_RenderDevice.h"
#include "R_Render_RenderDeviceFactory.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{
		//------------------------------------------------------------------------------------------------------------------------
		int C_RenderDevice::Initialize ()
		{
			C_BaseClass::Initialize();

			CreateRenderDeviceFactory ();

			m_VertexBufferManager = new C_VertexBufferManager ();	
			m_VertexBufferManager->Initialize();

			m_TextureManager = new C_TextureManager ();
			m_TextureManager->Initialize();

			m_ShaderManager = new C_ShaderManager ();
			m_ShaderManager->Initialize();

			m_MaterialLibrary = new C_MaterialLibrary ();
			m_MaterialLibrary->Initialize();

			m_RenderPipeline = new C_RenderPipeline ();
			m_RenderPipeline->Initialize();

			m_GeometryStage = new C_GeometryStage ();
			m_GeometryStage->Initialize(GEOMETRY_STAGE_PRIORITY);

			m_LightStage = new C_LightStage ();
			m_LightStage->Initialize(LIGHT_STAGE_PRIORITY);

			m_GUIStage = new C_GUIStage ();
			m_GUIStage->Initialize (GUI_STAGE_PRIORITY);

			m_PostProcessStage = new C_PostProcessStage ();
			m_PostProcessStage->Initialize (POSTPROCESS_STAGE_PRIORITY);

			//	Attach render stages
			m_RenderPipeline->AddRenderStage (GEOMETRY_STAGE_NAME, m_GeometryStage);
			m_RenderPipeline->AddRenderStage (LIGHT_STAGE_NAME, m_LightStage);
			m_RenderPipeline->AddRenderStage (GUI_STAGE_NAME, m_GUIStage);
			m_RenderPipeline->AddRenderStage (POSTPROCESS_STAGE_NAME, m_PostProcessStage);

			return RESULT_OK;	   
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_RenderDevice::Finalize ()
		{
			m_DeviceFactory->Finalize();
			REDEEMER_SAFE_DELETE (m_DeviceFactory);

			m_RenderPipeline->Finalize();
			m_ShaderManager->Finalize();
			m_TextureManager->Finalize();
			m_VertexBufferManager->Finalize();
			m_MaterialLibrary->Finalize();

			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderDevice::SetWindowedMode (bool isWindowed)
		{
			m_RenderVideoMode.Windowed = isWindowed;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_RenderDevice::GetWindowedMode () const
		{
			return m_RenderVideoMode.Windowed;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderDevice::SetFillMode (EFillMode mode)
		{
			m_RenderVideoMode.RenderState.FillMode = mode;
		}

		//------------------------------------------------------------------------------------------------------------------------

		EFillMode C_RenderDevice::GetFillMode () const
		{
			return m_RenderVideoMode.RenderState.FillMode;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderDevice::SetCullingMode (ECullMode mode)
		{
			m_RenderVideoMode.RenderState.CullMode = mode;
		}

		//------------------------------------------------------------------------------------------------------------------------

		ECullMode C_RenderDevice::GetCullingMode () const
		{
			return m_RenderVideoMode.RenderState.CullMode;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderDevice::SetZWriteEnabled (bool enabled)
		{
			m_RenderVideoMode.RenderState.ZWrite = enabled;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_RenderDevice::IsZWriteEnabled () const
		{
			return m_RenderVideoMode.RenderState.ZWrite;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderDevice::SetSourceBlendingMode (EBlendMode mode)
		{
			m_RenderVideoMode.RenderState.SourceBlending = mode;
		}

		//------------------------------------------------------------------------------------------------------------------------

		EBlendMode C_RenderDevice::GetSourceBlendingMode () const
		{
			return m_RenderVideoMode.RenderState.SourceBlending;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderDevice::SetDestinationBlendingMode (EBlendMode mode)
		{
			m_RenderVideoMode.RenderState.DestinationBlending = mode;
		}

		//------------------------------------------------------------------------------------------------------------------------

		EBlendMode C_RenderDevice::GetDestinationBlendingMode () const
		{
			return m_RenderVideoMode.RenderState.DestinationBlending;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderDevice::SetBlendOperation (EBlendOperation operation)
		{
			m_RenderVideoMode.RenderState.BlendOperation = operation;
		}

		//------------------------------------------------------------------------------------------------------------------------

		EBlendOperation C_RenderDevice::GetBlendOperation () const
		{
			return m_RenderVideoMode.RenderState.BlendOperation;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderDevice::SetAlphaTestEnabled (bool enabled)
		{
			m_RenderVideoMode.RenderState.AlphaTest = enabled;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_RenderDevice::IsAlphaTestEnabled () const
		{
			return m_RenderVideoMode.RenderState.AlphaTest;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderDevice::SetAlphaTestReference (DWORD value)
		{
			m_RenderVideoMode.RenderState.AlphaTestReference = value;
		}

		//------------------------------------------------------------------------------------------------------------------------

		DWORD C_RenderDevice::GetAlphaTestReference () const
		{
			return m_RenderVideoMode.RenderState.AlphaTestReference;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderDevice::SetAlphaBlendEnabled (bool enabled)
		{
			m_RenderVideoMode.RenderState.AlphaBlend = enabled;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_RenderDevice::IsAlphaBlendEnabled () const
		{
			return m_RenderVideoMode.RenderState.AlphaBlend;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderDevice::SetSeparateAlphaBlendEnabled (bool enabled)
		{
			m_RenderVideoMode.RenderState.SeparateAlpha = enabled;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_RenderDevice::IsSeparateAlphaBlendEnabled () const
		{
			return m_RenderVideoMode.RenderState.SeparateAlpha;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderDevice::SetSeparateAlphaSourceBlendingMode (EBlendMode mode)
		{
			m_RenderVideoMode.RenderState.SeparateAlphaSourceBlending = mode;
		}

		//------------------------------------------------------------------------------------------------------------------------

		EBlendMode C_RenderDevice::GetSeparateAlphaSourceBlendingMode () const
		{
			return m_RenderVideoMode.RenderState.SeparateAlphaSourceBlending;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderDevice::SetSeparateAlphaDestinationBlendingMode (EBlendMode mode)
		{
			m_RenderVideoMode.RenderState.SeparateAlphaDestinationBlending = mode;
		}

		//------------------------------------------------------------------------------------------------------------------------

		EBlendMode C_RenderDevice::GetSeparateAlphaDestinationBlendingMode () const
		{
			return m_RenderVideoMode.RenderState.SeparateAlphaDestinationBlending;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderDevice::SetScissorTestEnable (bool scissorTest)
		{
			m_RenderVideoMode.RenderState.ScissorTest = scissorTest;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_RenderDevice::IsScissorTestEnabled () const
		{
			return m_RenderVideoMode.RenderState.ScissorTest;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderDevice::SetScissorRectangle(CORE::C_Rectangle<int> rectangle)
		{
			m_RenderVideoMode.ScissorRectangle = rectangle;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::vector<S_RenderVideoMode>& C_RenderDevice::GetRenderVideoModes ()
		{
			return m_VideoModes;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderDevice::SetRenderVideoMode (S_RenderVideoMode& mode)
		{
			m_RenderVideoMode = mode;
		}

		//------------------------------------------------------------------------------------------------------------------------

		S_RenderVideoMode& C_RenderDevice::GetRenderVideoMode ()
		{
			return m_RenderVideoMode;
		}

		//------------------------------------------------------------------------------------------------------------------------

		S_RenderState& C_RenderDevice::GetRenderState ()
		{
			return m_RenderVideoMode.RenderState;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_RenderPipeline& C_RenderDevice::GetRenderPipeline ()
		{
			return *m_RenderPipeline;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderDevice::UpdatePipeline()
		{
			m_RenderPipeline->Update();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_RenderDevice::SetRenderMode (ERendererMode mode)
		{
			if (mode == RM_2D)
			{
				if (m_RenderVideoMode.RenderMode == RM_2D)
					return;

				m_RenderVideoMode.RenderMode = RM_2D;

				//	Save current matrices
				m_OldMatrixView = GetRenderMatrix(RMM_ModelView);
				m_OldMatrixProjection = GetRenderMatrix(RMM_Projection);
				m_OldMatrixWorld = GetRenderMatrix(RMM_World);

				CORE::C_Matrix<float> worldMatrix (CORE::C_Matrix<float>::CF_IDENTITY);
				CORE::C_Matrix<float> viewMatrix;
				CORE::C_Matrix<float> projectionMatrix;

				float width = (float)m_RenderVideoMode.ScreenResolution.Width;
				float height = (float)m_RenderVideoMode.ScreenResolution.Height;

				viewMatrix.SetTranslation(CORE::C_Vector3D<float>(-width + width * 0.5f, height - height * 0.5f, 0.5f));
				viewMatrix._m22 = -1.0f;
				projectionMatrix.BuildProjectionMatrixOrthoLH (width, height, 0.01f, 1000.0f);

				SetRenderMatrix(RMM_World, worldMatrix);
				SetRenderMatrix(RMM_ModelView, viewMatrix);
				SetRenderMatrix(RMM_Projection, projectionMatrix);
			}

			if (mode == RM_3D)
			{
				if (m_RenderVideoMode.RenderMode == RM_3D)
					return;

				//	Just restore saved matrices
				SetRenderMatrix (RMM_ModelView, m_OldMatrixView);
				SetRenderMatrix (RMM_Projection, m_OldMatrixProjection);
				SetRenderMatrix (RMM_World, m_OldMatrixWorld);

				m_RenderVideoMode.RenderMode = RM_3D;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_RenderDeviceFactory* C_RenderDevice::GetRenderDeviceFactory ()
		{
			return m_DeviceFactory;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_VertexBufferManager* C_RenderDevice::GetVertexBufferManager ()
		{
			return m_VertexBufferManager;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_TextureManager* C_RenderDevice::GetTextureManager ()
		{
			return m_TextureManager;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_ShaderManager* C_RenderDevice::GetShaderManager ()
		{
			return m_ShaderManager;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_MaterialLibrary* C_RenderDevice::GetMaterialLibrary ()
		{
			return m_MaterialLibrary;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER
