
PROFILER_LIB = $(top_srcdir)/src/libxcprof.a

# path to includes
AM_CPPFLAGS = -I../include

bin_PROGRAMS = xcprofsymboltable xcprof

xcprofsymboltable_SOURCES      = symboltable.cpp cmdutil.cpp
xcprofsymboltable_LDADD        = $(PROFILER_LIB)
xcprofsymboltable_LDFLAGS      = -static
xcprofsymboltable_DEPENDENCIES = $(PROFILER_LIB)

xcprof_SOURCES      = profiler.cpp cmdutil.cpp
xcprof_LDADD        = $(PROFILER_LIB)
xcprof_LDFLAGS      = -static
xcprof_DEPENDENCIES = $(PROFILER_LIB)

