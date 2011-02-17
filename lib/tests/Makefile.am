
PROFILER_LIB = $(top_srcdir)/src/libxc.a $(top_srcdir)/src++/libxc++.a

# path to includes
AM_CPPFLAGS = -I../include $(LIBCONFIG_CFLAGS) -DLOCALEDIR=\"$(localedir)\"
AM_LDFLAGS = -static -L../src -lxc -lxclog -lxctest -g

#noinst_PROGRAMS = test_tree test_file test_st test_string
noinst_PROGRAMS = log config debug text gc values files system serialize timer ipc

log_CPPFLAGS = $(AM_CPPFLAGS) -DLOGBACKTRACE=1
log_SOURCES = log.cc

config_SOURCES = config.cc
config_LDADD = -lxcconfig

debug_SOURCES = debug.cc

text_SOURCES = text.cc

gc_SOURCES = gc.cc

values_SOURCES = values.cc

files_SOURCES = files.cc

system_SOURCES = system.cc

serialize_SOURCES = serialize.cc

timer_SOURCES = timer.cc

ipc_CPPFLAGS = $(AM_CPPFLAGS) -DXC_IPC=1
ipc_SOURCES = ipc.cc
