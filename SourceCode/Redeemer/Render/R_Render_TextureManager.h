/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_TextureManager.h
**		Texture Manager holds a collection of textures
*/

//------------------------------------------------------------------------------------------------------------------------

#ifndef _R_RENDER_TEXTUREMANAGER_H_
#define _R_RENDER_TEXTUREMANAGER_H_

#include "R_Render_Texture.h"
#include "..\\Core\\R_Core_BaseClass.h"

#include <string>
#include <vector>
#include <map>

namespace REDEEMER
{
	namespace FILESYSTEM
	{
		class IDataFile;
	}

	namespace RENDER
	{
		class C_TextureSampler;

		/*!	Texture Manager holds a collection of textures
		*/
		class C_TextureManager : public CORE::C_BaseClass
		{
		public:
			/*!	Constructor
			*/
			C_TextureManager ();

			/*!	Destructor
			*/
			virtual ~C_TextureManager ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize ();

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Creates new texture
			*/
			C_Texture* CreateTexture (std::wstring name = L"", ERendererTextureFormat format = RTF_A8R8G8B8, ERendererTextureType type = RTT_2D, 
				ERendererTextureUsage usage = RTU_Normal, unsigned int width = 512, unsigned int height = 512, unsigned int depth = 1);

			/*!	Creates new texture from external file
			*/
			C_Texture* CreateTexture (FILESYSTEM::IDataFile* file, std::wstring name = L"", ERendererTextureFormat format = RTF_A8R8G8B8, 
				ERendererTextureType type = RTT_2D, ERendererTextureUsage usage = RTU_Normal);

			/*!	Adds newly created texture to the collection
			*/
			void AddTexture (const std::wstring& name, C_Texture* texture);

			/*!	Gets texture with given name
			*/
			C_Texture* GetTexture (const std::wstring& name);

			/*!	Removes texture from the manager
			*/
			void RemoveTexture (const std::wstring& name);

			/*!	Removes texture from the manager
			*/
			void RemoveTexture (C_Texture* texture);

			/*!	Returns texture count
			*/
			unsigned int GetTextureCount () const;

			//--------------------------------------------  TEXTURE SAMPLERS

			//	Creates new texture sampler
			C_TextureSampler* CreateTextureSampler ();

			//	Adds newly created texture sampler to the map
			void AddTextureSampler (C_TextureSampler* sampler);

			//	Removes texture sampler from library
			void RemoveTextureSampler (int index);

			//	Searches for texture sampler with given name
			C_TextureSampler* GetTextureSampler (int index);

			//	Returns number of texture samplers
			unsigned int GetTextureSamplersCount () const;

		private:
			std::map<std::wstring, C_Texture*>			m_Textures;
			std::vector <C_TextureSampler*>				m_TextureSamplers;
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_TEXTUREMANAGER_H_
