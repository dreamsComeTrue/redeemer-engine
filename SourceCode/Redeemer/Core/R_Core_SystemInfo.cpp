/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_SystemInfo.cpp
**		Provides some utilities to get info about system.
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Core_SystemInfo.h"
#include "..\\RedeemerEngine.h"

namespace REDEEMER
{
	namespace CORE
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_SystemInfo::C_SystemInfo ()
		{
			// Get the computer's capabilities and speed
			m_CPUSpeed = GetCPUSpeed ();
			m_CPUCaps = GetCPUCaps ();
			m_HasMMX = (GetCPUCaps () & CPU_FEATURE_MMX) != 0;

			GetWindowsVersion ();
			GlobalMemoryStatus (&m_MemInfo);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_SystemInfo::~C_SystemInfo ()
		{
			 //	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_SystemInfo::GetCPUSpeed ()
		{
			DWORD			dwFeatures;
			DWORD			dwStartTime;
			DWORD			dwStopTime;
			__int64			i64StartTicks;
			__int64			i64EndTicks;
			__int64			i64TotalTicks;
			int				iSpeed;
							
			// Get the capabilities of the cpu.
			dwFeatures = GetCPUCaps ();

			// The rdtsc instruction must be available.
			if (!(dwFeatures & CPU_FEATURE_RDTSC))
				return -1;						 

			// Get the start time.
			dwStartTime = timeGetTime ();

			//
			// Wait for a new millisecond to start.
			//
			do
			{
				dwStopTime = timeGetTime ();
			}
			while (( dwStopTime - dwStartTime) == 0);

			// Use the new start time.
			dwStartTime = dwStopTime;		 

			//
			// Get the start tick count.
			//
			__asm
			{
				//
				// Save registers.
				//
				push	eax
					push	ebx
					push	edx

					//
					// Get the tick count.
					//
					rdtsc
					lea		ebx, i64StartTicks
					mov		[ebx], eax
					mov		[ebx + 4], edx

					// 
					// Restore registers.
					//
					pop		edx
					pop		ebx
					pop		eax
			}


			//
			// Loop till time´s up.
			//
			while (true)
			{		
				// If one second is over ...
				if ((timeGetTime() - dwStartTime) >= 1000)
				{
					//
					// ... get the end tick count.
					//
					__asm
					{
						//
						// Save registers.
						//
						push	eax
							push	ebx
							push	edx

							//
							// Get the tick count.
							//
							rdtsc
							lea		ebx, i64EndTicks
							mov		[ebx], eax
							mov		[ebx + 4], edx

							// 
							// Restore registers.
							//
							pop		edx
							pop		ebx
							pop		eax
					}

					break;	
				}	
			}

			// Calculate the difference in clock ticks.
			i64TotalTicks = i64EndTicks - i64StartTicks;

			// Calculate the speed in Mhz.
			iSpeed = (int)(i64TotalTicks / 1000000);		

			// Return the speed in Mhz.
			return iSpeed;
		}

		//------------------------------------------------------------------------------------------------------------------------

		DWORD C_SystemInfo::GetCPUCaps ()
		{
			SYSTEM_INFO		si;
			DWORD			dwFeatures;
							  
			// Assume no features are present.
			dwFeatures = 0;

			// Get the system information.
			GetSystemInfo (&si);

			// If this is at least a pentium or compatibel ...
			if (si.dwProcessorType != PROCESSOR_INTEL_386 && si.dwProcessorType != PROCESSOR_INTEL_486)
			{
				//
				// ... get the features.
				//
				__asm
				{
					//
					// Save registers.
					//
					push	eax
						push	ebx
						push	ecx
						push	edx

						//
						// Get the features.
						//
						mov		eax, 1
						cpuid
						mov		dwFeatures, edx

						//
						// Restore registers.
						//
						pop		edx
						pop		ecx
						pop		ebx
						pop		eax
				}
			}

			// Return the features.
			return dwFeatures;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_SystemInfo::HasMMX ()
		{
			return m_HasMMX;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_SystemInfo::GetWindowsVersion ()
		{
			// Set the size of the struct
			m_OSInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

			// Get the version
			GetVersionEx (&m_OSInfo);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_SystemInfo::LogSystemInfo()
		{
			/* Write the CPU stat to the log file */

			REDEEMER_LOG << LOG_INFO << L"----------------- System -------------------\n" << LOG_ENDLINE;

			// Check for OS version
			if ((m_OSInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) && (m_OSInfo.dwMajorVersion == 4) 
				&& (m_OSInfo.dwMinorVersion == 0))
			{
				REDEEMER_LOG << LOG_INFO << L"Windows 95 " << m_OSInfo.szCSDVersion << " " << m_OSInfo.dwBuildNumber << LOG_ENDLINE;
			}
			else if((m_OSInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) && (m_OSInfo.dwMajorVersion == 4) 
				&& (m_OSInfo.dwMinorVersion > 0))
			{
				REDEEMER_LOG << LOG_INFO << L"Windows 98 " << m_OSInfo.szCSDVersion << " " << m_OSInfo.dwBuildNumber << LOG_ENDLINE;
			}
			else if(m_OSInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
			{
				REDEEMER_LOG << LOG_INFO << L"Windows NT " << m_OSInfo.dwMajorVersion << " " << m_OSInfo.szCSDVersion << " " << m_OSInfo.dwBuildNumber << LOG_ENDLINE;
			}
			else
			{
				REDEEMER_LOG << LOG_INFO << L"Unidentified OS: " << m_OSInfo.dwPlatformId << " " << m_OSInfo.dwMajorVersion << " " << m_OSInfo.dwMinorVersion << 
					" " << m_OSInfo.szCSDVersion << " " << m_OSInfo.dwBuildNumber << LOG_ENDLINE;
			}

			// Write memory info to the log
			REDEEMER_LOG << LOG_INFO << "Physical memory: " << m_MemInfo.dwTotalPhys / 1024 / 1024 << " Mb" << LOG_ENDLINE;
			REDEEMER_LOG << LOG_INFO << "Available memory: " << m_MemInfo.dwAvailPhys / 1024 / 1024 <<" Mb" << LOG_ENDLINE;
			REDEEMER_LOG << LOG_INFO << "Virtual memory: " << m_MemInfo.dwTotalVirtual / 1024 / 1024 << " Mb" << LOG_ENDLINE;
			REDEEMER_LOG << LOG_INFO << "Available memory: " << m_MemInfo.dwAvailVirtual / 1024 / 1024 << " Mb" << LOG_ENDLINE;

			// Write the CPU info to the log file
			REDEEMER_LOG << LOG_INFO << "CPU Speed: " << m_CPUSpeed << " MHz" << LOG_ENDLINE;

			if (m_CPUCaps & CPU_FEATURE_MMX) 
			{
				REDEEMER_LOG << LOG_INFO << "MMX: Yes" << LOG_ENDLINE;
			}
			else 
			{
				REDEEMER_LOG << LOG_INFO << "MMX: No" << LOG_ENDLINE;
			}

			if (m_CPUCaps & CPU_FEATURE_RDTSC) 
			{
				REDEEMER_LOG << LOG_INFO << "RDTSC: Yes" << LOG_ENDMESSAGE;
			}
			else {
				REDEEMER_LOG << LOG_INFO << "RDTSC: No" << LOG_ENDMESSAGE;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace CORE
}	//	namespace REDEEMER

