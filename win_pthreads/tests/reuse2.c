/*
 * File: reuse2.c
 *
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
 *
 * --------------------------------------------------------------------------
 *
 * Test Synopsis:
 * - Test that thread reuse works for detached threads.
 * - Analyse thread struct reuse.
 *
 * Test Method (Validation or Falsification):
 * -
 *
 * Requirements Tested:
 * -
 *
 * Features Tested:
 * -
 *
 * Cases Tested:
 * -
 *
 * Description:
 * -
 *
 * Environment:
 * -
 *
 * Input:
 * - None.
 *
 * Output:
 * - File name, Line number, and failed expression on failure.
 * - No output on success.
 *
 * Assumptions:
 * -
 *
 * Pass Criteria:
 * - Process returns zero exit status.
 *
 * Fail Criteria:
 * - Process returns non-zero exit status.
 */

#include "test.h"

enum {
	NUMTHREADS = 10000
};


static long done = 0;

void * func(void * arg)
{
  sched_yield();

  InterlockedIncrement(&done);

  return (void *) 0; 
}
 
int
main()
{
  pthread_t t[NUMTHREADS];
  pthread_attr_t attr;
  int i, j;
  int reuse,
      reuseMax = 0,
      reuseMin = NUMTHREADS,
      idTotal = 0;

  assert(pthread_attr_init(&attr) == 0);
  assert(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) == 0);

  for (i = 0; i < NUMTHREADS; i++)
    {
      assert(pthread_create(&t[i], &attr, func, NULL) == 0);
    }

  while (NUMTHREADS > done)
    Sleep(100);

  Sleep(100);

  /*
   * Analyse reuse by computing min and max number of times pthread_create()
   * returned the same pthread_t value.
   */
  for (i = 0; i < NUMTHREADS; i++)
    {
      reuse = 0;

      if (t[i] == NULL)
        continue;

      for (j = 0; j < i; j++)
        {
          if (t[j] == t[i])
            {
              reuse++;
              t[j] = NULL;
            }
        }
      for (j = i + 1; j < NUMTHREADS; j++)
        {
          if (t[j] == t[i])
            {
              reuse++;
              t[j] = NULL;
            }
        }
      if (reuseMax < reuse)
        reuseMax = reuse;
      if (reuseMin > reuse)
        reuseMin = reuse;
    }

  for (i = 0; i < NUMTHREADS; i++)
    {
      if (t[i] != NULL)
        idTotal++;
    }

  printf("For %d total threads:\n", NUMTHREADS);
  printf("Reuse maximum = %d\n", reuseMax);
  printf("Reuse minimum = %d\n", reuseMin);
  printf("Total thread IDs allocated = %d\n", idTotal);

  return 0;
}
