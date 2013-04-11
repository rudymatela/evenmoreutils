/*
 * sgetopt.c - Simple getopt version
 *
 * Copyright (C) 2013  Rudy Matela
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "sgetopt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define EPRINTFR(...) { fprintf(stderr,__VA_ARGS__); return 1; }


const static struct soption *sgetoptfromc(char c, const struct soption optable[]);
const static struct soption *sgetoptfroms(const char *s, const struct soption optable[]);


int sgetopt(int argc, char * const argv[], const struct soption optable[])
{
	int i, j, r;
	const struct soption *popt;
	for (i=1; i<argc; i++) {
		if (argv[i][0] != '-') { /* normal parameter */
			popt = sgetoptfromc(0, optable);
			if (popt->func)
				r = popt->func(argv[i], popt->arg);
		} else if (argv[i][1] == '-') { /* --long parameter */
			popt = sgetoptfroms(argv[i]+2, optable);
			if (!popt)
				EPRINTFR("Unknown option `%s'\n", argv[i]);
			if (popt->has_arg && i+1 >= argc)
				EPRINTFR("Missing argument to `%s'\n", argv[i]);
			r = popt->func(popt->has_arg?argv[++i]:NULL, popt->arg);
		} else for (j=1; argv[i][j]; j++) { /* -short parameters (-s -h -o -r -t) */
			popt = sgetoptfromc(argv[i][j], optable);
			if (!popt)
				EPRINTFR("Unknown option -%c\n", argv[i][j]);
			if (popt->has_arg && (i+1 >= argc || argv[i][j+1]))
				EPRINTFR("Missing argument to `-%c'\n", argv[i][j])
			r = popt->func(popt->has_arg?argv[++i]:NULL, popt->arg);
			if (r)
				return r;
			if (popt->has_arg)
				break;
		}
		if (r)
			return r;
	}
	return 0;
}


const static struct soption *sgetoptfromc(char c, const struct soption optable[])
{
	while (optable->sname || optable->lname) {
		if (optable->sname == c)
			return optable;
		optable++;
	}
	return c ? NULL: optable;
}


const static struct soption *sgetoptfroms(const char *s, const struct soption optable[])
{
	while (optable->sname || optable->lname) {
		if (optable->lname && strcmp(optable->lname,s) == 0)
			return optable;
		optable++;
	}
	return s ? NULL: optable;
}


/* TODO: Make a version of this that gives an error */
int capture_int(const char *carg, void *pvar)
{
	int *pi = pvar;
	*pi = atoi(carg);
	return 0;
}


int capture_charpointer(const char *carg, void *pvar)
{
	char **ppchar = pvar;
	*ppchar = (void*)carg;
	return 0;
}


int capture_presence(const char *carg, void *pvar)
{
	int *pi = pvar;
	*pi = 1;
	return 0;
}


int capture_nonoption(const char *carg, void *pvar)
{
    static int i = 0; /* Index on opts array */
    int r = 0; /* Return value */
    const char **opts = (const char**)pvar;

    if (opts) {
        if (!carg)
            r = i; /* Error: writing a null pointer on opts,
                      returning the number of collected
                      parameters */
        opts[i++] = carg;
    } else {
        /* when pvar is null, returns the number
         * of collected parameters and reset
         * the index */
        r = i;
        i = 0;
    }
    return r;
}


