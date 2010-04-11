
# path to includes
AM_CPPFLAGS = -g -I../include -DLOCALEDIR=\"$(localedir)\"
AM_LDFLAGS = -static -g -lxc -lxclog -lxctest ../src/libxctemplate.la

#noinst_PROGRAMS = test_tree test_file test_st test_string
noinst_PROGRAMS = scan-html db

scan_html_SOURCES = scan-html.cc

db_SOURCES = db.cc

