
SUBDIRS = . resource

# current[:revision[:age]]
VERSION_INFO=1:0:0

# path to includes
AM_CPPFLAGS = -I../include $(LIBCONFIG_CFLAGS) -DLOCALEDIR=\"$(localedir)\"

lib_LTLIBRARIES = libxc.la libxclog.la libxcconfig.la libxctest.la

libxc_la_SOURCES = text.cpp regex.cc log.cc \
				   pool.cc error.cc debug.cc debug_sys.cc \
				   fd.cc file.cc fs.cc paths.cc \
				   system.cc system_pidfile.cc refobj.cc \
				   version.cc serialize.cc unserialize.cc bytecode.cc timer.cc mutex.cc

libxc_la_LDFLAGS = -version-info $(VERSION_INFO)
libxc_la_LIBADD = -lpcre -ldl -liberty_pic

libxclog_la_SOURCES = log.cc
libxclog_la_LDFLAGS = -shared -version-info $(VERSION_INFO) -lxc -ldl -liberty_pic

libxcconfig_la_SOURCES = config.cc config_ini.cc
libxcconfig_la_LDFLAGS = -shared -version-info $(VERSION_INFO) -lxc $(LIBCONFIG_LIBS)

libxctest_la_SOURCES = test_main.cc
libxctest_la_LDFLAGS = -static
