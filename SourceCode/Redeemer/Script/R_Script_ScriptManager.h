/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Script_ScriptManager.h
**		Engine's script management system
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_SCRIPT_SCRIPTMANAGER_H_
#define _R_SCRIPT_SCRIPTMANAGER_H_

#include "..\\Core\\R_Core_Singleton.h"
#include "..\\Core\\R_Core_BaseClass.h"

#include <map>
#include <string>
#include <Windows.h>

class asIScriptEngine;
struct asSMessageInfo;
struct asSFuncPtr;

#define SCRIPT_OBJECT_METHOD(className,methodName) asMETHOD (className,methodName)

namespace REDEEMER
{
	namespace FILESYSTEM
	{
		class IDataFile;
	}

	namespace SCRIPT
	{
		class C_Script;
		class CBytecodeStream;

		/*!	Types of script object
		*/
		enum EScriptObjectType
		{
			SOT_ReferenceType			= 0x01,
			SOT_ValueType				= 0x02,
			SOT_GarbageCollected		= 0x04,
			SOT_PlainOldData			= 0x08,
			SOT_NoHandle				= 0x10,
			SOT_Scoped					= 0x20,
			SOT_AppClass				= 0x100,
			SOT_AppClassConstructor		= 0x200,
			SOT_AppClassDestructor		= 0x400,
			SOT_AppClassAssignment		= 0x800,
			SOT_AppPrimitive			= 0x1000,
			SOT_AppFloat				= 0x2000,
			SOT_ScriptObject			= 0x10000
		};

		/*!
		**	\brief Engine's script management system
		*/
		class C_ScriptManager : public CORE::C_Singleton<C_ScriptManager>, public CORE::C_BaseClass
		{
		public:
			/*!	Constructor
			*/
			C_ScriptManager ();

			/*!	Destructor
			*/
			virtual ~C_ScriptManager ();

			/*!	\brief	Initialization code
			*/
			virtual int Initialize ();

			/*!	\brief	Finalization code
			*/
			virtual int Finalize ();

			/*!	Creates new script
			*/
			C_Script* CreateScript (FILESYSTEM::IDataFile* file, const char* entryPoint, const char* moduleName);

			/*!	Adds script to the collection
			*/
			void AddScript (const std::wstring& name, C_Script* script);

			/*!	Removes script from the collection
			*/
			void RemoveScript (const std::wstring& name);

			/*!	Returns script with given name
			*/
			C_Script* GetScript (const std::wstring& name);

			/*!	Saves compiled script to file
			*/
			void SaveCompiledScriptToFile (const std::wstring& scriptName, const char* fileName);

			/*!	Loads compiled script from file
			*/
			C_Script* LoadCompiledScriptFromFile (FILESYSTEM::IDataFile* file, const char* entryPoint, const char* moduleName);

			/*!	Executes given script
			*/
			void ExecuteScript (C_Script* script);

			/*!	Executes script's function with give name
			**	\param script script where function resides
			**	\param functionName name of the function to execute
			**	\param withArguments true, if we set function parameters
			*/
			void ExecuteScriptFunction (C_Script* script, const char* functionName, bool withArguments = false);

			/*!	Registers global function for executing from script
			**	\return < 0 if failure
			*/
			int RegisterGlobalFunction (const char* declaration, void* functionPointer);

			/*!	Registers global property - variable for executing from script
			**	\return	< 0 if failure
			*/
			int RegisterGlobalProperty (const char* declaration, void* propertyPointer);

			/*!	Registers new enum type
			**	\return	< 0 if failure
			*/
			int RegisterEnumType (const char* declaration);

			/*!	Registers new enum value
			**	\return < 0 if failure
			*/
			int RegisterEnumValue (const char* enumDeclaration, const char* name, int value);

			/*!	Registers typedef
			**	\return < 0 if failure
			*/
			int RegisterTypedef (const char* type, const char* declaration);

			/*!	Registers new interface
			**	\return < 0 if failure
			*/
			int RegisterInterface (const char* name);

			/*!	Registers an interface method
			**	\return < 0 if failure
			*/
			int RegisterInterfaceMethod (const char* interfaceName, const char* declaration);

			/*!	Registers new object type (class)
			**	\return < 0 if failure
			*/
			int RegisterObjectType (const char* declaration, int objectSize, DWORD type);

			/*!	Registers new object type's property (class field)
			**	\return < 0 if failure
			*/
			int RegisterObjectProperty (const char* objectName, const char* declaration, int offset);

			/*!	Registers new object type's method (class method)
			**	\return < 0 if failure
			*/
			int RegisterObjectMethod (const char* objectName, const char* declaration, const asSFuncPtr &functionPointer);

			/*!	Updates all scripts
			*/
			void UpdateAllScripts ();

			/*!	Returns reference to scripting engine
			*/
			asIScriptEngine& GetScriptEngine ();

		private:	
			/*!	Provides callback mechanism for reporting script errors
			*/
			static void MessageCallback (const asSMessageInfo *msg, void *param);

			asIScriptEngine*					m_ScriptEngine;		///<	Script engine
			CBytecodeStream*					m_CompiledStream;	///<	Used for scripts precompilation 
			std::map<std::wstring, C_Script*>	m_Scripts;			///<	Collection of scripts
		};

	}	//	namespace SCRIPT
}	//	namespace REDEEMER

#endif	//	_R_SCRIPT_SCRIPTMANAGER_H_
