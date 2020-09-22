/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_VertexBufferManager.h
**		VB Manager keeps tracks about all created Vertex Buffers
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_RENDER_VERTEXBUFFERMANAGER_H_
#define _R_RENDER_VERTEXBUFFERMANAGER_H_

#include "..\\Core\\R_Core_BaseClass.h"
#include "..\\Core\\Math\\R_Core_Vector2D.h"
#include "..\\Core\\Math\\R_Core_Vector3D.h"

#include <map>
#include <vector>
#include <windows.h>

namespace REDEEMER
{
	namespace RENDER
	{	
		class C_VertexBuffer;
		class C_IndexBuffer;
		class C_VertexFormat;
		class C_RenderDeviceFactory;
		struct S_VertexFormatDescription;

		/*!	Valid vertex Format flags
		*/
		enum EVertexFormatFlags
		{
			VFF_XYZ           = 0x0001, /** Untransformed XYZ value set. */
			VFF_XYZRHW        = 0x0002, /** transformed XYZRHW value set. */
			VFF_Normal        = 0x0004, /** XYZ normal value set */
			VFF_Diffuse       = 0x0008, /** RGBA diffuse color value set */
			VFF_TexCoord     = 0x0010, /** Texture vertex flag. */
		};

		/*!	Vertex with position only
		*/
		struct S_Vertex
		{
			CORE::C_Vector3D<float> Position;	///<	3D position
		};

		/*!	Simple 3D vertex with color
		*/
		struct S_VertexColor
		{
			CORE::C_Vector3D<float> Position;	///<	3D position
			unsigned long			Color;		///<	Vertex color
		};

		/*!	Vertex with color and normal
		*/
		struct S_VertexNormalColor
		{
			CORE::C_Vector3D<float> Position;	///<	3D position
			CORE::C_Vector3D<float> Normal;		///<	3D normal
			unsigned long			Color;		///<	Vertex color
		};

		/*!	Vertex with color, and texture coordinates
		*/
		struct S_VertexColorTexture
		{
			CORE::C_Vector3D<float> Position;	///<	3D position
			CORE::C_Vector2D<float> TexCoord;	///<	Texture coordinates
			unsigned long			Color;		///<	Vertex color
		};

		/*!	Vertex with texture coordinates
		*/
		struct S_VertexTexture
		{
			CORE::C_Vector3D<float> Position;	///<	3D position
			CORE::C_Vector2D<float> TexCoord;	///<	Texture coordinates
		};

		/*!	Vertex with texture coordinates and normal
		*/
		struct S_VertexNormalTexture
		{
			CORE::C_Vector3D<float> Position;	///<	3D position
			CORE::C_Vector3D<float> Normal;		///<	3D normal
			CORE::C_Vector2D<float> TexCoord;	///<	Texture coordinates
		};

		/*!	Vertex with color, texture coordinates and normal
		*/
		struct S_VertexNormalColorTexture
		{
			CORE::C_Vector3D<float> Position;	///<	3D position
			CORE::C_Vector3D<float> Normal;		///<	3D normal
			CORE::C_Vector2D<float> TexCoord;	///<	Texture coordinates
			unsigned long			Color;		///<	Vertex color
		};

		/*!	Valid Vertex formats
		*/
		enum ERendererFVFFormat
		{
			FVF_VERTEX2TC	=	0,		// Vertex2 + Texture coord + Color.
			FVF_VERTEX3		=	1,		// Vertex3.
			FVF_VERTEX3C	=	2,	    // Vertex3 + Color.
			FVF_VERTEX3T	=	3,    	// Vertex3 + Texture coord.
			FVF_VERTEX3TC	=	4,    	// Vertex3 + Texture coord + Color.
			FVF_VERTEX3TN	=	5,		// Vertex3 + Texture coord + Normal vector.
			FVF_VERTEX3TNC	=	6		// Vertex3 + Texture coord + Normal vector + Color.
		};

		/*!
		**	\brief VB Manager keeps tracks about all created Vertex Buffers
		*/
		class C_VertexBufferManager : public CORE::C_BaseClass
		{
		public:
			/*!	Constructor
			*/
			C_VertexBufferManager ();

			/*!	Destructor
			*/
			virtual ~C_VertexBufferManager ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize ();

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Creates new Vertex Buffer
			*/
			C_VertexBuffer* CreateVertexBuffer (unsigned long verticesCount, ERendererFVFFormat vertexFormat, bool isDynamic);

			/*!	Removes Vertex Buffer from the manager
			*/
			void RemoveVertexBuffer (C_VertexBuffer* vertexBuffer);

			/*!	Creates new Index Buffer
			*/
			C_IndexBuffer* CreateIndexBuffer (unsigned long indicesCount, DWORD indexSize, bool isDynamic);

			/*!	Creates new Vertex Format declaration
			*/
			C_VertexFormat* CreateVertexFormat (unsigned int type, S_VertexFormatDescription* descriptions, unsigned int count);

			/*!	Returns Vertex Format based on it's index
			*/
			C_VertexFormat* GetVertexFormat (unsigned int type);
					
		private:
			/*!	Internally, creates vertex declarations
			*/
			void CreateVertexFormatDeclarations ();

		private:
			std::map<unsigned long, C_VertexBuffer*>	m_VertexBuffers;		///<	Managed VB's
			std::map<unsigned long, C_IndexBuffer*>		m_IndexBuffers;			///<	Managed IB's
			std::map<unsigned long, C_VertexFormat*>	m_VertexFormats;		///<	Vertex Format declarations

			unsigned long								m_VertexBufferIndex;	///<	Current VB index
			unsigned long								m_IndexBufferIndex;		///<	Current IB index
			C_RenderDeviceFactory*						m_DeviceFactory;		///<	Factory, used for creating render resources
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_VERTEXBUFFERMANAGER_H_
