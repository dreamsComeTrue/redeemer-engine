/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_RenderDevice.h
**		Generic rendering device, act as a base class for all renderers
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_RENDER_RENDERDEVICE_H_
#define _R_RENDER_RENDERDEVICE_H_

#include "R_Render_Color.h"
#include "R_Render_RenderPipeline.h"
#include "R_Render_GeometryStage.h"
#include "R_Render_LightStage.h"
#include "R_Render_GUIStage.h"
#include "R_Render_PostProcessStage.h"
#include "R_Render_VertexBufferManager.h"
#include "R_Render_TextureManager.h"
#include "R_Render_ShaderManager.h"
#include "R_Render_MaterialLibrary.h"

#include "..\\Core\\Math\\R_Core_Matrix.h"
#include "..\\Core\\Math\\R_Core_Rectangle.h"
#include "..\\Core\\\Math\\R_Core_Vector2D.h"
#include "..\\Core\\R_Core_BaseClass.h"
#include "..\\Core\\R_Core_SmartPointer.h"

#include <vector>

namespace REDEEMER
{
	namespace RENDER
	{	
		class C_RenderDeviceFactory;

		/*!	Enumeration of drawing modes
		*/
		enum EFillMode
		{
			FM_Dot			= (1<<1),
			FM_Wireframe	= (1<<2),
			FM_Solid		= (1<<3),
			FM_Textured		= (1<<4)
		};

		/*!	Culling types
		*/
		enum ECullMode
		{
			CM_None,	///<	Culling turned off
			CM_CW,		///< Cull clockwise
			CM_CCW		///< Cull counter-clockwise
		};

		/*!	Valid clear flags
		*/
		enum EClearFlags
		{
			CF_BackBuffer		= 0x0001,	///<	Clear color buffer 
			CF_ZBuffer			= 0x0002,	///<	Clear z-buffer 
			CF_StencilBuffer	= 0x0004,	///<	Clear stencil buffer 
		};

		/*!	Current matrix mode in the render device (indicate which matrix is modified by matrix function calls).
		*/
		enum ERenderMatrixMode
		{
			RMM_Projection	= 0,	///<	Projection matrix mode 
			RMM_ModelView	= 1,	///<	Model-view matrix mode 
			RMM_World		= 2		///<	World matrix mode
		};

		/*!	Renderer can work in 3D & 2D modes
		*/
		enum ERendererMode
		{
			RM_2D,		///< 2D mode
			RM_3D		///< 3D mode
		};

		/*!	Drawing primitive types
		*/
		enum ERendererPrimitiveType
		{
			RPT_PointList,
			RPT_LineList,
			RPT_LineStrip,
			RPT_TriangleList,
			RPT_TriangleStrip,
			RPT_TriangleFan
		};

		enum ERendererTestCapabilities
		{
			RTC_Unknown,
			RTC_MaxAnisotropy,				//	Maximum anisotropy for anisotropic texture filtering 
			RTC_MinAnisotropy,				//	Anisotropy for minifying textures is supported.
			RTC_MagAnisotropy,				//	Anisotropy for magnifying textures is supported. 
			RTC_MaxClipPlanes,				//	How many clipping planes could be defined and used 
			RTC_MaxIndicesCount,			//	How many vertex indices can be used	in a buffer 
			RTC_MaxPrimitivesCount,			//	How many primitives can be used in one draw call.
			RTC_MaxLights,					//	Maximum number of active lights being active at the same time
			RTC_MaxRTS,						//	How many simultaneous render targets	can be used 
			RTC_MaxTextureHeight,			//	Maximum texture height
			RTC_MaxTextureWidth,			//	Maximum texture width
			RTC_VertexShader,				//	What version of vertex shaders is supported?
			RTC_PixelShader					//	What version of pixel shaders is supported?
		};

