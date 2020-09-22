/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_VertexFormat.h
**		We can define FVF for our vertices, so they can contain other info, like texture coordinates etc.
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_RENDER_VERTEXFORMAT_H_
#define _R_RENDER_VERTEXFORMAT_H_

#include <vector>

namespace REDEEMER
{
	namespace RENDER
	{	
		/*!	Component of the vertex
		*/
		enum EVertexElementType
		{
			VET_Posiition,
			VET_Normal,
			VET_TexCoord,
			VET_Color,
			VET_Binormal,
			VET_Tangent,
			VET_Fog,
			VET_Depth
		};

		/*!	Description of FVF
		*/
		struct S_VertexFormatDescription
		{
			int						Stream;		///<	Stream ID
			unsigned int			Offset;		///<	Offset 
			unsigned int			Index;		///<	Index number
			EVertexElementType		Type;		///<	Type of element
			int						Size;		///<	Size of the element
		};

		/*!
		**	\brief We can define FVF for our vertices, so they can contain other info, like texture coordinates etc.
		*/
		class C_VertexFormat
		{
		public:
			/*!	Constructor
			*/
			C_VertexFormat ();

			/*!	Destructor
			*/
			virtual ~C_VertexFormat ();

			/*!	Adds new description to the format
			*/
			virtual void AddVertexFormatDescription (S_VertexFormatDescription& description);

			/*!	Adds variable size descriptions to the format
			*/
			virtual void AddVertexFormatDescriptions (S_VertexFormatDescription* descriptions, unsigned int count);

			/*!	Uses this format for rendering purposes
			*/
			virtual void BindForRendering () = 0;

		protected:
			std::vector<S_VertexFormatDescription>	m_Description;	///<	Our Vertex Format is a collection of vertex descriptions
		};

	}	//	namespace RENDER
}	//	namespace REDEEMER

#endif	//	_R_RENDER_VERTEXFORMAT_H_
