
PROFILER_LIB = $(top_srcdir)/src/libxcprof.a

# path to includes
AM_CPPFLAGS = -I../include

noinst_PROGRAMS = test_udump test_mem test_profiler

test_udump_SOURCES      = udump.cpp
test_udump_LDADD        = $(PROFILER_LIB)
test_udump_LDFLAGS      = -static
test_udump_DEPENDENCIES = $(PROFILER_LIB)

test_mem_SOURCES      = mem.cpp
test_mem_LDADD        = $(PROFILER_LIB)
test_mem_LDFLAGS      = -static
test_mem_DEPENDENCIES = $(PROFILER_LIB)

test_profiler_SOURCES      = profiler.cpp
test_profiler_LDADD        = $(PROFILER_LIB)
test_profiler_LDFLAGS      = -static
test_profiler_DEPENDENCIES = $(PROFILER_LIB)