		/*!	Blending modes
		*/
		enum EBlendMode
		{
			BM_Zero				= 1,
			BM_One				= 2,
			BM_SrcColor			= 3,
			BM_InvSrcColor		= 4,
			BM_SrcAlpha			= 5,
			BM_InvSrcAlpha		= 6,
			BM_DestAlpha		= 7,
			BM_InvDestAlpha		= 8,
			BM_DestColor		= 9,
			BM_InvDestColor		= 10,
			BM_SrcAlphaSat		= 11,
			BM_BothSrcAlpha		= 12,
			BM_BothInvSrcAlpha	= 13,
			BM_BlendFactor		= 14,
			BM_InvBlendFactor	= 15,
		};


		/*!	Blending methods
		*/
		enum EBlendOperation
		{
			BO_Add			= 1,
			BO_Subtract		= 2,
			BO_RevSubtract	= 3,
			BO_Min			= 4,
			BO_Max			= 5
		};

		/*!	Data of rendering state
		*/
		struct S_RenderState
		{
			/*!	Constructor
			*/
			S_RenderState () :
				FillMode (FM_Solid),
				CullMode (CM_CW),
				SourceBlending (BM_One),
				DestinationBlending (BM_Zero),
				SeparateAlphaSourceBlending (BM_One),
				SeparateAlphaDestinationBlending (BM_Zero),
				SeparateAlpha (false),
				BlendOperation (BO_Add),
				ScissorTest (false),
				AlphaTest (false),
				AlphaTestReference (1),
				AlphaBlend(false),
				ZWrite (true)
			{
				//	EMPTY
			}

			/*!	Checks, if states are not equal
			*/
			bool operator!= (const S_RenderState& state)
			{
				return (FillMode != state.FillMode || CullMode != state.CullMode || SourceBlending != state.SourceBlending ||
					DestinationBlending != state.DestinationBlending || BlendOperation != state.BlendOperation || ScissorTest != state.ScissorTest ||
					SeparateAlpha != state.SeparateAlpha || SeparateAlphaSourceBlending != state.SeparateAlphaSourceBlending ||
					SeparateAlphaDestinationBlending != state.SeparateAlphaDestinationBlending || AlphaTest != state.AlphaTest ||
					AlphaTestReference != state.AlphaTestReference || AlphaBlend != state.AlphaBlend || ZWrite != state.ZWrite);
			}

		public:
			EFillMode			FillMode;							///<	Fill mode
			ECullMode			CullMode;							///<	Cull mode
			EBlendMode			SourceBlending;						///<	Source blending
			EBlendMode			DestinationBlending;				///<	Destination blending
			EBlendMode			SeparateAlphaSourceBlending;		///<	Separate Alpha source blending
			EBlendMode			SeparateAlphaDestinationBlending;	///<	Separate Alpha destination blending
			EBlendOperation		BlendOperation;						///<	Type of blend operation
			bool				ScissorTest;						///<	Is scissor test enabled
			bool				SeparateAlpha;						///<	Are we using separate alpha blending modes?
			bool				AlphaTest;							///<	Is alpha test enabled?
			DWORD				AlphaTestReference;					///<	Alpha test reference value
			bool				AlphaBlend;							///<	Is alpha blending enabled?
			bool				ZWrite;								///<	Is writing to Z Buffer enabled?
		};

		/*!	Render device video mode
		*/
		struct S_RenderVideoMode
		{
			/*!	Constructor
			*/
			S_RenderVideoMode () :
				Windowed (true),
				RenderMode (RM_3D)
			{
				ScreenResolution.X = 800;
				ScreenResolution.Y = 600;				
			}

		public:
			bool							Windowed;				///<	Windowed or Fullscreen?
			ERendererMode					RenderMode;				///<	Render mode (3D or 2D)?
			S_RenderState					RenderState;			///<	Render state
			CORE::C_Vector2D<unsigned int>	ScreenResolution;		///<	Render window resolution
			CORE::C_Rectangle<int>			ScissorRectangle;		///<	Scissor area
		};

		/*!
		**	\brief Generic rendering device, act as a base class for all renderers
		*/
		class C_RenderDevice : public CORE::C_BaseClass
		{
			friend class C_VertexBufferManager;

