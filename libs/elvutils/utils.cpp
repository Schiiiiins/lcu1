#include "utils.h"
#ifdef WIN32
#include <windows.h>
#define ftime _ftime
#define timeb _timeb
#else
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#include "Logger.h"
#include <stdio.h>  //fuer sprintf
#include <stdlib.h> //atof, atoi
#include <string.h>


struct timespec millis2abstime(long millis)
{
  struct timespec abstime = { 0, 0 };
  struct timeb currSysTime;
  const unsigned long NANOSEC_PER_MILLISEC = 1000000;
  /* get current system time */
  ftime(&currSysTime);
  millis+=currSysTime.millitm;
  currSysTime.time+=millis/1000;
  currSysTime.millitm=millis%1000;
  abstime.tv_sec = currSysTime.time;
  abstime.tv_nsec = NANOSEC_PER_MILLISEC * currSysTime.millitm;
	
  return abstime;
}
bool operator<(const struct timespec& t1, const struct timespec& t2)
{
	if(t1.tv_sec<t2.tv_sec)return true;
	if(t1.tv_sec>t2.tv_sec)return false;
	return t1.tv_nsec<t2.tv_nsec;
}
unsigned long time_millis()
{
	struct timeb now;
	ftime(&now);
	return now.time*1000+now.millitm;
}
unsigned long time_difference(unsigned long t1, unsigned long t2)
{
	long diff=t1-t2;
	if(diff<0)diff *= -1;
	return diff;
}
void string_trim(std::string* s)
{
	std::string::size_type pos=s->find_first_not_of( ' ');
	if(pos)s->erase(0, pos);
	pos=s->find_last_not_of(' ');
	if(pos!=std::string::npos)s->erase(pos+1);
}
int number_running_processes()
{
#ifndef WIN32
	char buffer[32];
	int fd = open("/proc/loadavg", O_RDONLY | O_NOCTTY );
	if(fd<0){
		perror("open /proc/loadavg");
		return -1;
	}
	int count=read(fd, buffer, 31);
	close(fd);
	buffer[count]=0;
	char* field=strtok(buffer, " ");
	int fieldIndex=0;
	while(field){
		if(fieldIndex==3){
			return atoi(field);
		}
		if(++fieldIndex > 3)break;
	}
	LOG(Logger::LOG_ERROR, "Error getting number of threads from /proc/loadavg");
	return 1;
#else
	return 6;
#endif
}
//------------------------------------------------------------------------
int StrToInt(const std::string& s)
{
	return atoi(s.c_str());
}
//------------------------------------------------------------------------
int StrToInt(const char& c)
{
	return c - 48;
}
//------------------------------------------------------------------------
std::string IntToStr(const int& val)
{
	char buffer[11];
	sprintf(buffer, "%d", val);
	return std::string(buffer);
}
//------------------------------------------------------------------------
double StrToDouble (const std::string& s)
{
	return atof(s.c_str());
}
//------------------------------------------------------------------------
bool StrToBool (const std::string& s)
{
	return s == "true";
}
//------------------------------------------------------------------------
std::string BoolToStr (const bool& val)
{
	return val ? "true" : "false";
}
//------------------------------------------------------------------------
std::vector<std::string> SplitString(const std::string &val, const std::string &separator)
{
	std::string::size_type pos = 0;
	std::string value = val;
	std::string token;
	std::vector<std::string> v;
	

	while (value.length() > 0)
	{
		pos = value.find(separator, 0);

		if (pos == std::string::npos)
		{
			token = value;
			        value = "";
		}
		else
		{
			token = value.substr(0,    pos); 
			value = value.substr(pos+1);//+1 überspringt das separator-Zeichen
		}

		v.push_back(token);
	}

	return v;
}
//------------------------------------------------------------------------
