/*
 * File name: StdAfx.h
 * Date:      2010/12/06 04:17
 * Author:    
 */

#ifndef STDAFX_H
#define STDAFX_H

#include <stdio.h>
#include <errno.h>
#include <error.h>
#include <vector>
#include <string>
#include <stdint.h>
#include <stdexcept>
#include <iostream>
#include <stdlib.h>

#include <xc/log.h>
#include <xc/error.h>
#include <xc/text.h>
#include <xc/debug.h>

//TODO: to lib
namespace xc {
    template<typename Data_t>
    xc::string human(const Data_t& data, size_t limit)
    {
        return xc::human(data.data(), data.size(), limit);
    }
}

#endif

/* end of StdAfx.h */
