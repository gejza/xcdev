
PROFILER_LIB = $(top_srcdir)/src/libxc.a $(top_srcdir)/src++/libxc++.a

# path to includes
AM_CPPFLAGS = -I../include

noinst_PROGRAMS = test_tree test_file test_st test_string

test_tree_SOURCES      = tree.cpp
test_tree_LDADD        = $(PROFILER_LIB)
test_tree_LDFLAGS      = -static
test_tree_DEPENDENCIES = $(PROFILER_LIB)

test_file_SOURCES      = file.cpp
test_file_LDADD        = $(PROFILER_LIB)
test_file_LDFLAGS      = -static
test_file_DEPENDENCIES = $(PROFILER_LIB)

test_st_SOURCES      = symboltable.cpp
test_st_LDADD        = $(PROFILER_LIB)
test_st_LDFLAGS      = -static
test_st_DEPENDENCIES = $(PROFILER_LIB)

test_string_SOURCES      = string.cpp
test_string_LDADD        = $(PROFILER_LIB)
test_string_LDFLAGS      = -static
test_string_DEPENDENCIES = $(PROFILER_LIB)

