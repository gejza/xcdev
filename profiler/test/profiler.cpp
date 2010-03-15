
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include <time.h>
//#include <windows.h>
#include "xcprof/profiler.h"
#include "xcprof/times.h"
#include "xcprof/util.h"
#include "xc/error.h"

void test(xc_profiler_t* profiler)
{
	xcprof_begin(profiler,"Jiny Testovaci zaznam s userdaty");
	// open profiler
	PROF_INT("userId2", 13456);
	PROF_INT("userId4", 13458);
	PROF_INT("userId5", 13456);
	PROF_CHECKPOINT("test pointik");
	PROF_STRING("text", "popis");
	PROF_USERDATA("nejaka data", "aaaa", 5);
	PROF_TRACEBACK("testovaci trejsbek");

	xcprof_end(profiler);
}

const int runs = 1000000;

void run(xc_profiler_t* profiler)
{
	time_watch_t w;
	watch_start(&w);
	xcprof_child_init();
	for (int i=0;i < runs;i++)
		test(profiler);
	xcprof_child_destroy();
	watch_finish(&w, runs);
	watch_report(&w,stdout);putchar('\n');
}

#ifdef _WIN32
DWORD WINAPI ThreadProc( LPVOID lpParam ) 
{
	run((xc_profiler_t*)lpParam);
	return 0;
}
#endif

#define MAX_THREADS 50

int main()
{
	xc_profiler_t profiler;
	//error_sethandler(err);

	xcprof_init(&profiler);
	// maxirun
#ifdef _WIN32
	HANDLE hThread[MAX_THREADS]; 
	DWORD dwThreadId[MAX_THREADS];
	if (0)
	{
		for(int i=0; i<MAX_THREADS; i++ )
		{
			hThread[i] = CreateThread( 
				NULL,              // default security attributes
				0,                 // use default stack size  
				ThreadProc,        // thread function 
				&profiler,             // argument to thread function 
				0,                 // use default creation flags 
				&dwThreadId[i]);   // returns the thread identifier 
		}
		WaitForMultipleObjects(MAX_THREADS, hThread, TRUE, INFINITE);
	
	}
	else
#endif
		run(&profiler);
	xcprof_destroy(&profiler);
	return 1;
}