		public:
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
			virtual void ClearBuffers (DWORD buffers = CF_BackBuffer | CF_ZBuffer, C_Color backBufferColor = C_Color::COLOR_BLUE, float zBufferColor = 1.0f, DWORD stencilBufferColor = 0) = 0;

			/*!	Sets rendering matrix to the given type and value
			*/
			virtual void SetRenderMatrix (ERenderMatrixMode matrixMode, CORE::C_Matrix<float>& matrixData) = 0;

			/*!	Returns current values for given matrix type
			*/
			virtual CORE::C_Matrix<float> GetRenderMatrix (ERenderMatrixMode matrixMode) = 0;

			/*!	Sets render device to use windowed (or fullscreen) mode
			*/
			virtual void SetWindowedMode (bool isWindowed);

			/*!	Checks, if render device is in windowed (or fullscreen) mode
			*/
			bool GetWindowedMode () const;

			/*!	Sets filling mode
			*/
			virtual void SetFillMode (EFillMode mode);

			/*!	Gets filling method
			*/
			EFillMode GetFillMode () const;

			/*!	Sets culling mode
			*/
			virtual void SetCullingMode (ECullMode mode);

			/*!	Returns culling method
			*/
			ECullMode GetCullingMode () const;

			/*!	Enables writing to Z-Buffer
			*/
			virtual void SetZWriteEnabled (bool enabled);

			/*!	Checks, if writing to Z-Buffer is allowed
			*/
			bool IsZWriteEnabled () const;

			/*!	Sets source blending mode
			*/
			virtual void SetSourceBlendingMode (EBlendMode mode);

			/*!	Returns source blending mode
			*/
			EBlendMode GetSourceBlendingMode () const;

			/*!	Sets destination blending mode
			*/
			virtual void SetDestinationBlendingMode (EBlendMode mode);

			/*!	Returns destination blending mode
			*/
			EBlendMode GetDestinationBlendingMode () const;

			/*!	Sets blending operation.
			*/
			virtual void SetBlendOperation (EBlendOperation operation);

			/*!	Gets blending operation.
			*/
			EBlendOperation GetBlendOperation () const;

			/*!	Sets alpha test
			*/
			virtual void SetAlphaTestEnabled (bool enabled);

			/*!	Checks, if alpha test is enabled
			*/
			bool IsAlphaTestEnabled () const;

			/*!	Sets reference for alpha testing.
			*/
			virtual void SetAlphaTestReference (DWORD value);

			/*!	Returns reference value for alpha testing
			*/
			DWORD GetAlphaTestReference () const;

			/*!	Sets alpha blending
			*/
			virtual void SetAlphaBlendEnabled (bool enabled);

			/*!	Checks, if alpha blend is enabled
			*/
			bool IsAlphaBlendEnabled () const;

			/*!	Sets separate alpha blend mode enabled
			*/
			virtual void SetSeparateAlphaBlendEnabled (bool enabled);

			/*!	Checks, if separate alpha blending is enabled
			*/
			bool IsSeparateAlphaBlendEnabled () const;

			/*!	Sets separate alpha source blending mode
			*/
			virtual void SetSeparateAlphaSourceBlendingMode (EBlendMode mode);

			/*!	Returns separate alpha source blending mode
			*/
			EBlendMode GetSeparateAlphaSourceBlendingMode () const;

			/*!	Sets separate alpha destination blending mode
			*/
			virtual void SetSeparateAlphaDestinationBlendingMode (EBlendMode mode);

			/*!	Returns separate alpha destination blending mode
			*/
			EBlendMode GetSeparateAlphaDestinationBlendingMode () const;

			/*!	Sets scissor test enabled
			*/
			virtual void SetScissorTestEnable (bool scissorTest = true);

			/*!	Sets scissor rectangle
			*/
			virtual void SetScissorRectangle (CORE::C_Rectangle<int> rectangle) = 0;

