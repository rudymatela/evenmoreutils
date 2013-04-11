#!/bin/bash
#
# p - runs parallel processes tagging their output
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
#
# Runs parallel processes tagging the output with the process name when switch
# -t is passed
# $ p -t "proc1 args" "proc2 args" "proc3 args"
#
# proc1 args: output of the process
# proc2 args: output of the process2
# proc1 args: output of the process
# proc3 args: output of the process3
# proc3 args: output of the process3
# proc1 args: output of the process
# proc2 args: output of the process2
#
# Ideal for:
# $ p -t "hg pull project1" "hg pull project2" "hg pull project3"
# hg pull project1: pulling changes from
# hg pull project2: pulling changes from
# hg pull project3: pulling changes from
# hg pull project1: pulling changes from
# etc...
#
#
# To "tail -f" multiple log files into one:
# $ p "tail -f /var/log/foo" "tail -f /var/log/goo" "tail -f /var/log/bar"
# woohooo
# log lines
#
# TODO: This has a lot of other features. Document them.

sigint() {
	kill $pids
	exit $?
}

color-code() {
    echo '
        black   30
        red     31
        green   32
        yellow  33
        blue    34
        magenta 35
        cyan    36
        white   37
	' | grep $1 | sed -e "s/.* //"
}

N="\x1b[0m"
B="\x1b[1m"
U="\x1b[4m"
R="\x1b[7m"

p() {
	pids=
	code=
	trap sigint SIGINT
	while [ "$#" -gt 0 ]; do
		case $1 in
			-*b*) code="$code$B" ;;&
			-*u*) code="$code$U" ;;&
			-*r*) code="$code$R" ;;&
			-*n*) code="$code$N" ;;&
			-*C) code="$code\x1b[$2m"; shift ;;
			-*c) code="$code\x1b[`color-code $2`m"; shift ;;
			-*t) code="$2: $code"; shift ;;
			-*) ;;
			*)
				if [ -n "$code" ]; then
					$1 | sed -e "s/^/$code/" -e "s/$/$N/" &
				else
					$1 &
				fi
				pids="$pids $!"
				code=
				;;
		esac
		shift
	done
	wait $pids
}

p "$@"

