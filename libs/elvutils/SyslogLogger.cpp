// SyslogLogger.cpp: Implementierung der Klasse SyslogLogger.
//
//////////////////////////////////////////////////////////////////////

#include "SyslogLogger.h"
#ifndef WIN32
#include <syslog.h>
static const int PRIORITY[]={0, LOG_DEBUG, LOG_INFO, LOG_NOTICE, LOG_WARNING, LOG_ERR, LOG_CRIT};
#else
#define openlog(a,b,c)
#define closelog()
#define syslog(a,b)
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

SyslogLogger::SyslogLogger(const char* id)
{
	openlog( id, 0, LOG_USER);
	log_timestamp=false;
}

SyslogLogger::~SyslogLogger()
{
	closelog();
}

bool SyslogLogger::DoLog(Logger::LogLevel l, const char *t, const char *msg)
{
	syslog(PRIORITY[(int)l]|LOG_USER, msg);
	return true;
}
