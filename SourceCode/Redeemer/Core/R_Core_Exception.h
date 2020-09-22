/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_Exception.h
**		Exception base class
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_CORE_EXCEPTION_H_
#define _R_CORE_EXCEPTION_H_

#include <string>

//	Handy macro for throwing exceptions
#define SystemException(description) CORE::C_Excpetion (description, __FILE__, __FUNCTION__, __LINE__);

namespace REDEEMER
{
	namespace CORE
	{
		/*!
		**	\brief Base exception class - others classes should derive from this for more specialized kind of exceptions
		*/
		class C_Excpetion
		{
		public:
			/*!	Constructor
			**	\param	description description of the exception
			**	\param fileName file in which exception occurred
			**	\param function function name which thrown exception
			**	\param line number of line with exception
			*/
			C_Excpetion (const std::wstring& description, const std::string& fileName, const std::string& function, int line);

			/*!	Returns description of the exception
			*/
			std::wstring	Description() const;

			/*!	Returns file name of the exception
			*/
			std::string	File () const;

			/*!	Returns function name of the exception
			*/
			std::string	Function () const;

			/*!	Returns line number of the exception
			*/
			int			Line () const;

		private:			 
			std::wstring	m_Description;
			std::string		m_FileName;
			std::string		m_Function;
			int				m_Line;
		};
	}	//	namespace CORE
}	//	namespace REDEEMER

#endif	//	_R_CORE_EXCEPTION_H_
