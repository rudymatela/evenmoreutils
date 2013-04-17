#!/bin/bash


assert() {
	$* |
	diff -rud - correct.txt ||
	(
		echo ERROR RUNNING: \`\`$*\'\'
		exit 1
	)
}


sgetopt_test_test() {
	cat > correct.txt <<END
integer = 0, string = (null), float = 0.0, double = 0.0, bool(long) = 0
END
	assert ./sgetopt_test

	cat > correct.txt <<END
integer = 0, string = (null), float = 0.0, double = 0.0, bool(long) = 0
END
	assert ./sgetopt_test -no
}


sgetopt_test_test


rm correct.txt
