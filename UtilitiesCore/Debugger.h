//===============================================================================
// @ IvDebugger.h
// 
// Convenience class to dump debug information
// ------------------------------------------------------------------------------
// Copyright (C) 2008 by Elsevier, Inc. All rights reserved.
//
//
//
// Use DEBUG_OUT combined with operator<< to dump debug text
//===============================================================================

#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_

//-------------------------------------------------------------------------------
//-- Dependencies ---------------------------------------------------------------
//-------------------------------------------------------------------------------

#include <sstream>
#include <fstream>

//-------------------------------------------------------------------------------
//-- Typedefs, Structs ----------------------------------------------------------
//-------------------------------------------------------------------------------

#define ERROR_OUT( File, Function, Line, Msg, ... ) \
{ \
    gDebugger << File << "::" << Function << ":" << Line << " Log: " << Msg; \
    gDebugger.Flush(); \
}

#ifdef _DEBUG
#define DEBUG_OUT( x )  ERROR_OUT(  __FILE__, __FUNCTION__, __LINE__, x )
#else
#define DEBUG_OUT( x )
#endif

//-------------------------------------------------------------------------------
//-- Classes --------------------------------------------------------------------
//-------------------------------------------------------------------------------
namespace UtilitiesCore
{
	class Debugger : public std::ostringstream
	{
	public:
		// constructor/destructor
		Debugger();
		virtual ~Debugger();
    
		// Default (NULL or empty filename) will turn off file logging
		void DumpToFile( char* filename = 0 );
    
		void Flush( void );
    
	protected:
		std::ofstream mOutFile;
    
	private:
		// copy operations
		Debugger( const Debugger& other );
		Debugger& operator=( const Debugger& other );
	};
}
//-------------------------------------------------------------------------------
//-- Inlines --------------------------------------------------------------------
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//-- Externs --------------------------------------------------------------------
//-------------------------------------------------------------------------------

extern UtilitiesCore::Debugger gDebugger;

#endif // _DEBUGGER_H_
