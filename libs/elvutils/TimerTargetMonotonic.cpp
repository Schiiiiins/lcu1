#pragma once;

#include "TimerTargetMonotonic.h"
#include "TimerQueueMonotonic.h"

TimerTargetMonotonic::TimerTargetMonotonic(void)
{
}

TimerTargetMonotonic::~TimerTargetMonotonic()
{
	if( TimerQueueMonotonic::InstanceExists())
	{
		TimerQueueMonotonic::Instance()->KillAllTimers(this);
	}
}

void TimerTargetMonotonic::RequestTimer(long timeout, unsigned long cookie)
{
	TimerQueueMonotonic::Instance()->RequestTimer(this, timeout, cookie);
}

void TimerTargetMonotonic::KillTimer(unsigned long cookie)
{
	TimerQueueMonotonic::Instance()->KillTimer(this, cookie);
}

void TimerTargetMonotonic::KillAllTimers()
{
	TimerQueueMonotonic::Instance()->KillAllTimers(this);
}
