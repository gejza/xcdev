
VERSION_INFO="-version-info 0:0:0"

AM_CXXFLAGS = -W -Wall -g ${AM_CXXEXTRAFLAGS}
AM_CPPFLAGS = -I$(srcdir)/../include
AM_LDFLAGS = $(VERSION_INFO)

lib_LTLIBRARIES = libxccb.la libxccbmake.la
libxccb_la_CPPFLAGS = $(CDB_CFLAGS)
libxccb_la_SOURCES = cb.cc constdb.cc fd.cc
libxccb_la_LDFLAGS = $(VERSION_INFO) -shared $(CDB_LIBS)

libxccbmake_la_CPPFLAGS = $(CDB_CFLAGS) $(XMLXX_CFLAGS)
libxccbmake_la_SOURCES = cbmake.cc constdb.cc fd.cc pofile.cc pogen.cc xmlparser.cc xmlutils.cc xmltrans.cc
libxccbmake_la_LDFLAGS = $(VERSION_INFO) -shared $(CDB_LIBS) $(XMLXX_LIBS) -lgettextpo

#lib_LTLIBRARIES = libxcusers.la libxcusers-soap.la libxcusers-proxy.la libxcusers-test.la
#libxcusers_la_SOURCES = users.cc utils.cc
#libxcusers_la_LDFLAGS = $(VERSION_INFO) -shared -litissien-pgsql -lmhash

#libxcusers_proxy_la_SOURCES = users_proxy.cc
#libxcusers_proxy_la_LDFLAGS = -weak libxcusers.la -static

#libxcusers_test_la_SOURCES = users_test.cc
#libxcusers_test_la_LDFLAGS = -weak libxcusers.la -static

#libxcusers_soap_la_SOURCES = users_soap.cc
#libxcusers_soap_la_LDFLAGS = -static

bin_PROGRAMS = cbc cbdump
cbc_CPPFLAGS =
cbc_SOURCES = cbc.cc
cbc_LDADD = libxccbmake.la

cbdump_CPPFLAGS =
cbdump_SOURCES = cbdump.cc
cbdump_LDADD = libxccb.la

noinst_PROGRAMS = test
test_SOURCES = test.cc
test_LDFLAGS = -static
test_LDADD = libxccb.la libxccbmake.la

