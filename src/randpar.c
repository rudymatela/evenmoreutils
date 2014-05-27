/*
 * randpar.c - Outputs a random parameter
 *
 * Copyright (C) 2013  Rudy Matela
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "sgetopt.h"
#include "version.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <libgen.h>


static declare_fixed_capture(capture_nullchar, char, '\0');


void shuffle(int argc, char **argv)
{
	/* Fisher-Yates/Knuth Shuffle */
	int i,j;
	char *temp;
	for (i=0; i<argc; i++) {
		j = rand() % (argc-i) + i;
		temp    = argv[i];
		argv[i] = argv[j];
		argv[j] = temp;
	}
}


int main(int argc, char **argv)
{
	/* Program options */
	static int n = 1;
	static int all;
	static int help;
	static int version;
	static int repeat;
	static int seed = -1;

	int i;
	int delimiter = '\n';

	struct soption opttable[] = {
		{ 'n', 0,         1, capture_int,         &n },
		{ '0', "print0",  0, capture_nullchar,    &delimiter },
		{ 'd', "delimiter",1,capture_char,        &delimiter },
		{ 'a', "all",     0, capture_presence,    &all },
		{ 'h', "help",    0, capture_presence,    &help },
		{ 'v', "version", 0, capture_presence,    &version },
		{ 'r', "repeat",  0, capture_presence,    &repeat },
		{ 's', "seed",    1, capture_int,         &seed },
		{ 0,   0,         0, capture_nonoption,   0 }
	};

	/* After the call to getopt will point to an array of all nonoptions */
	char **nargv = argv + 1;

	sgetopt_setlastarg(opttable, nargv);
	if (sgetopt(argc, argv, opttable, NULL, 0)) {
		char *progname = basename(argv[0]);
		fprintf(stderr,"%s: error parsing one of the command line options\n", progname);
		return 1;
	}
	argc = sgetopt_nnonopts(opttable);

	srand(seed < 0 ? time(0) ^ getpid(): seed);

	if (help) {
		char *progname = basename(argv[0]);
		printf("Usage: %s [options] parameters...\n"
		       "  check source or manpage `man %s' for details.\n", progname, progname);
		return 0;
	}

	if (version) {
		print_version(basename(argv[0]));
		return 0;
	}

	if (all) {
		n = argc;
		repeat = 0;
	}

	if (n > argc) {
		repeat = 1;
	}

	if (repeat) {
		if (argc==0) {
			char *progname = basename(argv[0]);
			fprintf(stderr,"%s: error: no parameters to choose from.\n", progname);
			return 1;
		}
		for (i=0; i<n; i++) {
			fputs(nargv[rand()%argc],stdout);
			putc(delimiter,stdout);
		}
	} else {
		shuffle(argc, nargv);
		for (i=0; i<n; i++) {
			fputs(nargv[i],stdout);
			putc(delimiter,stdout);
		}
	}
	return 0;
}

