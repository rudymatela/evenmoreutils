#!/bin/bash
#
# hl - evenmoreutils common shell utilities
#
# Copyright (C) 2012, 2013  Rudy Matela
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
# This program is not intended to be called directly but instead it is intended
# to be sourced from the several evenmoreutils utilities

BASENAME="`basename $0`"

VERSION=":development version:"

VERSION_DISCLAIMER="$BASENAME (evenmoreutils) $VERSION
\nCopyright (C) 2012-2013  Rudy Matela
\nThis is free software; see the source for copying conditions.  There is NO
\nwarranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
\n"

show_version() {
	echo -ne $VERSION_DISCLAIMER
	exit 0
}

show_help() {
	exec man $BASENAME
}
