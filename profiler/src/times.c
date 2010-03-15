
#include <stdio.h>
#include <time.h>
#include "xcprof/times.h"

typedef struct _time_unit_t
{
	const char* name;
	double mult;
	clock_t clk;
} time_unit_t;

static time_unit_t time_unit[] = 
{
	{ "us", 1000000, CLOCKS_PER_SEC / 1000000 },
	{ "ms", 1000, CLOCKS_PER_SEC / 1000 },
	{ "s", 1, CLOCKS_PER_SEC },
	{ "min", 1/60.0, CLOCKS_PER_SEC * 60 },
	{ "hod", 1/3600.0, CLOCKS_PER_SEC * 3600 },
	0, // end
};

/********* Private functions ************/
static int watch_find_unit(time_watch_type_t t)
{
	int i;
	//time_watch_type_t freq;
	//QueryPerformanceFrequency(&freq);

	for (i=1; time_unit[i].name; i++)
	{
		if (time_unit[i].clk > t)
			return i - 1;
	}
	return TIME_UNIT_HOUR;
}

__inline time_watch_type_t watch_time_diff(time_watch_t* time)
{
	return time->finish - time->start;
}

/********* Public functions ************/
void watch_start(time_watch_t* time)
{
	//QueryPerformanceCounter(&time->start);
	time->start = clock();
}

void watch_finish(time_watch_t* time, size_t numop)
{
	//QueryPerformanceCounter(&time->finish);
	time->finish = clock();
	time->nums = numop;
}

double watch_one(const time_watch_t* time)
{
	return (double)watch_time_diff(time) / time->nums / CLOCKS_PER_SEC;
}

void watch_report(const time_watch_t* time, FILE* out)
{
	time_watch_type_t freq = CLOCKS_PER_SEC;//(QueryPerformanceFrequency(&freq), freq);

	const int t_unit = watch_find_unit(watch_time_diff(time));
	const int o_unit = watch_find_unit(watch_time_diff(time) / time->nums);
	const double diff = (double)(time->finish - time->start)
		* (time_unit[t_unit].mult / freq);
	const double one = (double)(time->finish - time->start)
		* (time_unit[o_unit].mult / freq) / time->nums;

	fprintf(out, "%lf%s(%ld=%lf%s)",
		one, time_unit[o_unit].name,
		time->nums,
		diff, time_unit[t_unit].name);
}

