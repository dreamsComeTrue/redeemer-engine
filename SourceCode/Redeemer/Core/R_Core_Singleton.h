/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_Singleton.h
**		Implementation of singleton design pattern
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_CORE_SINGLETON_H_
#define _R_CORE_SINGLETON_H_

#include <cassert>

namespace REDEEMER
{
	namespace CORE
	{
		/*!
		**	\brief Singleton is used, when only one instance of given object is required. When you attempt to create
		**	more than one, an error is raised
		**	\remarks
		**	If you want to create a singleton object - you should derive from this class
		*/
		template <typename T> class C_Singleton
		{
		public:
			//------------------------------------------------------------------------------------------------------------------------
			///	Constructor - creates singleton of type T
			C_Singleton ()
			{
				assert (!s_Singleton && "Singleton class already instantiated.");

				s_Singleton = static_cast<T*>(this);
			}

			//------------------------------------------------------------------------------------------------------------------------

			///	Destructor
			virtual ~C_Singleton ()
			{
				assert (s_Singleton && "Singleton class not instantiated.");

				s_Singleton = 0;
			}

			//------------------------------------------------------------------------------------------------------------------------

			/*! Returns reference to a singleton.
			*/
			static T& GetSingleton ()
			{
				assert (s_Singleton && "Singleton class not instantiated.");

				return (*s_Singleton);
			}

			//------------------------------------------------------------------------------------------------------------------------

			/*! Returns pointer to a singleton.
			*/
			static T* GetSingletonPointer ()
			{
				return s_Singleton;
			}

			//------------------------------------------------------------------------------------------------------------------------

		public:
			/*!	This static field is "heart" of the singleton - it allows to exist only one copy 
			**	of class
			*/
			static T* s_Singleton;
		};

		/*!	Static fields needs to be initialized outside the class
		*/
		template <typename T> T* C_Singleton <T>::s_Singleton = 0;

	}	//	namespace CORE
}	//	namespace REDEEMER

#endif	//	_R_CORE_SINGLETON_H_
