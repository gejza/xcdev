
VERSION_INFO="-version-info 0:0:0"

AM_CXXFLAGS = -W -Wall -g ${AM_CXXEXTRAFLAGS}
AM_CPPFLAGS = -I$(srcdir)/../include
AM_LDFLAGS = $(VERSION_INFO)

lib_LTLIBRARIES = libmfis-registry.la
libmfis_registry_la_SOURCES = registry.cc
libmfis_registry_la_LDFLAGS = $(VERSION_INFO) -shared -lcdb

#lib_LTLIBRARIES = libmfis-users.la libmfis-users-soap.la libmfis-users-proxy.la libmfis-users-test.la
#libmfis_users_la_SOURCES = users.cc utils.cc
#libmfis_users_la_LDFLAGS = $(VERSION_INFO) -shared -litissien-pgsql -lmhash

#libmfis_users_proxy_la_SOURCES = users_proxy.cc
#libmfis_users_proxy_la_LDFLAGS = -weak libmfis-users.la -static

#libmfis_users_test_la_SOURCES = users_test.cc
#libmfis_users_test_la_LDFLAGS = -weak libmfis-users.la -static

#libmfis_users_soap_la_SOURCES = users_soap.cc
#libmfis_users_soap_la_LDFLAGS = -static

bin_PROGRAMS = regcomp
regcomp_CPPFLAGS = $(LIBXMLXX_CFLAGS)
regcomp_SOURCES = regcomp.cc build.cc parse_xml.cc
regcomp_LDADD = $(LIBXMLXX_LIBS)

noinst_PROGRAMS = test
test_SOURCES = test.cc
test_LDFLAGS = -static
test_LDADD = -lmfis-registry

