#
# cnext - Modern "NonStandard" C library.
#
# Copyright (c) 2026-present Tanvir.
# SPDX-License-Identifier: MPL-2.0
#

# Makefile for cnext library

PREFIX ?= /usr
DESTDIR ?=
INCLUDEDIR := $(DESTDIR)$(PREFIX)/include/cnext

HEADERS := cnext.h ds.h mem.h algorithm.h ds/list.h

.PHONY: install uninstall

install:
	mkdir -p $(INCLUDEDIR)/ds
	install -m 644 cnext.h ds.h mem.h algorithm.h $(INCLUDEDIR)/
	install -m 644 ds/list.h $(INCLUDEDIR)/ds/

uninstall:
	rm -rf $(INCLUDEDIR)
