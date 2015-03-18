#include <sgetopt.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	static int flags, nsecs = -1;

	static struct soption opttable[] = {
		{ 'n', NULL, 0, capture_presence, &flags },
		{ 't', NULL, 1, capture_int,      &nsecs },
		{ 0,   0,    0, 0,                0 }
	};

	if (sgetopt(argc, argv, opttable, argv+1, 0)) {
		fprintf(stderr,"Usage: %s [-t nsecs] [-n] name\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	printf("flags=%d; nsecs=%d\n", flags, nsecs);

	if (argv[1] == NULL) {
		fprintf(stderr, "Expected argument after options\n");
		exit(EXIT_FAILURE);
	}

	printf("name argument = %s\n", argv[1]);

	/* Other code ommitted */

	exit(EXIT_SUCCESS);
}
