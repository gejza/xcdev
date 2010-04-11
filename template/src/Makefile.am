
# current[:revision[:age]]
VERSION_INFO="-version-info 1:0:0"

# path to includes
AM_CPPFLAGS = -g -I../include -DLOCALEDIR=\"$(localedir)\"
AM_LDFLAGS = -g -lxc -lxclog

lib_LTLIBRARIES = libxctemplate.la

libxctemplate_la_SOURCES = \
	env.cc template.cc debug.cc vm.cc \
	bytecode.cc preprocess.cc scan_html.ll \
	db.cc fs.cc \
	output.cc
libxctemplate_la_LDFLAGS = -lxc -lxclog -lcdb

bin_PROGRAMS = xct xct-comp xct-dump
xct_SOURCES = xct.cc
xct_LDFLAGS = libxctemplate.la -lxclog -lxc

xct_comp_SOURCES = xct-comp.cc
xct_comp_LDFLAGS = libxctemplate.la -lxclog -lxc

xct_dump_SOURCES = xct-dump.cc
xct_dump_LDFLAGS = libxctemplate.la -lxclog -lxc

#libxc_la_LDFLAGS = $(VERSION_INFO)
#libxc_la_LIBADD = -lpcre

#libxclog_la_LDFLAGS = -shared $(VERSION_INFO) -lxc -ldl -liberty_pic

#libxcconfig_la_SOURCES = config.cc config_ini.ll
#libxcconfig_la_LDFLAGS = -shared $(VERSION_INFO) -lxc $(LIBCONFIG_LIBS)
