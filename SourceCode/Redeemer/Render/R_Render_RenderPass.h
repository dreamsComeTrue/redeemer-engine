/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_RenderPass.h
**		Render pass represents settings for material (vertex + pixel shaders and textures)
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_RENDER_RENDERPASS_H_
#define _R_RENDER_RENDERPASS_H_

#include "..\\FileSystem\\R_FileSystem_XMLDocument.h"

#include <string>
#include <vector>

namespace REDEEMER
{
	namespace RENDER
	{
		class C_TextureSampler;
		class C_RenderTechnique;
		class C_ShaderEntity;

		/*!	\brief	Render pass represents settings for material (vertex + pixel shaders and textures)
		*/
		class C_RenderPass
		{
			friend class C_RenderTechnique;

		public:
			/*!	Constructor
			*/
			C_RenderPass ();

			/*!	Copy constructor
			*/
			C_RenderPass (const C_RenderPass& pass);

			/*!	Constructor with name
			*/
			C_RenderPass (const std::wstring& name);

			/*!	Destructor
			*/
			virtual ~C_RenderPass ();

			/*!	Sets pass name
			*/
			void SetName (std::wstring& name);

			/*!	Returns pass name
			*/
			std::wstring GetName () const;

			/*!	Add new texture sampler
			*/
			void AddTextureSampler (C_TextureSampler* sampler);

			/*!	Gets texture sampler with given id
			*/
			C_TextureSampler& GetTextureSampler (unsigned int index);

			/*!	Add new render target
			*/
			void AddRenderTarget (C_TextureSampler* sampler);

			/*!	Gets render target sampler with given id
			*/
			C_TextureSampler& GetRenderTarget (unsigned int index);

			/*!	Sets vertex shader
			*/
			void SetVertexShader (C_ShaderEntity* shader);

			/*!	Gets vertex shader
			*/
			C_ShaderEntity* GetVertexShader ();

			/*!	Sets pixel shader
			*/
			void SetPixelShader (C_ShaderEntity* shader);

			/*!	Gets pixel shader
			*/
			C_ShaderEntity* GetPixelShader ();

			/*!	Update shaders routine
			*/
			void UpdateShaders ();

			/*!	Use current pass for rendering process
			*/
			void BindForRendering ();

			/*!	Unbind pass from rendering
			*/
			void UnbindFromRendering ();

		private:
			/*!	Parses XML data describing technique
			*/
			void Parse (FILESYSTEM::C_XMLDocument::C_XMLNode* xmlFile);

		private:
			std::wstring					m_Name;				///<	Pass name
			std::vector<C_TextureSampler*>	m_TextureSamplers;	///<	Render pass can have multiple texture samplers
			std::vector<C_TextureSampler*>	m_RenderTargets;	///<	Render pass can also have render targets
			C_ShaderEntity*					m_VertexShader;		///<	Associated vertex shader
			C_ShaderEntity*					m_PixelShader;		///<	Associated pixel shader
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_RENDERPASS_H_

