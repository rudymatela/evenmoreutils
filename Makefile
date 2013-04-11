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

BINS=sgetopt_test hilite randpar untl
# TODO: Make whle be installed as a symbolic link!!!!
SHS=easy-getopt fit hl mime nup p pad whle
OBJS=sgetopt.o
MANS=randpar.1
CFLAGS=-ansi -pedantic -Wall -Werror -Wno-variadic-macros
PREFIX=/usr
MANPREFIX=$(PREFIX)/share

.PHONY: all clean install homeinstall

all: $(BINS)

sgetopt_test: sgetopt.o

randpar: sgetopt.o

clean:
	rm -f $(BINS) $(OBJS)

install: all
	mkdir -p               $(DESTDIR)$(PREFIX)/bin
	install -s     $(BINS) $(DESTDIR)$(PREFIX)/bin
	install        $(SHS)  $(DESTDIR)$(PREFIX)/bin
	mkdir -p               $(DESTDIR)$(MANPREFIX)/man/man1
	install -m 644 $(MANS) $(DESTDIR)$(MANPREFIX)/man/man1

homeinstall:
	make DESTDIR=~ PREFIX= MANPREFIX= install

dist:
	./dist.sh $(VERSION) $(DESTDIR)

