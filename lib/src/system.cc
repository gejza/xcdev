/*
   File name:  system.cc
   Date:       2010/03/25 17:18
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include <xc/log.h>
#include <xc/error.h>
#include "xc/system.h"

uid_t xc::system::get_uid(const char* user)
{
    if (user) {
        ::passwd* u = ::getpwnam(user);
        if (!u)
            ERROR(xc::error_t, "Attempt to use unknown user <%s>.", user);
        return u->pw_uid;
    }
    return -1;
}

gid_t xc::system::get_gid(const char* group)
{
    if (group) {
        ::group* g = ::getgrnam(group);
        if (!g)
            ERROR(xc::error_t, "Attempt to use unknown group <%s>.", group);
        return g->gr_gid;
    }
    return -1;
}

void xc::system::set_user(const char* user, const char* group)
{
    set_user(get_uid(user), get_gid(group));
}

void xc::system::set_user(uid_t uid, gid_t gid)
{
    ::passwd* u = ::getpwuid(uid);
    ::group* g = gid >= 0 ? ::getgrgid(gid):NULL;
    if (g) {
        LOG_INFO( 4, "Switching to user:group <%s:%s>.", u->pw_name, g->gr_name);
    } else {
        LOG_INFO( 4, "Switching to user <%s>.", u->pw_name);
    }

    // switch group
    if (gid >= 0 && ::getegid() != gid) {

        const char* name = g ? g->gr_name : "unknown";

        if (::setegid(gid) == -1) {
            LOG_FATAL("Unable to switch to group <%s> (gid %d): <%d, %s>.",
                    name, int(gid), errno, strerror(errno));
        }
    }

    // switch persona
    if (::geteuid() != uid) {

        const char* name = u ? u->pw_name : "unknown";
        if (::seteuid(uid) == -1) {
            LOG_FATAL("Unable to switch to user <%s> (uid %d): <%d, %s>.",
                    name, int(uid), errno, strerror(errno));
        }
    }
}


void xc::system::daemonize()
{
    /* Fork off the parent process */
    pid_t pid = ::fork();
    if (pid < 0) {
        LOG_FATAL("Forking failed.");
        ::exit(EXIT_FAILURE);
    }

    /* If we got a good PID, then
       we can exit the parent process. */
    if (pid > 0) {
        ::exit(EXIT_SUCCESS);
    }

    /* Change the file mode mask */
    ::umask(0);

    /* Create a new SID for the child process */
    pid_t sid = setsid();
    if (sid < 0) {
        LOG_FATAL("setsid failed.");
        ::exit(EXIT_FAILURE);
    }

    /* Change the current working directory */
    //if (::chdir("/") < 0) {

    //}

    /* Close out the standard file descriptors */
    ::close(STDIN_FILENO);
    ::close(STDOUT_FILENO);
    ::close(STDERR_FILENO);
}


