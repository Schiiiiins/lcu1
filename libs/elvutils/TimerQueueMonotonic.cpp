// TimerQueueMonotonic.cpp: Implementierung der Klasse TimerQueueMonotonic.
#include "TimerQueueMonotonic.h"
#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#include <time.h>
#endif
#include "TimerTargetMonotonic.h"
#include <typeinfo>
#include <limits.h>

TimerQueueMonotonic* TimerQueueMonotonic::_instance = NULL;

/*static*/ TimerQueueMonotonic* TimerQueueMonotonic::Instance()
{
	if( !_instance )
	{
		_instance = new TimerQueueMonotonic();
	}
	return _instance;
}

/*static*/ bool TimerQueueMonotonic::InstanceExists()
{
	return _instance != NULL;
}

TimerQueueMonotonic::TimerQueueMonotonic()
{
}

TimerQueueMonotonic::~TimerQueueMonotonic()
{
	_instance = NULL;
}

TimerQueueMonotonic::Entry::Entry(TimerTargetMonotonic* target, unsigned long timeout, unsigned long cookie)
{
	_target=target;
	_cookie=cookie;
	_dueTime = GetMonotonicTime() + timeout;
}

void TimerQueueMonotonic::RequestTimer(TimerTargetMonotonic* target, unsigned long timeout, unsigned long cookie)
{
	Entry entry(Entry(target, timeout, cookie));
	queue_t::iterator it=_queue.begin();
	while(it!=_queue.end()){
		if(! ((*it) <= entry))break;
		it++;
	}
	_queue.insert(it, entry);
}

void TimerQueueMonotonic::KillTimer(TimerTargetMonotonic* target, unsigned long cookie)
{
	Entry entry(Entry(target, 0, cookie));
	_queue.remove(entry);
}

void TimerQueueMonotonic::KillAllTimers(TimerTargetMonotonic* target)
{
	queue_t::iterator it=_queue.begin();
	while(it!=_queue.end()){
		if(it->_target==target)it=_queue.erase(it);
		else it++;
	}
}

unsigned long TimerQueueMonotonic::TimeBeforeNextDue()
{
	if(_queue.empty())return ULONG_MAX;
	Entry& e=_queue.front();
	unsigned long long now = GetMonotonicTime();

	long long retval=(long long) e._dueTime - now;
	if(retval<0)retval=0;
	if( retval > ULONG_MAX )retval = ULONG_MAX;
	return (unsigned long)retval;
}

void TimerQueueMonotonic::Execute()
{
	while(!_queue.empty()){
		Entry& e=_queue.front();
		Entry now(0, 0, 0);
		if(e <= now){
			TimerTargetMonotonic* target=e._target;
			unsigned long cookie=e._cookie;
			_queue.pop_front();
			target->OnTimer(cookie);
		}else{
			break;
		}
	}
}

/*static*/ unsigned long long TimerQueueMonotonic::GetMonotonicTime()
{
#ifdef WIN32
//	return ::GetTickCount64();
	return ::GetTickCount();
#else
	struct timespec ts;
	clock_gettime( CLOCK_MONOTONIC, &ts );
	return (ts.tv_sec * 1000) + (ts.tv_nsec / 1000);
#endif
}
