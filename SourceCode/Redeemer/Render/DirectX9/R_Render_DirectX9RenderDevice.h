/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_DirectX9RenderDevice.h
**		DirectX 9 implementation of render device
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_RENDER_DIRECTX9RENDERDEVICE_H_
#define _R_RENDER_DIRECTX9RENDERDEVICE_H_

#include "..\\R_Render_RenderDevice.h"

#include <d3d9.h>
#include <d3dx9.h>

namespace REDEEMER
{
	namespace RENDER
	{	
		/*!
		**	\brief DirectX 9 implementation of render device
		*/
		class C_DirectX9RenderDevice : public C_RenderDevice
		{
		public:
			/*!	Constructor
			*/
			C_DirectX9RenderDevice ();

			/*!	Destructor
			*/
			virtual ~C_DirectX9RenderDevice ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize ();

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Clears the backbuffer, Z-Buffer and stencil buffer to given values if relevant flags are set.
			**	\param buffers A combination of any of the EClearFlag
			**	\param backBufferColor Backbuffer color
			**	\param zBufferColor Color of the Z Buffer
			**	\param stencilBufferColor value for the stencil buffer
			*/
			virtual void ClearBuffers (DWORD buffers = CF_BackBuffer | CF_ZBuffer, C_Color backBufferColor = C_Color::COLOR_BLUE, float zBufferColor = 1.0f, DWORD stencilBufferColor = 0);

			/*!	Sets rendering matrix to the given type and value
			*/
			virtual void SetRenderMatrix (ERenderMatrixMode matrixMode, CORE::C_Matrix<float>& matrixData);

			/*!	Returns current values for given matrix type
			*/
			virtual CORE::C_Matrix<float> GetRenderMatrix (ERenderMatrixMode matrixMode);

			/*!	Sets render device to use windowed (or fullscreen) mode
			*/
			virtual void SetWindowedMode (bool isWindowed);

			/*!	Sets filling mode
			*/
			virtual void SetFillMode (EFillMode mode);

			/*!	Sets culling mode
			*/
			virtual void SetCullingMode (ECullMode mode);

			/*!	Enables writing to Z-Buffer
			*/
			virtual void SetZWriteEnabled (bool enabled);

			/*!	Sets source blending mode
			*/
			virtual void SetSourceBlendingMode (EBlendMode mode);

			/*!	Sets destination blending mode
			*/
			virtual void SetDestinationBlendingMode (EBlendMode mode);

			/*!	Sets blending operation.
			*/
			virtual void SetBlendOperation (EBlendOperation operation);

			/*!	Sets alpha test
			*/
			virtual void SetAlphaTestEnabled (bool enabled);

			/*!	Sets reference for alpha testing.
			*/
			virtual void SetAlphaTestReference (DWORD value);

			/*!	Sets alpha blending
			*/
			virtual void SetAlphaBlendEnabled (bool enabled);

			/*!	Sets separate alpha blend mode enabled
			*/
			virtual void SetSeparateAlphaBlendEnabled (bool enabled);

			/*!	Sets separate alpha source blending mode
			*/
			virtual void SetSeparateAlphaSourceBlendingMode (EBlendMode mode);

			/*!	Sets separate alpha destination blending mode
			*/
			virtual void SetSeparateAlphaDestinationBlendingMode (EBlendMode mode);

			/*!	Sets scissor test enabled
			*/
			virtual void SetScissorTestEnable (bool scissorTest = true);

			/*!	Sets scissor rectangle
			*/
			virtual void SetScissorRectangle (CORE::C_Rectangle<int> rectangle);

			/*!	Sets currently used video mode
			*/
			void SetRenderVideoMode (S_RenderVideoMode& mode);

			/*!	Process geometry to be rendered
			*/
			virtual void Render ();

			/*!	Actual rendering routine
			*/
			virtual bool DrawPrimitives (C_VertexBuffer* vb);

			/*!	Actual rendering routine
			*/
			virtual bool DrawPrimitives (C_VertexBuffer* vb, unsigned int startVertex, unsigned int numVertices, unsigned int startIndex, unsigned int primitivesCount);

			/*!	Returns DirectX object
			*/
			IDirect3D9* GetDirectX9Object ();

			/*!	Returns DirectX device
			*/
			IDirect3DDevice9* GetDirectX9Device ();

			/*!	Test some renderer internal capabilities
			*/
			virtual bool TestRendererCapabilities (ERendererTestCapabilities capabilities, void* parameter);

		private:
			/*!	Internal method for creating D3D Device
			*/
			bool CreateDirect3DDevice ();

			/*!	Create factory for creating resources like buffers
			*/
			virtual void CreateRenderDeviceFactory ();

		private:
			IDirect3D9*			m_Direct3DObject;		///<	Object, used to create DirectX Device
			IDirect3DDevice9*	m_Direct3DDevice;		///<	Direct3D Device
			D3DCAPS9			m_Direct3DCapabilities;	///<	Caps of the device
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_DIRECTX9RENDERDEVICE_H_
