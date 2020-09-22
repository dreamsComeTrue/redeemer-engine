/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_Exception.cpp
**		Exception base class
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Core_Exception.h"

namespace REDEEMER
{
	namespace CORE
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_Excpetion::C_Excpetion (const std::wstring& description, const std::string& fileName, const std::string& function, int line) :
			m_Description (description),
			m_FileName (fileName),
			m_Function (function),
			m_Line (line)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::wstring C_Excpetion::Description() const
		{
			return m_Description;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::string C_Excpetion::File () const
		{
			return m_FileName;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::string C_Excpetion::Function () const
		{
			return m_Function;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int	C_Excpetion::Line () const
		{
			return m_Line;
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace CORE
}	//	namespace REDEEMER
