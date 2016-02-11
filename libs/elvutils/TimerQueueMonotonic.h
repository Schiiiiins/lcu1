#ifndef _TIMERQUEUEMONOTONIC_H_
#define _TIMERQUEUEMONOTONIC_H_

#include <list>
#include "dllexport.h"

class TimerTargetMonotonic;

/// <summary>
/// This class provides the infrastructure for a set of timer targets.
/// </summary>

class ELVUTILS_DLLEXPORT TimerQueueMonotonic  
{
public:

	/// <summary>
	/// Executes all timers that are due.
	/// </summary>

	void Execute();

	/// <summary>
	/// Calculates the time until Execute() needs to be called next.
	/// </summary>
	///
	/// <returns>
	/// Time in ms.
	/// </returns>

	unsigned long TimeBeforeNextDue();

	static TimerQueueMonotonic* Instance();

	static bool InstanceExists();

	/// <summary>
	/// Gets the monotonic time with ms resolution.
	/// </summary>
	///
	/// <returns>
	/// Time in ms.
	/// </returns>

	static unsigned long long GetMonotonicTime();
private:
	TimerQueueMonotonic();
	TimerQueueMonotonic(const TimerQueueMonotonic&){};
	TimerQueueMonotonic& operator=(const TimerQueueMonotonic&){return *this;};
	~TimerQueueMonotonic();

protected:
	void RequestTimer(TimerTargetMonotonic* target, unsigned long timeout, unsigned long cookie);
	void KillTimer(TimerTargetMonotonic* target, unsigned long cookie);
	void KillAllTimers(TimerTargetMonotonic* target);
	class Entry{
	public:
		Entry(TimerTargetMonotonic* target, unsigned long timeout, unsigned long cookie);
		bool operator<(const Entry& e)
		{
			return _dueTime < e._dueTime;
		}
		bool operator<=(const Entry& e)
		{
			return _dueTime <= e._dueTime;
		}
		bool operator==(const Entry& e)
		{
			return (_target==e._target) && (_cookie==e._cookie);
		}
		TimerTargetMonotonic* _target;
		unsigned long long _dueTime;
		unsigned long _cookie;
	};
	typedef std::list<Entry> queue_t;
	queue_t _queue;
	friend class TimerTargetMonotonic;
	static TimerQueueMonotonic* _instance;
};
#endif
