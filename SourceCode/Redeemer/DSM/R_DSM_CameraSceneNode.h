/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_DSM_CameraSceneNode.h
**		Basic camera class
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_DSM_CAMERASCENENODE_H_
#define _R_DSM_CAMERASCENENODE_H_

#include "R_DSM_SceneNode.h"
#include "R_DSM_Frustum.h"
#include "..\\Core\\R_Core_SmartPointer.h"

#include <windows.h>

namespace REDEEMER
{
	namespace DSM
	{
		/*!	\brief	Basic camera class
		*/
		class C_DSMCameraSceneNode : public C_DSMSceneNode
		{
		public:
			/*!	Constructor
			*/
			C_DSMCameraSceneNode ();

			/*!	Destructor
			*/
			virtual ~C_DSMCameraSceneNode ();

			/*!	Updates camera.
			*/
			virtual void UpdateCamera();

			/*!	Gets camera frustum - for reading & changing aspect ratio. FOV, etc.
			*/
			C_DSMFrustum& GetFrustum ();

			/*!	Sets camera position
			*/
			virtual void SetPosition (const CORE::C_Vector3D<float>& position);	

			/*!	Sets camera rotation
			*/
			virtual void SetRotation (const CORE::C_Vector3D<float>& vector);

			/*!	Sets camera rotation
			*/
			virtual void SetRotation (const CORE::C_Vector3D<float>& axis, const float angle);

			/*!	Sets camera rotation
			*/
			virtual void SetRotation (const CORE::C_Quaternion& quaternion);

			/*!	Sets forward vector
			*/
			void SetForwardVector (CORE::C_Vector3D<float>& forward);

			/*!	Returns forward vector
			*/
			CORE::C_Vector3D<float> GetForwardVector ();

			/*!	Gets right vector
			*/
			CORE::C_Vector3D<float> GetRightVector ();

			/*!	Sets up vector
			*/
			void SetUpVector (CORE::C_Vector3D<float>& up);

			/*!	Returns up vector
			*/
			CORE::C_Vector3D<float> GetUpVector ();

		protected:
			/*!	Updates frustum data
			*/
			void UpdateFrustum ();

			/*!	Recalculates camera's axes
			*/
			virtual void RecalculateAxes ();

		protected:
			CORE::C_SmartPointer<C_DSMFrustum>	m_Frustum;				///<	Camera's frustum
			CORE::C_Vector3D<float>				m_ForwardVector;		///<	Forward vector
			CORE::C_Vector3D<float>				m_UpVector;				///<	Up vector
			CORE::C_Vector3D<float>				m_RightVector;			///<	Right vector
			bool								m_IsCameraChanged;		///<	Has camera data changed
			DWORD								m_PreviousTime;			///<	Used for movement
		};

	}	//	namespace DSM
}	//	namespace REDEEMER

#endif	//	_R_DSM_CAMERASCENENODE_H_
