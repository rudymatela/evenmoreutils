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
 *
 *
 * TODO: This program needs error treating to be fixed (not checking if
 * functions that should fail are failing).
 */
#include "sgetopt.h"
#include "version.h"
#include "muni.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>

#include <errno.h>

#include <sys/types.h>
#include <bsd/md5.h>

#define BUFSIZE 0x1000
#define DEFAULT_CACHE_TIMEOUT 10.0
#define CACHE_PARENT_PATH ".cache"
#define CACHE_PATH ".cache/ched"


/* pork -- pipe-fork -- Forks creating a pipe */
/* the parent process stdin is linked with child process stdout */
/* child's stdout --> parent's stdin */
int pork() {
	int fildes[2];
	int rpid;
	int err;

	err = pipe(fildes);
	if (err)
		return -1;

	rpid = fork();
	if (rpid < 0) {
		close(fildes[0]);
		close(fildes[1]);
		return -1;
	}

	if (rpid) { /* pop */
		close(fildes[1]);
		dup2(fildes[0], 0); /* stdin becomes reading end of pipe */
		close(fildes[0]);
	} else { /* kid */
		close(fildes[0]);
		dup2(fildes[1], 1); /* stdout becomes writing end of pipe */
		close(fildes[1]);
		setvbuf(stdout, NULL, _IONBF, 0); /* newly created stdout is unbuffered */
	}

	return rpid;
}


/* Consume stdin, writing to stdout and out */
/* Returns 0 on success, negative otherwise */
int tee_file(FILE *out) {
	char buffer[BUFSIZE];
	int nbytes;
	while ((nbytes = read(0, buffer, BUFSIZE)) > 0) {
		write(1, buffer, nbytes);
		fwrite(buffer, nbytes, 1, out);
	}
	return nbytes;
}


int tee_file_path(const char *path, int append)
{
	/* TODO: Check for errors here */
	FILE *pf = fopen(path, append ? "a" : "w");
	int r = tee_file(pf);
	fclose(pf);
	return r;
}


void MD5UpdateWithCwd(MD5_CTX* pcontext)
{
	char wd[BUFSIZE];
	if (getcwd(wd, BUFSIZE) == NULL)
		return; /* ignore dir if it's too big */
	MD5Update(pcontext, (const u_int8_t*)wd, strlen(wd));
}


/* buf must be of size MD5_DIGEST_STRING_LENGTH */
/* must free return value */
char *MD5Args(char **args, char *digest_buf, int include_cwd)
{
	int i;
	MD5_CTX context;
	MD5Init(&context);
	for (i=0; args[i]; i++)
		MD5Update(&context, (const u_int8_t*)args[i], strlen(args[i])+1);
	if (include_cwd)
		MD5UpdateWithCwd(&context);
	return MD5End(&context, digest_buf);
}


void cat_file(FILE *pf)
{
	char buffer[BUFSIZE];
	int nbytes;
	do {
		nbytes = fread(buffer, 1, BUFSIZE, pf);
		fwrite(buffer, nbytes, 1, stdout);
	} while (nbytes == BUFSIZE);
}


int cat_file_path(char *path)
{
	FILE *pf = fopen(path, "r");
	if (!pf)
		return -1;
	cat_file(pf);
	fclose(pf);
	return 0;
}


char *cache_path(char *digest)
{
	char *home = getenv("HOME");
	size_t lenhome = strlen(home),
	       lencpath = strlen("/" CACHE_PATH "/"),
           len = lenhome + lencpath + MD5_DIGEST_STRING_LENGTH;
	char *cache_path = malloc(len);
	memcpy(cache_path, home, lenhome);
	memcpy(cache_path+lenhome, "/" CACHE_PATH "/", lencpath);
	memcpy(cache_path+lenhome+lencpath, digest, MD5_DIGEST_STRING_LENGTH);
	return cache_path;
}


char *cache_custom_path(char *digest, char *dir)
{
	size_t lendir = strlen(dir),
	       len = lendir + 1 + MD5_DIGEST_STRING_LENGTH;
	char *cache_path = malloc(len);
	memcpy(cache_path, dir, lendir);
	cache_path[lendir] = '/';
	memcpy(cache_path+lendir+1, digest, MD5_DIGEST_STRING_LENGTH);
	return cache_path;
}


/* Flags no error when directory exists */
int mkdir_(char *path)
{
	int status = mkdir(path, S_IRWXU);
	return status != 0 && errno == EEXIST ? 0 : status;
}



int main(int argc, char **argv)
{
	/* Program options */
	static int help;
	static int version;
	static int ignore_wd;
	static double timeout = DEFAULT_CACHE_TIMEOUT;

	struct soption opttable[] = {
		{ 'h', "help",                0, capture_presence,    &help },
		{ 'v', "version",             0, capture_presence,    &version },
		{ 'i', "ignore-working-dir",  0, capture_presence,    &ignore_wd },
		{ 't', "timeout",             1, capture_double,      &timeout },
		{ 0,   0,                     0, capture_nonoption,   0 }
	};

	int i;
	/* For now storing on current dir, to ease implementation */
	/* on final version store on user-wide cache, this might be useful because
	 * of running ched on directories with no write permissions */
	/* also easy to cleanup */
	char digest[MD5_DIGEST_STRING_LENGTH];
	char *cachefile;

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

	MD5Args(nargv,digest,!ignore_wd);
	cachefile = cache_path(digest);
	
	mkdir_(CACHE_PARENT_PATH);
	mkdir_(CACHE_PATH);

	if (stat_age(cachefile,'m') < timeout) { /* age of cache < timeout */
		cat_file_path(cachefile);
	} else {                           /* no cache, run and tee */
		if (pork()) { /* pop */
			tee_file_path(cachefile, 0);
		} else { /* kid */
			free(cachefile);
			execvp(nargv[0],nargv);
			fprintf(stderr,"%s: error, unable to run command `%s",basename(argv[0]),nargv[0]);
			for (i=1; nargv[i]; i++)
				fprintf(stderr," %s",argv[i]);
			fprintf(stderr,"'\n");
			return 1;
		}
	}
	free(cachefile);
	return 0;
}


