#ifndef _TIMERTARGETMONOTONIC_H_
#define _TIMERTARGETMONOTONIC_H_

#include "TimerQueueMonotonic.h"
#include <vector>

/// <summary>
/// Timer target. This class is the base class for classes using asynchronous timers.
/// </summary>

class ELVUTILS_DLLEXPORT TimerTargetMonotonic  
{
public:

	/// <summary>
	/// Default constructor.
	/// </summary>

	TimerTargetMonotonic(void);

	/// <summary>
	/// Destructor
	/// </summary>

	virtual ~TimerTargetMonotonic();

protected:
	void KillTimer(unsigned long cookie);
	void KillAllTimers();
	virtual void OnTimer(unsigned long cookie)=0;
	void RequestTimer(long timeout, unsigned long cookie);
	friend class TimerQueueMonotonic;
};

#endif
