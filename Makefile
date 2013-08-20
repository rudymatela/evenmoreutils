# Makefile for evenmoreutils
#
#
#
# Copyright (C) 2013  Rudy Matela
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


# Implicit rules
%.1: %.1.txt
	a2x --format manpage $<


BINS=sgetopt_test hilite randpar untl fit age
SHS=anywait easy-getopt hl mime nup p evenmoreutils-common.sh spongif repeat
SYMS=pad whle # symlinks to binaries or shells
OBJS=sgetopt.o
SMANS=pad.1 whle.1 # symlinks to manpages
MANS=anywait.1 fit.1 hilite.1 hl.1 mime.1 nup.1 p.1 randpar.1 untl.1 age.1 repeat.1
CFLAGS=-Wall -Werror -Wno-variadic-macros
LDFLAGS=-lm
PREFIX=/usr
MANPREFIX=$(PREFIX)/share

.PHONY: all clean install homeinstall dist test

all: $(BINS)

untl: sgetopt.o

sgetopt_test: sgetopt.o

randpar: sgetopt.o

fit: sgetopt.o

age: sgetopt.o

clean:
	rm -f $(BINS) $(OBJS)

install: all
	mkdir -p               $(DESTDIR)$(PREFIX)/bin
	install -s     $(BINS) $(DESTDIR)$(PREFIX)/bin
	install        $(SHS)  $(DESTDIR)$(PREFIX)/bin
	cp -P          $(SYMS) $(DESTDIR)$(PREFIX)/bin
	mkdir -p               $(DESTDIR)$(MANPREFIX)/man/man1
	install -m 644 $(MANS) $(DESTDIR)$(MANPREFIX)/man/man1
	cp -P         $(SMANS) $(DESTDIR)$(MANPREFIX)/man/man1

homeinstall:
	make DESTDIR=~ PREFIX= MANPREFIX= install

dist:
	./dist.sh $(VERSION) $(DESTDIR)

test: all
	./test.sh
