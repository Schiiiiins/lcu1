/*
 * -------------------------------------------------------------
 *
 * Module: sem_timedwait.c
 *
 * Purpose:
 *	Semaphores aren't actually part of the PThreads standard.
 *	They are defined by the POSIX Standard:
 *
 *		POSIX 1003.1b-1993	(POSIX.1b)
 *
 * -------------------------------------------------------------
 *
 * --------------------------------------------------------------------------
 *
 *      Pthreads-win32 - POSIX Threads Library for Win32
 *      Copyright(C) 1998 John E. Bossom
 *      Copyright(C) 1999,2003 Pthreads-win32 contributors
 * 
 *      Contact Email: rpj@callisto.canberra.edu.au
 * 
 *      The current list of contributors is contained
 *      in the file CONTRIBUTORS included with the source
 *      code distribution. The list can also be seen at the
 *      following World Wide Web location:
 *      http://sources.redhat.com/pthreads-win32/contributors.html
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#ifndef _UWIN
//#include <process.h>
#endif
#ifndef NEED_FTIME
#include <sys/timeb.h>
#endif

#include "pthread.h"
#include "semaphore.h"
#include "implement.h"


int
sem_timedwait (sem_t * sem, const struct timespec *abstime)
     /*
      * ------------------------------------------------------
      * DOCPUBLIC
      *      This function waits on a semaphore possibly until
      *      'abstime' time.
      *
      * PARAMETERS
      *      sem
      *              pointer to an instance of sem_t
      *
      *      abstime
      *              pointer to an instance of struct timespec
      *
      * DESCRIPTION
      *      This function waits on a semaphore. If the
      *      semaphore value is greater than zero, it decreases
      *      its value by one. If the semaphore value is zero, then
      *      the calling thread (or process) is blocked until it can
      *      successfully decrease the value or until interrupted by
      *      a signal.
      *
      *      If 'abstime' is a NULL pointer then this function will
      *      block until it can successfully decrease the value or
      *      until interrupted by a signal.
      *
      * RESULTS
      *              0               successfully decreased semaphore,
      *              -1              failed, error in errno
      * ERRNO
      *              EINVAL          'sem' is not a valid semaphore,
      *              ENOSYS          semaphores are not supported,
      *              EINTR           the function was interrupted by a signal,
      *              EDEADLK         a deadlock condition was detected.
      *              ETIMEDOUT       abstime elapsed before success.
      *
      * ------------------------------------------------------
      */
{
  int result = 0;

#ifdef NEED_FTIME

  struct timespec currSysTime;

#else /* NEED_FTIME */

  struct _timeb currSysTime;

#endif /* NEED_FTIME */

  const DWORD NANOSEC_PER_MILLISEC = 1000000;
  const DWORD MILLISEC_PER_SEC = 1000;
  DWORD milliseconds;
  DWORD tmpAbsMilliseconds;
  DWORD tmpCurrMilliseconds;

  if (sem == NULL)
    {
      result = EINVAL;
    }
  else
    {
      if (abstime == NULL)
	{
	  milliseconds = INFINITE;
	}
      else
	{
	  /* 
	   * Calculate timeout as milliseconds from current system time. 
	   */

	  /*
	   * subtract current system time from abstime in a way that checks
	   * that abstime is never in the past, or is never equivalent to the
	   * defined INFINITE value (0xFFFFFFFF).
	   *
	   * Assume all integers are unsigned, i.e. cannot test if less than 0.
	   */
	  tmpAbsMilliseconds =  abstime->tv_sec * MILLISEC_PER_SEC;
	  tmpAbsMilliseconds += (abstime->tv_nsec + (NANOSEC_PER_MILLISEC/2)) / NANOSEC_PER_MILLISEC;

	  /* get current system time */

#ifdef NEED_FTIME

	  {
	    FILETIME ft;
	    SYSTEMTIME st;

	    GetSystemTime(&st);
	    SystemTimeToFileTime(&st, &ft);
	    /*
	     * GetSystemTimeAsFileTime(&ft); would be faster,
	     * but it does not exist on WinCE
	     */

	    ptw32_filetime_to_timespec(&ft, &currSysTime);
	  }

	  tmpCurrMilliseconds = currSysTime.tv_sec * MILLISEC_PER_SEC;
	  tmpCurrMilliseconds += (currSysTime.tv_nsec + (NANOSEC_PER_MILLISEC/2)) / NANOSEC_PER_MILLISEC;

#else /* ! NEED_FTIME */

	  _ftime(&currSysTime);

	  tmpCurrMilliseconds = (DWORD) currSysTime.time * MILLISEC_PER_SEC;
	  tmpCurrMilliseconds += (DWORD) currSysTime.millitm;

#endif /* NEED_FTIME */

	  if (tmpAbsMilliseconds > tmpCurrMilliseconds)
	    {
	      milliseconds = tmpAbsMilliseconds - tmpCurrMilliseconds;
	      if (milliseconds == INFINITE)
		{
		  /* Timeouts must be finite */
		  milliseconds--;
		}
	    }
	  else
	    {
	      /* The abstime given is in the past */
	      milliseconds = 0;
	    }
	}

#ifdef NEED_SEM

      result = (pthreadCancelableTimedWait ((*sem)->event, milliseconds));

#else /* NEED_SEM */

      result = (pthreadCancelableTimedWait ((*sem)->sem, milliseconds));

#endif

    }

  if (result != 0)
    {

      errno = result;
      return -1;

    }

#ifdef NEED_SEM

  ptw32_decrease_semaphore (sem);

#endif /* NEED_SEM */

  return 0;

}				/* sem_timedwait */
