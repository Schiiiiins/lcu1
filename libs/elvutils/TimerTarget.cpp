// TimerTarget.cpp: Implementierung der Klasse TimerTarget.
#include "TimerTarget.h"
#include "TimerQueue.h"
extern TimerQueue timerQueue;
// Konstruktion/Destruktion
TimerQueue TimerTarget::s_timerQueue;
bool TimerTarget::timerQueueExists=true;
TimerTarget::TimerTarget()
{
}
TimerTarget::~TimerTarget()
{
	if(timerQueueExists)s_timerQueue.KillAllTimers(this);
}
void TimerTarget::RequestTimer(long timeout, unsigned long cookie)
{
	s_timerQueue.RequestTimer(this, timeout, cookie);
}
void TimerTarget::KillTimer(unsigned long cookie)
{
	s_timerQueue.KillTimer(this, cookie);
}
void TimerTarget::KillAllTimers()
{
	s_timerQueue.KillAllTimers(this);
}
std::vector<unsigned long> TimerTarget::GetAllTimerCookies()
{
	TimerQueue::queue_t::iterator it;
	std::vector<unsigned long> v_cookies;

 	it = s_timerQueue.queue.begin();
	
	while (it != s_timerQueue.queue.end())
	{
		v_cookies.push_back((*it).cookie);
		++it;
	}

	return v_cookies;
}
void TimerTarget::ElapseAllTimersNOW()
{
	TimerQueue::queue_t::iterator it;

 	it = s_timerQueue.queue.begin();
	
	while (it != s_timerQueue.queue.end())
	{
		KillTimer(      (*it).cookie);
		RequestTimer(1, (*it).cookie); //1ms
		++it;
	}
}
//Diese Funktion löscht alle Entries mit einem cookie zwischen min und max
void TimerTarget::KillTimersIntervall(unsigned long min, unsigned long max)
{
	TimerQueue::queue_t::iterator it;
	unsigned long c;
	
 	it = s_timerQueue.queue.begin();
	
	while (it != s_timerQueue.queue.end())
	{
		c = (*it).cookie;

		if (c >= min && c <= max) it = s_timerQueue.queue.erase(it);
		else                      it++;
	}
}
