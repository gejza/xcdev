
VERSION_INFO="-version-info 0:0:0"

AM_CXXFLAGS = -W -Wall -g ${AM_CXXEXTRAFLAGS}
AM_CPPFLAGS = -I$(srcdir)/../include
AM_LDFLAGS = $(VERSION_INFO)

lib_LTLIBRARIES = libxcrd.la
libxcrd_la_CPPFLAGS = -DDEBUG $(XC_CFLAGS) $(LIBCONFIGXX_CFLAGS) $(CDB_CFLAGS)
libxcrd_la_SOURCES = env.cc cdb.cc config.cc make.cc out_cdb.cc out_debug.cc
libxcrd_la_LDFLAGS = $(VERSION_INFO) -shared $(XC_LIBS) $(LIBCONFIGXX_LIBS) $(CDB_LIBS)

bin_PROGRAMS = xcrdlookup xcrddump

xcrdlookup_CPPFLAGS = $(READLINE_CFLAGS)
xcrdlookup_SOURCES = lookup.cc
xcrdlookup_LDADD = libxcrd.la $(READLINE_CLIBS)

xcrddump_CPPFLAGS =
xcrddump_SOURCES = dump.cc
xcrddump_LDADD = libxcrd.la

noinst_PROGRAMS = test

test_SOURCES = test.cc
test_LDADD = libxcrd.la -lxcconfig -lxctest

testcbc:
	./cbc test.cb *.xml
