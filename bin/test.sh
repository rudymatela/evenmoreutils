#!/bin/bash
#
# test.sh - evenmoreutils' automated test script
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
# This is an automated test script, just run it, and it works as a "unit test"
# for the utils.  If it replies with silence, everything is ok.


# Auxiliary function
stdin_md5() {
	md5sum | sed -e "s/ *-//"
}


# Assert function, usage:
#   assert command parameters <<REF
#   desired output
#   REF
assert() {
	diff -rud <($* 2>&1) - ||
	(
		echo ERROR RUNNING: \`\`$*\'\'
		exit 1
	)
}


# Same as above assert, but evals the parameters
assert_evl() {
	diff -rud <(eval $*) - ||
	(
		echo ERROR RUNNING: \`\`$*\'\'
		exit 1
	)
}


sgetopt_test_test() {
	assert ./sgetopt_test <<REF
integer = 0, string = (null), float = 0.0, double = 0.0, bool(long) = 0
REF

	assert ./sgetopt_test -no <<REF
0
integer = 0, string = (null), float = 0.0, double = 0.0, bool(long) = 0
REF

	assert ./sgetopt_test -no a b c <<REF
3
a
b
c
integer = 0, string = (null), float = 0.0, double = 0.0, bool(long) = 0
REF

	assert ./sgetopt_test -no wohoo <<REF
1
wohoo
integer = 0, string = (null), float = 0.0, double = 0.0, bool(long) = 0
REF

	assert ./sgetopt_test -i 10 <<REF
integer = 10, string = (null), float = 0.0, double = 0.0, bool(long) = 0
REF

	assert ./sgetopt_test -no --string abcd <<REF
0
integer = 0, string = abcd, float = 0.0, double = 0.0, bool(long) = 0
REF

	assert ./sgetopt_test -no a b c -f 123 --double 3.14 <<REF
3
a
b
c
integer = 0, string = (null), float = 123.0, double = 3.1, bool(long) = 0
REF

	assert ./sgetopt_test -no wohoo --long <<REF
1
wohoo
integer = 0, string = (null), float = 0.0, double = 0.0, bool(long) = 1
REF

	assert ./sgetopt_test -i asdf <<REF
Error parsing one of the command line options
REF

	assert ./sgetopt_test -iasdf <<REF
Error parsing one of the command line options
REF

	assert ./sgetopt_test -i <<REF
Missing argument to \`-i'
Error parsing one of the command line options
REF

	assert ./sgetopt_test --integer <<REF
Missing argument to \`--integer'
Error parsing one of the command line options
REF
}


randpar_test() {
	assert_evl ./randpar    1 2 3       \| wc <<<"      1       1       2"
	assert_evl ./randpar -a 1 2 3       \| wc <<<"      3       3       6"
	assert_evl ./randpar -a 1x2 3       \| wc <<<"      2       2       6"
	assert_evl ./randpar -a0 1x 3       \| wc <<<"      0       1       5"
	assert_evl ./randpar -n 10 abc de f \| wc -l <<<"10"
	assert_evl ./randpar `seq 1 100`    \| wc -l <<<"1"
	assert_evl ./randpar 1 2 3 -n0 4    \| wc -l <<<"0"
}


fit_test() {
	testfile=`mktemp evenmoreutils-test-XXXXXXXXXX`

	cat > $testfile <<REF
Well, this is a text file
with two lines.
REF

	assert fit -w 10 $testfile <<REF
Well, this
with two l
REF

	assert pad -w 30 $testfile <<REF
Well, this is a text file     
with two lines.               
REF

	assert fit -w 20 --pad $testfile <<REF
Well, this is a text
with two lines.     
REF

	rm $testfile
}


ched_test() {
	wd=`pwd`
	assert ched echo "blah" <<REF
blah
REF
	assert ched echo "blah" <<REF
blah
REF
	assert cat ~/.cache/ched/"`echo -ne "echo\\x00blah\\x00$wd" | stdin_md5`" <<REF
blah
REF
	assert ched echo -e "blah\nbleh" <<REF
blah
bleh
REF
	assert ched echo -e "blah\nbleh" <<REF
blah
bleh
REF
}


sgetopt_test_test
randpar_test
fit_test
ched_test
