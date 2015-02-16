// ConsoleMsg.h
// Andre Berthiaume, June 2012

// Tools to make errors more verbose/visible

#include "ConsoleMsg.h"
#include <Windows.h>
#include "Tools.h"

using namespace std;

void ConsoleMsg::DebugOutput( std::string msg)
{
	cout << msg;

	std::wstring stemp = std::wstring(msg.begin(), msg.end());
	LPCWSTR sw = stemp.c_str();
	OutputDebugString( sw ) ;
}


void ConsoleMsg::WriteLine( std::string msg)
{
	DebugOutput( msg + "\n");
}

void ConsoleMsg::ShowError( std::string msg)
{
	msg =  "TEAL ERROR: " + msg + "\n";
	DebugOutput(msg);
	throw TEALexception(msg);	// We throw the exception as a way to stop and allow the using to see the call stack.
}

void ConsoleMsg::ShowStatus( std::string msg)
{	
	DebugOutput( "STATUS: " + msg + "\n");
}

