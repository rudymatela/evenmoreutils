/*
 * untl.c - runs a program until it is sucessful
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
 *
 *
 * Runs a command until it returns true (default interval 1 second).
 */
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <libgen.h>
#include <time.h>
#include <math.h>
#include "sgetopt.h"
#include "version.h"


/* wtf... */
typedef __pid_t pid_t;


int execvpfw(const char *path, char *const argv[])
{
	/* TODO: Refactor this, output errors on stdout, etc... */
	int r;
	pid_t pid = fork();
	if (pid) {
		waitpid(pid, &r, 0);
		return WEXITSTATUS(r);
	} else {
		execvp(path, argv);
		fprintf(stderr, "Errno: %i\n", errno);
		exit(errno?errno:-1);
	}
}


/* sleeps for seconds seconds, returns the remaining number of seconds if call
 * was interrupted more or less like nanosleep */
int fsleep(double seconds)
{
	/* I've once read in a book that multiples of 1 are not magic numbers */
	struct timespec time, remaining;
	time.tv_sec = floor(seconds);
	time.tv_nsec = (seconds - floor(seconds)) * 1000000000.;
	if (nanosleep(&time, &remaining)) {
		return (double)remaining.tv_sec + (double)remaining.tv_nsec / 1000000000.;
	} else {
		return 0.0;
	}
}


int main(int argc, char **argv)
{
	static int help;
	static int version;
	static double interval = 1.;
	static int limit = 0; /* 0 for unlimited */

	int i;

	struct soption opttable[] = {
		{ 'h', "help",     0, capture_presence,    &help },
		{ 'v', "version",  0, capture_presence,    &version },
		{ 'i', "interval", 1, capture_double,      &interval },
		{ 'l', "limit",    1, capture_int,         &limit },
		{ 0,   0,          0, 0,                   0 }
	};

	if (sgetopt(argc, argv, opttable, argv+1, 1)) {
		printf("Error parsing one of the command line options\n");
		return 1;
	}

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

	/* whle waits for 0 status, everything else for everything else */
	int status = strcmp(basename(argv[0]), "whle") != 0;
	if (argc <= 1) {
		fprintf(stderr, "%s: error: no command provided\n", argv[0]);
		return 1;
	}
	for (i = 0; (!limit || i < limit) && !!execvpfw(argv[1], argv+1) == status; i ++) {
		fsleep(interval);
	}
	return 0;
}

