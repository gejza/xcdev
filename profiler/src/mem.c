 /* Prototypes for __malloc_hook, __free_hook */
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <crtdbg.h>
#endif
#include "xcprof/util.h"
#include "xcprof/mem.h"

size_t memalloc = 0;

#ifndef _WIN32

/* Prototypes for our hooks.  */
static void my_init_hook (void);
static void *my_malloc_hook (size_t, const void *);
static void my_free_hook (void*, const void *);

void (*old_malloc_hook)(size_t size, const void *caller);
void (*old_free_hook)(size_t size, const void *caller);

/* Override initializing hook from the C library. */
void (*__malloc_initialize_hook) (void) = my_init_hook;


static void
my_init_hook (void)
{
	old_malloc_hook = __malloc_hook;
	old_free_hook = __free_hook;
	__malloc_hook = my_malloc_hook;
	__free_hook = my_free_hook;
}

static void *
my_malloc_hook (size_t size, const void *caller)
{
	void *result;
	/* Restore all old hooks */
	__malloc_hook = old_malloc_hook;
	__free_hook = old_free_hook;
	/* Call recursively */
	result = malloc (size);
	/* Save underlying hooks */
	old_malloc_hook = __malloc_hook;
	old_free_hook = __free_hook;
	/* printf might call malloc, so protect it too. */
	//printf ("malloc (%u) returns %p\n", (unsigned int) size, result);
	memalloc += size;
	/* Restore our own hooks */
	__malloc_hook = my_malloc_hook;
	__free_hook = my_free_hook;
	return result;
}

static void
my_free_hook (void *ptr, const void *caller)
{
	/* Restore all old hooks */
	__malloc_hook = old_malloc_hook;
	__free_hook = old_free_hook;
	/* Call recursively */
	free (ptr);
	/* Save underlying hooks */
	old_malloc_hook = __malloc_hook;
	old_free_hook = __free_hook;
	/* printf might call free, so protect it too. */
	//printf ("freed pointer %p\n", ptr);
	/* Restore our own hooks */
	__malloc_hook = my_malloc_hook;
	__free_hook = my_free_hook;
}

void memdbg_start()
{
}

void memdbg_end()
{
}

#else

// Disable deprecation warning.  The unsecure version of strcpy is
// used intentionally to illustrate bad coding practices.
#pragma warning (disable : 4996)

/*****************************************************************
 *  DATA DECLARATIONS AND DEFINES                                *
 *****************************************************************/

#define  TIME_STR_LENGTH      10
#define  DATE_STR_LENGTH      10
#define  TEST_RECS            5

#define  FILE_IO_ERROR        0
#define  OUT_OF_MEMORY        1

#define  TRUE                 7
#define  FALSE                0


///////////////////////////////
// global values
typedef struct _mem_info_t
{
	FILE* log;
	size_t used;
	int num;
} mem_info_t;

GV_DECL_TS(mem_info_t,info); 

/*****************************************************************
 *  DEBUG C RUNTIME LIBRARY HOOK FUNCTIONS AND DEFINES           *
 *****************************************************************/

/* CLIENT DUMP HOOK FUNCTION
   -------------------------
   A hook function for dumping a Client block usually reports some
   or all of the contents of the block in question.  The function
   below also checks the data in several ways, and reports corruption
   or inconsistency as an assertion failure.
*/
void __cdecl MyDumpClientHook(
   void * pUserData,
   size_t nBytes
   )
{

}


/* ALLOCATION HOOK FUNCTION
   -------------------------
   An allocation hook function can have many, many different
   uses. This one simply logs each allocation operation in a file.
*/
int __cdecl MyAllocHook(
   int      nAllocType,
   void   * pvData,
   size_t   nSize,
   int      nBlockUse,
   long     lRequest,
   const unsigned char * szFileName,
   int      nLine
   )
{
   char *operation[] = { "", "allocating", "re-allocating", "freeing" };
   char *blockType[] = { "Free", "Normal", "CRT", "Ignore", "Client" };

   if ( nBlockUse == _CRT_BLOCK )   // Ignore internal C runtime library allocations
      return( TRUE );

	switch (nAllocType)
	{
	case _HOOK_ALLOC:
		GV_TS(mem_info_t,info)->used += nSize; 
		GV_TS(mem_info_t,info)->num++; 
		break;
	case _HOOK_FREE:
		GV_TS(mem_info_t,info)->used -= nSize; break;
	};

   _ASSERT( ( nAllocType > 0 ) && ( nAllocType < 4 ) );
   _ASSERT( ( nBlockUse >= 0 ) && ( nBlockUse < 5 ) );

   /*fprintf( GV_TS(mem_info_t,info)->log, 
            "Memory operation in %s, line %d: %s a %d-byte '%s' block (#%ld)\n",
            szFileName, nLine, operation[nAllocType], nSize, 
            blockType[nBlockUse], lRequest );
   if ( pvData != NULL )
      fprintf( GV_TS(mem_info_t,info)->log, " at %p", pvData );
	*/
   return( TRUE );         // Allow the memory operation to proceed
}


