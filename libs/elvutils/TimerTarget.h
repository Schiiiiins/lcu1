// TimerTarget.h: Schnittstelle für die Klasse TimerTarget.
#ifndef _TIMERTARGET_H_
#define _TIMERTARGET_H_
#include "dllexport.h"
#include "TimerQueue.h"
#include <vector>

class ELVUTILS_DLLEXPORT TimerTarget  
{
public:
	TimerTarget();
	virtual ~TimerTarget();
	static TimerQueue s_timerQueue;
protected:
	void KillTimer(unsigned long cookie);
	void KillAllTimers();
	void KillTimersIntervall(unsigned long min, unsigned long max);
	std::vector<unsigned long> GetAllTimerCookies();
	void ElapseAllTimersNOW();
	virtual void OnTimer(unsigned long cookie)=0;
	void RequestTimer(long timeout, unsigned long cookie);
	friend class TimerQueue;
	static bool timerQueueExists;
};
#endif
