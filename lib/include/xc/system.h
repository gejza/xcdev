/*
   File name:  system.h
   Date:       2010/03/25 12:29
   Subversion: $Id$
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

/**
 * @example system.cc
 */
#ifndef _XC_SYSTEM_H_
#define _XC_SYSTEM_H_
#pragma once

#include <pwd.h>

namespace xc {
namespace system {

    /** 
     * @short Get signal name
     * @param signal 
     * @return name
     */
    const char* signal_name(int signal);

    uid_t get_uid(const char* user);
    gid_t get_gid(const char* group);

    void set_user(const char* user, const char* group = 0x0);
    void set_user(uid_t uid, gid_t gid = 0);

    void daemonize();

} // namespace system
} // namespace xc

#include "system/pidfile.h"

#endif // _XC_SYSTEM_H_
/* end of system.h */

