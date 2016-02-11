// FileLogger.h: Schnittstelle für die Klasse FileLogger.
//
//////////////////////////////////////////////////////////////////////

#ifndef _FILELOGGER_H_
#define _FILELOGGER_H_

#include "dllexport.h"
#include "Logger.h"
#include <string>

class ELVUTILS_DLLEXPORT FileLogger : public Logger  
{
public:
	void SetStream(std::ostream* of);
	void SetFilename(const char* fn);
	FileLogger();
	virtual ~FileLogger();

protected:
	std::string m_strFilename;
	bool DoLog(LogLevel l, const char *t, const char *msg);
	std::ostream* stream;
};

#endif
