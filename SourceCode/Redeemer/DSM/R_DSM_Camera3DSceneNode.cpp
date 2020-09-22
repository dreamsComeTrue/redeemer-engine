/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_DSM_Camera3DSceneNode.cpp
**		Camera 3D (FPP style) implementation
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_DSM_Camera3DSceneNode.h"
#include "..\\Render\\R_Render_RenderDevice.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace DSM
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_DSMCamera3DSceneNode::C_DSMCamera3DSceneNode (float velocity) :
			m_Velocity (velocity),
			m_RotX (0.0f),
			m_RotY (0.0f)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMCamera3DSceneNode::~C_DSMCamera3DSceneNode ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMCamera3DSceneNode::Move (float direction, bool lockFlyMode)
		{
			if (!direction)
				return;

			float fDiff = float(C_RedeemerEngine::GetSingleton().GetTimer()->GetMiliseconds() - m_PreviousTime) * 0.001f;

			if (lockFlyMode)
			{
				CORE::C_Quaternion quat (m_Rotation);

				quat.X = 0;
				quat.Z = 0;

				m_Position += (quat * CORE::C_Vector3D<float> (0.0f, 0.0f, direction * m_Velocity * fDiff));
			}
			else
				m_Position += (m_Rotation * CORE::C_Vector3D<float> (0.0f, 0.0f, direction * m_Velocity * fDiff));

			m_IsCameraChanged = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMCamera3DSceneNode::Strafe (float direction)
		{
			if (!direction)
				return;

			float fDiff = float(C_RedeemerEngine::GetSingleton().GetTimer()->GetMiliseconds() - m_PreviousTime) * 0.001f;

			m_Position += (m_Rotation * CORE::C_Vector3D<float> (direction * m_Velocity * fDiff, 0.0f, 0.0f));

			m_IsCameraChanged = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMCamera3DSceneNode::RotateWithMouse (long int x, long int y)
		{
			m_RotX += x * 0.002f;
			m_RotY += y * 0.002f;

			CORE::C_Quaternion quatRot1 (CORE::C_Vector3D<float>(0.0f, 1.0f, 0.0f), m_RotX);
			CORE::C_Quaternion quatRot2 (CORE::C_Vector3D<float>(1.0f, 0.0f, 0.0f), m_RotY);

			SetRotation (quatRot1 * quatRot2);
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Vector3D<float> C_DSMCamera3DSceneNode::GetTarget ()
		{
			return m_ForwardVector;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMCamera3DSceneNode::SetVelocity (float velocity)
		{
			m_Velocity = velocity;
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_DSMCamera3DSceneNode::GetVelocity () const
		{
			return m_Velocity;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMCamera3DSceneNode::RecalculateAxes ()
		{
			// Update the vectors
			m_RightVector = GetRotationLocal() * CORE::C_Vector3D<float>::VECTOR_UNIT_X;
			m_UpVector = GetRotationLocal() * CORE::C_Vector3D<float>::VECTOR_UNIT_Y;
			m_ForwardVector = GetRotationLocal() * CORE::C_Vector3D<float>::VECTOR_UNIT_Z;

			// Normalize all vectors
			m_ForwardVector.SetNormalized();
			m_RightVector.SetNormalized();
			m_UpVector.SetNormalized();

			CORE::C_Vector3D<float> vecPosition = GetPositionLocal ();

			if (m_ParentNode)
				vecPosition += m_ParentNode->GetPositionLocal();

			m_Frustum->BuildViewMatrix (vecPosition, vecPosition + m_ForwardVector, m_UpVector);

			CORE::C_Matrix<float> matrix = m_Frustum->GetViewMatrix ();

			C_RedeemerEngine::GetSingleton ().GetRenderManager ()->GetRenderDevice ()->SetRenderMatrix (RENDER::RMM_ModelView, matrix);
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace DSM
}	//	namespace REDEEMER
