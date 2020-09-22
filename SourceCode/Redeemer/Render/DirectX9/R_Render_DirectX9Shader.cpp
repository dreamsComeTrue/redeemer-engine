/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_DirectX9Shader.cpp
**		DirectX 9 shader implementation
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_DirectX9Shader.h"	
#include "R_Render_DirectX9RenderDevice.h"
#include "..\\..\\Core\\R_Core_Convert.h"
#include "..\\..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace RENDER
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_DirectX9Shader::C_DirectX9Shader (ERendererShaderType type) :
			C_Shader (type)
		{
			m_VertexShader = NULL;
			m_VertexShaderConstTable = NULL;
			m_PixelShader = NULL;
			m_PixelShaderConstTable = NULL;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_DirectX9Shader::~C_DirectX9Shader ()
		{
			REDEEMER_SAFE_RELEASE (m_VertexShader);
			REDEEMER_SAFE_RELEASE (m_VertexShaderConstTable);
			REDEEMER_SAFE_RELEASE (m_PixelShader);
			REDEEMER_SAFE_RELEASE (m_PixelShaderConstTable);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9Shader::BindForRendering ()
		{
			C_DirectX9RenderDevice* device = static_cast<C_DirectX9RenderDevice*>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice());

			if (m_Type == RST_VertexShader)
			{
				if (m_VertexShader)
					device->GetDirectX9Device()->SetVertexShader (m_VertexShader);
			}

			if (m_Type == RST_PixelShader)
			{
				if (m_PixelShader)
					device->GetDirectX9Device()->SetPixelShader (m_PixelShader);
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9Shader::UnbindFromRendering ()
		{
			C_DirectX9RenderDevice* device = static_cast<C_DirectX9RenderDevice*>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice());

			if (m_Type == RST_VertexShader)
			{
				if (m_VertexShader)
					device->GetDirectX9Device()->SetVertexShader (NULL);
			}

			if (m_Type == RST_PixelShader)
			{
				if (m_PixelShader)
					device->GetDirectX9Device()->SetPixelShader (NULL);
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_DirectX9Shader::CreateFromData (const char* shaderData, unsigned int size, const char* mainFunction)
		{
			C_DirectX9RenderDevice* device = static_cast<C_DirectX9RenderDevice*>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice());
			ID3DXBuffer* shader	= NULL;
			ID3DXBuffer* errorBuffer = NULL;

			if (!device)
				return false;

			if ((!shaderData) || (!mainFunction))
				return false;

			typedef HRESULT (WINAPI *D3DXCompileShaderFunction)(LPCSTR pSrcFile, UINT srcDataLen, CONST D3DXMACRO* pDefines, LPD3DXINCLUDE pInclude, 
				LPCSTR pFunctionName,LPCSTR pProfile, DWORD Flags, LPD3DXBUFFER* ppShader, LPD3DXBUFFER* ppErrorMsgs, LPD3DXCONSTANTTABLE* ppConstantTable);

			static bool loadFailed = false;
			static D3DXCompileShaderFunction compileFunction = 0;

			if (loadFailed)
				return false;

			//	Lets try to load DLL
			if (!compileFunction && !loadFailed)
			{
				std::string DLLName;
				std::wstring out;

				DLLName = "d3dx9_";
				DLLName += CORE::C_Convert::ConvertIntegerToString ((int)D3DX_SDK_VERSION);
				DLLName += ".dll";

				CORE::C_Convert::ConvertCharsToUnicode (&out, DLLName);

				HMODULE hMod = LoadLibrary(out.c_str ());

				if (hMod)
					compileFunction = (D3DXCompileShaderFunction)GetProcAddress(hMod, "D3DXCompileShader");

				if (!compileFunction)
				{
					loadFailed = true;

					REDEEMER_LOG << LOG_ERROR << L"RENDER: Could not load shader function D3DXCompileShader! Shaders disabled." << LOG_ENDMESSAGE;

					return false;
				}
			}

			HRESULT result = S_OK;
			std::string typeText = "";

			DWORD dwFlags = 0;

#if defined( _DEBUG ) || defined( DEBUG )
			dwFlags |= D3DXSHADER_DEBUG;
#endif

			//	We are creating vertex shader
			if (m_Type == RST_VertexShader)
			{
				int a =		  strlen(shaderData);
				result = (*compileFunction)(shaderData, size, NULL, NULL, mainFunction, 
					D3DXGetVertexShaderProfile (device->GetDirectX9Device()), dwFlags, &shader, &errorBuffer, &m_VertexShaderConstTable);

				typeText = "vertex";
			}

			//	We are creating pixel shader
			if (m_Type == RST_PixelShader)
			{
				result = (*compileFunction)(shaderData, size, NULL, NULL, mainFunction, 
					D3DXGetPixelShaderProfile (device->GetDirectX9Device()), dwFlags, &shader, &errorBuffer, &m_PixelShaderConstTable);

				typeText = "pixel";
			}

			if (errorBuffer)
			{
				REDEEMER_LOG << LOG_ERROR << "Render: Can't compile " << typeText << " shader code! -> " <<   (char*)errorBuffer->GetBufferPointer() << LOG_ENDMESSAGE;

				errorBuffer->Release ();

				return false;
			}

			if (FAILED(result))
			{
				REDEEMER_LOG << LOG_ERROR << "Render: D3DXCompileShader failed!" << LOG_ENDMESSAGE;

				return false;
			}

			result = S_OK;

			if (m_Type == RST_VertexShader)
			{
				result = device->GetDirectX9Device()->CreateVertexShader ((DWORD*)shader->GetBufferPointer(), &m_VertexShader);

				if (FAILED (result))
				{
					REDEEMER_LOG << LOG_ERROR << "Render: Can't create vertex shader!" << LOG_ENDMESSAGE;

					return false;
				}

				shader->Release ();
			}

			if (m_Type == RST_PixelShader)
			{
				result = device->GetDirectX9Device()->CreatePixelShader ((DWORD*)shader->GetBufferPointer(), &m_PixelShader);

				if (FAILED (result))
				{
					REDEEMER_LOG << LOG_ERROR << "Render: Can't create pixel shader!" << LOG_ENDMESSAGE;

					return false;
				}

				shader->Release ();
			}			

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		char* C_DirectX9Shader::GetConstant (char* name)
		{
			char* h;

			if (m_Type == RST_VertexShader)
			{
				h = (char*)(m_VertexShaderConstTable->GetConstantByName (NULL, name));
			}

			if (m_Type == RST_PixelShader)
			{
				h = (char*)(m_PixelShaderConstTable->GetConstantByName (NULL, name));
			}

			if (!h)
			{
				REDEEMER_LOG << LOG_ERROR << "Render: Can't find constant: '" << name << "' in HLSL code!" << LOG_ENDMESSAGE;
			}

			return h;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9Shader::SetData (const char* handleName, float* data, unsigned int size)
		{
			C_DirectX9RenderDevice* device = static_cast<C_DirectX9RenderDevice*>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice());

			if (m_Type == RST_VertexShader)
			{
				if (FAILED(m_VertexShaderConstTable->SetFloatArray (device->GetDirectX9Device (), (D3DXHANDLE)handleName, data, size)))
				{
					REDEEMER_LOG << LOG_ERROR << "Render: Can not set vertex shader variable: '" << handleName << "' !" << LOG_ENDMESSAGE;
				}
			} else
			if (m_Type == RST_PixelShader)
			{
				if (FAILED(m_PixelShaderConstTable->SetFloatArray (device->GetDirectX9Device (), (D3DXHANDLE)handleName, data, size)))
				{
					REDEEMER_LOG << LOG_ERROR << "Render: Can not set pixel shader variable: '" << handleName << "' !" << LOG_ENDMESSAGE;
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9Shader::SetData (const char* handleName, float data)
		{
			C_DirectX9RenderDevice* device = static_cast<C_DirectX9RenderDevice*>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice());

			if (m_Type == RST_VertexShader)
			{
				if (FAILED(m_VertexShaderConstTable->SetFloat (device->GetDirectX9Device (), (D3DXHANDLE)handleName, data)))
				{
					REDEEMER_LOG << LOG_ERROR << "Render: Can not set vertex shader variable: '" << handleName << "' !" << LOG_ENDMESSAGE;
				}
			} else
				if (m_Type == RST_PixelShader)
				{
					if (FAILED(m_PixelShaderConstTable->SetFloat (device->GetDirectX9Device (), (D3DXHANDLE)handleName, data)))
					{
						REDEEMER_LOG << LOG_ERROR << "Render: Can not set pixel shader variable: '" << handleName << "' !" << LOG_ENDMESSAGE;
					}
				}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9Shader::SetData (const char* handleName, int data)
		{
			C_DirectX9RenderDevice* device = static_cast<C_DirectX9RenderDevice*>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice());

			if (m_Type == RST_VertexShader)
			{
				if (FAILED(m_VertexShaderConstTable->SetInt (device->GetDirectX9Device (), (D3DXHANDLE)handleName, data)))
				{
					REDEEMER_LOG << LOG_ERROR << "Render: Can not set vertex shader variable: '" << handleName << "' !" << LOG_ENDMESSAGE;
				}
			} else
				if (m_Type == RST_PixelShader)
				{
					if (FAILED(m_PixelShaderConstTable->SetInt (device->GetDirectX9Device (), (D3DXHANDLE)handleName, data)))
					{
						REDEEMER_LOG << LOG_ERROR << "Render: Can not set pixel shader variable: '" << handleName << "' !" << LOG_ENDMESSAGE;
					}
				}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_DirectX9Shader::SetData (const char* handleName, bool data)
		{
			C_DirectX9RenderDevice* device = static_cast<C_DirectX9RenderDevice*>(C_RedeemerEngine::GetSingleton().GetRenderManager()->GetRenderDevice());

			if (m_Type == RST_VertexShader)
			{
				if (FAILED(m_VertexShaderConstTable->SetBool (device->GetDirectX9Device (), (D3DXHANDLE)handleName, data)))
				{
					REDEEMER_LOG << LOG_ERROR << "Render: Can not set vertex shader variable: '" << handleName << "' !" << LOG_ENDMESSAGE;
				}
			} else
				if (m_Type == RST_PixelShader)
				{
					if (FAILED(m_PixelShaderConstTable->SetBool (device->GetDirectX9Device (), (D3DXHANDLE)handleName, data)))
					{
						REDEEMER_LOG << LOG_ERROR << "Render: Can not set pixel shader variable: '" << handleName << "' !" << LOG_ENDMESSAGE;
					}
				}
		}

	}	//	namespace RENDER
}	//	namespace REDEEMER
