# Makefile for evenmoreutils
#
#
#
# Copyright (C) 2013, 2014  Rudy Matela
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#
# Targets: all, clean, install, homeinstall, dist VERSION=x.x
#
#
# TODO: Also install libraries and includes on /usr/lib and /usr/include.  When
# doing that, separate targets: install (all), installlib (only libraries),
# installbin (only executables).  Also only install manuals of the tools
# installed.


# Implicit rules
%.1: %.1.adoc
	a2x -a mansource=evenmoreutils -a revdate=`date +%Y-%m-%d` -a manmanual='Evenmoreutils Manual' --doctype manpage --format manpage $<


BINS=src/sgetopt_test src/hilite src/randpar src/untl src/fit src/age src/ched
SHS=src/anywait src/easy-getopt src/hl src/mime src/p src/evenmoreutils-common.sh src/spongif
SYMS=src/pad src/whle src/repeat # symlinks to binaries or shells
OBJS=lib/sgetopt.o lib/muni.o
MANS=man/anywait.1 man/fit.1 man/hilite.1 man/hl.1 man/mime.1 man/p.1 man/randpar.1 man/untl.1 man/age.1 man/ched.1 man/sgetopt.1
GMANS=man/pad.1 man/whle.1 man/repeat.1 # copies generated automatically by asciidoc
AMANS=$(MANS) $(GMANS) # all manpages
CFLAGS=-Wall -Werror -Wno-variadic-macros -I./lib
PREFIX=/usr
MANPREFIX=$(PREFIX)/share

.PHONY: all clean install homeinstall dist test

all: $(BINS) $(MANS)

man: $(MANS)

src/untl: LDLIBS=-lm
src/untl: lib/sgetopt.o

src/sgetopt_test: lib/sgetopt.o

src/randpar: lib/sgetopt.o

src/fit: lib/sgetopt.o

src/age: LDLIBS=-lm
src/age: lib/sgetopt.o lib/muni.o

src/ched: LDLIBS=-lbsd
src/ched: lib/sgetopt.o lib/muni.o

lib/muni.o: LDLIBS=-lrt
lib/muni.o: lib/muni.c lib/muni.h

lib/sgetopt.o: lib/sgetopt.c lib/sgetopt.h

clean:
	rm -f $(BINS) $(OBJS) $(AMANS)

install: all
	mkdir -p                $(DESTDIR)$(PREFIX)/bin
	install -s     $(BINS)  $(DESTDIR)$(PREFIX)/bin
	install        $(SHS)   $(DESTDIR)$(PREFIX)/bin
	cp -P          $(SYMS)  $(DESTDIR)$(PREFIX)/bin
	mkdir -p                $(DESTDIR)$(MANPREFIX)/man/man1
	install -m 644 $(AMANS) $(DESTDIR)$(MANPREFIX)/man/man1

homeinstall:
	make DESTDIR=~ PREFIX= MANPREFIX= install

dist:
	./bin/dist.sh $(VERSION) $(DESTDIR)

test: all
	./bin/test.sh
