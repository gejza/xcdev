
PROFILER_LIB = $(top_srcdir)/src/libxcprof.a

noinst_PROGRAMS = xcprof.pyd

xcprof_pyd_CFLAGS = ${CPPFLAGS} -I/usr/include/python2.5
xcprof_pyd_SOURCES = module.c profiler.c symboltable.c
xcprof_pyd_LDADD = -lxcprof
xcprof_pyd_LDFLAGS = -shared -L../src 
xcprof_pyd_DEPENDENCIES = $(PROFILER_LIB) 

