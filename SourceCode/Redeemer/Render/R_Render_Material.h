/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_Material.h
**		Material controls how render techniques are processed
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_RENDER_MATERIAL_H_
#define _R_RENDER_MATERIAL_H_

#include "R_Render_Color.h"
#include "R_Render_RenderDevice.h"
#include "..\\FileSystem\\R_FileSystem_XMLDocument.h"

#include <string>

namespace REDEEMER
{
	namespace RENDER
	{	
		class C_MaterialManager;
		class C_RenderQueue;   
		class C_RenderTechnique;

		/*!	 \brief Material controls how render techniques are processed
		*/
		class C_Material
		{
			friend class C_MaterialManager;

		public:
			/*!	Constructor
			*/
			C_Material (std::wstring name);

			/*!	Destructor
			*/
			virtual ~C_Material ();

			/*!	Sets material name
			*/
			void SetName (const std::wstring& name);

			/*!	Returns material's name
			*/
			const std::wstring& GetName ();

			/*!	Sets ambient color
			*/
			void SetAmbientColor (C_Color color);

			/*!	Returns ambient color
			*/
			C_Color GetAmbientColor ();

			/*!	Sets diffuse color
			*/
			void SetDiffuseColor (C_Color color);

			/*!	Returns diffuse color
			*/
			C_Color GetDiffuseColor ();

			/*!	Sets specular color
			*/
			void SetSpecularColor (C_Color color);

			/*!	Returns specular color
			*/
			C_Color GetSpecularColor ();

			/*!	Sets emissive color
			*/
			void SetEmissiveColor (C_Color color);

			/*!	Returns emissive color
			*/
			C_Color GetEmissiveColor ();

			/*!	Sets shininess 
			*/
			void SetShininess (float shine);

			/*!	Returns shininess
			*/
			float GetShininess ();

			/*!	Sets alpha blending
			*/
			void SetAlphaBlending (bool alpha);

			/*!	Checks, if material is alpha blended
			*/
			bool GetAlphaBlending ();

			/*!	Use current material for rendering process
			*/
			virtual void BindForRendering ();

			/*!	Unbind material from rendering
			*/
			virtual void UnbindFromRendering ();

			/*!	Sets source blending mode
			*/
			void SetSourceBlendingMode (EBlendMode mode);

			/*!	Returns source blending mode
			*/
			EBlendMode GetSourceBlendingMode () const;

			/*!	Sets destination blending mode
			*/
			void SetDestinationBlendingMode (EBlendMode mode);

			/*!	Returns destination blending mode
			*/
			EBlendMode GetDestinationBlendingMode () const;

			/*!	Sets blending operation.
			*/
			void SetBlendOperation (EBlendOperation operation);

			/*!	Gets blending operation.
			*/
			EBlendOperation GetBlendOperation () const;

			/*!	Sets alpha test enabled
			*/
			void SetAlphaTestEnabled (bool enable);

			/*!	Checks, if alpha test is enabled
			*/
			bool GetAlphaTestEnabled () const;

			/*!	Sets alpha test reference value
			*/
			void SetAlphaTestReference (DWORD value);

			/*!	Returns alpha test reference value
			*/
			DWORD GetAlphaTestReference () const;

			/*!	Enables writing to Z buffer
			*/
			void SetZWriteEnable (bool enable);

			/*!	Checks, if writing to Z buffer is enabled
			*/
			bool GetZWriteEnable () const;

			/*!	Adds new render technique to the material.
			*/
			void AddRenderTechnique (C_RenderTechnique* technique);

			/*!	Returns render technique with the given index or NULL if not found.
			*/
			C_RenderTechnique* GetRenderTechnique (unsigned int index);

			/*!	Returns render technique with the given name or NULL if not found.
			*/
			C_RenderTechnique* GetRenderTechnique (const std::wstring& name);

			/*!	Sets render technique with the given index as active.
			*/
			void SetActiveRenderTechnique (unsigned int index);

			/*!	Sets render technique for this material.
			*/
			void SetActiveRenderTechnique (C_RenderTechnique* technique);

			/*!	Returns active render technique.
			*/
			C_RenderTechnique* GetActiveRenderTechnique() const;

			/*!	Returns number of render techniques.
			*/
			unsigned int GetRenderTechniquesCount() const;

		protected:
			/*!	Parses XML data to get material description
			*/
			void Parse (FILESYSTEM::C_XMLDocument::C_XMLNode* xmlFile);

			std::wstring						m_Name;						///<	Material's name
			C_Color								m_AmbientColor;				///<	Surface's ambient color
			C_Color								m_DiffuseColor;				///<	Surface's diffuse color
			C_Color								m_SpecularColor;			///<	Surface's specular color
			C_Color								m_EmissiveColor;			///<	Surface's emissive color
			float								m_Shininess;				///<	Surface's shininess
			bool								m_AlphaBlending;			///<	Is Alpha Blend enabled?
			bool								m_AlphaTest;				///<	Is Alpha Test enabled?
			DWORD								m_AlphaTestReference;		///<	Alpha test reference value
			bool								m_ZWrite;					///<	Is Z-Write enabled

			EBlendOperation						m_BlendOperation;			///<	Blend operation
			EBlendMode							m_SourceBlend;				///<	Source blend mode
			EBlendMode							m_DestinationBlend;			///<	Destination blend mode

			std::vector <C_RenderTechnique*>	m_RenderTechniques;			///<	Techniques in which material is rendered
			C_RenderTechnique*					m_ActiveRenderTechnique;	///<	Active RenderTechnique for this material

			S_RenderState						m_PreviousState;
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_MATERIAL_H_
