/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_DSM_CameraSceneNode.cpp
**		Basic camera class
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_DSM_CameraSceneNode.h"
#include "..\\Core\\R_Core_Timer.h"
#include "..\\Render\\R_Render_RenderDevice.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace DSM
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_DSMCameraSceneNode::C_DSMCameraSceneNode ()  :
			m_Frustum (new C_DSMFrustum ()),
			m_ForwardVector (CORE::C_Vector3D<float>::VECTOR_UNIT_Z),
			m_UpVector (CORE::C_Vector3D<float>::VECTOR_UNIT_Y),
			m_RightVector (CORE::C_Vector3D<float>::VECTOR_UNIT_X),
			m_IsCameraChanged (true)
		{
			Initialize();

			UpdateCamera();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMCameraSceneNode::~C_DSMCameraSceneNode ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMCameraSceneNode::UpdateCamera()
		{
			//	Is node changed or camera data changed?
			if (m_IsChanged || m_IsCameraChanged)
			{
				RecalculateAxes ();

				UpdateFrustum();

				m_IsChanged = false;
			}

			//	Is frustum data changed
			if (m_Frustum->IsChanged ())
				UpdateFrustum();

			m_PreviousTime = C_RedeemerEngine::GetSingleton ().GetTimer ()->GetMiliseconds ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMFrustum& C_DSMCameraSceneNode::GetFrustum ()
		{
			return *(m_Frustum.GetPointer());
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMCameraSceneNode::SetPosition (const CORE::C_Vector3D<float>& position)
		{
			C_DSMSceneNode::SetPosition(position);

			m_IsCameraChanged = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMCameraSceneNode::SetRotation (const CORE::C_Vector3D<float>& vector)
		{
			if ((!vector.X) && (!vector.Y) && (!vector.Z))
				return;

			C_DSMSceneNode::SetRotation (vector);

			m_IsCameraChanged = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMCameraSceneNode::SetRotation (const CORE::C_Vector3D<float>& axis, const float angle)
		{
			if (!angle)
				return;

			C_DSMSceneNode::SetRotation (axis, angle);

			m_IsCameraChanged = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMCameraSceneNode::SetRotation (const CORE::C_Quaternion& quaternion)
		{
			if ((quaternion.X == 0.0f) && (quaternion.Y == 0.0f) && (quaternion.Z == 0.0f) && (quaternion.W == 0.0f))
				return;

			C_DSMSceneNode::SetRotation (quaternion);
	
			m_IsCameraChanged = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMCameraSceneNode::SetForwardVector (CORE::C_Vector3D<float>& forward)
		{
			m_ForwardVector = forward;

			m_IsCameraChanged = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Vector3D<float> C_DSMCameraSceneNode::GetForwardVector ()
		{
			return m_ForwardVector;
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Vector3D<float> C_DSMCameraSceneNode::GetRightVector ()
		{
			return m_RightVector;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMCameraSceneNode::SetUpVector (CORE::C_Vector3D<float>& up)
		{
			m_UpVector = up;

			m_IsCameraChanged = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Vector3D<float> C_DSMCameraSceneNode::GetUpVector ()
		{
			return m_UpVector;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMCameraSceneNode::UpdateFrustum ()
		{
			m_Frustum->Update();

			// Update projection matrix of the renderer
			if (C_RedeemerEngine::GetSingleton ().GetDSMWorldSceneManager ()->GetActiveCamera () == this)
				C_RedeemerEngine::GetSingleton ().GetRenderManager ()->GetRenderDevice ()->SetRenderMatrix(RENDER::RMM_Projection, m_Frustum->GetProjectionMatrix());
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMCameraSceneNode::RecalculateAxes ()
		{
			m_Frustum->BuildViewMatrix (m_Position, m_Position + m_ForwardVector, m_UpVector);

			CORE::C_Matrix<float> matrix = m_Frustum->GetViewMatrix ();

			C_RedeemerEngine::GetSingleton ().GetRenderManager ()->GetRenderDevice ()->SetRenderMatrix (RENDER::RMM_ModelView, matrix);

			m_RightVector.X = matrix._m11;
			m_RightVector.Y = matrix._m21;
			m_RightVector.Z = matrix._m31;

			m_ForwardVector.X = matrix._m13;
			m_ForwardVector.Y = matrix._m23;
			m_ForwardVector.Z = matrix._m33;

			// Normalize all vectors
			m_ForwardVector.SetNormalized();
			m_RightVector.SetNormalized();
			m_UpVector.SetNormalized();
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace DSM
}	//	namespace REDEEMER
