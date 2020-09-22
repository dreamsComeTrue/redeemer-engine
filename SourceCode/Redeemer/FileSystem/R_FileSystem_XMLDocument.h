/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_FileSystem_XMLDocument.h
**		XML documents are used as configuration files and self-describing data files	
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_FILESYSTEM_XMLDOCUMENT_H_
#define _R_FILESYSTEM_XMLDOCUMENT_H_

#include <vector>

namespace REDEEMER
{
	namespace FILESYSTEM
	{	 	
		class IDataFile;

		std::string ConvertToXML (const std::string& data);
		std::string ConvertFromXML (const std::string& data, bool& ok);


		/*!	\brief	XML documents are used as configuration files and self-describing data files	
		*/
		class C_XMLDocument
		{
		public:
			/*!	\brief	Single XML element
			*/
			class C_XMLElement
			{
			public:
				/*!	Enumeration of element types
				*/
				enum EElementType
				{
					ET_Comment,
					ET_Declaration,
					ET_Normal,
					ET_Count
				};

				/*!	Constructor
				*/
				C_XMLElement(EElementType type = ET_Count);

				/*!	Overload of "==" operator
				*/
				friend bool operator == (C_XMLElement const& a, C_XMLElement const& b);

				/*!	Overload of "==" operator
				*/
				friend bool operator != (C_XMLElement const& a, C_XMLElement const& b);

			private:
				EElementType m_Type;
			};

			/*!	Class used for getting info about parsing errors 
			*/
			class C_XMLError
			{
			public:
				/*!	Enumeration of possible errors
				*/
				enum EErrorType
				{
					ET_OpenFileFail,
					ET_CreateFileFail,
					ET_IncorrectContent,
					ET_NotClosedElements,
					ET_NoXMLDeclaration,
					ET_CloseNotOpenedElement,
					ET_InconsistentOpenCloseElements,
					ET_MoreThanOneXMLDeclaration,
					ET_MoreThanOneRootElement,
					ET_IncorrectAttribute,
					ET_Count
				};

				/*!	Constructor
				*/
				C_XMLError(EErrorType type = ET_Count);

				/*!	Returns self-describing info about error
				*/
				std::string GetErrorString () const;

			private:
				/*!	Returns string containing info about error
				*/
				const char * GetErrorDescription (int index) const;

			private:
				EErrorType m_Type;	///<	Error type
			};

			//	Various typedefs

			class C_XMLNode;

			/*!	A pair of "key-value" type
			*/
			typedef std::pair<std::string, std::string> PairAttribute;

			/*!	Vector of attributes
			*/
			typedef std::vector<PairAttribute> VectorAttributes;

			/*!	Container for XML nodes
			*/
			typedef std::vector<C_XMLNode*> VectorNode;

			/*!	\brief	Enumerator is used to traverse XML tree
			*/
			class C_XMLNodeEnumerator
			{
				friend class C_XMLNode;

			private:
				/*!	Constructor
				*/
				C_XMLNodeEnumerator(VectorNode::iterator begin, VectorNode::iterator end);

			public:
				/*!	Returns next element
				*/
				bool Next ();

				/*!	Returns next element with given name
				*/
				bool Next (const std::string& name);

				/*!	Returns current element
				*/
				C_XMLNode* operator->() const;

				/*!	Returns current element
				*/
				C_XMLNode* Current ();

			private:
				bool					m_First;
				VectorNode::iterator m_Current, m_End;
			};

			/*!	C_XMLNode is basic building block of each XML document
			*/
			class C_XMLNode
			{
				friend class C_XMLDocument;

			private:
				/*!	Constructor
				*/
				C_XMLNode(const std::string &name, C_XMLNode* parent, C_XMLElement type = C_XMLElement::ET_Normal, const std::string& content = "");

				/*!	Saves element to a stream
				*/
				void Save (std::ostream& stream, size_t level);

			public:
				/*!	Destructor
				*/
				~C_XMLNode();

				/*!	Creates child of this node
				*/
				C_XMLNode* CreateChild (const std::string& name, const std::string& content = "");

