#!/bin/bash
#
# evenmoreutils' test script
#
# just run it, and it works as a "unit test" for the utils


# Assert function, usage:
#   assert command parameters <<REF
#   desired output
#   REF
assert() {
	diff -rud <($*) - ||
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
}


sgetopt_test_test

