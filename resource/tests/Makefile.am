#
# File name:  Makefile.am
# Date:       2010/12/13 07:09
# Subversion: $Id: $
# Author:     Milan Dunghubel <milan@mfis.cz>
#
# Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
#

# path to includes
AM_CPPFLAGS = -I../include $(LIBCONFIG_CFLAGS) -DLOCALEDIR=\"$(localedir)\" -ggdb -DLOGBACKTRACE=1
AM_LDFLAGS = -static -L../src -lxc -lxclog -lxctest $(top_srcdir)/src/libxcreg.la $(top_srcdir)/src/libxcregmake.la -g

#noinst_PROGRAMS = test_tree test_file test_st test_string
noinst_PROGRAMS = test

#config_SOURCES = config.cc
#config_LDADD = -lxcconfig

test_SOURCES = test.cc


