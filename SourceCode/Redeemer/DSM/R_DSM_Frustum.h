/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_DSM_Frustum.h
**		A frustum represents a pyramid, capped at the near and far end which is used to represent either a visible area or a projection area.
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_DSM_FRUSTUM_H_
#define _R_DSM_FRUSTUM_H_

#include "..\\Core\\Math\\R_Core_Matrix.h"

namespace REDEEMER
{
	namespace DSM
	{
		/*!	Specifies perspective (realistic) or orthographic (architectural) projection.
		*/
		enum EFrustumProjectionType
		{
			FPT_Orthographic,
			FPT_Perspective
		};

		/*! Frustum clipping planes.
		*/
		enum EFrustumClippingPlane
		{
			FCP_Near   = 0,
			FCP_Far    = 1,
			FCP_Left   = 2,
			FCP_Right  = 3,
			FCP_Top    = 4,
			FCP_Bottom = 5
		};


		/*!	\brief	A frustum represents a pyramid, capped at the near and far end which is used to represent either a visible area or a projection area.
		*/
		class C_DSMFrustum
		{
		public:
			/*!	Constructor
			*/
			C_DSMFrustum (float nearPlane = 0.01f, float farPlane = 1000.0f, float aspectRatio = 1.0f, float FOV = CORE::C_MathUtils::PI_HALF);

			/*!	Destructor
			*/
			virtual ~C_DSMFrustum ();

			/*! Sets the Y-dimension Field Of View (FOV) of the frustum. THIS VALUE SHOLUD BE GIVEN IN RADIANS!
			 */
			void SetFOV (const float& FOV);

			/*!	Retrieves the frustums Field Of View (FOV).
			*/
			const float& GetFOV (void) const;

			/*!	Sets the position of the near clipping plane.
				\param
					near The distance to the near clipping plane from the frustum in world coordinates.
			 */
			void SetNearClippingDistance (float nearDistance);

			/*! Gets the position of the near clipping plane.
			*/
			float GetNearClippingDistance (void) const;

			/*!	Sets the distance to the far clipping plane.
				\param
					far The distance to the far clipping plane from the frustum in 	world coordinates.
			*/
			void SetFarClippingDistance (float farDistance);

			/*!	Retrieves the distance from the frustum to the far clipping plane.
			*/
			float GetFarClippingDistance (void) const;

			/*!	Sets the aspect ratio for the frustum viewport.
			*/
			void SetAspectRatio (float aspectRatio);

			/*! Retrieves the current aspect ratio.
			*/
			float GetAspectRatio (void) const;

			/*!	Sets the width of view volume for the frustum viewport.
			*/
			void SetWidthOfViewVolume (float width);

			/*! Retrieves the width of view volume.
			*/
			float GetWidthOfViewVolume (void) const;

			/*!	Sets the height of view volume for the frustum viewport.
			*/
			void SetHeightOfViewVolume (float height);

			/*! Retrieves the height of view volume.
			*/
			float GetHeightOfViewVolume (void) const;

			/*! Gets the projection matrix for this frustum.
			*/
			CORE::C_Matrix<float>& GetProjectionMatrix (void);
			
			/*!	Gets the view matrix for this frustum.
		    */
			CORE::C_Matrix<float>& GetViewMatrix (void);

			/*! Sets the type of projection to use (orthographic or perspective). Default is perspective.
			*/
			void SetProjectionType (EFrustumProjectionType projectionType);

			/*! Retrieves info on the type of projection used (orthographic or perspective).
			*/
			EFrustumProjectionType GetProjectionType (void) const;

			/*!	Updates frustum data
			*/
			void Update ();

			/*!	Builds view matrix using position, target, and up vectors
			*/
			void BuildViewMatrix (CORE::C_Vector3D<float> poisition, CORE::C_Vector3D<float> target, CORE::C_Vector3D<float> up);

			/*!	Checks, if data has changed
			*/
			bool IsChanged () const;

		private:
			EFrustumProjectionType		m_ProjectionType;	///<	Type of projection (orthographic or perspective)
			float						m_FOV;				///<	Field Of View
			float						m_FarDistance;		///<	Far clip distance - default 10000
			float 						m_NearDistance;		///<	Near clip distance - default 100
			float						m_AspectRatio;		///<	x/y viewport ratio - default 1.3333
			float						m_ViewVolumeWidth;	///<	Used in orthograpic projection
			float						m_ViewVolumeHeight;	///<	Used in orthograpic projection
			bool						m_IsChanged;		///<	Has frustum data changed since last Update call?
			CORE::C_Matrix<float>		m_ProjectionMatrix;	///<	Frustum projection matrix
			CORE::C_Matrix<float>		m_ViewMatrix;		///<	Frustum view matrix
		};

	}	//	namespace DSM
}	//	namespace REDEEMER

#endif	//	_R_DSM_FRUSTUM_H_
