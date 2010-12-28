
VERSION_INFO="-version-info 0:0:0"

AM_CXXFLAGS = -W -Wall -g ${AM_CXXEXTRAFLAGS}
AM_CPPFLAGS = -I$(srcdir)/../include
AM_LDFLAGS = $(VERSION_INFO)

lib_LTLIBRARIES = libxcrd.la libxcrdmake.la
libxcrd_la_CPPFLAGS = $(XC_CFLAGS) $(LIBCONFIGXX_CFLAGS) $(CDB_CFLAGS)
libxcrd_la_SOURCES = env.cc php.cc web.cc cdb.cc config.cc
libxcrd_la_LDFLAGS = $(VERSION_INFO) -shared $(XC_LIBS) $(LIBCONFIGXX_LIBS) $(CDB_LIBS)

libxcrdmake_la_CPPFLAGS = $(XC_CFLAGS) $(CDB_CFLAGS) $(XMLXX_CFLAGS)
libxcrdmake_la_SOURCES = make.cc out_cdb.cc out_debug.cc 
libxcrdmake_la_LDFLAGS = $(VERSION_INFO) -shared $(XC_LIBS) $(CDB_LIBS) $(XMLXX_LIBS) -lgettextpo

#lib_LTLIBRARIES = libxcusers.la libxcusers-soap.la libxcusers-proxy.la libxcusers-test.la
#libxcusers_la_SOURCES = users.cc utils.cc
#libxcusers_la_LDFLAGS = $(VERSION_INFO) -shared -litissien-pgsql -lmhash

#libxcusers_proxy_la_SOURCES = users_proxy.cc
#libxcusers_proxy_la_LDFLAGS = -weak libxcusers.la -static

#libxcusers_test_la_SOURCES = users_test.cc
#libxcusers_test_la_LDFLAGS = -weak libxcusers.la -static

#libxcusers_soap_la_SOURCES = users_soap.cc
#libxcusers_soap_la_LDFLAGS = -static

bin_PROGRAMS = reglookup regdump

reglookup_CPPFLAGS =
reglookup_SOURCES = reglookup.cc
reglookup_LDADD = libxcrd.la

regdump_CPPFLAGS =
regdump_SOURCES = regdump.cc
regdump_LDADD = libxcrd.la

testcbc:
	./cbc test.cb *.xml
