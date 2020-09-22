/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_RenderTechnique.h
**		Render technique contains multiple passes for material
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_RENDER_RENDERTECHNIQUE_H_
#define _R_RENDER_RENDERTECHNIQUE_H_

#include <string>
#include <vector>

#include "..\\FileSystem\\R_FileSystem_XMLDocument.h"

namespace REDEEMER
{
	namespace RENDER
	{
		class C_Material;
		class C_RenderPass;
		class C_RenderQueue;

		/*!	\brief Render technique contains multiple passes for material
		*/
		class C_RenderTechnique
		{
			friend class C_Material;

		public:
			/*!	Constructor
			*/
			C_RenderTechnique ();

			/*!	Destructor
			*/
			virtual ~C_RenderTechnique ();

			/*!	Sets name for this technique
			*/
			void SetName (const std::wstring& name);

			/*!	Returns technique name
			*/
			std::wstring GetName () const;

			/*!	Adds new render pass
			*/
			void AddRenderPass (C_RenderPass* pass);

			/*!	Returns render pass with given ID
			*/
			C_RenderPass* GetRenderPass (unsigned int index);

			/*!	Returns number of render passes with this technique
			*/
			unsigned int GetRenderPassesCount () const;

			/*!	Sets material for this technique
			*/
			void SetMaterial (C_Material* material);

			/*!	Returns material for this technique
			*/
			C_Material* GetMaterial () const;

			/*!	Update technique routine
			*/
			void Update ();

			/*!	Use current technique for rendering process
			*/
			void BindForRendering ();

			/*!	Unbind technique from rendering
			*/
			void UnbindFromRendering ();

		private:
			/*!	Parses XML data describing technique
			*/
			void Parse (FILESYSTEM::C_XMLDocument::C_XMLNode* xmlFile);

			std::wstring				m_Name;
			C_Material*					m_Material;
			std::vector<C_RenderPass*>	m_RenderPasses;
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_RENDERTECHNIQUE_H_

