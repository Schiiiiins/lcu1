// ConsoleLogger.h: Schnittstelle für die Klasse ConsoleLogger.
//
//////////////////////////////////////////////////////////////////////

#ifndef _CONSOLELOGGER_H_
#define _CONSOLELOGGER_H_

#include "dllexport.h"
#include "Logger.h"
#include <string>

class ELVUTILS_DLLEXPORT ConsoleLogger : public Logger  
{
public:
	ConsoleLogger();
	virtual ~ConsoleLogger();

protected:
	std::string m_strFilename;
	bool DoLog(LogLevel l, const char *t, const char *msg);
	std::ostream* stream;
};

#endif
