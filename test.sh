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
	reference=`mktemp -t evenmoreutils-test-XXXXXXXXXX`
	cat > $reference
	$* |
	diff -rud - $reference ||
	(
		echo ERROR RUNNING: \`\`$*\'\'
		exit 1
	)
	rm $reference
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
}


sgetopt_test_test

