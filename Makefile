#
# File name:  Makefile
# Date:       2010/12/17 21:00
# Subversion: $Id: $
# Author:     Milan Dunghubel <milan@mfis.cz>
#
# Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
#

#CD = CDPATH="$${ZSH_VERSION+.}$(PATH_SEPARATOR)" && cd
CD = cd
SUDO = sudo

BUILD_PACKAGES = libconfig++-dev

all: lib registry php5-registry

# library
lib: 
	$(CD) lib && $(MAKE)

registry: lib
	$(CD) registry && $(MAKE)

php5-registry: registry
	$(CD) php5-registry && $(MAKE)

depbuild:
	$(SUDO) aptitude install $(BUILD_PACKAGES)

.PHONY: lib registry php5-registry
