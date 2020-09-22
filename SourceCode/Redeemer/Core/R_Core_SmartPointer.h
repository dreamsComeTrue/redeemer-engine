/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_SmartPointer.h
**		Implementation of smart pointer pattern
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_CORE_SMARTPOINTER_H_
#define _R_CORE_SMARTPOINTER_H_

#include <map>

namespace REDEEMER
{
	namespace CORE
	{
		/*!
		**	\brief Smart pointers should be used instead of regular pointers because they are safier in memory management.
		**	Since they are created on the stack rather on the heap it is easier to release memory occupied by object
		**	when pointer goes out of the scope.
		*/
		template <typename T> class C_SmartPointer
		{
		public:
			//------------------------------------------------------------------------------------------------------------------------

			/*!	Constructor
			**	\param pointer pointer to hold inside
			*/
			C_SmartPointer (T* pointer)
			{
				InitializePointer (pointer);
			}

			//	Constructor - empty
			C_SmartPointer() :
				m_PointerObject (NULL)	
			{
				//	EMPTY
			}

			/*!	Copy constructor
			**	\param pointer pointer to copy from
			*/
			C_SmartPointer(const C_SmartPointer<T>& pointer)
			{
				InitializePointer (pointer->m_PointerObject);
			}

			/*!	Assignment operator
			**	\param rhs pointer to copy from
			*/
			C_SmartPointer<T>& operator=(const C_SmartPointer<T>& rhs)
			{
				if (this == &rhs)
					return (*this);

				FinalizePointer();
				InitializePointer (rhs.m_PointerObject);

				return (*this);
			}

			/*!	Assignment operator
			**	\param rhs raw pointer to assign
			*/
			C_SmartPointer<T>& operator=(T* rhs)
			{
				if (m_PointerObject == rhs)
					return (*this);

				if (rhs)
				{
					if (m_PointerObject)
						FinalizePointer ();

					InitializePointer (rhs);
				}
				else
					FinalizePointer ();

				return (*this);
			}

			///	Destructor
			virtual ~C_SmartPointer ()
			{
				FinalizePointer ();
			}

			///	Returns held pointer
			operator T*() const
			{
				return (m_PointerObject);
			}

			///	Returns reference to pointer
			const T& operator*() const
			{
				return (*m_PointerObject);
			}

			///	Returns held pointer
			const T* operator->() const
			{
				return (m_PointerObject);
			}

			/*!	Equality operator
			**	\param rhs pointer to compare
			**	\return True, if pointers are the same
			*/
			bool operator==(const C_SmartPointer<T>& rhs)
			{
				return (m_PointerObject == rhs.m_PointerObject);
			}

			/*!	Inequality operator
			**	\param rhs pointer to compare
			**	\return True, if pointers are NOT the same
			*/
			bool operator!=(const C_SmartPointer<T>& rhs)
			{
				return (m_PointerObject != rhs.m_PointerObject);
			}

			/*!	Equality operator
			**	\param rhs pointer to compare
			**	\return True, if pointers are the same
			*/
			bool operator==(T* rhs)
			{
				return (m_PointerObject == rhs);
			}

			/*!	Inequality operator
			**	\param rhs pointer to compare
			**	\return True, if pointers are NOT the same
			*/
			bool operator!=(T* rhs)
			{
				return (m_PointerObject != rhs);
			}

			///	Returns reference to pointer
			T& operator*()
			{
				return (*m_PointerObject);
			}

			///	Returns held pointer
			T* operator->()
			{
				return (m_PointerObject);
			}

			/*!	Sets new pointer
			**	\param pointer pointer to point to
			*/
			void SetPointer (T* pointer)
			{
				if (m_PointerObject == pointer)
					return;

				if (pointer)
				{
					if (m_PointerObject)
						FinalizePointer ();

					InitializePointer (pointer);
				}
				else
					FinalizePointer ();
			}

			/*!	Returns pointer to an object
			**	\return
			**	Pointer to object
			*/
			T* GetPointer() const 
			{
				return m_PointerObject;
			}

			/*!	Returns reference to an object
			**	\return
			**	Reference to object
			*/
			T& GetReference() const 
			{
				return *m_PointerObject;
			}

		private:
			T*									m_PointerObject;		///<	Pointer to actual object
			static	std::map <T*, unsigned int> s_ReferenceCounter;		///<	Map containing all reference counters

			/*!	Initializes pointer
			**	\param pointer pointer to point to
			*/
			void InitializePointer (T* pointer)
			{
				m_PointerObject = pointer;

				if (s_ReferenceCounter.find(m_PointerObject) == s_ReferenceCounter.end())
					s_ReferenceCounter[m_PointerObject] = 1;
				else
					++s_ReferenceCounter[m_PointerObject];
			}

			///	Releases pointer.
			void FinalizePointer ()
			{
				if(s_ReferenceCounter.find(m_PointerObject) == s_ReferenceCounter.end())
					return;

				--s_ReferenceCounter[m_PointerObject];

				if(!s_ReferenceCounter[m_PointerObject])
				{
					// No more references, so pointer can be safely deleted
					s_ReferenceCounter.erase(m_PointerObject);

					if (m_PointerObject)
					{
						delete m_PointerObject;

						m_PointerObject = NULL;
					}
				}
			}
		};

		template <typename T> std::map <T*, unsigned int> C_SmartPointer <T>::s_ReferenceCounter;

	}	//	namespace CORE
}	//	namespace REDEEMER

#endif	//	_R_CORE_SMARTPOINTER_H_