				/*!	Adds attribute as key-pair to this node (templated version)
				*/
				template <typename T> void AddAttribute (const std::string &key, const T& value);

				/*!	Adds attribute as key-pair to this node (string version)
				*/
				void AddAttribute (const std::string& key, const std::string& value);

				/*!	Removes attribute with given key
				*/
				void RemoveAttribute (const std::string& key);

				/*!	Sets attribute with given key
				*/
				void SetAttribute (const std::string& key, const std::string& value);

				/*!	Adds content to the node (templated version)
				*/
				template <typename T> void AddContent (const T& content);

				/*!	Adds content to the node (string version)
				*/
				void AddContent (const std::string& content);

				/*!	Sets node content (templated version)
				*/
				template <typename T> void SetContent (const T& content);

				/*!	Sets node content (string version)
				*/
				void SetContent (const std::string& content);

				/*!	Clears node content
				*/
				void Clear ();

				/*!	Checks, if given attribute exists, returns its value
				*/
				bool FindAttribute (const std::string& name, std::string& value);

				/*!	Tries to find attribute with given name, returns it's value
				*/
				std::string FindAttribute (const std::string& name);

				/*!	Returns node's name
				*/
				const std::string& GetName () const;

				/*!	Returns node's content
				*/
				const std::string& GetContent() const;

				/*!	Returns node's attributes list
				*/
				const VectorAttributes& GetAttributes () const;

				/*!	Returns node's parent node
				*/
				C_XMLNode* GetParent () const;

				/*!	Returns node's element enumerator
				*/
				C_XMLNodeEnumerator GetNodeEnumerator ();

				/*!	Returns node's type
				*/
				C_XMLElement GetType() const;

				/*!	Creates copy of this node
				*/
				C_XMLNode* CreateCopy ();

			private:
				std::string			m_Name;
				std::string			m_Content;
				VectorAttributes	m_Attributes;
				VectorNode			m_Children;
				C_XMLNode*			m_Parent;
				C_XMLElement		m_Type;
			};

		public:
			/*!	Constructor
			*/
			C_XMLDocument ();

			/*!	Destructor
			*/
			virtual ~C_XMLDocument ();

			/*!	Opens XML IDataFile
			*/
			bool OpenFile (FILESYSTEM::IDataFile* file);

			/*!	Save XML document to a file
			*/
			bool SaveFile (const std::wstring& fileName);

			/*!	Save XML document to a stream
			*/
			bool SaveFile (std::ostream& stream);

			/*!	Clears XML document content
			*/
			void Clear ();

			/*!	Returns last parsing error
			*/
			std::string GetLastError ();

			/*!	Clears last error
			*/
			void ClearLastError ();

			/*!	Create XML declaration
			*/
			C_XMLNode* CreateDeclaration (const std::string& version = "1.0", const std::string& encoding = "UTF-8");

			/*!	Creates XML root node
			*/
			C_XMLNode* CreateRootNode (const std::string& name);

			/*!	Returns root node
			*/
			C_XMLNode* GetRootNode () const;

		private:
			/*!	Sets last file error
			*/
			void SetLastFileError(const std::wstring& fileName);

			/*!	Parses node
			*/
			bool ParseTag (C_XMLNode* &currentNode, std::string content);

			bool CheckPair (std::string &key, std::string &value);

			bool ParseLine (std::string& line, C_XMLNode*& element);

			size_t Find (const std::string& text, char character, size_t start = 0);

			/*!	Clears XML declaration
			*/
			void ClearDeclaration ();

			/*!	Clears root node
			*/
			void ClearRoot ();

		private:
			C_XMLNode*		m_Root;
			C_XMLNode*		m_Declaration;
			C_XMLError		m_LastError;
			std::wstring	m_LastErrorFile;
			size_t			m_Line;
			size_t			m_Column;
		};

	}	//	namespace FILESYSTEM
}	//	namespace REDEEMER

#endif	//	_R_FILESYSTEM_XMLDOCUMENT_H_
