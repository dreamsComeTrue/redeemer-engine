/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Script_Script.cpp
**		Object representing script entity
*/

//------------------------------------------------------------------------------------------------------------------------
#include "R_Script_Script.h"
#include "..\\RedeemerEngine.h"

#include "angelscript.h"
#include "scriptbuilder\\scriptbuilder.h"


// disable:  forcing value to bool 'true' or 'false' (performance warning)
#pragma warning (disable: 4800)

namespace REDEEMER
{
	namespace SCRIPT
	{
		//------------------------------------------------------------------------------------------------------------------------
		C_Script::C_Script (asIScriptEngine* engine, const char* script, const char* entryPoint, const char* moduleName) :
			m_FunctionID (-1),
			m_EntryPoint (entryPoint),
			m_Context (NULL),
			m_Engine (engine),
			m_ModuleName (moduleName),
			m_IsEnabled (true),
			m_UpdateFunctionID (-1)
		{
			m_Builder = new CScriptBuilder ();

			int result = m_Builder->StartNewModule(m_Engine,moduleName);

			if (result >= 0)
				m_Builder->AddSectionFromMemory(script);

			if (result >= 0)
				m_Builder->BuildModule();

			m_Context = engine->CreateContext();

			//	Failed to create context
			if (m_Context == 0)
				return;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Script::C_Script (asIScriptEngine* engine, const char* entryPoint, const char* moduleName) :
			m_FunctionID (-1),
			m_EntryPoint (entryPoint),
			m_Context (NULL),
			m_Engine (engine),
			m_ModuleName (moduleName),
			m_Builder (NULL),
			m_IsEnabled (true),
			m_UpdateFunctionID (-1)
		{
			m_Context = engine->CreateContext();

			//	Failed to create context
			if (m_Context == 0)
				return;
		}

		//------------------------------------------------------------------------------------------------------------------------

		C_Script::~C_Script ()
		{
			REDEEMER_SAFE_RELEASE (m_Context);
			REDEEMER_SAFE_DELETE (m_Builder);
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_Script::SetFunctionArgument_BYTE (const char* functionName, unsigned int argumentIndex, asBYTE value)
		{
			PrepareFunctionID(functionName);

			int result = m_Context->SetArgByte(argumentIndex, value);

			if (result < 0)
				REDEEMER_LOG << LOG_ERROR << L"Script: Wrong argument (" << argumentIndex << "): " << value << L"for function: " << functionName << LOG_ENDMESSAGE;

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_Script::SetFunctionArgument_WORD (const char* functionName, unsigned int argumentIndex, asWORD value)
		{
			PrepareFunctionID(functionName);

			int result = m_Context->SetArgWord (argumentIndex, value);

			if (result < 0)
				REDEEMER_LOG << LOG_ERROR << L"Script: Wrong argument (" << argumentIndex << "): " << value << L"for function: " << functionName << LOG_ENDMESSAGE;

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_Script::SetFunctionArgument_DOUBLEWORD (const char* functionName, unsigned int argumentIndex, asDWORD value)
		{
			PrepareFunctionID(functionName);

			int result = m_Context->SetArgDWord (argumentIndex, value);

			if (result < 0)
				REDEEMER_LOG << LOG_ERROR << L"Script: Wrong argument (" << argumentIndex << "): " << value << L"for function: " << functionName << LOG_ENDMESSAGE;

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_Script::SetFunctionArgument_QUADWORD (const char* functionName, unsigned int argumentIndex, asQWORD value)
		{
			PrepareFunctionID(functionName);

			int result = m_Context->SetArgQWord (argumentIndex, value);

			if (result < 0)
				REDEEMER_LOG << LOG_ERROR << L"Script: Wrong argument (" << argumentIndex << "): " << value << L"for function: " << functionName << LOG_ENDMESSAGE;

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_Script::SetFunctionArgument_FLOAT (const char* functionName, unsigned int argumentIndex, float value)
		{
			PrepareFunctionID(functionName);

			int result = m_Context->SetArgFloat (argumentIndex, value);

			if (result < 0)
				REDEEMER_LOG << LOG_ERROR << L"Script: Wrong argument (" << argumentIndex << "): " << value << L"for function: " << functionName << LOG_ENDMESSAGE;

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_Script::SetFunctionArgument_DOUBLE (const char* functionName, unsigned int argumentIndex, double value)
		{
			PrepareFunctionID(functionName);

			int result = m_Context->SetArgDouble (argumentIndex, value);

			if (result < 0)
				REDEEMER_LOG << LOG_ERROR << L"Script: Wrong argument (" << argumentIndex << "): " << value << L"for function: " << functionName << LOG_ENDMESSAGE;

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_Script::SetFunctionArgument_ADDRESS (const char* functionName, unsigned int argumentIndex, void* value)
		{
			PrepareFunctionID(functionName);

			int result = m_Context->SetArgAddress (argumentIndex, value);

			if (result < 0)
				REDEEMER_LOG << LOG_ERROR << L"Script: Wrong argument (" << argumentIndex << "): " << value << L"for function: " << functionName << LOG_ENDMESSAGE;

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_Script::SetFunctionArgument_OBJECT (const char* functionName, unsigned int argumentIndex, void* value)
		{
			PrepareFunctionID(functionName);

			int result = m_Context->SetArgObject (argumentIndex, value);

			if (result < 0)
				REDEEMER_LOG << LOG_ERROR << L"Script: Wrong argument (" << argumentIndex << "): " << value << L"for function: " << functionName << LOG_ENDMESSAGE;

			return result;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void* C_Script::GetFunctionArgumentPointer (const char* functionName, unsigned int argumentIndex)
		{
			PrepareFunctionID (functionName);

			return m_Context->GetAddressOfArg (argumentIndex);
		}

		//------------------------------------------------------------------------------------------------------------------------

		asBYTE C_Script::GetFunctionReturn_BYTE ()
		{
			return m_Context->GetReturnByte();
		}

		//------------------------------------------------------------------------------------------------------------------------

		asWORD C_Script::GetFunctionReturn_WORD ()
		{
			return m_Context->GetReturnWord();
		}

		//------------------------------------------------------------------------------------------------------------------------

		asDWORD C_Script::GetFunctionReturn_DOUBLEWORD ()
		{
			return m_Context->GetReturnDWord();
		}

		//------------------------------------------------------------------------------------------------------------------------

		asQWORD C_Script::GetFunctionReturn_QUADWORD ()
		{
			return m_Context->GetReturnQWord();
		}

		//------------------------------------------------------------------------------------------------------------------------

		float C_Script::GetFunctionReturn_FLOAT ()
		{
			return m_Context->GetReturnFloat();
		}

		//------------------------------------------------------------------------------------------------------------------------

		double C_Script::GetFunctionReturn_DOUBLE ()
		{
			return m_Context->GetReturnDouble();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void* C_Script::GetFunctionReturn_ADDRESS ()
		{
			 return m_Context->GetReturnAddress();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void* C_Script::GetFunctionReturn_OBJECT ()
		{
			 return m_Context->GetReturnObject();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void* C_Script::GetFunctionAddressOfReturnValue ()
		{
			return m_Context->GetAddressOfReturnValue();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Script::SetEnabled (bool enable)
		{
			m_IsEnabled = enable;
		}

		//------------------------------------------------------------------------------------------------------------------------

		bool C_Script::IsEnabled () const
		{
			return m_IsEnabled;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Script::SetUpdateFunction (const char* functionName)
		{
			//	Failed to create context
			if (m_Context == 0)
				return;

			// Find the function id for the function we want to execute.
			m_UpdateFunctionID = m_Engine->GetModule (m_ModuleName.c_str())->GetFunctionIdByName (functionName);

			if (m_UpdateFunctionID < 0)
				REDEEMER_LOG << LOG_ERROR << L"SCRIPT: The update function: " << functionName << L" was not found." << LOG_ENDMESSAGE;
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Script::Update ()
		{
			if (m_UpdateFunctionID < 0)
				return;

			//	Prepare the script context with the function we wish to execute. Prepare() must be called on the context before each new script 
			//	function that will be executed. Note, that if you intend to execute the same function several times, it might be a good idea to 
			//	store the function id returned by GetFunctionIDByDecl(), so that this relatively slow call can be skipped.
			int result = m_Context->Prepare (m_UpdateFunctionID);

			if (result < 0) 
			{
				REDEEMER_LOG << LOG_ERROR << L"SCRIPT: Failed to prepare the context." << LOG_ENDMESSAGE;

				m_Context->Release();

				return;
			}

			//	Actual executing th script
			m_Context->Execute();
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_Script::Execute ()
		{
			//	Failed to create context
			if (m_Context == 0)
				return -1;

			// Find the function id for the function we want to execute.
			int funcId = m_Engine->GetModule(m_ModuleName.c_str())->GetFunctionIdByDecl (m_EntryPoint.c_str());

			if (funcId < 0)
			{
				REDEEMER_LOG << LOG_ERROR << L"SCRIPT: The function: " << m_EntryPoint << L" was not found." << LOG_ENDMESSAGE;

				return -1;
			}

			//	Prepare the script context with the function we wish to execute. Prepare() must be called on the context before each new script 
			//	function that will be executed. Note, that if you intend to execute the same function several times, it might be a good idea to 
			//	store the function id returned by GetFunctionIDByDecl(), so that this relatively slow call can be skipped.
			int result = m_Context->Prepare(funcId);

			if (result < 0) 
			{
				REDEEMER_LOG << LOG_ERROR << L"SCRIPT: Failed to prepare the context." << LOG_ENDMESSAGE;

				m_Context->Release();

				return -1;
			}

			//	Actual executing th script
			return m_Context->Execute();   
		}

		//------------------------------------------------------------------------------------------------------------------------

		int C_Script::ExecuteFunction (const char* functionName, bool withArguments)
		{
			if (!withArguments)
				PrepareFunctionID (functionName);

			//	Actual executing th script
			return m_Context->Execute();
		}

		//------------------------------------------------------------------------------------------------------------------------

		void C_Script::PrepareFunctionID (const char* functionName)
		{
			//	Failed to create context
			if (m_Context == 0)
				return;

			// Find the function id for the function we want to execute.
			m_FunctionID = m_Engine->GetModule(m_ModuleName.c_str())->GetFunctionIdByName(functionName);

			if (m_FunctionID < 0)
			{
				REDEEMER_LOG << LOG_ERROR << L"SCRIPT: The function: " << functionName << L" was not found." << LOG_ENDMESSAGE;

				return;
			}

			//	Prepare the script context with the function we wish to execute. Prepare() must be called on the context before each new script 
			//	function that will be executed. Note, that if you intend to execute the same function several times, it might be a good idea to 
			//	store the function id returned by GetFunctionIDByDecl(), so that this relatively slow call can be skipped.
			int result = m_Context->Prepare (m_FunctionID);

			if (result < 0) 
			{
				REDEEMER_LOG << LOG_ERROR << L"SCRIPT: Failed to prepare the context." << LOG_ENDMESSAGE;

				m_Context->Release();
			}
		}

		//------------------------------------------------------------------------------------------------------------------------

	}	//	namespace SCRIPT
}	//	namespace REDEEMER

