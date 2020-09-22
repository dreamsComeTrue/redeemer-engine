/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_FileSystem_DataFile.h
**		Base class for all file types
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_FILESYSTEM_DATAFILE_H_
#define _R_FILESYSTEM_DATAFILE_H_

#include <string>

namespace REDEEMER
{
	namespace FILESYSTEM
	{	 	
#define FILE_BUFFER_SIZE 2048

		typedef unsigned long DWORD;

		/*!	Modes for opening files
		*/
		enum EFileOpenMode
		{
			FOM_Unknown		= (1<<0),
			FOM_Read		= (1<<1),		///<	Open file for reading
			FOM_Write		= (1<<2),		///<	Open file for writing
			FOM_Binary		= (1<<3),		///<	Open file as a binary
			FOM_Append		= (1<<4),		///<	Open for writing, add to end of file(append).
			FOM_NoCreate	= (1<<5),		///<	Do not create the file,open only if it exists
			FOM_NoReplace	= (1<<6),		///<	Open and create a new file if the specified file does not exist
			FOM_Truncate	= (1<<7),		///<	Open a file and empty it.(Boom, all the data is gone,if any)
			FOM_Ate			= (1<<8)		///<	Goes to end of file instead of the begining
		};

		/*!	Possible file seeking directions
		*/
		enum EFileSeekDirection
		{
			FSD_Unknown,
			FSD_Current,			///< Seek from current position
			FSD_Begin,				///< Seek from the beginning of the file
			FSD_End				///< Seek from the end of the file
		};

		/*!
		**	\brief IDataFile provides basic functionality of all file types
		**	When one wants to create either physical file or virtual (archive-type) file, or memory file, etc. - he should
		**	derive from this class
		*/
		class IDataFile 
		{
		public:
			/*!	Default constructor. 
			*/
			IDataFile();

			/*!  Default destructor. 
			*/
			virtual ~IDataFile ();

			/*!	Opens the File.
			**	\param fileName name of the file to open.
			**	\param mode combination of flags of OPEN_MODE type. You
				can combine any of them.
			*/
			virtual void Open (const std::wstring& fileName, DWORD mode) = 0;

			/*!	Closes the file.
			**	\remarks This function is automatically called on object's destruction.
			*/
			virtual void Close () = 0;

			/*! Puts C-formatted string of characters to the file.
			**	\remarks When you use "%s" format identifier make sure you pass a char* (not string!) variable. Otherwise the data written will be (null).
			*/
			virtual void Write (const char* data, ...) = 0;

			/*! Puts wchar_t-formatted string of characters to the file.
			**	\remarks When you use "%s" format identifier make sure you pass a char* (not string!) variable. Otherwise the data written will be (null).
			*/
			virtual void Write (const wchar_t* data, ...) = 0;

			/*!	Reads binary data from file.
			**	\remarks Reads size bytes and returns them as a char pointer.
			**	\return number of bytes read. 
			*/
			virtual long int Read (char* buffer, unsigned int size) = 0;

			/*!	Reads line from a file.
			**	\param buffer where to put the data
			**	\param delimiter delimiter character
			**	\return number of bytes read. 
			*/
			virtual long int ReadLine (std::string& buffer, char delimiter) = 0;

			/*!	Puts block of data into file.
			*/
			virtual void Put (const char* data, unsigned int size) = 0;

			/*!	Gets the name of the file.
			*/
			virtual const std::wstring& GetFileName () const = 0;

			/*!	Gets open mode of the file.
			*/
			virtual DWORD GetFileOpenMode () const = 0;

			/*!	Gets size of the file in bytes.
			*/
			virtual long int GetSize () const = 0;

			/*!	Returns the whole file as a buffer of chars.
			*/
			virtual std::wstring GetBuffer () const = 0;

			/*!	Returns data
			*/
			virtual std::string GetData () = 0;
	
			/*!	Checks whether End-Of-File has been reached.
			*/
			virtual bool Eof () const = 0;

			/*!	Checks whether the file is valid (no error conditions occurred).
			*/
			virtual bool IsValid () const = 0;

			/*!	Checks whether the file is opened.
			*/
			virtual bool IsOpened () const = 0;

			/*!	Sets the put/get pointer position.
			*/
			virtual void Seek (int position, EFileSeekDirection direction) = 0;

			/*!	Tells the put/get pointer position.
			**	\return current position in the file. If file is not opened -1 is returned.
			*/
			virtual long int Tell () = 0;

		protected:
			/*!	Returns STL seek direction based on	 EFileSeekDirection format
			*/
			std::ios::seekdir GetSeekDir (EFileSeekDirection direction) const;
		};

	}	//	namespace FILESYSTEM
}	//	namespace REDEEMER

#endif	//	_R_FILESYSTEM_DATAFILE_H_
