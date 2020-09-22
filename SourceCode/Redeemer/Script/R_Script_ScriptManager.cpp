/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Script_ScriptManager.cpp
**		Engine's script management system
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Script_ScriptManager.h"
#include "R_Script_Script.h"
#include "..\\Core\\R_Core_Convert.h"
#include "..\\RedeemerEngine.h"

#include "angelscript.h"
#include "scriptstdstring\\scriptstdstring.h"
#include "scriptmath\\scriptmath.h"

// disable: deprecation warnings when using CRT calls in VC8
#pragma warning (disable: 4996)

namespace REDEEMER
{
	namespace SCRIPT
	{
		/*!	Class used for reading/writing compiled bytecode
		*/
		class CBytecodeStream : public asIBinaryStream
		{
		public:
			/*!	Constructor
			*/
			CBytecodeStream () 
			{
				m_WritePointer = 0;
				m_ReadPointer = 0;
			}

			/*!	Writes bytecode data
			*/
			void Write (const void *ptr, asUINT size) 
			{
				if (size == 0) 
					return; 
				
				Buffer.resize (Buffer.size () + size); 				
				memcpy(&Buffer[m_WritePointer], ptr, size);
				
				m_WritePointer += size;
			}

			/*!	Reads bytecode data
			*/
			void Read (void *ptr, asUINT size) 
			{
				memcpy (ptr, &Buffer[m_ReadPointer], size); 
				
				m_ReadPointer += size;
			}

			std::vector<asBYTE> Buffer;				///<	Buffer for holding data

		private:
			int					m_ReadPointer;
			int					m_WritePointer;
		};


