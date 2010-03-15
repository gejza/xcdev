
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "profiler/traceback.h"
#include "profiler/addr2line.h"
#include "udump/symboltable.h"
#include "udump/udump.h"

typedef struct {
	void* addr;
	symid_t sym;
	symid_t src;
	symid_t line;
} trace_t;

void traceback_dump(traceback_t* t, udump_buffer_t* buff, symboltable_t* table)
{
	void *array[50];
  size_t size = backtrace (array, 50);
  char **strings;
  size_t i;

  strings = backtrace_symbols (array, size);
  for (i = 0; i < size;i++)
  {
    symboltable_get(table, strings[0]);
  }
  free (strings);
}
/* Obtain a backtrace and print it to stdout. */

/*void
print_trace (void)
{
  void *array[10];
  size_t size;
  char **strings;
  size_t i;

  size = backtrace (array, 10);
  strings = backtrace_symbols (array, size);

  printf ("Obtained %zd stack frames.\n", size);

  for (i = 0; i < size; i++) {
     Dl_info inf;
     dladdr(array[i], &inf);
     //printf ("%p: %s %p %s %p\n", array[i], inf.dli_fname, inf.dli_fbase, inf.dli_sname, inf.dli_saddr);
     //fprintf (stderr, "%p %s %p %s %p\n", array[i], inf.dli_fname, inf.dli_fbase, inf.dli_sname, inf.dli_saddr);
     //printf("%p\n", (unsigned long)array[i] - (unsigned long)inf.dli_fbase);
     //printf("%p\n", array[i]);
     Addr2Line a2l(inf.dli_fname);
     AddrInfo ainf = {0};
     //printf("'%s' ", strings[i]);   
     //a2l.lookup((void*)((unsigned long)array[i] - (unsigned long)inf.dli_fbase));
     if (a2l.lookup(array[i], &ainf)) {
	     printf("%s - %s:%d\n", ainf.method_name, ainf.file_name, ainf.line);
     }
     else {
           a2l.lookup((void*)((unsigned long)array[i] - (unsigned long)inf.dli_fbase), &ainf);
	     printf("%s: %s - %s:%d\n", ainf.hex, ainf.method_name, ainf.file_name, ainf.line);
     }
  }

  free (strings);
}

*/

