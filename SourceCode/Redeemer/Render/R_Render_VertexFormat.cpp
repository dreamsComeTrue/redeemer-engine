/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Render_VertexFormat.cpp
**		We can define FVF for our vertices, so they can contain other info, like texture coordinates etc.
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Render_VertexFormat.h"

namespace REDEEMER
{
	namespace RENDER
	{	
		//------------------------------------------------------------------------------------------------------------------------
		C_VertexFormat::C_VertexFormat ()
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_VertexFormat::~C_VertexFormat ()
		{
			m_Description.clear ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_VertexFormat::AddVertexFormatDescription (S_VertexFormatDescription& description)
		{
			m_Description.push_back(description);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_VertexFormat::AddVertexFormatDescriptions (S_VertexFormatDescription* descriptions, unsigned int count)
		{
			for (unsigned int i = 0; i < count; ++ i)
				m_Description.push_back (descriptions[i]);
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace RENDER
}	//	namespace REDEEMER

