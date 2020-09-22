/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_FileSystem_XMLDocument.cpp
**		
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_FileSystem_XMLDocument.h"
#include "..\\Core\\R_Core_Utility.h"
#include "..\\RedeemerEngine.h"

#include <assert.h>
#include <fstream>

namespace REDEEMER
{
	namespace FILESYSTEM
	{	 	
		//------------------------------------------------------------------------------------------------------------------------

		std::string ConvertToXML (const std::string& data)
		{
			std::string ret;

			size_t pos = data.find_first_of("&<>'\"");

			if (pos == std::string::npos) 
				return data;

			ret.reserve(data.size() * 2);

			size_t old = 0;

			while (pos != std::string::npos)
			{
				ret += data.substr(old, pos - old);

				if (data[pos] == '&') 
					ret += "&amp;";
				else 
					if (data[pos] == '<') 
						ret += "&lt;";
					else
						if (data[pos] == '>') 
							ret += "&gt;";
						else 
							if (data[pos] == '\'') 
								ret += "&apos;";
							else 
								if (data[pos] == '\"')
									ret += "&quot;";

				old = pos + 1;
				pos = data.find_first_of("&<>'\"", old);
			}

			ret += data.substr(old, std::string::npos);

			return ret;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::string ConvertFromXML (const std::string& data, bool& ok)
		{
			std::string ret;

			ok = true;

			size_t pos = data.find("&");

			if (pos == std::string::npos) 
				return data;

			ret.reserve(data.size());

			size_t old = 0;

			while (pos != std::string::npos)
			{
				ret += data.substr(old, pos - old);

				size_t end = data.find(";", pos + 1);

				if (end == std::string::npos)
				{
					ok = false;

					return ret;
				}
				else
				{
					std::string tag = data.substr(pos, end - pos + 1);

					if (tag == "&amp;")
						ret += '&';
					else 
						if (tag == "&lt;")
							ret += '<';
						else 
							if (tag == "&gt;") 
								ret += '>';
							else 
								if (tag == "&apos;") 
									ret += '\'';
								else 
									if (tag == "&quot;")
										ret += '\"';
					else
					{
						ok = false;

						return ret;
					}
				}

				old = end + 1;
				pos = data.find ("&", old);
			}

			ret += data.substr(old, std::string::npos);

			return ret;
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_XMLDocument::C_XMLElement::C_XMLElement(EElementType type) :
			m_Type (type)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool operator == (C_XMLDocument::C_XMLElement const& a, C_XMLDocument::C_XMLElement const& b)
		{
			return a.m_Type == b.m_Type;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool operator != (C_XMLDocument::C_XMLElement const& a, C_XMLDocument::C_XMLElement const& b)
		{
			return a.m_Type != b.m_Type;
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_XMLDocument::C_XMLError::C_XMLError(EErrorType type) :
			m_Type (type)
		{
			//	EMPTY
		}	

		//------------------------------------------------------------------------------------------------------------------------

		std::string C_XMLDocument::C_XMLError::GetErrorString () const
		{
			return GetErrorDescription (m_Type);
		}

		//------------------------------------------------------------------------------------------------------------------------

		const char * C_XMLDocument::C_XMLError::GetErrorDescription (int index) const
		{
			static const char * values[ET_Count + 1] =
			{
				"Failed to open XML file",
				"Failed to create XML file",
				"XML file contain incorrect content",
				"XML file contain not closed elements",
				"XML file without declaration",
				"XML file contain closed but not opened element",
				"XML file contain inconsistent elements",
				"XML file contain more than one declaration",
				"XML file contain more than one root element",
				"XML file contain incorrect attribute",
				""
			};
			return values[(index < ET_Count && index >= 0) ? index : ET_Count];
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_XMLDocument::C_XMLNodeEnumerator::C_XMLNodeEnumerator(C_XMLDocument::VectorNode::iterator begin, VectorNode::iterator end) :
			m_First (true),
			m_Current (begin),
			m_End (end)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_XMLDocument::C_XMLNodeEnumerator::Next ()
		{
			if (m_Current == m_End) 
				return false;
			else 
				if (m_First)
				{
					m_First = false;

					return true;
				}

			++ m_Current;
			
			if (m_Current == m_End) 
				return false;

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_XMLDocument::C_XMLNodeEnumerator::Next (const std::string& name)
		{
			while (Next())
			{
				if ((*m_Current)->GetName () == name) 
					return true;
			}

			return false;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_XMLDocument::C_XMLNode* C_XMLDocument::C_XMLNodeEnumerator::operator->() const
		{
			assert (m_Current != m_End); 
			
			return (*m_Current); 
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_XMLDocument::C_XMLNode* C_XMLDocument::C_XMLNodeEnumerator::Current ()
		{
			   assert (m_Current != m_End); 
			   
			   return (*m_Current);
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------
																																  
		C_XMLDocument::C_XMLNode::C_XMLNode(const std::string &name, C_XMLNode* parent, C_XMLDocument::C_XMLElement type, const std::string& content) :
			m_Name (name),
			m_Content (content),
			m_Parent (parent),
			m_Type (type)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_XMLDocument::C_XMLNode::~C_XMLNode()
		{
			for (VectorNode::iterator it = m_Children.begin(); it != m_Children.end(); ++it)
			{
				delete *it;
			}

			m_Children.clear ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_XMLDocument::C_XMLNode::Save (std::ostream& stream, size_t level)
		{
			//	Save some indentation
			for (size_t tab = 0; tab < level; ++ tab) 
				stream  << "    ";

			// Save node type
			if (m_Type == C_XMLElement::ET_Declaration) 
				stream << "<?";
			else 
				stream << "<";

			stream << m_Name;

			for (VectorAttributes::iterator it = m_Attributes.begin(); it != m_Attributes.end(); ++ it)
				stream << " " << it->first << "=\"" << ConvertToXML (it->second) << "\"";

			bool empty = m_Children.empty();

			// If there is no children and content
			if (empty && m_Content.empty())
			{
				if (m_Type == C_XMLElement::ET_Declaration) 
					stream << "?>\n";
				else 
					stream << "/>\n";
			}
			else
			{
				stream << ">";

				if (!empty) 
					stream << "\n";

				// If there is a content
				if (!m_Content.empty())
				{
					if (!empty)
					{
						for (size_t tab = 0; tab <=level; ++ tab) 
							stream  << "    ";
					}

					stream << ConvertToXML (m_Content);

					if (!empty) 
						stream << "\n";
				}

				//	Save recursively each child node
				for (size_t child = 0; child < m_Children.size(); child ++)
				{
					m_Children[child]->Save (stream, level + 1);
				}

				if (!empty)
				{
					for (size_t tab = 0; tab < level; ++tab)
						stream << "    ";
				}

				stream << "</" << m_Name << ">\n";
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_XMLDocument::C_XMLNode* C_XMLDocument::C_XMLNode::CreateChild (const std::string& name, const std::string& content)
		{
			C_XMLNode* node = new C_XMLNode (name, this, C_XMLElement::ET_Normal, content);

			m_Children.push_back(node);

			return node;
		}

		//------------------------------------------------------------------------------------------------------------------------

		template <typename T> void C_XMLDocument::C_XMLNode::AddAttribute (const std::string &key, const T& value)
		{
			m_Attributes.push_back (PairAttribute (key, CORE::C_Utility::ToString (value)));
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_XMLDocument::C_XMLNode::AddAttribute (const std::string& key, const std::string& value)
		{
			  m_Attributes.push_back (PairAttribute (key, value));
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_XMLDocument::C_XMLNode::RemoveAttribute (const std::string& key)
		{
			for (size_t index = 0; index < m_Attributes.size(); ++index)
			{
				if (m_Attributes[index].first == key)
				{
					m_Attributes.erase (m_Attributes.begin() + index);

					return;
				}
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_XMLDocument::C_XMLNode::SetAttribute (const std::string& key, const std::string& value)
		{
			for (size_t index = 0; index < m_Attributes.size(); ++index)
			{
				if (m_Attributes[index].first == key)
				{
					m_Attributes[index].second = value;

					return;
				}
			}

			m_Attributes.push_back (PairAttribute (key, value));
		}

		//------------------------------------------------------------------------------------------------------------------------

		template <typename T> void C_XMLDocument::C_XMLNode::AddContent (const T& content)
		{
			m_Content.empty() ? m_Content = CORE::C_Utility::ToString (content) : m_Content += CORE::C_Utility::ToString (" ", content);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_XMLDocument::C_XMLNode::AddContent (const std::string& content)
		{
			if (m_Content.empty()) 
				m_Content = content;
			else
			{
				m_Content += " ";
				m_Content += content;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		template <typename T> void C_XMLDocument::C_XMLNode::SetContent (const T& content)
		{
			m_Content = CORE::C_Utility::ToString (content);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_XMLDocument::C_XMLNode::SetContent (const std::string& content)
		{
			m_Content = content;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_XMLDocument::C_XMLNode::Clear ()
		{
			for (VectorNode::iterator it = m_Children.begin(); it != m_Children.end(); ++ it)
				delete *it;

			m_Children.clear();
			m_Content.clear();
			m_Attributes.clear();
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_XMLDocument::C_XMLNode::FindAttribute (const std::string& name, std::string& value)
		{
			for (VectorAttributes::iterator it = m_Attributes.begin(); it != m_Attributes.end(); ++ it)
			{
				if ( (*it).first == name)
				{
					value = (*it).second;

					return true;
				}
			}
			return false;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::string C_XMLDocument::C_XMLNode::FindAttribute (const std::string& name)
		{
			for (VectorAttributes::iterator it = m_Attributes.begin(); it != m_Attributes.end(); ++it)
			{
				if ((*it).first == name)
					return (*it).second;
			}

			return "";
		}

		//------------------------------------------------------------------------------------------------------------------------

		const std::string& C_XMLDocument::C_XMLNode::GetName () const
		{
			return m_Name;
		}

		//------------------------------------------------------------------------------------------------------------------------

		const std::string& C_XMLDocument::C_XMLNode::GetContent() const
		{
			return m_Content;
		}

		//------------------------------------------------------------------------------------------------------------------------

		const C_XMLDocument::VectorAttributes& C_XMLDocument::C_XMLNode::GetAttributes () const
		{
			return m_Attributes;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_XMLDocument::C_XMLNode* C_XMLDocument::C_XMLNode::GetParent () const
		{
			return m_Parent;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_XMLDocument::C_XMLNodeEnumerator C_XMLDocument::C_XMLNode::GetNodeEnumerator ()
		{
			 return C_XMLNodeEnumerator (m_Children.begin(), m_Children.end());
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_XMLDocument::C_XMLElement C_XMLDocument::C_XMLNode::GetType() const
		{
			return m_Type;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_XMLDocument::C_XMLNode* C_XMLDocument::C_XMLNode::CreateCopy ()
		{
			C_XMLNode* elem = new C_XMLNode (m_Name, NULL, m_Type, m_Content);

			elem->m_Attributes = m_Attributes;

			for (VectorNode::iterator it = m_Children.begin(); it != m_Children.end(); ++it)
			{
				C_XMLNode* child = (*it)->CreateCopy ();

				child->m_Parent = elem;
				elem->m_Children.push_back(child);
			}

			return elem;
		}

		//------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------

		C_XMLDocument::C_XMLDocument () :
			m_Root (0),
			m_Declaration (0),
			m_LastErrorFile (L""),
			m_Line (0),
			m_Column (0)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_XMLDocument::~C_XMLDocument ()
		{
			Clear ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_XMLDocument::OpenFile (FILESYSTEM::IDataFile* file)
		{
			Clear();

			std::string line;
			std::string read;
			
			C_XMLNode* currentNode = 0;

			while (!file->Eof())
			{
				file->ReadLine (read, '\n');

				if (read.empty())
					continue;

				if (read[read.size()-1] == '\r')
					read.erase (read.size () - 1, 1);

				if (read.empty ())
					continue;

				m_Line ++;
				m_Column = 0; 

				if (read.empty()) 
					continue;

				//	Increase line counter
				line += read;

				if (!ParseLine (line, currentNode))
				{
					return false;
				}

			} // while (!stream.eof())

			if (currentNode)
			{
				m_LastError = C_XMLError::ET_NotClosedElements;

				return false;
			}

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_XMLDocument::SaveFile (const std::wstring& fileName)
		{
			std::ofstream stream;

			stream.open(fileName.c_str ());

			if (!stream.is_open())
			{
				m_LastError = C_XMLError::ET_CreateFileFail;

				SetLastFileError(fileName);

				return false;
			}

			bool result = SaveFile (stream);

			if (!result)
			{
				SetLastFileError (fileName);
			}

			stream.close();

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_XMLDocument::SaveFile (std::ostream& stream)
		{
			if (!m_Declaration)
			{
				m_LastError = C_XMLError::ET_NoXMLDeclaration;

				return false;
			}

			// UTF8 encoding chars
			stream << (char)0xEFu;
			stream << (char)0xBBu;
			stream << (char)0xBFu;

			m_Declaration->Save (stream, 0);

			if (m_Root)
				m_Root->Save (stream, 0);

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_XMLDocument::Clear ()
		{
			ClearDeclaration ();
			ClearRoot ();
			
			m_Line = 0;
			m_Column = 0;
		}

		//------------------------------------------------------------------------------------------------------------------------

		std::string C_XMLDocument::GetLastError ()
		{
			const std::string& error = m_LastError.GetErrorString();

			if (error.empty()) 
				return error;

			std::string result = CORE::C_Utility::ToString ("'", error, "' ,  file = '", m_LastErrorFile.c_str(), "' ,  line = ", m_Line, " ,  column = ", m_Column);

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_XMLDocument::ClearLastError ()
		{
			m_LastError = C_XMLError::ET_Count;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_XMLDocument::C_XMLNode* C_XMLDocument::CreateDeclaration (const std::string& version, const std::string& encoding)
		{
			ClearDeclaration ();

			m_Declaration = new C_XMLNode ("xml", 0, C_XMLElement::ET_Declaration);
			m_Declaration->AddAttribute ("version", version);
			m_Declaration->AddAttribute ("encoding", encoding);

			return m_Declaration;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_XMLDocument::C_XMLNode* C_XMLDocument::CreateRootNode (const std::string& name)
		{
			ClearRoot();

			m_Root = new C_XMLNode (name, 0, C_XMLElement::ET_Normal);

			return m_Root;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_XMLDocument::C_XMLNode* C_XMLDocument::GetRootNode () const
		{
			return m_Root;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_XMLDocument::SetLastFileError(const std::wstring& fileName)
		{
			m_LastErrorFile = fileName;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_XMLDocument::ParseTag (C_XMLDocument::C_XMLNode* &currentNode, std::string content)
		{
			CORE::C_Utility::Trim (content);

			//	If there is nothing in the node
			if (content.empty())
			{					
				//	Create empty child
				if (currentNode) 
					currentNode = currentNode->CreateChild ("");
				else
				{
					//	Create empty node
					currentNode = new C_XMLNode ("", 0);

					//	If root node is not assigned
					if (!m_Root)
						m_Root = currentNode;
				}

				return true;
			}

			char simbol = content[0];
			bool tag_info = false;

			//	<!
			if (simbol == '!') 
				return true; 
				
			//	<?
			if (simbol == '?')
			{
				tag_info = true;

				//	Remove "?" char
				content.erase (0, 1); 
			}

			size_t start, end;
						
			//	We found "</" - are we closing tag?
			if (simbol == '/')
			{
				//	Empty element
				if (currentNode == 0)
				{				
					//	No root element
					if (!m_Root)
					{
						m_LastError = C_XMLError::ET_CloseNotOpenedElement;

						return false;
					}
				}

				//	Try to find something else than: " ", "\t"				
				start = content.find_first_not_of(" \t", 1);

				//	Not found
				if (start == content.npos)
				{
					//	empty content
					content.clear();
				}
				else
				{
					//	Try to find something else than: " ", "\t"
					end = content.find_last_not_of(" \t");

					//	Trim the content
					content = content.substr (start, end - start + 1);
				}
				
				//	Node name is different than it's closing tag
				if (currentNode->GetName() != content)
				{
					m_LastError = C_XMLError::ET_InconsistentOpenCloseElements;

					return false;
				}
				
				//	Get parent node
				currentNode = currentNode->GetParent();
			}
			else
			{					
				std::string cut = content;

				//	Try to find chars other than: " ", "\t", "/", "?"
				start = content.find_first_of(" \t/?", 1);

				//	Founded!
				if (start != content.npos)
				{
					//	Read all valid chars
					cut = content.substr(0, start);
					content = content.substr(start);
				}
				else
				{
					//	Not founded
					content.clear();
				}
								
				//	Create child node
				if (currentNode) 
					currentNode = currentNode->CreateChild(cut);
				else
				{
					if (tag_info)
					{							
						//	There is already a declaration
						if (m_Declaration)
						{
							m_LastError = C_XMLError::ET_MoreThanOneXMLDeclaration;

							return false;
						}

						currentNode = new C_XMLNode (cut, 0, C_XMLElement::ET_Comment);

						//	Create declaration
						m_Declaration = currentNode;
					}
					else
					{			
						//	There is already a root node
						if (m_Root)
						{
							m_LastError = C_XMLError::ET_MoreThanOneRootElement;

							return false;
						}
						
						//	Create root node
						currentNode = new C_XMLNode (cut, 0, C_XMLElement::ET_Normal);
						m_Root = currentNode;
					}
				}
						
				//	Read valid last chars that are not: " ", "\t"
				start = content.find_last_not_of(" \t");

				//	No more chars
				if (start == content.npos) 
					return true;
								
				bool close = false;

				//	Read closing tag
				if ((content[start] == '/') || (content[start] == '?'))
				{
					close = true;
									
					content[start] = ' ';												
					start = content.find_last_not_of(" \t");

					if (start == content.npos)
					{
						currentNode = currentNode->GetParent();

						return true;
					}
				}

				//	Read attributes
				while (true)
				{
					start = content.find('=');

					//	Not found "=" sign
					if (start == content.npos)
					{
						m_LastError = C_XMLError::ET_IncorrectAttribute;

						return false;
					}

					//	Try to find ' or " chars at he beginning
					end = content.find_first_of("\"\'", start + 1);

					//	Not found quotation marks
					if (end == content.npos)
					{
						m_LastError = C_XMLError::ET_IncorrectAttribute;

						return false;
					}

					//	Try to find ' or " chars at he end
					end = content.find_first_of("\"\'", end + 1);

					//	Not found quotation marks
					if (end == content.npos)
					{
						m_LastError = C_XMLError::ET_IncorrectAttribute;

						return false;
					}

					//	Get key and value strings
					std::string key = content.substr (0, start);
					std::string value = content.substr (start + 1, end - start);

					//	
					if (!CheckPair (key, value))
					{
						m_LastError = C_XMLError::ET_IncorrectAttribute;

						return false;
					}

					//	Add key and value as attribute pair
					currentNode->AddAttribute (key, value);

					content = content.substr(end + 1);
					start = content.find_first_not_of(" \t");

					if (start == content.npos)
						break;

					m_Column += start;
				}

				if (close)
				{
					currentNode = currentNode->GetParent();
				}

			}

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_XMLDocument::CheckPair (std::string &key, std::string &value)
		{
			CORE::C_Utility::Trim (key);

			if (key.empty()) 
				return false;

			size_t start = key.find_first_of(" \t\"\'&");

			if (start != key.npos) 
				return false;

			CORE::C_Utility::Trim (value);

			if (value.size() < 2) 
				return false;

			if (((value[0] != '"') || (value[value.length() - 1] != '"')) &&
				((value[0] != '\'') || (value[value.length() - 1] != '\''))) 
				return false;

			bool ok = true;

			value = ConvertFromXML (value.substr (1, value.length() - 2), ok);

			return ok;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_XMLDocument::ParseLine (std::string& line, C_XMLDocument::C_XMLNode*& element)
		{				
			while (true)
			{					
				//	Find opening tag
				size_t start = Find (line, '<');

				//	Opening tag not found
				if (start == line.npos) 
					break;

				size_t end = line.npos;
							
				//	Try to find comment chars: !--
				if ((start + 3 < line.size()) && (line[start + 1] == '!') && (line[start + 2] == '-') && (line[start + 3] == '-'))
				{
					end = line.find("-->", start + 4);
											   
					//	Closing comment chars: --> not found
					if (end == line.npos) 
						break;

					end += 2;
				}
				else
				{
					//	Find closing tag
					end = Find (line, '>', start + 1);

					//	Closing tag: > not found
					if (end == line.npos)
						break;
				}

				//	Try to find any characters other than: " ", "\t", "<"
				size_t body = line.find_first_not_of(" \t<");

				//	Found data before "<" sign
				if (body < start)
				{
					//	Cut content
					std::string body_str = line.substr(0, start);
					
					m_Column = 0;

					if (element != 0)
					{
						bool ok = true;

						//	Try to set content for passed element
						element->SetContent (ConvertFromXML(body_str, ok));

						if (!ok)
						{
							m_LastError = C_XMLError::ET_IncorrectContent;

							return false;
						}
					}
				}

				//	Parse tag element				
				if (!ParseTag (element, line.substr (start + 1, end - start - 1)))
				{
					return false;
				}
				
				//	Get next tag pair "<..>"
				line = line.substr (end + 1);
			}

			return true;
		}

		//------------------------------------------------------------------------------------------------------------------------

		size_t C_XMLDocument::Find (const std::string& text, char character, size_t start)
		{				
			bool kov = false;			
			char buff[16] = "\"_\0";

			buff[1] = character;

			size_t pos = start;

			while (true)
			{
				pos = text.find_first_of (buff, pos);

				if (pos == text.npos)
					break;
				else 
					if (text[pos] == '"')
					{
						kov = !kov;
						pos ++;
					}
					else 
						if (kov) 
							pos ++;
						else 
							break;
			}

			return pos;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_XMLDocument::ClearDeclaration ()
		{
			REDEEMER_SAFE_DELETE (m_Declaration);
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_XMLDocument::ClearRoot ()
		{
			REDEEMER_SAFE_DELETE (m_Root);			
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace FILESYSTEM
}	//	namespace REDEEMER

