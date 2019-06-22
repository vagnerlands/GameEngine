//===============================================================================
// @ IvDebugger.cpp
// 
// Convenience class to dump debug information
// ------------------------------------------------------------------------------
// Copyright (C) 2008 by Elsevier, Inc. All rights reserved.
//
//
//
// Use DEBUG_OUT combined with operator<< to dump debug text
//===============================================================================

//-------------------------------------------------------------------------------
//-- Dependencies ---------------------------------------------------------------
//-------------------------------------------------------------------------------

#ifdef WIN32
#include <windows.h>
#endif
#include "Debugger.h"

//-------------------------------------------------------------------------------
//-- Static Members -------------------------------------------------------------
//-------------------------------------------------------------------------------

UtilitiesCore::Debugger gDebugger;

//-------------------------------------------------------------------------------
//-- Methods --------------------------------------------------------------------
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// @ IvDebugger::IvDebugger()
//-------------------------------------------------------------------------------
// Default constructor
//-------------------------------------------------------------------------------
UtilitiesCore::Debugger::Debugger()
{
}   // End of IvDebugger::IvDebugger()


//-------------------------------------------------------------------------------
// @ IvDebugger::~IvDebugger()
//-------------------------------------------------------------------------------
// Destructor
//-------------------------------------------------------------------------------
UtilitiesCore::Debugger::~Debugger()
{
    if (mOutFile.is_open())
        mOutFile.close();

}   // End of IvDebugger::~IvDebugger()

//-------------------------------------------------------------------------------
// @ IvDebugger::DumpToFile()
//-------------------------------------------------------------------------------
// Set file to save debug info to
//-------------------------------------------------------------------------------
void
UtilitiesCore::Debugger::DumpToFile( char* string )
{
    if (mOutFile.is_open())
        mOutFile.close();

    // If a NULL filename is passed in, return without trying to open a file
    if (!string || string[0] == '\0')
        return;

    mOutFile.open(string);    

}   // End of IvDebugger::Put()


//-------------------------------------------------------------------------------
// @ IvDebugger::Flush()
//-------------------------------------------------------------------------------
// Flush debugger info
//-------------------------------------------------------------------------------
void
UtilitiesCore::Debugger::Flush( void )
{
    static int executeFlush = 0;
    // Get the buffer    
    std::string strText =  str();

    const char* cString = strText.c_str();

    // Dump to screen
#ifdef WIN32
 //   OutputDebugString( cString );
#endif

    // Write to output file, if open
    if (mOutFile.is_open())
    {
        mOutFile << cString;
        if (++executeFlush > 5)
        {
            mOutFile.flush();
            executeFlush = 0;
        }
    }

    // reset the buffer for the next debugging info
    str("");

}   // End of IvDebugger::Flush()

