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


static int stat_time(const char *path, struct timespec *buf, char type);
static int stat_age(const char *path, struct timespec *buf, char type);


int main(int argc, char **argv)
{
	/* Program options */
	static int help;
	static int version;
	
	struct timespec file_age;

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

	if (stat_age(argv[1],&file_age,'m')!=0) {
		char *progname = basename(argv[0]);
		fprintf(stderr,"%s: error, unable to retrieve `%s' attributes\n", progname, argv[1]);
		return 1;
	}

	printf("%li.%09li\n", file_age.tv_sec, file_age.tv_nsec);

	return 0;
}


static int stat_age(const char *path, struct timespec *buf, char type)
{
	struct timespec file_time;
	struct timespec curr_time;
	int r;
	r = stat_time(path, &file_time, type) || clock_gettime(CLOCK_REALTIME, &curr_time);
	if (r)
		return r;
	buf->tv_sec  = curr_time.tv_sec  - file_time.tv_sec;
	buf->tv_nsec = curr_time.tv_nsec - file_time.tv_nsec;
	if (buf->tv_nsec < 0) {
		buf->tv_nsec += 1000000000;
		buf->tv_sec --;
	}
	return r;
}


static int stat_time(const char *path, struct timespec *buf, char type)
{
	struct stat statbuf;
	int r;

	r = stat(path,&statbuf);
	if (r)
		return r;

	switch (type) {
	case 'a':
		(*buf) = statbuf.st_atim;
		break;

	case 'm':
		(*buf) = statbuf.st_mtim;
		break;

	case 'c':
		(*buf) = statbuf.st_ctim;
		break;
	}

	return r;
}


