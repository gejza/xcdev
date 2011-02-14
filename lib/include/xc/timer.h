/*
   File name:  timer.h
   Date:       2011/02/14 17:04
   Subversion: $Id$
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_TIMER_H_
#define _XC_TIMER_H_
#pragma once

namespace xc {

class timer_t
{
public:
    
    typedef double value_type;

    /**
     * Default constructor
     */
    timer_t();

    /**
     * Destructor
     */
    ~timer_t();

    static value_type get_time();

    void start();

    value_type finish();

    value_type get();

private:
    value_type _start;
    value_type _finish;
};

} // namespace xc

#endif // _XC_TIMER_H_
/* end of timer.h */

