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

SUBDIRS = include src tests
#test include
EXTRA_DIST = build-aux/config.rpath m4/ChangeLog  libxc.pc.in $(DX_CONFIG) doc/html
MAINTAINERCLEANFILES = Makefile.in aclocal.m4 config.guess config.h.in \
                       config.sub configure depcomp install-sh ltmain.sh missing

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libxc.pc

dist-hook:
	echo "$(VERSION)" > $(distdir)/VERSION

## gettext
include $(top_srcdir)/am/gettext.am

## test
include $(top_srcdir)/am/test.am

## includes additional rules
@INC_AMINCLUDE@

.PHONY: $(DX_PHONY) $(GETTEXT_PHONY)
