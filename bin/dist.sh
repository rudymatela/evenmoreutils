#!/bin/bash
#
# dist.sh - evenmoreutils' package generator
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

version=$1
destdir=$2

if [ -z "$version" ]; then
	echo "Error: no version supplied" > /dev/stderr
	exit 1
fi

if [ -z "$destdir" ]; then
	destdir=..
fi

dest=$destdir/evenmoreutils-${version}

if [ -e "${dest}" ]; then
	echo "Error: ${dest} exists"
	exit 1
fi

if [ -e "${dest}.tar.xz" ]; then
	echo "Error: ${dest}.tar.xz exists"
	exit 1
fi

mkdir -p ${dest}

# Copies every file that is not hidden
cp -r * ${dest}

# Set versions -- TODO: make this undependant of :development version: string (match whole line)
cat lib/version.h               | sed -e "s/:development version:/$version/" > ${dest}/lib/version.h
cat src/evenmoreutils-common.sh | sed -e "s/:development version:/$version/" > ${dest}/src/evenmoreutils-common.sh

# Removes .md from README and INSTALL if necessary
test -f ${dest}/README.md &&  mv ${dest}/README.md  ${dest}/README
test -f ${dest}/INSTALL.md && mv ${dest}/INSTALL.md ${dest}/INSTALL

# Cleanup -- tarbalize -- remove tempdir
make -C ${dest} clean
tar -cJf ${dest}.tar.xz ${dest}
tar -czf ${dest}.tar.gz ${dest}
rm -r ${dest}

echo "Written ${dest}.tar.xz and ${dest}.tar.gz.  Don't forget to test them."

