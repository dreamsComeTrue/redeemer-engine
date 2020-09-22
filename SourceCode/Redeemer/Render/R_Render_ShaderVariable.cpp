/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_ShaderVariable.cpp
**		 Variables are used for communication between shader and engine
*/

//------------------------------------------------------------------------------------------------------------------------

#include "R_Render_ShaderVariable.h"
#include "R_Render_RenderDevice.h"
#include "..\\DSM\\R_DSM_CameraSceneNode.h"
#include "..\\Core\\Math\R_Core_Matrix.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_ShaderVariable::C_ShaderVariable () :
			m_Values (0), 
			m_Count (0), 
			m_Function (SUF_None)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_ShaderVariable::C_ShaderVariable (char* handle, EShaderUpdateFunction function) :
			m_Values (0), 
			m_Count (0), 
			m_Function (function)
		{
			m_Values = new float[16];
			m_Handle = new char[strlen(handle)+1];

			memcpy (m_Handle, handle, strlen(handle));
			m_Handle[strlen(handle)] = '\0';
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_ShaderVariable::C_ShaderVariable (const C_ShaderVariable& variable)
		{
			const char* handle = variable.GetHandle();
			m_Handle = new char[strlen(handle + 1)];

			memcpy (m_Handle, handle, strlen(handle));
			m_Handle[strlen(handle)] = '\0';

			m_Function = variable.m_Function;

			m_Values = new float[16];

			memcpy (m_Values, variable.m_Values, 16 * sizeof (float));
		}	

		//------------------------------------------------------------------------------------------------------------------------

		C_ShaderVariable::~C_ShaderVariable ()
		{
			REDEEMER_SAFE_DELETE_ARRAY (m_Values);
			REDEEMER_SAFE_DELETE_ARRAY (m_Handle);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_ShaderVariable::SetData (float* values, unsigned int count)
		{
			m_Count = count;

			memset(m_Values, 0, 16 * sizeof (float));

			memcpy (m_Values, values, count * sizeof (float));
		}

		//------------------------------------------------------------------------------------------------------------------------

		const char* C_ShaderVariable::GetHandle () const
		{
			return m_Handle;
		}

		//------------------------------------------------------------------------------------------------------------------------

		float* C_ShaderVariable::GetValues ()
		{
			return m_Values;
		}

		//------------------------------------------------------------------------------------------------------------------------

		unsigned int C_ShaderVariable::GetCount ()
		{
			return m_Count;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_ShaderVariable::UpdateValues ()
		{
			C_RenderDevice* device = C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice();

			switch (m_Function)
			{
			case SUF_None:
				return;

			case SUF_World:
				{
					CORE::C_Matrix<float> world = device->GetRenderMatrix (RMM_World);

					SetData (world.M, 16);
				}

				break;

			case SUF_View:
				{
					CORE::C_Matrix<float> view = device->GetRenderMatrix (RMM_ModelView);

					SetData (view.M, 16);
				}
				break;

			case SUF_Projection:
				{
					CORE::C_Matrix<float> projection = device->GetRenderMatrix (RENDER::RMM_Projection);

					SetData (projection.M, 16);
				}
				break;

			case SUF_WorldView:
				{
					CORE::C_Matrix<float> world = device->GetRenderMatrix (RENDER::RMM_World);
					CORE::C_Matrix<float> view = device->GetRenderMatrix (RENDER::RMM_ModelView);

					CORE::C_Matrix<float> m = world * view;

					SetData (m.M, 16);
				}
				break;

			case SUF_WorldViewInv:
				{
					CORE::C_Matrix<float> world = device->GetRenderMatrix (RENDER::RMM_World);
					CORE::C_Matrix<float> view = device->GetRenderMatrix (RENDER::RMM_ModelView);

					CORE::C_Matrix<float> m = world * view;
					m.MakeInverse();

					SetData (m.M, 16);
				}
				break;

			case SUF_WorldViewProjection:
				{
					CORE::C_Matrix<float> world = device->GetRenderMatrix (RENDER::RMM_World);
					CORE::C_Matrix<float> view = device->GetRenderMatrix (RENDER::RMM_ModelView);
					CORE::C_Matrix<float> projection = device->GetRenderMatrix (RENDER::RMM_Projection);

					CORE::C_Matrix<float> m = world * view * projection;

					SetData (m.M, 16);
				}
				break;

			case SUF_ViewInverseTranspose:
				{
					CORE::C_Matrix<float> view = device->GetRenderMatrix (RENDER::RMM_ModelView);

					CORE::C_Matrix<float> m = view;
					m.MakeInverse();
					m.GetTransposed();

					SetData (m.M, 16);
				}
				break;

			case SUF_EyePosition:
				{
					DSM::C_DSMCameraSceneNode* camera = C_RedeemerEngine::GetSingleton().GetDSMWorldSceneManager()->GetActiveCamera();

					if (camera)
					{
						CORE::C_Vector3D<float> eyePosition = camera->GetPositionWorld();

						SetData (&eyePosition.X, 3);
					}	
				}

			case SUF_CameraView:
				{
					DSM::C_DSMCameraSceneNode* camera = C_RedeemerEngine::GetSingleton().GetDSMWorldSceneManager()->GetActiveCamera();

					if (camera)
					{
						CORE::C_Matrix<float> cameraView = camera->GetFrustum().GetViewMatrix();

						SetData (cameraView.M, 16);
					}	
				}

				break;
						
			default: 
				break;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_ShaderVariable::Parse (FILESYSTEM::C_XMLDocument::C_XMLNode* xmlFile)
		{
			// Parse child nodes
			FILESYSTEM::C_XMLDocument::C_XMLNode* node = xmlFile;

			std::string constName = node->FindAttribute ("Name");
			std::string updateFunction = node->FindAttribute ("UpdateFunction");

			m_Function = GetShaderUpdateFunction (updateFunction);

			m_Values = new float[16];
			m_Handle = new char[strlen(constName.c_str())+1];

			memcpy (m_Handle, constName.c_str(), strlen(constName.c_str()));
			m_Handle[strlen(constName.c_str())] = '\0';
		}

		//------------------------------------------------------------------------------------------------------------------------

		EShaderUpdateFunction C_ShaderVariable::GetShaderUpdateFunction (std::string function)
		{
			if (function == "World")
				return SUF_World;

			if (function == "View")
				return SUF_View;

			if (function == "Projection")
				return SUF_Projection;

			if (function == "WorldView")
				return SUF_WorldView;

			if (function == "WorldViewProjection")
				return SUF_WorldViewProjection;

			if (function == "WorldViewInv")
				return SUF_WorldViewInv;

			if (function == "ViewInverseTranspose")
				return SUF_ViewInverseTranspose;

			if (function == "EyePosition")
				return SUF_EyePosition;

			if (function == "CameraView")
				return SUF_CameraView;

			return SUF_None;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER
