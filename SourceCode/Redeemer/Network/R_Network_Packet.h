/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Network_Packet.h
**		Packets are used to interchange data between computers in the internet
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_NETWORK_PACKET_H_
#define _R_NETWORK_PACKET_H_

#include <vector>
#include <string>

namespace REDEEMER
{
	namespace NETWORK
	{
		/*!
		**	\brief Packets are used to interchange data between computers in the internet
		*/
		class C_Packet
		{
		public:
			/*!	Constructor. Creates an empty packet.
			*/
			C_Packet ();

			/*!	Virtual destructor
			*/
			virtual ~C_Packet ();

			/*!	Append data to the end of the packet
			**	\param data Pointer to the sequence of bytes to append
			**	\param sizeInBytes Number of bytes to append
			*/
			void Append (const void* data, std::size_t sizeInBytes);

			/*!	Clear the packet, the packet will be empty
			*/
			void Clear ();

			/*!	Get a pointer to the data contained in the packet
			*/
			const char* GetData () const;

			/*!	Get the size of the data contained in the packet
			*/
			std::size_t GetDataSize () const;

			/*!	Tell if the reading position has reached the end of the packet
			**	This function is useful to know if there is some data left to be read, without actually reading it.
			**	\return True if all data was read, false otherwise
			*/
			bool EndOfPacket () const;

			/*!	Test the validity of the packet, for reading
			**	This operator allows to test the packet as a boolean variable, to check if a reading operation was successful.
			**	A packet will be in an invalid state if it has no more data to read.
			**
			**	\return True if last data extraction from packet was successful
			*/
			operator bool () const;

			/*!	Overloads of operator >> to read data from the packet
			*/
			C_Packet& operator >>(bool& data);
			
			/*!	Overloads of operator >> to read data from the packet
			*/
			C_Packet& operator >>(char& data);
			
			/*!	Overloads of operator >> to read data from the packet
			*/
			C_Packet& operator >>(unsigned char& data);
			
			/*!	Overloads of operator >> to read data from the packet
			*/
			C_Packet& operator >>(short& data);
			
			/*!	Overloads of operator >> to read data from the packet
			*/
			C_Packet& operator >>(unsigned short& data);
			
			/*!	Overloads of operator >> to read data from the packet
			*/
			C_Packet& operator >>(int& data);
			
			/*!	Overloads of operator >> to read data from the packet
			*/
			C_Packet& operator >>(unsigned int& data);
			
			/*!	Overloads of operator >> to read data from the packet
			*/
			C_Packet& operator >>(float& data);
			
			/*!	Overloads of operator >> to read data from the packet
			*/
			C_Packet& operator >>(double& data);
			
			/*!	Overloads of operator >> to read data from the packet
			*/
			C_Packet& operator >>(char* data);
			
			/*!	Overloads of operator >> to read data from the packet
			*/
			C_Packet& operator >>(std::string&  data);
			
			/*!	Overloads of operator >> to read data from the packet
			*/
			C_Packet& operator >>(wchar_t* data);
			
			/*!	Overloads of operator >> to read data from the packet
			*/
			C_Packet& operator >>(std::wstring& data);
			
			/*!	Overloads of operator << to write data into the packet
			*/
			C_Packet& operator <<(bool data);

			/*!	Overloads of operator << to write data into the packet
			*/
			C_Packet& operator <<(char data);

			/*!	Overloads of operator << to write data into the packet
			*/
			C_Packet& operator <<(unsigned char data);

			/*!	Overloads of operator << to write data into the packet
			*/
			C_Packet& operator <<(short data);

			/*!	Overloads of operator << to write data into the packet
			*/
			C_Packet& operator <<(unsigned short data);

			/*!	Overloads of operator << to write data into the packet
			*/
			C_Packet& operator <<(int data);

			/*!	Overloads of operator << to write data into the packet
			*/
			C_Packet& operator <<(unsigned int data);

			/*!	Overloads of operator << to write data into the packet
			*/
			C_Packet& operator <<(float data);

			/*!	Overloads of operator << to write data into the packet
			*/
			C_Packet& operator <<(double data);

			/*!	Overloads of operator << to write data into the packet
			*/
			C_Packet& operator <<(const char* data);

			/*!	Overloads of operator << to write data into the packet
			*/
			C_Packet& operator <<(const std::string& data);

			/*!	Overloads of operator << to write data into the packet
			*/
			C_Packet& operator <<(const wchar_t* data);

			/*!	Overloads of operator << to write data into the packet
			*/
			C_Packet& operator <<(const std::wstring& data);

			/*!	Called before the packet is sent over the network
			**
			**	\param size Variable to fill with the size of data to send
			**
			**	\return Pointer to the array of bytes to send
			*/
			virtual const char* OnSend(std::size_t& size);

			/*!	Called after the packet is received over the network
			**	
			**	\param data Pointer to the received bytes
			**	\param size Number of bytes
			*/
			virtual void OnReceive (const char* data, std::size_t size);

		private: 
			/*!	Check if the packet can extract a given number of bytes
			**	\param size Size to check
			**	\return True if \a size bytes can be read from the packet
			*/
			bool CheckSize(std::size_t size);

		private:
			std::vector<char> m_Data;			///< Data stored in the packet
			std::size_t       m_ReadPosition;	///< Current reading position in the packet
			bool              m_IsValid;		///< Reading state of the packet
		};

	}	//	namespace NETWORK
}	//	namespace REDEEMER

#endif	//	_R_NETWORK_PACKET_H_
