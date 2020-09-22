/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_FileSystem_CompressFile.h
**		Compress files are files contained in PACK archives
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_FILESYSTEM_COMPRESSFILE_H_
#define _R_FILESYSTEM_COMPRESSFILE_H_

#include "R_FileSystem_DataFile.h"

namespace REDEEMER
{
	namespace FILESYSTEM
	{	 	
		/*!
		**	\brief Compress files are files contained in PACK archives
		*/
		class C_CompressFile : public IDataFile
		{
		public:
			/*!	Default constructor. 
			*/
			C_CompressFile();

			/*!	Constructs the File and opens it.
			**	\param fileName wide char file name.
			**	\param data file data
			**	\param size size of the data
			*/
			C_CompressFile (const std::string& fileName, void* data, unsigned int size);

			/*!	Copy constructor.
			*/
			C_CompressFile (const C_CompressFile& src);

			/*!	Assignment operator.
			*/
			C_CompressFile& operator= (const C_CompressFile& rhs);

			/*!  Default destructor. 
			*/
			virtual ~C_CompressFile ();

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

			/*! Puts wchar_t*-formatted string of characters to the file.
			**	\remarks When you use "%s" format identifier make sure you pass a char* (not string!) variable. Otherwise the data written will be (null).
			*/
			virtual void Write (const wchar_t* data, ...);

			/*!	Reads binary data from file.
			**	\remarks Reads size bytes and returns them as a wchar_t pointer.
			**	\return number of bytes read. 
			*/
			virtual long int Read (wchar_t* buffer, unsigned int size);

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

			/*!	Returns pointer to data
			*/
			char* GetDataPointer ();

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
			char			*m_Data;		///<	File data
			char			*m_FilePointer;	///<	File pointer
			long int		m_Size;			///<	File size
			long int		m_Position;		///<	Actual position in the stream
			std::wstring	m_FileName;		///<	File name			
		};

	}	//	namespace FILESYSTEM
}	//	namespace REDEEMER

#endif	//	_R_FILESYSTEM_COMPRESSFILE_H_
