/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_DSM_Frustum.cpp
**		A frustum represents a pyramid, capped at the near and far end which is used to represent either a visible area or a projection area.
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_DSM_Frustum.h"

namespace REDEEMER
{
	namespace DSM
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_DSMFrustum::C_DSMFrustum (float nearPlane, float farPlane, float aspectRatio, float FOV) :
			m_ProjectionType (FPT_Perspective),
			m_NearDistance (nearPlane),
			m_FarDistance (farPlane),
			m_AspectRatio (aspectRatio),
			m_FOV (FOV),
			m_IsChanged (true),
			m_ViewVolumeWidth (800.0f),
			m_ViewVolumeHeight (600.0f)
		{
			Update();
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DSMFrustum::~C_DSMFrustum ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMFrustum::SetFOV (const float& FOV)
		{
			m_FOV = FOV;	   
			m_IsChanged = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		const float& C_DSMFrustum::GetFOV (void) const
		{
			return m_FOV;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMFrustum::SetNearClippingDistance (float nearDistance)
		{
			m_NearDistance = nearDistance;
			m_IsChanged = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_DSMFrustum::GetNearClippingDistance (void) const
		{
			return m_NearDistance;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMFrustum::SetFarClippingDistance (float farDistance)
		{
			m_FarDistance = farDistance;
			m_IsChanged = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_DSMFrustum::GetFarClippingDistance (void) const
		{
			return m_FarDistance;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMFrustum::SetAspectRatio (float aspectRatio)
		{
			m_AspectRatio = aspectRatio;
			m_IsChanged = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_DSMFrustum::GetAspectRatio (void) const
		{
			return m_AspectRatio;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMFrustum::SetWidthOfViewVolume (float width)
		{
			m_ViewVolumeWidth = width;
			m_IsChanged = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_DSMFrustum::GetWidthOfViewVolume (void) const
		{
			return m_ViewVolumeWidth;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMFrustum::SetHeightOfViewVolume (float height)
		{
			m_ViewVolumeHeight = height;
			m_IsChanged = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_DSMFrustum::GetHeightOfViewVolume (void) const
		{
			return m_ViewVolumeHeight;
		}
																																  
		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Matrix<float>& C_DSMFrustum::GetProjectionMatrix (void) 
		{
			return m_ProjectionMatrix;
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Matrix<float>& C_DSMFrustum::GetViewMatrix (void) 
		{
			return m_ViewMatrix;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMFrustum::SetProjectionType (EFrustumProjectionType projectionType)
		{
			m_ProjectionType = projectionType;
			m_IsChanged = true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		EFrustumProjectionType C_DSMFrustum::GetProjectionType (void) const
		{
			return m_ProjectionType;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMFrustum::Update ()
		{
			// No need to update
			if (!m_IsChanged)
				return;

			//	Operate on frustum projection matrix
			if (m_ProjectionType == FPT_Perspective)
			{
				m_ProjectionMatrix.BuildProjectionMatrixPerspectiveFovLH (m_FOV, m_AspectRatio, m_NearDistance, m_FarDistance);
			}
			else if (m_ProjectionType == FPT_Orthographic)
			{
				m_ProjectionMatrix.BuildProjectionMatrixOrthoLH (m_ViewVolumeWidth, m_ViewVolumeHeight, m_NearDistance, m_FarDistance);
			}

			m_IsChanged = false;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DSMFrustum::BuildViewMatrix (CORE::C_Vector3D<float> poisition, CORE::C_Vector3D<float> target, CORE::C_Vector3D<float> up)
		{
			m_ViewMatrix.BuildCameraLookAtMatrixLH (poisition, target, up);
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_DSMFrustum::IsChanged () const
		{
			return m_IsChanged;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace DSM
}	//	namespace REDEEMER
