
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include "xc/system.h"

/********* Public functions ************/
threadid_t system_get_thread_id()
{
	return 0;//GetCurrentThreadId();
}

mutex_t system_mutex_create()
{
	return 0;/*CreateMutex( 
    NULL,                       // default security attributes
    FALSE,                      // initially not owned
    NULL);                      // unnamed mutex
   */
}

void system_mutex_delete(mutex_t mutex)
{
	//CloseHandle(mutex);
}

void system_lock(mutex_t mutex)
{
	/*while (WaitForSingleObject( 
        mutex,   // handle to mutex
        5000L)   // five-second time-out interval
		== WAIT_TIMEOUT);
*/
}

void system_unlock(mutex_t mutex)
{
	//ReleaseMutex(mutex);
}
