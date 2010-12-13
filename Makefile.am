ACLOCAL_AMFLAGS = -I m4

SUBDIRS = include src tests

MAINTAINERCLEANFILES = Makefile.in aclocal.m4 config.guess config.h.in \
					   config.sub configure depcomp install-sh ltmain.sh missing

test-install:
	$(MAKE) DESTDIR=`pwd`/tmp install

EXTRA_DIST = $(DX_CONFIG) doc/html