/* REPORT HOOK FUNCTION
   --------------------
   Again, report hook functions can serve a very wide variety of purposes.
   This one logs error and assertion failure debug reports in the
   log file, along with 'Damage' reports about overwritten memory.

   By setting the retVal parameter to zero, we are instructing _CrtDbgReport
   to return zero, which causes execution to continue. If we want the function
   to start the debugger, we should have _CrtDbgReport return one.
*/
int MyReportHook(
   int   nRptType,
   char *szMsg,
   int  *retVal
   )
{
   char *RptTypes[] = { "Warning", "Error", "Assert" };

   if ( ( nRptType > 0 ) || ( strstr( szMsg, "HEAP CORRUPTION DETECTED" ) ) )
      fprintf( GV_TS(mem_info_t,info)->log, "%s: %s", RptTypes[nRptType], szMsg );

   retVal = 0;

   return( TRUE );         // Allow the report to be made as usual
   
}

void memdbg_start()
{
   _CrtMemState checkPt1;
   char timeStr[TIME_STR_LENGTH], dateStr[DATE_STR_LENGTH];         // Used to set up log file

	GV_INC_TS(mem_info_t,info);
   // Send all reports to STDOUT, since this example is a console app
   _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
   _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
   _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
   _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
   _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
   _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);

   // Set the debug heap to report memory leaks when the process terminates,
   // and to keep freed blocks in the linked list.
   _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_DELAY_FREE_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));

   // Open a log file for the hook functions to use 
   fopen_s( &GV_TS(mem_info_t,info)->log, "MEM-LOG.TXT", "a" );
   _strtime_s( timeStr, TIME_STR_LENGTH );
   _strdate_s( dateStr, DATE_STR_LENGTH );
   fprintf( GV_TS(mem_info_t,info)->log, 
            "Memory Allocation Log File for Example Program, run at %s on %s.\n",
            timeStr, dateStr );

   // Install the hook functions
   _CrtSetDumpClient( MyDumpClientHook );
   _CrtSetAllocHook( MyAllocHook );
   _CrtSetReportHook( MyReportHook );

}

void memdbg_end()
{
	printf("used: %ld, num: %d\n", GV_TS(mem_info_t,info)->used, GV_TS(mem_info_t,info)->num);

   // Check the debug heap, and dump the new birthday record. --Note that 
   // debug C runtime library functions such as _CrtCheckMemory( ) and 
   // _CrtMemDumpAllObjectsSince( ) automatically disappear in a release build.
   _CrtMemDumpAllObjectsSince( NULL );
   _CrtCheckMemory( );
   //_CrtMemCheckpoint( &checkPt1 );

   // Examine the results
   /*_CrtMemDumpAllObjectsSince( &checkPt1 );
   _CrtMemCheckpoint( &checkPt1 );
   _CrtMemDumpStatistics( &checkPt1 );
   _CrtCheckMemory( );*/

   // This fflush needs to be removed...
   fflush( GV_TS(mem_info_t,info)->log );

   // OK, time to go. Did I forget to turn out any lights? I could check
   // explicitly using _CrtDumpMemoryLeaks( ), but I have set 
   // _CRTDBG_LEAK_CHECK_DF, so the C runtime library debug heap will
   // automatically alert me at exit of any memory leaks.

   fclose( GV_TS(mem_info_t,info)->log );
	_CrtSetDbgFlag(0);
   // Uninstall the hook functions
   _CrtSetDumpClient( NULL );
   _CrtSetAllocHook( NULL );
   _CrtSetReportHook( NULL );


	GV_DEC_TS(mem_info_t,info);
}


#endif
