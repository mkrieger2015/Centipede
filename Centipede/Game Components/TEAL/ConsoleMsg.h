// ConsoleMsg.h
// Andre Berthiaume, Juen 2012

// Tools to make errors more verbose/visible

#ifndef _ConsoleMsg
#define _ConsoleMsg

#include <stdexcept>

class ConsoleMsg
{
private:
	static void DebugOutput( std::string msg);

public:
	static void WriteLine( std::string msg);
	static void ShowError( std::string msg);
	static void ShowStatus( std::string msg);
};

class TEALexception : public std::runtime_error
{
public:
	TEALexception( const std::string &msg): std::runtime_error(msg){};
};


#endif _ConsoleMsg