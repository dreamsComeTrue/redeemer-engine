/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_TextureSampler.h
**		Class used for handling various parameters for textures, such as addressing modes, etc.
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_RENDER_TEXTURESAMPLER_H_
#define _R_RENDER_TEXTURESAMPLER_H_

#include "..\\FileSystem\\R_FileSystem_XMLDocument.h"
#include "R_Render_Texture.h"

#include <string>
#include <vector>

namespace REDEEMER
{
	namespace RENDER
	{
		class C_RenderPass;


		/*!	Texture filtering type
		*/
		enum ETextureFilteringType
		{
			TFT_Min,
			TFT_Mag,
			TFT_Mip
		};

		/*!	Texture filtering mode
		*/
		enum ETextureFilteringMode
		{
			TFM_Point,
			TFM_Linear,
			TFM_Anisotropic
		};

		/*!	Texture addressing type
		*/
		enum ETextureAddressingType
		{
			TAT_AddressU,
			TAT_AddressV
		};

		/*!	Texture addressing mode
		*/
		enum ETextureAddressingMode
		{
			TAM_Wrap,
			TAM_Border,
			TAM_Clamp,
			TAM_Mirror
		};

		/*!	\brief   Class used for handling various parameters for textures, such as addressing modes, etc.
		*/
		class C_TextureSampler
		{
			friend class C_RenderPass;

		public:
			/*!	Constructor
			*/
			C_TextureSampler ();

			/*!	Destructor
			*/
			virtual ~C_TextureSampler ();

			/*!	Set filter state
			*/
			virtual void SetFilterState (ETextureFilteringType type, ETextureFilteringMode mode);

			/*!	Set addressing state
			*/
			virtual void SetAddressingState (ETextureAddressingType type, ETextureAddressingMode mode);

			/*!	Bind the texture to this sampler
			*/
			void SetTexture (C_Texture* texture);

			/*!	Gets data from texture
			*/
			C_Texture* GetTexture ();

			/*!	Use this sampler for rendering process
			*/
			virtual void BindForRendering () = 0;

			/*!	Unbind this sampler from rendering process
			*/
			virtual void UnbindFromRendering () = 0;

			/*!	Should bounded texture be always used as normal target in binding?
			*/
			bool IsForceSet() const;

		protected:
			/*!	Parses XML data describing technique
			*/
			void Parse (FILESYSTEM::C_XMLDocument::C_XMLNode* xmlFile);

			ETextureFilteringMode ParseFiltering(const std::string& data);
			ERendererTextureFormat ParseFormat(const std::string& data);

			C_Texture*					m_Texture;			///<	Internal texture object
			unsigned int				m_TextureStage;		///<	Stage, this texture is assigned to
			bool						m_ForceSet;

			ETextureFilteringMode		m_MinFilteringMode;
			ETextureFilteringMode		m_MagFilteringMode;
			ETextureFilteringMode		m_MipFilteringMode;

			ETextureAddressingMode		m_AddressingModeU;
			ETextureAddressingMode		m_AddressingModeV;
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_TEXTURESAMPLER_H_

