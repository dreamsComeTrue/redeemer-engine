/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_DirectX9RenderDevice.cpp
**		DirectX 9 implementation of render device
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_DirectX9RenderDevice.h"
#include "R_Render_DirectX9RenderDeviceFactory.h"
#include "..\\R_Render_VertexBuffer.h"
#include "..\\R_Render_VertexFormat.h"
#include "..\\..\\Core\\R_Core_Window.h"
#include "..\\..\\RedeemerEngine.h"

// disable: deprecation warnings when using CRT calls in VC8
#pragma warning (disable: 4996)

namespace REDEEMER
{
	namespace RENDER
	{	
		//------------------------------------------------------------------------------------------------------------------------
		C_DirectX9RenderDevice::C_DirectX9RenderDevice () :
			m_Direct3DObject (NULL),
			m_Direct3DDevice (NULL)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DirectX9RenderDevice::~C_DirectX9RenderDevice ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_DirectX9RenderDevice::Initialize ()
		{
			if (CreateDirect3DDevice() == false)
			{
				REDEEMER_LOG << LOG_FATAL << L"Render: Can not create Direct3D 9 renderer!" << LOG_ENDMESSAGE;

				return RESULT_FAIL;
			}			

			C_RenderDevice::Initialize();

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_DirectX9RenderDevice::Finalize ()
		{
			REDEEMER_SAFE_RELEASE (m_Direct3DDevice);
			REDEEMER_SAFE_RELEASE (m_Direct3DObject);

			return C_RenderDevice::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9RenderDevice::ClearBuffers (DWORD buffers, C_Color backBufferColor, float zBufferColor, DWORD stencilBufferColor)
		{
			DWORD d3dFlags = 0;

			if (buffers & CF_BackBuffer)
				d3dFlags |= D3DCLEAR_TARGET;

			if (buffers & CF_ZBuffer)
				d3dFlags |= D3DCLEAR_ZBUFFER;

			if (buffers & CF_StencilBuffer)
				d3dFlags |= D3DCLEAR_STENCIL;

			m_Direct3DDevice->Clear (0, NULL, d3dFlags, backBufferColor.GetColorARGB(), zBufferColor, stencilBufferColor);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9RenderDevice::SetRenderMatrix (ERenderMatrixMode matrixMode, CORE::C_Matrix<float>& matrixData)
		{
			D3DMATRIX tmp;

			memcpy (&tmp, matrixData.M, sizeof (D3DMATRIX));

			switch (matrixMode)
			{
			case RMM_World:
				//	Set new D3D matrix
				m_Direct3DDevice->SetTransform (D3DTS_WORLD, &tmp);
				break;

			case RMM_ModelView:
				//	Set new D3D matrix
				m_Direct3DDevice->SetTransform (D3DTS_VIEW, &tmp);
				break;

			case RMM_Projection:
				//	Set new D3D matrix
				m_Direct3DDevice->SetTransform (D3DTS_PROJECTION, &tmp);
				break;
			}	  
		}

		//------------------------------------------------------------------------------------------------------------------------

		CORE::C_Matrix<float> C_DirectX9RenderDevice::GetRenderMatrix (ERenderMatrixMode matrixMode)
		{
			D3DMATRIX tmp;
			CORE::C_Matrix<float> result;

			switch (matrixMode)
			{
			case RMM_World:
				m_Direct3DDevice->GetTransform (D3DTS_WORLD, &tmp);

				break;

			case RMM_ModelView:
				m_Direct3DDevice->GetTransform (D3DTS_VIEW, &tmp);

				break;

			case RMM_Projection:
				m_Direct3DDevice->GetTransform (D3DTS_PROJECTION, &tmp);

				break;
			}

			memcpy (&result, &tmp, sizeof (D3DMATRIX));

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9RenderDevice::SetWindowedMode (bool isWindowed)
		{
			C_RenderDevice::SetWindowedMode(isWindowed);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9RenderDevice::SetFillMode (EFillMode mode)
		{
			C_RenderDevice::SetFillMode(mode);

			DWORD value;

			switch (mode)
			{
			case FM_Dot:
				value = D3DFILL_POINT;
				break;

			case FM_Wireframe:
				value = D3DFILL_WIREFRAME;
				break;
			
			case FM_Solid:
				value = D3DFILL_SOLID;
				break;

			case FM_Textured:
				value = D3DFILL_SOLID;
				break;

			default:
				value = D3DFILL_SOLID;
			}

			m_Direct3DDevice->SetRenderState (D3DRS_FILLMODE, value);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9RenderDevice::SetCullingMode (ECullMode mode)
		{
			C_RenderDevice::SetCullingMode(mode);

			DWORD value;

			switch (mode)
			{
			case CM_None:
				value = D3DCULL_NONE;
				break;

			case CM_CW:
				value = D3DCULL_CW;
				break;

			case CM_CCW:
				value = D3DCULL_CCW;
				break;

			default:
				value = D3DCULL_CW;
			}

			m_Direct3DDevice->SetRenderState (D3DRS_CULLMODE, value);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9RenderDevice::SetZWriteEnabled(bool enabled)
		{
			C_RenderDevice::SetZWriteEnabled(enabled);

			m_Direct3DDevice->SetRenderState (D3DRS_ZWRITEENABLE, (DWORD)enabled);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9RenderDevice::SetSourceBlendingMode (EBlendMode mode)
		{
			C_RenderDevice::SetSourceBlendingMode(mode);

			m_Direct3DDevice->SetRenderState(D3DRS_SRCBLEND, mode);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9RenderDevice::SetDestinationBlendingMode (EBlendMode mode)
		{
			C_RenderDevice::SetDestinationBlendingMode(mode);

			m_Direct3DDevice->SetRenderState(D3DRS_DESTBLEND, mode);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9RenderDevice::SetBlendOperation (EBlendOperation operation)
		{
			C_RenderDevice::SetBlendOperation(operation);

			m_Direct3DDevice->SetRenderState (D3DRS_BLENDOP, (DWORD)operation);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9RenderDevice::SetAlphaTestEnabled (bool enabled)
		{
			C_RenderDevice::SetAlphaTestEnabled (enabled);

			m_Direct3DDevice->SetRenderState (D3DRS_ALPHATESTENABLE, (DWORD)enabled);

			DWORD function = 0;

			m_Direct3DDevice->GetRenderState(D3DRS_ALPHAFUNC, &function);

			if (function != D3DCMP_GREATEREQUAL)
				m_Direct3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9RenderDevice::SetAlphaTestReference (DWORD value)
		{
			C_RenderDevice::SetAlphaTestReference (value);

			m_Direct3DDevice->SetRenderState (D3DRS_ALPHAREF, value);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9RenderDevice::SetAlphaBlendEnabled (bool enabled)
		{
			C_RenderDevice::SetAlphaBlendEnabled (enabled);

			m_Direct3DDevice->SetRenderState (D3DRS_ALPHABLENDENABLE, (DWORD)enabled);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9RenderDevice::SetSeparateAlphaBlendEnabled (bool enabled)
		{
			C_RenderDevice::SetSeparateAlphaBlendEnabled (enabled);

			m_Direct3DDevice->SetRenderState (D3DRS_SEPARATEALPHABLENDENABLE, enabled);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9RenderDevice::SetSeparateAlphaSourceBlendingMode (EBlendMode mode)
		{
			C_RenderDevice::SetSeparateAlphaSourceBlendingMode (mode);

			m_Direct3DDevice->SetRenderState (D3DRS_SRCBLENDALPHA, mode);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9RenderDevice::SetSeparateAlphaDestinationBlendingMode (EBlendMode mode)
		{
			C_RenderDevice::SetSeparateAlphaDestinationBlendingMode (mode);

			m_Direct3DDevice->SetRenderState (D3DRS_DESTBLENDALPHA, mode);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9RenderDevice::SetScissorTestEnable (bool scissorTest)
		{
			C_RenderDevice::SetScissorTestEnable(scissorTest);

			m_Direct3DDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, (DWORD)scissorTest);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9RenderDevice::SetScissorRectangle(CORE::C_Rectangle<int> rectangle)
		{
			C_RenderDevice::SetScissorRectangle(rectangle);

			RECT rect;

			rect.top = rectangle.UpperLeftCorner.Y;
			rect.left = rectangle.UpperLeftCorner.X;
			rect.right = rectangle.LowerRightCorner.X;
			rect.bottom = rectangle.LowerRightCorner.Y;

			m_Direct3DDevice->SetScissorRect (&rect);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9RenderDevice::SetRenderVideoMode (S_RenderVideoMode& mode)
		{
			C_RenderDevice::SetRenderVideoMode(mode);

		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9RenderDevice::Render()
		{
 			ClearBuffers (CF_BackBuffer | CF_ZBuffer, C_Color::COLOR_BLACK);
 
 			m_Direct3DDevice->BeginScene();
 										
 				UpdatePipeline();
 
 			m_Direct3DDevice->EndScene();
 
 			m_Direct3DDevice->Present (NULL, NULL, NULL, NULL);
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_DirectX9RenderDevice::CreateDirect3DDevice ()
		{
			// Create Direct3D object
			m_Direct3DObject = Direct3DCreate9 (D3D_SDK_VERSION);

			if (!m_Direct3DObject)
			{
				REDEEMER_LOG << LOG_FATAL << L"Render: Can not create Direct3D 9 object!" << LOG_ENDMESSAGE;

				return false;
			}

			// Retrieve hardware capabilities and some useful stuff
			m_Direct3DObject->GetDeviceCaps (D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_Direct3DCapabilities);

			// Build the creation parameters
			D3DPRESENT_PARAMETERS d3dpp; 
			ZeroMemory (&d3dpp, sizeof (D3DPRESENT_PARAMETERS));

			HWND hWND = C_RedeemerEngine::GetSingleton().GetSystem()->GetMainWindow()->GetHWND();

			m_RenderVideoMode.Windowed = !C_RedeemerEngine::GetSingleton().GetSystem()->GetMainWindow()->IsFullScreen();

			d3dpp.Windowed					= m_RenderVideoMode.Windowed;
			d3dpp.hDeviceWindow				= hWND;
			d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
			d3dpp.BackBufferCount			= 1;
			d3dpp.BackBufferFormat			= D3DFMT_X8R8G8B8;
			d3dpp.BackBufferWidth			= m_RenderVideoMode.ScreenResolution.Width;
			d3dpp.BackBufferHeight			= m_RenderVideoMode.ScreenResolution.Height;
			d3dpp.EnableAutoDepthStencil	= true;
			d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;
			d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_DEFAULT;

			// Try to require hardware T&L (Best case)
			if (FAILED (m_Direct3DObject->CreateDevice (D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWND, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_Direct3DDevice)))
			{
				REDEEMER_LOG << LOG_FATAL << L"Render: Can not create Direct3D 9 Device with D3DCREATE_HARDWARE_VERTEXPROCESSING!" << LOG_ENDMESSAGE;

				// Failed ! try to get software T&L (Not that bad after all)
				if (FAILED (m_Direct3DObject->CreateDevice (D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWND, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_Direct3DDevice)))
				{
					REDEEMER_LOG << LOG_FATAL << L"Render: Can not create Direct3D 9 Device with D3DCREATE_SOFTWARE_VERTEXPROCESSING!" << LOG_ENDMESSAGE;

					// Failed ! try to get software T&L AND reference device (Worst case)
					if (FAILED (m_Direct3DObject->CreateDevice (D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWND, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_Direct3DDevice)))
					{
						REDEEMER_LOG << LOG_FATAL << L"Render: Can not create Direct3D 9 Device with D3DCREATE_SOFTWARE_VERTEXPROCESSING " <<
							L"and REF rasterizer. Buy another graphics card !" << LOG_ENDMESSAGE;

						return false;
					}
				}
			}

			//	Let's check all available video modes
			unsigned int adapterCount = m_Direct3DObject->GetAdapterCount ();

			for (unsigned int i = 0; i < adapterCount; ++ i)
			{
				unsigned int modeCount = m_Direct3DObject->GetAdapterModeCount (i, D3DFMT_X8R8G8B8);

				for (unsigned int j = 0; j < modeCount; ++ j)
				{
					D3DDISPLAYMODE mode;

					m_Direct3DObject->EnumAdapterModes (i, D3DFMT_X8R8G8B8, j, &mode);

					S_RenderVideoMode renderVideoMode;

					renderVideoMode.ScreenResolution.Width = mode.Width;
					renderVideoMode.ScreenResolution.Height = mode.Height;

					m_VideoModes.push_back (renderVideoMode);
				}
			}

			m_Direct3DDevice->SetRenderState(D3DRS_LIGHTING, false);

			SetCullingMode (CM_None);			

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_DirectX9RenderDevice::DrawPrimitives (C_VertexBuffer* vb)
		{
 			if (vb == NULL)
 				return false;
 
 			if (!vb->BindForRendering())
				return false;				 

 			//	Actual drawing
 			if (vb->GetIndexBuffer())
 			{
 				if (FAILED (m_Direct3DDevice->DrawIndexedPrimitive ((D3DPRIMITIVETYPE)(vb->GetPrimitiveType() + 1), 0, 0, vb->GetVerticesCount (), 0, vb->GetPrimitivesCount())))
 					return false;
 			}
 			else
 			{
 				if (FAILED (m_Direct3DDevice->DrawPrimitive ((D3DPRIMITIVETYPE)(vb->GetPrimitiveType() + 1), 0, vb->GetPrimitivesCount())))
 					return false;
 			}

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_DirectX9RenderDevice::DrawPrimitives (C_VertexBuffer* vb, unsigned int startVertex, unsigned int numVertices, unsigned int startIndex, unsigned int primitivesCount)
		{
			if (vb == NULL)
				return false;

			if (!vb->BindForRendering())
				return false;				 

			//	Actual drawing
			if (vb->GetIndexBuffer())
			{
				if (FAILED (m_Direct3DDevice->DrawIndexedPrimitive ((D3DPRIMITIVETYPE)(vb->GetPrimitiveType() + 1), 0, startVertex, numVertices, startIndex, primitivesCount)))
					return false;
			}

			return true;
		}		 

		//------------------------------------------------------------------------------------------------------------------------

		IDirect3D9* C_DirectX9RenderDevice::GetDirectX9Object ()
		{
			return m_Direct3DObject;
		}

		//------------------------------------------------------------------------------------------------------------------------

		IDirect3DDevice9* C_DirectX9RenderDevice::GetDirectX9Device ()
		{
			return m_Direct3DDevice;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9RenderDevice::CreateRenderDeviceFactory ()
		{
			m_DeviceFactory = new C_DirectX9RenderDeviceFactory ();
			m_DeviceFactory->Initialize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_DirectX9RenderDevice::TestRendererCapabilities (ERendererTestCapabilities capabilities, void* parameter)
		{
			// Find the capability
			switch (capabilities)
			{
			case RTC_MaxAnisotropy: memcpy(parameter, &m_Direct3DCapabilities.MaxAnisotropy, sizeof(DWORD));
				break;

			case RTC_MinAnisotropy: memset(parameter, (m_Direct3DCapabilities.TextureFilterCaps & D3DPTFILTERCAPS_MINFANISOTROPIC ? true : false),
										 sizeof(bool));
				break;

			case RTC_MagAnisotropy: memset(parameter, (m_Direct3DCapabilities.TextureFilterCaps & D3DPTFILTERCAPS_MAGFANISOTROPIC ? true : false),
										 sizeof(bool));
				break;

			case RTC_MaxClipPlanes: memcpy(parameter, &m_Direct3DCapabilities.MaxUserClipPlanes, sizeof(DWORD));
				break;

			case RTC_MaxIndicesCount: memcpy(parameter, &m_Direct3DCapabilities.MaxVertexIndex, sizeof(DWORD));
				break;

			case RTC_MaxPrimitivesCount: memcpy(parameter, &m_Direct3DCapabilities.MaxPrimitiveCount, sizeof(DWORD));
				break;

			case RTC_MaxLights:	  memcpy(parameter, &m_Direct3DCapabilities.MaxActiveLights, sizeof(DWORD));
				break;

			case RTC_MaxRTS:		  memcpy(parameter, &m_Direct3DCapabilities.NumSimultaneousRTs, sizeof(DWORD));
				break;

			case RTC_MaxTextureHeight: memcpy(parameter, &m_Direct3DCapabilities.MaxTextureHeight, sizeof(DWORD));
				break;

			case RTC_MaxTextureWidth: memcpy(parameter, &m_Direct3DCapabilities.MaxTextureWidth, sizeof(DWORD));
				break;

			case RTC_VertexShader:  strcpy((char*)parameter, D3DXGetVertexShaderProfile(m_Direct3DDevice));
				break;

			case RTC_PixelShader:	  strcpy((char*)parameter, D3DXGetPixelShaderProfile(m_Direct3DDevice));
				break;

			default:
				break;
			}

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER
