
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include <time.h>
#include "xcprof/profiler.h"
#include "xcprof/times.h"
//#include "xcprof/traceback.h"
#include "xcprof/util.h"
#include "xcprof/udump.h"

typedef struct _profile_t
{
	udump_entry_t entry;
	time_watch_t* timing;
} profile_t;

GV_DECL_TS(profile_t, profile_map);
#define FETCH_PROFILE GV_TS(profile_t, profile_map)
#define PROFILE profile_t* profile = FETCH_PROFILE;\
					if (!profile->entry.dump) return;

#if 1
#define DBGVAL printf
#else
#define DBGVAL(format, argv...)
#endif

/********* Private functions ************/
__inline symid_t symbol_get(udump_t* dump, const char* symbol)
{
	return symboltable_get(&dump->symbols, symbol);
}

/********* Public functions ************/
int xcprof_init(xc_profiler_t* profiler)
{
	udump_open(&profiler->dump);
	// write basic information
	xcprof_dumpsysinfo(profiler SRC_INFO);
	return 0;
}

void xcprof_destroy(xc_profiler_t* profiler)
{
	udump_close(&profiler->dump);
}

void xcprof_child_init()
{
	GV_INC_TS(profile_t, profile_map);
}

void xcprof_child_destroy()
{
	profile_t* profile;
	profile = FETCH_PROFILE;
	udump_entry_free(&profile->entry);
	GV_DEC_TS(profile_t, profile_map);
}

void xcprof_begin(xc_profiler_t* profiler, const char* entryname)
{
	//LARGE_INTEGER tbegin;
	profile_t* profile;
	threadid_t thi = system_get_thread_id();

	DBGVAL("#Begin %s\n", entryname);
	GV_INC_TS(profile_t, profile_map);
	profile = FETCH_PROFILE;
	udump_entry_open(&profiler->dump, &profile->entry, entryname);

	// insert main informations
	udump_entry_record(&profile->entry,
		10, _SYM("ThreadId"), &thi, sizeof(threadid_t) SRC_INFO);
	profile->timing = udump_entry_record(&profile->entry,
		11, _SYM("Timing"), NULL, sizeof(time_watch_t) SRC_INFO);
	watch_start(profile->timing);
	//QueryPerformanceCounter(&tbegin);
	//udump_entry_record(&profile->entry, 22, "StartTime", &tbegin, sizeof(tbegin) SRC_INFO);
}

void xcprof_end(xc_profiler_t* profiler)
{
	//LARGE_INTEGER tend;
	profile_t* profile = FETCH_PROFILE;
	DBGVAL("#End\n");

	watch_finish(profile->timing, 1);
	//QueryPerformanceCounter(&tend);
	//udump_entry_record(&profile->entry, 22, "FinishTime", &tend, sizeof(tend) SRC_INFO);

	// write to file
	udump_entry_flush(&profiler->dump, &profile->entry);
	GV_DEC_TS(profile_t, profile_map);
}

void xcprof_dumpsysinfo(xc_profiler_t* profiler  SRC_DECL)
{
	udump_entry_t entry = {0};
	udump_entry_open(&profiler->dump, &entry, _SYM("SystemInfo"));
	udump_entry_record(&entry, 2, _SYM("SymbolTableUUID"), 
		&profiler->dump.symbols.uuid, sizeof(xc_uuid_t) SRC_INFO);
	
	udump_entry_flush(&profiler->dump, &entry);
	udump_entry_free(&entry);
}

void xcprof_value_int(const char* symbol, int value SRC_DECL)
{
	PROFILE;

	DBGVAL("%s: %d\n", symbol, value);
	udump_entry_record(&profile->entry, 1, symbol, &value, sizeof(int) SRC_CC);
}

void xcprof_value_userdata(const char* symbol, const void* ptr, size_t size SRC_DECL)
{
	PROFILE;

	DBGVAL("%s: data of size %ld\n", symbol, size);
	udump_entry_record(&profile->entry, 2, symbol, ptr, size SRC_CC);
}

typedef struct _checkpoint_t
{
	clock_t curr;
	symid_t src;
	int line;
	symid_t func;
} checkpoint_t;

void xcprof_value_checkpoint(const char* symbol,
							   const char* srcfile, int srcline, const char* function)
{
	checkpoint_t* ch;
	PROFILE;

	DBGVAL("%s: %s(%d):%s\n", symbol, srcfile, srcline, function);
	// ulozit extra info o source a o casu
	ch = udump_entry_record(&profile->entry, 257, symbol, NULL, sizeof(checkpoint_t) SRC_CC);
	//QueryPerformanceCounter(&(ch->curr));
	ch->curr = clock();
	ch->src = symbol_get(profile->entry.dump, srcfile);
	ch->line = srcline;
	ch->func = symbol_get(profile->entry.dump, function);
}

void xcprof_value_string(const char* symbol, const char* str SRC_DECL)
{
	PROFILE;

	DBGVAL("%s: %s\n", symbol, str);
	udump_entry_record(&profile->entry, 134, symbol, str, strlen(str) SRC_CC);
}

void xcprof_value_symbol(const char* symbol, const char* str SRC_DECL)
{
	symid_t sid;
	PROFILE;

	sid = symbol_get(profile->entry.dump, str);
	DBGVAL("%s: %s (%d)\n", symbol, str, sid);
	udump_entry_record(&profile->entry, 138, symbol, &sid, sizeof(sid) SRC_CC);
}

void xcprof_value_watch(const char* symbol, const time_watch_t* watch SRC_DECL)
{
	PROFILE;

	DBGVAL("%s: %lf\n", symbol, watch_one(watch));
	udump_entry_record(&profile->entry, 135, symbol, watch, sizeof(time_watch_t) SRC_CC);
}

void xcprof_value_traceback(const char* symbol SRC_DECL)
{
#if 0
	PROFILE;
	traceback_t *t = udump_entry_record(&profile->entry, 132, symbol, NULL, sizeof(traceback_t));
	traceback_dump(t, &profile->entry.buffer, &profile->entry.dump->symbols);
#endif
}

