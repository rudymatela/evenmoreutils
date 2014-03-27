/*
 * ched.c - cached run
 *
 * Copyright (C) 2013, 2014  Rudy Matela
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
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>

#include <sys/types.h>
#include <bsd/md5.h>


/* buf must be of size MD5_DIGEST_STRING_LENGTH */
char *MD5Args(char **args, char *buf)
{
	int i;
	MD5_CTX context;
	MD5Init(&context);
	for (i=0; args[i]; i++)
		MD5Update(&context, (const u_int8_t*)args[i], strlen(args[i])+1);
	return MD5End(&context, buf);
}


int main(int argc, char **argv)
{
	/* Program options */
	static int help;
	static int version;
	static int timeout;

	struct soption opttable[] = {
		{ 'h', "help",                0, capture_presence,    &help },
		{ 'v', "version",             0, capture_presence,    &version },
		{ 't', "timeout",             1, capture_int,         &timeout },
		{ 0,   0,                     0, capture_nonoption,   0 }
	};

	int i;
	/* For now storing on current dir, to ease implementation */
	/* on final version store on user-wide cache, this might be useful because
	 * of running ched on directories with no write permissions */
	/* also easy to cleanup */
	char cachefile[MD5_DIGEST_STRING_LENGTH+1] = ".";
	char *digest = cachefile+1;

	/* After the call to getopt will point to an array of all nonoptions */
	char **nargv = argv + 1;

	sgetopt_setlastarg(opttable, nargv);
	if (sgetopt(argc, argv, opttable, NULL, 1)) {
		fprintf(stderr,"%s: error parsing one of the command line options\n", basename(argv[0]));
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

	if (nargv[0]==NULL) {
		fprintf(stderr,"%s: error, no command given\n",basename(argv[0]));
		return 1;
	}

	MD5Args(nargv,digest);
	fprintf(stderr,"DEBUG: md5 of cache file: %s\n",cachefile);
	execvp(nargv[0],nargv);

	fprintf(stderr,"%s: error, unable to run command `%s",basename(argv[0]),nargv[0]);
	for (i=1; nargv[i]; i++)
		fprintf(stderr," %s",argv[i]);
	fprintf(stderr,"'\n");

	return 1;
}


