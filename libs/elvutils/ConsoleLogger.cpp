// ConsoleLogger.cpp: Implementierung der Klasse ConsoleLogger.
//
//////////////////////////////////////////////////////////////////////

#include "ConsoleLogger.h"
#include <iostream>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

ConsoleLogger::ConsoleLogger()
{
	log_timestamp=false;
}

ConsoleLogger::~ConsoleLogger()
{
}

bool ConsoleLogger::DoLog(Logger::LogLevel l, const char *t, const char *msg)
{
//	std::cout<<t<<" ";
	switch(l){
	case LOG_DEBUG:
		std::cerr<<"<Debug>";
		break;
	case LOG_INFO:
		std::cerr<<"<Info>";
		break;
	case LOG_NOTICE:
		std::cerr<<"<Notice>";
		break;
	case LOG_WARNING:
		std::cerr<<"<Warning>";
		break;
	case LOG_ERROR:
		std::cerr<<"<Error>";
		break;
	case LOG_FATAL_ERROR:
		std::cerr<<"<Fatal error>";
		break;
	default:
		break;
	}
	std::cerr<<" "<<msg<<std::endl;
	return true;
}
