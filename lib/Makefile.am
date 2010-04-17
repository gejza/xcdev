# Makefile.am - main makefile for Xross Core Library
#     	Copyright (C) 2010, 2004 Free Software Foundation, Inc.
# 
# This file is part of Xross Core Library.
# 
# GnuPG is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
# 
# GnuPG is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, see <http://www.gnu.org/licenses/>.

## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = dist-bzip2 no-dist-gzip
DISTCHECK_CONFIGURE_FLAGS = --enable-symcryptrun --enable-mailto

SUBDIRS = include src test
#test include
EXTRA_DIST = build-aux/config.rpath m4/ChangeLog  libxc.pc.in
MAINTAINERCLEANFILES = Makefile.in aclocal.m4 config.guess config.h.in \
                       config.sub configure depcomp install-sh ltmain.sh missing

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libxc.pc

dist-hook:
	echo "$(VERSION)" > $(distdir)/VERSION

test-install:
	$(RM) -rf `pwd`/tmp
	$(MAKE) DESTDIR=`pwd`/tmp install

check-gettext:
	@if test x$(USE_NLS) != "xyes" ; then echo "Missing gettext. Rerun configure and check for" \
	"'checking whether to use NLS... yes'!" ; exit 1 ; fi

update-po: check-gettext
	@find $(srcdir)/src/ -name "*.cpp" -print | sort > $(srcdir)/po/POTFILES.in.2 ; \
	if diff $(srcdir)/po/POTFILES.in $(srcdir)/po/POTFILES.in.2 >/dev/null 2>&1 ; then \
		rm -f $(srcdir)/po/POTFILES.in.2 ; \
	else \
		mv $(srcdir)/po/POTFILES.in.2 $(srcdir)/po/POTFILES.in ; \
	fi
	cd po && $(MAKE) $(AM_MAKEFLAGS) update-po

update-gmo: check-gettext
	cd po && $(MAKE) $(AM_MAKEFLAGS) update-gmo

force-update-gmo: check-gettext
	touch po/*.po
	cd po && $(MAKE) $(AM_MAKEFLAGS) update-gmo

#force-update-gmo-%: check-gettext
#	@language=`echo $@ | sed s/force-update-gmo-//` ; \
#	if test ! -f po/$$language.po ; then echo "file po/$$language.po does not exist" ; exit 1 ; fi ; \
#	touch po/$$language.po ; \
#	cd po && $(MAKE) $(AM_MAKEFLAGS) update-gmo

.PHONY: check-gettext update-po update-gmo force-update-gmo

