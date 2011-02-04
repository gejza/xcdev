
VERSION_INFO="-version-info 0:0:0"

AM_CPPFLAGS = -I$(top_srcdir)/include -DENDCPP
AM_CXXFLAGS = -W -Wall -g ${AM_CXXEXTRAFLAGS} -DMAMCXX
AM_LDFLAGS = $(VERSION_INFO)

lib_LTLIBRARIES = libxcrd.la
libxcrd_la_CPPFLAGS = $(AM_CPPFLAGS) $(LIBCONFIGXX_CFLAGS) $(CDB_CFLAGS)
libxcrd_la_SOURCES = env.cc cdb.cc config.cc make.cc out_cdb.cc out_debug.cc
libxcrd_la_LDFLAGS = $(VERSION_INFO) -shared -L.. -lxc $(LIBCONFIGXX_LIBS) $(CDB_LIBS)

bin_PROGRAMS = xcrdlookup xcrddump

xcrdlookup_CPPFLAGS = $(AM_CPPFLAGS)  $(READLINE_CFLAGS)
xcrdlookup_SOURCES = lookup.cc
xcrdlookup_LDADD = libxcrd.la $(READLINE_CLIBS) -lxclog

xcrddump_CPPFLAGS = $(AM_CPPFLAGS) 
xcrddump_SOURCES = dump.cc
xcrddump_LDADD = libxcrd.la

noinst_PROGRAMS = test

test_SOURCES = test.cc
test_LDADD = libxcrd.la -lxcconfig -lxctest

testcbc:
	./cbc test.cb *.xml
