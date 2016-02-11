// Logger.cpp: Implementierung der Klasse Logger.
//
//////////////////////////////////////////////////////////////////////

#include "Logger.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>

#ifdef WIN32
#define vsnprintf _vsnprintf
#endif

/*static*/ Logger* Logger::s_logger=NULL;

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

Logger::Logger()
{
	m_level=LOG_ERROR;
	pthread_mutex_init(&mutex, NULL);
	bufpointer=m_buffer;
	log_timestamp=true;
}

Logger::~Logger()
{
	pthread_mutex_destroy(&mutex);
}

bool Logger::Log(Logger::LogLevel l, const char *format ...)
{
	if(!s_logger)return false;
	if(l<s_logger->GetLevel())return false;
	char* timestamp=NULL;
	if(s_logger->log_timestamp){
		struct tm* t;
		time_t ti=time(NULL);
		t=localtime(&ti);
        timestamp=new char[128];
		strftime(timestamp, 128, "%d.%m.%Y %H:%M:%S", t);
	}
	pthread_mutex_lock(&s_logger->mutex);
	va_list args;
	va_start(args, format);
	vsnprintf(s_logger->bufpointer, sizeof(s_logger->m_buffer)-(s_logger->bufpointer-s_logger->m_buffer), format, args);
	va_end(args);	
	bool retval=s_logger->DoLog(l, timestamp, s_logger->m_buffer);
	pthread_mutex_unlock(&s_logger->mutex);
    if(timestamp)delete timestamp;
	return retval;
}

bool Logger::LogRealm(const char* realm, Logger::LogLevel l, const char *format ...)
{
	if(!s_logger)return false;
	if(l<s_logger->GetLevelRealm(realm))return false;
	char* timestamp=NULL;
	if(s_logger->log_timestamp){
		struct tm* t;
		time_t ti=time(NULL);
		t=localtime(&ti);
		timestamp=new char[128];
		strftime(timestamp, 128, "%d.%m.%Y %H:%M:%S", t);
	}
	pthread_mutex_lock(&s_logger->mutex);
	va_list args;
	va_start(args, format);
	vsnprintf(s_logger->bufpointer, sizeof(s_logger->m_buffer)-(s_logger->bufpointer-s_logger->m_buffer), format, args);
	va_end(args);	
	bool retval=s_logger->DoLog(l, timestamp, s_logger->m_buffer);
	pthread_mutex_unlock(&s_logger->mutex);
	if(timestamp)delete timestamp;
	return retval;
}

bool Logger::DoLog(Logger::LogLevel l, const char *t, const char *msg)
{
	return false;
}

void Logger::SetLevel(Logger::LogLevel l)
{
	pthread_mutex_lock(&mutex);
	m_level=l;
	pthread_mutex_unlock(&mutex);
}

void Logger::SetLevelRealm(const char* realm, Logger::LogLevel l)
{
	pthread_mutex_lock(&mutex);
	m_level_by_realm[realm] = l;
	pthread_mutex_unlock(&mutex);
}

Logger::LogLevel Logger::GetLevel()
{
	LogLevel l;
	pthread_mutex_lock(&mutex);
	l=m_level;
	pthread_mutex_unlock(&mutex);
	return l;
}

Logger::LogLevel Logger::GetLevelRealm(const char* realm)
{
	LogLevel l;
	pthread_mutex_lock(&mutex);
	t_level_by_realm::iterator it = m_level_by_realm.find(realm);
	if( it != m_level_by_realm.end() )l = it->second;
	else l=m_level;
	pthread_mutex_unlock(&mutex);
	return l;
}

void Logger::SetExtraInfo(const char *s)
{
	sprintf(m_buffer, "%s: ", s);
	bufpointer=m_buffer+strlen(m_buffer);
}

void Logger::SetLogTimestamp(bool log_timestamp)
{
	this->log_timestamp = log_timestamp;
}
