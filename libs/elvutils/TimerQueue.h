// TimerQueue.h: Schnittstelle für die Klasse TimerQueue.
#ifndef _TIMERQUEUE_H_
#define _TIMERQUEUE_H_

#include "dllexport.h"
#include <sys/timeb.h>
#ifdef WIN32
#define timeb _timeb
#endif
#include <list>
class TimerTarget;
class ELVUTILS_DLLEXPORT TimerQueue  
{
public:
	void Execute();
	long TimeBeforeNextDue();
	TimerQueue();
	virtual ~TimerQueue();
protected:
	void BackwarpAdjust(unsigned int offset);
	void RequestTimer(TimerTarget* target, unsigned long timeout, unsigned long cookie);
	void KillTimer(TimerTarget* target, unsigned long cookie);
	void KillAllTimers(TimerTarget* target);
	class Entry{
	public:
		Entry(TimerTarget* target, unsigned long timeout, unsigned long cookie);
		bool operator<(const Entry& e)
		{
			return 
				(dueTime.time < e.dueTime.time) || 
				((dueTime.time == e.dueTime.time) && (dueTime.millitm < e.dueTime.millitm));
		}
		bool operator<=(const Entry& e)
		{
			return 
				(dueTime.time < e.dueTime.time) || 
				((dueTime.time == e.dueTime.time) && (dueTime.millitm <= e.dueTime.millitm));
		}
		bool operator==(const Entry& e)
		{
			return (target==e.target) && (cookie==e.cookie);
		}
		TimerTarget* target;
		struct timeb dueTime;
		unsigned long cookie;
	};
	typedef std::list<Entry> queue_t;
	queue_t queue;
	friend class TimerTarget;
	struct timeb last_time;
};

#endif
