/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Core_UUID.h
**		Tool class for Universal Unique IDentifier.
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_CORE_UUID_H_
#define _R_CORE_UUID_H_

/*!	Macro to define a UUID.
*/
#define UUID_DEFINE(UUID_Name, d1, d2, d3, d40, d41, d42, d43, d44, d45, d46, d47) \
	const REDEEMER::CORE::UUID C_UUID (d1, d2, d3, d40, d41, d42, d43, d44, d45, d46, d47)

namespace REDEEMER
{
	namespace CORE
	{
		/*!
		**	\brief Tool class for Universal Unique IDentifier.
		*/
		class C_UUID
		{
		public:
			/*!	Default constructor.
			**	All values are set to 0.
			*/
			C_UUID ();

			/*!	Constructor from values.
			*/
			C_UUID (unsigned long iD1, unsigned short iD2, unsigned short iD3, unsigned char iD40, unsigned char iD41, unsigned char iD42,
				unsigned char iD43, unsigned char iD44, unsigned char iD45, unsigned char iD46, unsigned char iD47);

			/*!	Constructor from a formatted UUID string {xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}.
			**	C_UUID("{deadbeaf-baad-f00d-0102-030405060708}"); is equivalent to C_UUID (0xdeadbeaf, 0xbaad, 0xf00d, 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08);
			//	\param iprString A reference pointer on the UUID formatted string.
			*/
			C_UUID (const char * iprString);

			/*!	Copy constructor.
			**	\param iUUID The UUID to copy.
			*/
			C_UUID (const C_UUID& iUUID);

			/*!	Assignment operator=.
			**	\param iUUID The UUID to assign.
			*/
			void operator= (const C_UUID& iUUID);

			/*!	Equivalence operator == .
			**	\param iUUID The UUID to compare.
			*/
			bool operator == (const C_UUID& iUUID) const;

			/*!	Inequivalence operator != .
			**	\param iUUID The UUID to compare.
			*/
			bool operator != (const C_UUID& iUUID) const;

			/*!	Get the formatted UUID string {xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}.
			**	Example : "{deadbeaf-baad-f00d-0102-030405060708}"
			**	\param oString The string that will be filled in UUID formatted fashion.
			*/
			void GetString (char oString[39]) const;

		public:
			unsigned long	m_Data1;			///<	First part of the UUID
			unsigned short	m_Data2;			///<	Second part of the UUID
			unsigned short	m_Data3;			///<	Third part of the UUID
			unsigned char	m_Data4[8];			///<	Fourth part of the UUID
		};

	}	//	namespace CORE
}	//	namespace REDEEMER

#endif	//	_R_CORE_UUID_H_
