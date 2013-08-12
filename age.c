/*
 * age.c - determines file age
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
#include <sys/stat.h>


int main(int argc, char **argv)
{
	/* Program options */
	static int help;
	static int version;
	
	struct stat statbuf;

	struct soption opttable[] = {
		{ 'h', "help",    0, capture_presence,    &help },
		{ 'v', "version", 0, capture_presence,    &version },
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

	if (stat(argv[1],&statbuf)!=0) {
		char *progname = basename(argv[0]);
		fprintf(stderr,"%s: error, unable to retrieve `%s' attributes\n", progname, argv[1]);
		return 1;
	}
	
	printf("%li.%09li\n", statbuf.st_mtim.tv_sec, statbuf.st_mtim.tv_nsec);

	return 0;
}

