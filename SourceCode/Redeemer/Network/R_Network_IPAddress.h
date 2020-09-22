/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Network_IPAddress.h
**		Encapsulate an IPv4 network address
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_NETWORK_IPADDRESS_H_
#define _R_NETWORK_IPADDRESS_H_

#include <istream>
#include <ostream>
#include <string>

namespace REDEEMER
{
	namespace NETWORK
	{
		/*!
		**	\brief Encapsulate an IPv4 network address
		*/
		class C_IPAddress
		{
		public:
			/*!	Constructor. Creates an empty address.
			*/
			C_IPAddress ();

			/*!	Construct the address from a string
			**	here \a address can be either a decimal address (ex: "192.168.1.56") or a network name (ex: "localhost").
			**
			**	\param address IP address or network name
			*/
			C_IPAddress (const std::string& address);

			/*!	Construct the address from a string
			**	Here \a address can be either a decimal address (ex: "192.168.1.56") or a network name (ex: "localhost").
			**
			**	\param address IP address or network name
			*/
			C_IPAddress (const char* address);

			/*!	Construct the address from 4 bytes
			**
			**	\param byte0 First byte of the address
			**	\param byte1 Second byte of the address
			**	\param byte2 Third byte of the address
			**	\param byte3 Fourth byte of the address
			*/
			C_IPAddress (unsigned char byte0, unsigned char byte1, unsigned char byte2, unsigned char byte3);

			/*!	Construct the address from a 32-bits integer
			**	This constructor uses the internal representation of the address directly. It should be used for optimization
			**	purposes, and only if you got that representation from C_IPAddress::ToInteger().
			**
			**	\param address 4 bytes of the address packed into a 32-bits integer
			*/
			explicit C_IPAddress (unsigned int address);

			/*!	Get a string representation of the address
			**	The returned string is the decimal representation of the IP address (like "192.168.1.56"), even if it was constructed from a host name.
			*/
			std::string ToString () const;

			/*!	Get an integer representation of the address
			**	The returned number is the internal representation of the address, and should be used for optimization purposes only
			**	(like sending the address through a socket).
			*/
			unsigned int ToInteger () const;

			/*!	Get the computer's local address
			**	The local address is the address of the computer from the LAN point of view, i.e. something like 192.168.1.56. It is
			**	meaningful only for communications over the local network.
			*/
			static C_IPAddress GetLocalAddress();

			/*!	Get the computer's public address
			**	The public address is the address of the computer from the internet point of view, i.e. something like 89.54.1.169.
			**	Because this function depends on the network connection and on a distant server, you may use a time limit if you don't want your program
			**	to be possibly stuck waiting in case there is a problem; this limit is deactivated by default.
			**
			**	\param timeout Maximum time to wait, in seconds
			*/
			static C_IPAddress GetPublicAddress (float timeout = 0.f);

		public:
			static const C_IPAddress None;			///< Value representing an empty/invalid address
			static const C_IPAddress LocalHost;		///< The "localhost" address (for connecting a computer to itself locally)

		private:
			unsigned int m_Address;					///< Address stored as an unsigned 32 bits integer
		};

		/*!	Overload of == operator to compare two IP addresses
		**
		**	\param left  Left operand (a IP address)
		**	\param right Right operand (a IP address)
		**
		**	\return True if both addresses are equal
		*/
		bool operator ==(const C_IPAddress& left, const C_IPAddress& right);

		/*!	Overload of != operator to compare two IP addresses
		**
		**	\param left  Left operand (a IP address)
		**	\param right Right operand (a IP address)
		**
		**	\return True if both addresses are different
		*/
		bool operator !=(const C_IPAddress& left, const C_IPAddress& right);

		/*!	Overload of < operator to compare two IP addresses
		**
		**	\param left  Left operand (a IP address)
		**	\param right Right operand (a IP address)
		**
		**	\return True if \a left is lesser than \a right
		*/
		bool operator <(const C_IPAddress& left, const C_IPAddress& right);

		/*!	Overload of > operator to compare two IP addresses
		**
		**	\param left  Left operand (a IP address)
		**	\param right Right operand (a IP address)
		**
		**	\return True if \a left is greater than \a right
		*/
		bool operator >(const C_IPAddress& left, const C_IPAddress& right);

		/*!	Overload of <= operator to compare two IP addresses
		**
		**	\param left  Left operand (a IP address)
		**	\param right Right operand (a IP address)
		**
		**	\return True if \a left is lesser or equal than \a right
		*/
		bool operator <=(const C_IPAddress& left, const C_IPAddress& right);

		/*!	Overload of >= operator to compare two IP addresses
		**
		**	\param left  Left operand (a IP address)
		**	\param right Right operand (a IP address)
		**
		**	\return True if \a left is greater or equal than \a right
		*/
		bool operator >=(const C_IPAddress& left, const C_IPAddress& right);

		/*!	Overload of >> operator to extract an IP address from an input stream
		**
		**	\param stream  Input stream
		**	\param address IP address to extract
		**
		**	\return Reference to the input stream
		*/
		std::istream& operator >>(std::istream& stream, C_IPAddress& address);

		/*!	Overload of << operator to print an IP address to an output stream
		**
		**	\param stream  Output stream
		**	\param address IP address to extract
		**
		**	\return Reference to the output stream
		*/
		std::ostream& operator <<(std::ostream& stream, const C_IPAddress& address);

	}	//	namespace NETWORK
}	//	namespace REDEEMER

#endif	//	_R_NETWORK_IPADDRESS_H_
