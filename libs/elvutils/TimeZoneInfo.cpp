#include "TimeZoneInfo.h"

TimeZoneInfo::TimeZoneInfo(void)
{
    m_time=time(NULL);
}

TimeZoneInfo::TimeZoneInfo(time_t t)
{
    m_time=t;
}

TimeZoneInfo::~TimeZoneInfo(void)
{
}

bool TimeZoneInfo::IsDst(time_t t)
{
    return localtime(&t)->tm_isdst!=0;
}

bool TimeZoneInfo::CalcNextChange(int days_advance, time_t* change_time, time_t* change_offset)
{
  time_t t1=(m_time+CHANGE_RESOLUTION-1)/CHANGE_RESOLUTION;
  time_t t2=t1+(days_advance*86400/CHANGE_RESOLUTION);

  if( IsDst(t2*CHANGE_RESOLUTION) == IsDst(m_time) ){
      return false;
  }

  int diff;

  while( (diff=int(t2-t1))>1 ){
     if( IsDst((t2-diff/2)*CHANGE_RESOLUTION) != IsDst(t1*CHANGE_RESOLUTION) )t2=t2-diff/2;
     else t1=t2-diff/2;
  }

  *change_time=t2*CHANGE_RESOLUTION;
  
  struct tm* t_tm=localtime(&m_time);
  int t_minutes=t_tm->tm_min+t_tm->tm_hour*60-int(m_time%86400)/60;

  struct tm* t2_tm=localtime(change_time);
  int t2_minutes=t2_tm->tm_min+t2_tm->tm_hour*60-int(*change_time%86400)/60;

  int offset_minutes=t2_minutes - t_minutes;
  if(offset_minutes < -(12*60))offset_minutes+=(24*60);
  if(offset_minutes > (12*60))offset_minutes-=(24*60);

  *change_offset=offset_minutes*60;
  return true;
}

time_t TimeZoneInfo::GetUTCOffset()
{
    struct tm tm_utc=*(gmtime(&m_time));
    struct tm tm_local=*(localtime(&m_time));
    int offset_minutes=(tm_local.tm_hour*60+tm_local.tm_min) - (tm_utc.tm_hour*60+tm_utc.tm_min);

    if(offset_minutes < -(12*60))offset_minutes+=(24*60);
    if(offset_minutes > (12*60))offset_minutes-=(24*60);
    return offset_minutes*60;
}
