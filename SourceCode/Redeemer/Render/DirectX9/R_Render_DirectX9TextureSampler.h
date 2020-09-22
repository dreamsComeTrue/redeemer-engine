/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_DirectX9TextureSampler.h
**		Class used for handling various parameters for textures in DirectX 9, such as addressing modes, etc.
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_RENDER_DIRECTXTEXTURESAMPLER_H_
#define _R_RENDER_DIRECTXTEXTURESAMPLER_H_

#include "..\\R_Render_TextureSampler.h"

namespace REDEEMER
{
	namespace RENDER
	{
		/*!	\brief   Class used for handling various parameters for textures in DirectX 9, such as addressing modes, etc.
		*/
		class C_DirectX9TextureSampler : public C_TextureSampler
		{
		public:
			/*!	Constructor
			*/
			C_DirectX9TextureSampler ();

			/*!	Destructor
			*/
			virtual ~C_DirectX9TextureSampler ();

			/*!	Set filter state
			*/
			virtual void SetFilterState (ETextureFilteringType type, ETextureFilteringMode mode);

			/*!	Set addressing state
			*/
			virtual void SetAddressingState (ETextureAddressingType type, ETextureAddressingMode mode);

			/*!	Use this sampler for rendering process
			*/
			virtual void BindForRendering ();

			/*!	Unbind this sampler from rendering process
			*/
			virtual void UnbindFromRendering ();
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_DIRECTXTEXTURESAMPLER_H_

