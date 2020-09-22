/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_DirectX9Material.h
**		DirectX9 Material implementation
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_RENDER_DIRECTX9MATERIAL_H_
#define _R_RENDER_DIRECTX9MATERIAL_H_

#include "..\\R_Render_Material.h"

#include <d3d9.h>

namespace REDEEMER
{
	namespace RENDER
	{	
		/*!	 \brief DirectX9 Material implementation
		*/
		class C_DirectX9Material : public C_Material
		{
		public:
			/*!	Constructor
			*/
			C_DirectX9Material (std::wstring name);

			/*!	Destructor
			*/
			virtual ~C_DirectX9Material ();
			
			/*!	Use current material for rendering process
			*/
			virtual void BindForRendering ();

			/*!	Unbind material from rendering
			*/
			virtual void UnbindFromRendering ();

		private:
			D3DMATERIAL9			m_Material;
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_DIRECTX9MATERIAL_H_
