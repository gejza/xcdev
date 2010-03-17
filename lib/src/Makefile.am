
# current[:revision[:age]]
VERSION_INFO="-version-info 1:0:0"

# path to includes
AM_CPPFLAGS = -I../include $(LIBCONFIG_CFLAGS)

lib_LTLIBRARIES = libxc.la libxclog.la libxcconfig.la

libxc_la_SOURCES = text.cpp uuid.cpp
libxc_la_LDFLAGS = $(VERSION_INFO)

libxclog_la_SOURCES = log.cc debug.cpp
libxclog_la_LDFLAGS = -shared $(VERSION_INFO)

libxcconfig_la_SOURCES = config.cc config_ini.ll
libxcconfig_la_LDFLAGS = -shared $(VERSION_INFO) $(LIBCONFIG_LIBS)
