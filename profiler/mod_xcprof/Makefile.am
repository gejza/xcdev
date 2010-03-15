
xcprof_LIB = $(top_srcdir)/src/libxcprof.a

noinst_PROGRAMS = mod_xcprof.so

mod_xcprof_so_CFLAGS = -I/usr/include/httpd -I/usr/include/apr-1 -I../include -DLINUX=2 -D_REENTRANT -D_GNU_SOURCE -D_LARGEFILE64_SOURCE -pthread
mod_xcprof_so_SOURCES = mod_xcprof.c
mod_xcprof_so_LDADD = -lxcprof
mod_xcprof_so_LDFLAGS = -shared -L../src 
mod_xcprof_so_DEPENDENCIES = $(xcprof_LIB) 

