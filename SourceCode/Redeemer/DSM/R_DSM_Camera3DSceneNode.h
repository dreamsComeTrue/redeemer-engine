/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_DSM_Camera3DSceneNode.h
**		Camera 3D (FPP style) implementation
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_DSM_CAMERA3DSCENENODE_H_
#define _R_DSM_CAMERA3DSCENENODE_H_

#include "R_DSM_CameraSceneNode.h"

namespace REDEEMER
{
	namespace DSM
	{
		/*!	\brief	Camera 3D (FPP style) implementation
		*/
		class C_DSMCamera3DSceneNode : public C_DSMCameraSceneNode
		{
		public:
			/*!	Constructor
			*/
			C_DSMCamera3DSceneNode (float velocity = 20.0f);

			/*!	Destructor
			*/
			virtual ~C_DSMCamera3DSceneNode ();

			/*!	Moves the camera in the z-direction.
			**	\param direction pass 1 to make camera go forward and -1, to go backwards.
			**	\param lockFlyMode If true, user can not move in Y direction
			*/
			void Move (float direction, bool lockFlyMode = false);

			/*!	Moves the camera in the x-direction.
			*	\param	direction pass 1 to make camera go right and -1, to go left.
			*/
			void Strafe (float direction);

			/*!	Rotate camera's view with mouse
			**	\param	x Relative X mouse movement 
			**	\param	y Relative Y mouse movement
			*/
			void RotateWithMouse (long int x, long int y);

			/*!	Returns camera's target
			*/
			CORE::C_Vector3D<float> GetTarget ();

			/*!	Sets camera's velocity
			*/
			void SetVelocity (float velocity);

			/*!	Returns camera's velocity
			*/
			float GetVelocity () const;

		protected:
			/*!	Recalculates camera's axes
			*/
			virtual void RecalculateAxes ();

		protected:			
			float			m_Velocity;		///<	Camera's speed
			float			m_RotX;
			float			m_RotY;
		};

	}	//	namespace DSM
}	//	namespace REDEEMER

#endif	//	_R_DSM_CAMERA3DSCENENODE_H_
