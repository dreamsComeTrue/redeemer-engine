/*!
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	R_Script_Script.h
**		Object representing script entity
*/

//------------------------------------------------------------------------------------------------------------------------
#ifndef _R_SCRIPT_SCRIPT_H_
#define _R_SCRIPT_SCRIPT_H_

#include <string>

//	From AngelScript.h

//
// asBYTE  =  8 bits
// asWORD  = 16 bits
// asDWORD = 32 bits
// asQWORD = 64 bits
// asPWORD = size of pointer
//
typedef unsigned char  asBYTE;
typedef unsigned short asWORD;
typedef unsigned int   asUINT;
typedef size_t         asPWORD;
#ifdef __LP64__
typedef unsigned int  asDWORD;
typedef unsigned long asQWORD;
typedef long asINT64;
#else
typedef unsigned long asDWORD;
#if defined(__GNUC__) || defined(__MWERKS__)
typedef unsigned long long asQWORD;
typedef long long asINT64;
#else
typedef unsigned __int64 asQWORD;
typedef __int64 asINT64;
#endif
#endif

class CScriptBuilder;
class asIScriptContext;
class asIScriptEngine;

namespace REDEEMER
{
	namespace SCRIPT
	{
		class C_ScriptManager;

		/*!
		**	\brief Object representing script entity
		*/
		class C_Script 
		{
			friend class C_ScriptManager;

		public:
			/*!	Constructor
			*/
			C_Script (asIScriptEngine* engine, const char* script, const char* entryPoint, const char* moduleName);

			/*!	Constructor
			*/
			C_Script (asIScriptEngine* engine, const char* entryPoint, const char* moduleName);

			/*!	Destructor
			*/
			virtual ~C_Script ();

			/*!	Sets function argument (BYTE)
			**	\param functionName name of the function to parameters are bound to
			**	\param argumentIndex index of function's argument (starts with 0)
			**	\param value actual value
			*/
			int SetFunctionArgument_BYTE (const char* functionName, unsigned int argumentIndex, asBYTE value);
		
			/*!	Sets function argument (WORD)
			**	\param functionName name of the function to parameters are bound to
			**	\param argumentIndex index of function's argument (starts with 0)
			**	\param value actual value
			*/
			int SetFunctionArgument_WORD (const char* functionName, unsigned int argumentIndex, asWORD value);

			/*!	Sets function argument (DOUBLEWORD)
			**	\param functionName name of the function to parameters are bound to
			**	\param argumentIndex index of function's argument (starts with 0)
			**	\param value actual value
			*/
			int SetFunctionArgument_DOUBLEWORD (const char* functionName, unsigned int argumentIndex, asDWORD value);

			/*!	Sets function argument (QUADWORD)
			**	\param functionName name of the function to parameters are bound to
			**	\param argumentIndex index of function's argument (starts with 0)
			**	\param value actual value
			*/
			int SetFunctionArgument_QUADWORD (const char* functionName, unsigned int argumentIndex, asQWORD value);

			/*!	Sets function argument (FLOAT)
			**	\param functionName name of the function to parameters are bound to
			**	\param argumentIndex index of function's argument (starts with 0)
			**	\param value actual value
			*/
			int SetFunctionArgument_FLOAT (const char* functionName, unsigned int argumentIndex, float value);

			/*!	Sets function argument (DOUBLE)
			**	\param functionName name of the function to parameters are bound to
			**	\param argumentIndex index of function's argument (starts with 0)
			**	\param value actual value
			*/
			int SetFunctionArgument_DOUBLE (const char* functionName, unsigned int argumentIndex, double value);

			/*!	Sets function argument (ADDRESS)
			**	\param functionName name of the function to parameters are bound to
			**	\param argumentIndex index of function's argument (starts with 0)
			**	\param value actual value
			*/
			int SetFunctionArgument_ADDRESS (const char* functionName, unsigned int argumentIndex, void* value);

			/*!	Sets function argument (OBJECT)
			**	\param functionName name of the function to parameters are bound to
			**	\param argumentIndex index of function's argument (starts with 0)
			**	\param value actual value
			*/
			int SetFunctionArgument_OBJECT (const char* functionName, unsigned int argumentIndex, void* value);

			/*!	Returns a pointer to the argument for assignment. 
			*/
			void* GetFunctionArgumentPointer (const char* functionName, unsigned int argumentIndex);

			/*!	Gets function's return value (BYTE)
			*/
			asBYTE GetFunctionReturn_BYTE ();

			/*!	Gets function's return value (WORD)
			*/
			asWORD GetFunctionReturn_WORD ();

			/*!	Gets function's return value (DOUBLEWORD)
			*/
			asDWORD GetFunctionReturn_DOUBLEWORD ();

			/*!	Gets function's return value (QUADWORD)
			*/
			asQWORD GetFunctionReturn_QUADWORD ();

			/*!	Gets function's return value (FLOAT)
			*/
			float GetFunctionReturn_FLOAT ();

			/*!	Gets function's return value (DOUBLE)
			*/
			double GetFunctionReturn_DOUBLE ();

			/*!	Gets function's return value (ADDRESS)
			*/
			void* GetFunctionReturn_ADDRESS ();

			/*!	Gets function's return value (OBJECT)
			*/
			void* GetFunctionReturn_OBJECT ();

			/*!	Returns the address of returned value
			*/
			void* GetFunctionAddressOfReturnValue ();

			/*!	Sets script enabled
			*/
			void SetEnabled (bool enable = true);

			/*!	Checks, if script is enabled
			*/
			bool IsEnabled () const;

			/*!	Sets update function
			*/
			void SetUpdateFunction (const char* functionName);

			/*!	Main update routine
			*/
			void Update ();

		private:
			/*!	Try to execute script context
			*/
			int Execute ();

			/*!	Try to execute script's function
			**	\param functionName name of the function to execute
			**	\param withArguments true, if we set function parameters
			*/
			int ExecuteFunction (const char* functionName, bool withArguments = false);

			/*!	Helper function for Set* methods
			*/
			void PrepareFunctionID (const char* functionName);

			std::string				m_EntryPoint;
			std::string				m_ModuleName;
			int						m_FunctionID;			///<	ID of the function to execute
			int						m_UpdateFunctionID;
			bool					m_IsEnabled;

			//	AngelScript variables
			CScriptBuilder*			m_Builder;
			asIScriptContext*		m_Context;
			asIScriptEngine*		m_Engine;
		};

	}	//	namespace SCRIPT
}	//	namespace REDEEMER

#endif	//	_R_SCRIPT_SCRIPT_H_