		//------------------------------------------------------------------------------------------------------------------------
		C_ScriptManager::C_ScriptManager () :
			m_ScriptEngine (NULL)
		{
			//	EMPTY
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_ScriptManager::~C_ScriptManager ()
		{
			if (IsInitialized())
				Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_ScriptManager::Initialize ()
		{
			C_BaseClass::Initialize();

			m_ScriptEngine = asCreateScriptEngine (ANGELSCRIPT_VERSION);

			m_CompiledStream = new CBytecodeStream ();

			// Set the message callback to receive information on errors in human readable form.
			// It's recommended to do this right after the creation of the engine, because if
			// some registration fails the engine may send valuable information to the message
			// stream.
			int result = m_ScriptEngine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL); assert (result >= 0);

			// AngelScript doesn't have a built-in string type, as there is no definite standard 
			// string type for C++ applications. Every developer is free to register it's own string type.
			// The SDK do however provide a standard add-on for registering a string type, so it's not
			// necessary to implement the registration yourself if you don't want to.
			RegisterStdString (m_ScriptEngine);

			RegisterScriptMath (m_ScriptEngine);

			REDEEMER_LOG << LOG_INFO << L"Script Manager (AngelScript ver. " << ANGELSCRIPT_VERSION_STRING << ") initialized!" << LOG_ENDMESSAGE;

			return RESULT_OK;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_ScriptManager::MessageCallback (const asSMessageInfo *msg, void *param)
		{
			switch (msg->type)
			{
			case asMSGTYPE_WARNING:
				REDEEMER_LOG << LOG_WARNING << msg->section << " (" << msg->row << ", " << msg->col << ") : " << msg->message << LOG_ENDMESSAGE;

				break;

			case asMSGTYPE_INFORMATION:
				REDEEMER_LOG << LOG_INFO << msg->section << " (" << msg->row << ", " << msg->col << ") : " << msg->message << LOG_ENDMESSAGE;

				break;

			case asMSGTYPE_ERROR:
				REDEEMER_LOG << LOG_ERROR << msg->section << " (" << msg->row << ", " << msg->col << ") : " << msg->message << LOG_ENDMESSAGE;

				break;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_ScriptManager::Finalize ()
		{
			// Release the engine
			REDEEMER_SAFE_RELEASE (m_ScriptEngine);

			if (!m_Scripts.empty())
			{				
				for (std::map <std::wstring, C_Script*>::iterator it = m_Scripts.begin(); it != m_Scripts.end(); ++it)
					REDEEMER_SAFE_DELETE (it->second);

				m_Scripts.clear();
			}				  

			REDEEMER_SAFE_DELETE (m_CompiledStream);

			REDEEMER_LOG << LOG_INFO << L"Script Manager finalized!" << LOG_ENDMESSAGE;

			return C_BaseClass::Finalize();
		}

		//------------------------------------------------------------------------------------------------------------------------

		asIScriptEngine& C_ScriptManager::GetScriptEngine ()
		{
			return *m_ScriptEngine;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Script* C_ScriptManager::CreateScript (FILESYSTEM::IDataFile* file, const char* entryPoint, const char* moduleName)
		{
			// Create new script
			C_Script* script = new C_Script (m_ScriptEngine, file->GetData().c_str(), entryPoint, moduleName);

			return script;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_ScriptManager::AddScript (const std::wstring& name, C_Script* script)
		{
			// The script already exists, so return
			if (m_Scripts.find (name) != m_Scripts.end())
				return;

			m_Scripts [name] = script;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_ScriptManager::RemoveScript (const std::wstring& name)
		{
			// Script does not exist, so exit
			if (m_Scripts.find (name) == m_Scripts.end())
				return;

			REDEEMER_SAFE_DELETE (m_Scripts [name]);
			m_Scripts.erase (name);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Script* C_ScriptManager::GetScript (const std::wstring& name)
		{
			if (m_Scripts.find (name) == m_Scripts.end())
			{
				return NULL;
			}

			return m_Scripts[name];
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_ScriptManager::SaveCompiledScriptToFile (const std::wstring& scriptName, const char* fileName)
		{
			if (m_Scripts.find (scriptName) == m_Scripts.end())
				return;

			std::string out;

			CORE::C_Convert::ConvertUnicodeToChars(&out, scriptName);

			m_ScriptEngine->GetModule(m_Scripts[scriptName]->m_ModuleName.c_str())->SaveByteCode(m_CompiledStream);

			FILE* file;

			file = fopen (fileName, "wb+");

			for (unsigned int i = 0; i < m_CompiledStream->Buffer.size(); i ++)
				fwrite (&m_CompiledStream->Buffer[i], 1, sizeof(asBYTE), file);

			fclose (file);
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Script* C_ScriptManager::LoadCompiledScriptFromFile (FILESYSTEM::IDataFile* file, const char* entryPoint, const char* moduleName)
		{
			// Create new script
			C_Script* script = new C_Script (m_ScriptEngine, entryPoint, moduleName);

			m_CompiledStream->Buffer.clear();

			for (int i = 0; i < file->GetSize(); i ++)
				m_CompiledStream->Buffer.push_back(file->GetData().c_str()[i]);

			m_ScriptEngine->GetModule (moduleName, asGM_ALWAYS_CREATE)->LoadByteCode(m_CompiledStream);

			return script;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_ScriptManager::ExecuteScript (C_Script* script)
		{
			if (!script)
				return;

			//	We found script - try to execute it
			script->Execute ();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_ScriptManager::ExecuteScriptFunction (C_Script* script, const char* functionName, bool withArguments)
		{
			if (!script)
				return;

			//	We found script - try to execute it
			script->ExecuteFunction (functionName, withArguments);
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_ScriptManager::RegisterGlobalFunction (const char* declaration, void* functionPointer)
		{
			int result = m_ScriptEngine->RegisterGlobalFunction (declaration, asFUNCTION(functionPointer), asCALL_CDECL);

			if (result < 0)
				REDEEMER_LOG << LOG_ERROR << L"SCRIPT: Can't register global function: " << declaration << LOG_ENDMESSAGE;

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_ScriptManager::RegisterGlobalProperty (const char* declaration, void* propertyPointer)
		{
			int result = m_ScriptEngine->RegisterGlobalProperty(declaration, propertyPointer);

			if (result < 0)
				REDEEMER_LOG << LOG_ERROR << L"SCRIPT: Can't register global property: " << declaration << LOG_ENDMESSAGE;

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_ScriptManager::RegisterEnumType (const char* declaration)
		{
			int result = m_ScriptEngine->RegisterEnum (declaration);

			if (result < 0)
				REDEEMER_LOG << LOG_ERROR << L"SCRIPT: Can't register enum type: " << declaration << LOG_ENDMESSAGE;

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_ScriptManager::RegisterEnumValue (const char* enumDeclaration, const char* name, int value)
		{
			int result = m_ScriptEngine->RegisterEnumValue (enumDeclaration, name, value);

			if (result < 0)
				REDEEMER_LOG << LOG_ERROR << L"SCRIPT: Can't register enum value: " << enumDeclaration << L" " << name << " = " << value << LOG_ENDMESSAGE;

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_ScriptManager::RegisterTypedef (const char* type, const char* declaration)
		{
			int result = m_ScriptEngine->RegisterTypedef (type, declaration);

			if (result < 0)
				REDEEMER_LOG << LOG_ERROR << L"SCRIPT: Can't register typedef: " << type << LOG_ENDMESSAGE;

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_ScriptManager::RegisterInterface (const char* name)
		{
			int result = m_ScriptEngine->RegisterInterface (name);

			if (result < 0)
				REDEEMER_LOG << LOG_ERROR << L"SCRIPT: Can't register interface: " << name << LOG_ENDMESSAGE;

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_ScriptManager::RegisterInterfaceMethod (const char* interfaceName, const char* declaration)
		{
			int result = m_ScriptEngine->RegisterInterfaceMethod (interfaceName, declaration);

			if (result < 0)
				REDEEMER_LOG << LOG_ERROR << L"SCRIPT: Can't register interface method: " << interfaceName << L"::" << declaration << LOG_ENDMESSAGE;

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_ScriptManager::RegisterObjectType (const char* declaration, int objectSize, DWORD type)
		{
			int result = m_ScriptEngine->RegisterObjectType (declaration, objectSize, static_cast<asDWORD>(type));

			if (result < 0)
				REDEEMER_LOG << LOG_ERROR << L"SCRIPT: Can't register object type: " << declaration << LOG_ENDMESSAGE;

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_ScriptManager::RegisterObjectProperty (const char* objectName, const char* declaration, int offset)
		{
			int result = m_ScriptEngine->RegisterObjectProperty(objectName, declaration, offset);

			if (result < 0)
				REDEEMER_LOG << LOG_ERROR << L"SCRIPT: Can't register object property: " << objectName << L"::" << declaration << LOG_ENDMESSAGE;

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_ScriptManager::RegisterObjectMethod (const char* objectName, const char* declaration, const asSFuncPtr &functionPointer)
		{
			int result = m_ScriptEngine->RegisterObjectMethod (objectName, declaration, functionPointer, asCALL_THISCALL);

			if (result < 0)
				REDEEMER_LOG << LOG_ERROR << L"SCRIPT: Can't register object method: " << objectName << L"::" << declaration << LOG_ENDMESSAGE;

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_ScriptManager::UpdateAllScripts ()
		{
			for (std::map <std::wstring, C_Script*>::iterator it = m_Scripts.begin(); it != m_Scripts.end(); ++it)
				if ((*it->second).IsEnabled())
					(*it->second).Update();
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace SCRIPT
}	//	namespace REDEEMER

