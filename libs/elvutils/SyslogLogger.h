// SyslogLogger.h: Schnittstelle für die Klasse SyslogLogger.
//
//////////////////////////////////////////////////////////////////////

#ifndef _SYSLOGLOGGER_H_
#define _SYSLOGLOGGER_H_

#include "dllexport.h"
#include "Logger.h"

class ELVUTILS_DLLEXPORT SyslogLogger : public Logger  
{
public:
	SyslogLogger(const char* id="");
	virtual ~SyslogLogger();
protected:
	virtual bool DoLog(LogLevel l, const char* t, const char* msg);

};

#endif
