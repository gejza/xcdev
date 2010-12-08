ACLOCAL_AMFLAGS = -I m4

SUBDIRS = include src

MAINTAINERCLEANFILES = Makefile.in aclocal.m4 config.guess config.h.in \
					   config.sub configure depcomp install-sh ltmain.sh missing

test-install:
	$(MAKE) DESTDIR=`pwd`/tmp install

include aminclude.am

EXTRA_DIST = $(DX_CONFIG) doc/html
