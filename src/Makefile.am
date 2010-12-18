
VERSION_INFO="-version-info 0:0:0"

AM_CXXFLAGS = -W -Wall -g ${AM_CXXEXTRAFLAGS}
AM_CPPFLAGS = -I$(srcdir)/../include
AM_LDFLAGS = $(VERSION_INFO)

lib_LTLIBRARIES = libxcreg.la libxcregmake.la
libxcreg_la_CPPFLAGS = $(XC_CFLAGS) $(LIBCONFIGXX_CFLAGS) $(CDB_CFLAGS)
libxcreg_la_SOURCES = env.cc php.cc web.cc cdb.cc config.cc
libxcreg_la_LDFLAGS = $(VERSION_INFO) -shared $(XC_LIBS) $(LIBCONFIGXX_LIBS) $(CDB_LIBS)

libxcregmake_la_CPPFLAGS = $(XC_CFLAGS) $(CDB_CFLAGS) $(XMLXX_CFLAGS)
libxcregmake_la_SOURCES = make.cc makecdb.cc pofile.cc pogen.cc xmlparser.cc xmlutils.cc xmltrans.cc
libxcregmake_la_LDFLAGS = $(VERSION_INFO) -shared $(XC_LIBS) $(CDB_LIBS) $(XMLXX_LIBS) -lgettextpo

#lib_LTLIBRARIES = libxcusers.la libxcusers-soap.la libxcusers-proxy.la libxcusers-test.la
#libxcusers_la_SOURCES = users.cc utils.cc
#libxcusers_la_LDFLAGS = $(VERSION_INFO) -shared -litissien-pgsql -lmhash

#libxcusers_proxy_la_SOURCES = users_proxy.cc
#libxcusers_proxy_la_LDFLAGS = -weak libxcusers.la -static

#libxcusers_test_la_SOURCES = users_test.cc
#libxcusers_test_la_LDFLAGS = -weak libxcusers.la -static

#libxcusers_soap_la_SOURCES = users_soap.cc
#libxcusers_soap_la_LDFLAGS = -static

bin_PROGRAMS = regc regdump
regc_CPPFLAGS =
regc_SOURCES = cbc.cc
regc_LDADD = libxcregmake.la

regdump_CPPFLAGS =
regdump_SOURCES = cbdump.cc
regdump_LDADD = libxcreg.la

testcbc:
	./cbc test.cb *.xml
