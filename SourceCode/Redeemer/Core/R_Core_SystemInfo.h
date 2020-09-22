/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_SystemInfo.h
**		Provides some utilities to get info about system.
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_CORE_SYSTEMINFO_H_
#define _R_CORE_SYSTEMINFO_H_

#include <windows.h>
#include <winbase.h>

#define CPU_FEATURE_MMX		0x00800000
#define CPU_FEATURE_RDTSC	0x00000010
#define CPU_FEATURE_3DNOW	0x80000000

namespace REDEEMER
{
	namespace CORE
	{
		/*!
		**	\brief Provides some utilities to get info about system.
		*/
		class C_SystemInfo
		{
		public:
			/*!	Constructor
			*/
			C_SystemInfo ();

			/*!	Destructor
			*/
			~C_SystemInfo ();

			/*!	The get speed of the users CPU
			*/
			int GetCPUSpeed ();

			/*!	Get the capabilities of the users CPU
			*/
			DWORD GetCPUCaps ();

			/*!	Does the computer have MMX
			*/
			bool HasMMX ();

			/*!	Get the current windows version
			*/
			void GetWindowsVersion ();

			/*!	Log the users CPU characteristic
			*/
			void LogSystemInfo();

		private:
			DWORD			m_CPUCaps;		// The capabilities of the CPU
			int				m_CPUSpeed;		// The speed of the CPU
			bool			m_HasMMX;		// Does the CPU have MMX instructions
			OSVERSIONINFO	m_OSInfo;		// The OS info
			MEMORYSTATUS	m_MemInfo;		// The memory info
		};

	}	//	namespace CORE
}	//	namespace REDEEMER

#endif	//	_R_CORE_SYSTEMINFO_H_
