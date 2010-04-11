/*
   File name:  stdlibs.h
   Date:       2010/03/30 13:46
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_STDLIBS_H_
#define _XC_STDLIBS_H_
#pragma once

/* Define to 1 if you have the ANSI C header files. */
#ifdef STDC_HEADERS
# include <stdio.h>
#endif

/* types */
#ifdef HAVE_STDINT_H
# include <stdint.h>
#endif

#ifdef HAVE_INTTYPES_H
# include <inttypes.h>
#endif

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

/* std libs */
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

/* memory */
#ifdef HAVE_MALLOC_H
# include <malloc.h>
#endif

#ifdef HAVE_MEMORY_H
# include <memory.h>
#endif

/* string */
#ifdef HAVE_STRING_H
# include <string.h>
#endif

#ifdef HAVE_STRINGS_H
# include <strings.h>
#endif

/* files */
#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif

#ifdef HAVE_FCNTL_H 1
# include <fcntl.h>
#endif

#ifdef HAVE_SYS_FILE_H
# include <sys/file.h>
#endif

/* dirs */
#ifdef HAVE_DIRENT_H
# include <dirent.h>
#endif

#ifdef HAVE_SYS_DIR_H
# include <sys/dir.h>
#endif

#ifdef HAVE_NDIR_H
# include <ndir.h>
#endif

#ifdef HAVE_SYS_NDIR_H
# include <sys/ndir.h>
#endif

/* socket & network*/
#ifdef HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#endif

#ifdef HAVE_NETINET_IN_H
# include <netinet/in.h>
#endif

#ifdef HAVE_ARPA_INET_H
# include <arpa/inet.h>
#endif

#ifdef HAVE_NETDB_H
# include <netdb.h>
#endif

/* shared objects */
#ifdef HAVE_DLFCN_H
# include <dlfcn.h>
#endif

/* time */
#ifdef HAVE_SYS_TIME_H
# include <sys/time.h>
#endif

#ifdef HAVE_SYS_WAIT_H
# include <sys/wait.h>
#endif

#ifdef HAVE_VFORK_H
# include <vfork.h>
#endif

#endif // _XC_STDLIBS_H_
/* end of stdlibs.h */

