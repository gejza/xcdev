
PROFILER_LIB = $(top_srcdir)/src/libxc.a $(top_srcdir)/src++/libxc++.a

# path to includes
AM_CPPFLAGS = -I../include $(LIBCONFIG_CFLAGS)
AM_LDFLAGS = -static -L../src

#noinst_PROGRAMS = test_tree test_file test_st test_string
noinst_PROGRAMS = log config debug text values

log_SOURCES = log.cc
log_LDADD = -lxclog

config_SOURCES = config.cc
config_LDADD = -lxclog -lxcconfig

debug_SOURCES      = debug.cc
text_SOURCES      = text.cc
values_SOURCES      = values.cc

