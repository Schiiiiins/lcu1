// TimerQueue.cpp: Implementierung der Klasse TimerQueue.
#include "TimerQueue.h"
#ifdef WIN32
#include <windows.h>
#define ftime _ftime
#define timeb _timeb
#else
#include <sys/time.h>
#endif
#include "TimerTarget.h"
//TimerQueue timerQueue;
// Konstruktion/Destruktion
#include <typeinfo>
#include <limits.h>

TimerQueue::TimerQueue()
{
	ftime(&last_time);
}
TimerQueue::~TimerQueue()
{
	TimerTarget::timerQueueExists=false;
}
TimerQueue::Entry::Entry(TimerTarget* target, unsigned long timeout, unsigned long cookie)
{
	this->target=target;
	this->cookie=cookie;
	ftime(&dueTime);
	timeout+=dueTime.millitm;
	dueTime.time+=timeout/1000;
	dueTime.millitm=timeout%1000;
}
void TimerQueue::RequestTimer(TimerTarget* target, unsigned long timeout, unsigned long cookie)
{
	Entry entry(Entry(target, timeout, cookie));
	queue_t::iterator it=queue.begin();
	while(it!=queue.end()){
		if(! ((*it) <= entry))break;
		it++;
	}
	queue.insert(it, entry);
}
void TimerQueue::KillTimer(TimerTarget* target, unsigned long cookie)
{
	Entry entry(Entry(target, 0, cookie));
	queue.remove(entry);
}
void TimerQueue::KillAllTimers(TimerTarget* target)
{
	queue_t::iterator it=queue.begin();
	while(it!=queue.end()){
		if(it->target==target)it=queue.erase(it);
		else it++;
	}
}
long TimerQueue::TimeBeforeNextDue()
{
	if(queue.empty())return -1;
	Entry& e=queue.front();
	struct timeb now;
	ftime(&now);
	int secs_since_last_call=(int)(now.time-last_time.time);
	if(secs_since_last_call<0){
		//time warp to the past detected
		BackwarpAdjust(-1 * secs_since_last_call);
	}
	last_time=now;
    long diffSeconds = e.dueTime.time-now.time;
    int diffMilliseconds = e.dueTime.millitm-now.millitm;
    if( diffMilliseconds < 0 )
    {
        diffSeconds--;
        diffMilliseconds += 1000;
    }
    //negative due time -> expire immediately
    if( diffSeconds < 0 )return 0;
    //due time too long to be represented by return value, return a very long time
    if( diffSeconds + 1 >= LONG_MAX / 1000 )return LONG_MAX - 1000;
    return diffSeconds * 1000 + diffMilliseconds;
}
void TimerQueue::BackwarpAdjust(unsigned int offset)
{
	for(queue_t::iterator it=queue.begin();it!=queue.end();it++){
		it->dueTime.time-=offset;
	}
}
void TimerQueue::Execute()
{
	while(!queue.empty()){
		Entry& e=queue.front();
		Entry now(0, 0, 0);
		if(e <= now){
			TimerTarget* target=e.target;
			int cookie=e.cookie;
			queue.pop_front();
			target->OnTimer(cookie);
		}else{
			break;
		}
	}
}
