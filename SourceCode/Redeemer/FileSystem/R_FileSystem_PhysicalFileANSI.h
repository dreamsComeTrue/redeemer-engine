/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_FileSystem_PhysicalFileANSI.h
**		Physical file is raw file stored on a disk
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_FILESYSTEM_PHYSICALFILEANSI_H_
#define _R_FILESYSTEM_PHYSICALFILEANSI_H_

#include "R_FileSystem_DataFile.h"

#include <fstream>

namespace REDEEMER
{
	namespace FILESYSTEM
	{	 	
		/*!
		**	\brief C_PhysicalFile is an implementation of raw file stored on a disk
		*/
		class C_PhysicalFileANSI : public IDataFile
		{
		public:
			/*!	Default constructor. 
			*/
			C_PhysicalFileANSI();

			/*!	Constructs the File and opens it.
			**	\param fileName wide char file name.
			**	\param mode combination of flags of FileOpenMode type. You can combine any of them.
			*/
			C_PhysicalFileANSI (const std::wstring& fileName, DWORD mode);

			/*!	Copy constructor.
			*/
			C_PhysicalFileANSI (const C_PhysicalFileANSI& src);

			/*!	Assignment operator.
			*/
			C_PhysicalFileANSI& operator= (const C_PhysicalFileANSI& rhs);

			/*!  Default destructor. 
			*/
			virtual ~C_PhysicalFileANSI ();

			/*!	Opens the File.
			**	\param fileName name of the file to open.
			**	\param mode combination of flags of OPEN_MODE type. You
				can combine any of them.
			*/
			virtual void Open (const std::wstring& fileName, DWORD mode);

			/*!	Closes the file.
			**	\remarks This function is automatically called on object's destruction.
			*/
			virtual void Close ();

			/*! Puts C-formatted string of characters to the file.
			**	\remarks When you use "%s" format identifier make sure you pass a char* (not string!) variable. Otherwise the data written will be (null).
			*/
			virtual void Write (const char* data, ...);

			/*! Puts wchar_t-formatted string of characters to the file.
			**	\remarks When you use "%s" format identifier make sure you pass a char* (not string!) variable. Otherwise the data written will be (null).
			*/
			virtual void Write (const wchar_t* data, ...);

			/*!	Reads binary data from file.
			**	\remarks Reads size bytes and returns them as a char pointer.
			**	\return number of bytes read. 
			*/
			virtual long int Read (char* buffer, unsigned int size);

			/*!	Reads line from a file.
			**	\param buffer where to put the data
			**	\param delimiter delimiter character
			**	\return number of bytes read. 
			*/
			virtual long int ReadLine (std::string& buffer, char delimiter);

			/*!	Puts block of data into file.
			*/
			virtual void Put (const char* data, unsigned int size);

			/*!	Gets the name of the file.
			*/
			virtual const std::wstring& GetFileName () const;

			/*!	Gets open mode of the file.
			*/
			virtual DWORD GetFileOpenMode () const;

			/*!	Gets size of the file in bytes.
			*/
			virtual long int GetSize () const;

			/*!	Returns the whole file as a buffer of chars.
			*/
			virtual std::wstring GetBuffer () const;

			/*!	Returns data
			*/
			virtual std::string GetData ();

			/*!	Checks whether End-Of-File has been reached.
			*/
			virtual bool Eof () const;

			/*!	Checks whether the file is valid (no error conditions occurred).
			*/
			virtual bool IsValid () const;

			/*!	Checks whether the file is opened.
			*/
			virtual bool IsOpened () const;

			/*!	Sets the put/get pointer position.
			*/
			virtual void Seek (int position, EFileSeekDirection direction);

			/*!	Tells the put/get pointer position.
			**	\return current position in the file. If file is not opened -1 is returned.
			*/
			virtual long int Tell ();

		protected:		
			/*!	Returns STL seek direction based on	 EFileSeekDirection format
			*/
			std::ios::seekdir GetSeekDirection (EFileSeekDirection direction) const;

			std::fstream	*m_File;	///<	File stream
			std::wstring	m_FileName;	///<	File name			
			DWORD			m_OpenMode;	///<	The mode in which the file is opened
		};

	}	//	namespace FILESYSTEM
}	//	namespace REDEEMER

#endif	//	_R_FILESYSTEM_PHYSICALFILEANSI_H_
