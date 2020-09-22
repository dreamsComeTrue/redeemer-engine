/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_BaseClass.h
**		Base class is the basic parent for almost all types in engine
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_CORE_BASECLASS_H_
#define _R_CORE_BASECLASS_H_

#define RESULT_OK	1
#define RESULT_FAIL	0

namespace REDEEMER
{
	namespace CORE
	{
		/*!
		**	\brief A C_BaseClass class is the basic parent for almost all types in engine
		*/
		class C_BaseClass
		{
		public:
			///	Constructor
			C_BaseClass ();

			///	Destructor
			virtual ~C_BaseClass ();

			/*!	\brief	Initialization code
			**	\remarks This method should be overriden in child classes
			*/
			virtual int Initialize ();

			/*!	\brief	Finalization code
			**	\remarks This method should be overriden in child classes
			*/
			virtual int Finalize ();

			/*!	\brief	Returns true, if object is initialized
			*/
			bool IsInitialized () const;

		private:
			bool	m_IsInitialized;
		};
	}	//	namespace CORE
}	//	namespace REDEEMER

#endif	//	_R_CORE_BASECLASS_H_