			/*!	Gets if scissor test is enabled
			*/
			 bool IsScissorTestEnabled () const;

			/*!	Returns available video modes
			*/
			std::vector<S_RenderVideoMode>& GetRenderVideoModes ();

			/*!	Sets currently used video mode
			*/
			virtual void SetRenderVideoMode (S_RenderVideoMode& mode);

			/*!	Gets currently used video mode
			*/
			S_RenderVideoMode& GetRenderVideoMode ();

			/*!	Gets engine's internal state
			*/
			S_RenderState& GetRenderState ();

			/*!	Returns reference to rendering pipeline, object, which contains rendering stages
			*/
			C_RenderPipeline& GetRenderPipeline ();

			/*!	Updates rendering pipeline
			*/
			void UpdatePipeline ();

			/*!	Sets render mode: 2D or 3D
			*/
			void SetRenderMode (ERendererMode mode);

			/*!	Process geometry to be rendered
			*/
			virtual void Render () = 0;

			/*!	Actual rendering routine
			*/
			virtual bool DrawPrimitives (C_VertexBuffer* vb) = 0;

			/*!	Actual rendering routine
			*/
			virtual bool DrawPrimitives (C_VertexBuffer* vb, unsigned int startVertex, unsigned int numVertices, unsigned int startIndex, unsigned int primitivesCount) = 0;

			/*!	Returns Render Device Factory
			*/
			C_RenderDeviceFactory* GetRenderDeviceFactory (); 

			/*!	Returns Vertex Buffer manager
			*/
			C_VertexBufferManager* GetVertexBufferManager ();

			/*!	Returns Texture manager
			*/
			C_TextureManager* GetTextureManager ();

			/*!	Returns Shader manager
			*/
			C_ShaderManager* GetShaderManager ();

			/*!	Returns Material library
			*/
			C_MaterialLibrary* GetMaterialLibrary ();

			/*!	Test some renderer internal capabilities
			*/
			virtual bool TestRendererCapabilities (ERendererTestCapabilities capabilities, void* parameter) = 0;

		protected:
			/*!	Internal, creates factory for creating rendering resources, like VB's, IB's, textures, etc.
			*/
			virtual void CreateRenderDeviceFactory () = 0;

			//	Render pipeline
			CORE::C_SmartPointer<C_RenderPipeline>		m_RenderPipeline;		///<	Holder for render stages			
			CORE::C_SmartPointer<C_GeometryStage>		m_GeometryStage;		///<	Stage 1 - Geometry
			CORE::C_SmartPointer<C_LightStage>			m_LightStage;			///<	Stage 2 - Light
			CORE::C_SmartPointer<C_GUIStage>			m_GUIStage;				///<	Stage 3 - GUI
			CORE::C_SmartPointer<C_PostProcessStage>	m_PostProcessStage;		///<	Stage 4 - Post Processing

			CORE::C_SmartPointer<C_VertexBufferManager>	m_VertexBufferManager;	///<	VB Manager
			CORE::C_SmartPointer<C_TextureManager>		m_TextureManager;		///<	Texture Manager
			CORE::C_SmartPointer<C_ShaderManager>		m_ShaderManager;		///<	Shader Manager
			CORE::C_SmartPointer<C_MaterialLibrary>		m_MaterialLibrary;		///<	Material library

			C_RenderDeviceFactory*						m_DeviceFactory;		///<	Device's factory

			S_RenderVideoMode							m_RenderVideoMode;		///<	Description of engine's internal state
			std::vector<S_RenderVideoMode>				m_VideoModes;			///<	All available modes

			//	These matrices are used to restore transformations when switching back to 3d mode
			CORE::C_Matrix<float>						m_OldMatrixWorld;		///<	Old World matrix
			CORE::C_Matrix<float>						m_OldMatrixView;		///<	Old View matrix
			CORE::C_Matrix<float>						m_OldMatrixProjection;	///<	Old projection matrix
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_RENDERDEVICE_H_
