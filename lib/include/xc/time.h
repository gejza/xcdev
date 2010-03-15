
/*
* Copyright
*/

#pragma once
#ifndef _XC_TIME_H_
#define _XC_TIME_H_

#include "config.h"
#ifdef _WIN32
#include <clock.h>
#endif
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	XCTIME_UNIT_MICRO,
	XCTIME_UNIT_MILI,
	XCTIME_UNIT_SEC,
	XCTIME_UNIT_MIN,
	XCTIME_UNIT_HOUR,
} XCTIME_UNIT;

typedef clock_t xc_timer_type_t;

/** Structure for time timer */
typedef struct _xc_timer_t
{
	xc_timer_type_t start; ///< start operation
	xc_timer_type_t finish; ///< finish operation
	size_t nums; ///< num operations
} xc_timer_t;

//todo doc
XC_API void xc_timer_start(xc_timer_t* time);
XC_API void xc_timer_stop(xc_timer_t* time, size_t numop);
XC_API void xc_timer_report(const xc_timer_t* time, FILE* out);
XC_API double xc_timer_avg(const xc_timer_t* time);

#ifdef __cplusplus 
} // extern "C"
#endif

#endif // _XC_TIME_H_

/*
* Copyright
*/

#pragma once
#ifndef _XC_TIME_HPP_
#define _XC_TIME_HPP_

#include "time.h"

namespace xc {

class Timer
{
	xc_timer_t t;
public:
	Timer()
	{
		xc_timer_start(&t);
	}
	void Stop(int numop)
	{
		xc_timer_stop(&t, numop);
	}
	void Report()
	{
		xc_timer_report(&t, stdout);
	}
};

} // namespace xc

#endif // _XC_TIME_HPP_
