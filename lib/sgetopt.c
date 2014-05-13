/*
 * sgetopt.c - Simple getopt version
 *
 * Copyright (C) 2013-2014  Rudy Matela
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
 *
 *
 * NOTE: This implementation is (hopefully) effective but a bit inefficient.
 * The parsing of arguments takes O(N*M) time, when it could be done in O(N)
 * time. N is the number of parameters and M is the number of options in the
 * option table.
 *
 * TODO: Maybe? Transform stop_at_nonoption on flags so other boolean options
 * can be passed.
 *
 * TODO: Maybe? Add switch (flag?) to supress error messages (so the user can
 * print his own).
 *
 * TODO: Maybe? Change all internal errors on sgetopt to negative values, have
 * them well described on defines and let the user know that the space of
 * positive values can be used in his own callbacks.  This can have also the
 * good side-effect of, when using negative values, using the lower part of the
 * int, as the position of which command line argument had problem?
 */
#include "sgetopt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define EPRINTFR(...) { fprintf(stderr,__VA_ARGS__); return 1; }


const static struct soption *sgetoptnormal(const struct soption optable[]);
const static struct soption *sgetoptfromc(char c, const struct soption optable[]);
const static struct soption *sgetoptfroms(const char *s, const struct soption optable[]);


int sgetopt(int argc, char * const argv[], const struct soption optable[], char *normal_args[], int stop_at_nonoption)
{
	int i, j, r;
	int end_options = 0; /* reached `--' parameter   or   first nonoption when stop_at_nonoption is true */
	int n_normal_args = 0;
	const struct soption *popt;
	for (i=1; i<argc; i++) {
		r = 0;
		/*  reached --  or not start with -  or one char "-" */
		if (end_options || argv[i][0] != '-' || argv[i][1] == 0) { /* normal parameter */
			if (stop_at_nonoption)
				end_options = 1;
			popt = sgetoptnormal(optable);
			if (popt->func)
				r = popt->func(argv[i], popt->arg);
			if (normal_args)
				normal_args[n_normal_args++] = argv[i];
		} else if (argv[i][1] == '-') { /* --long parameter */
			const char *option = argv[i]+2;
			const char *argument = NULL;
			if (option[0] == '\0') { /* `--', end of parsing */
				end_options = 1;
				continue;
			}
			popt = sgetoptfroms(option, optable);
			if (!popt)
				EPRINTFR("Unknown option `--%s'\n", option);
			if (popt->has_arg) {
				argument = strchr(option, '=');
				if (argument) { /* argument is --like=this */
					argument++; /* skips = */
				} else { /* argument should be --like this */
					if (i+1 >= argc)
						EPRINTFR("Missing argument to `--%s'\n", option);
					argument = argv[++i];
				}
			}
			r = popt->func(argument, popt->arg);
		} else for (j=1; argv[i][j]; j++) { /* -short parameters (-s -h -o -r -t) */
			char option = argv[i][j];
			popt = sgetoptfromc(option, optable);
			if (!popt)
				EPRINTFR("Unknown option -%c\n", option);
			if (popt->has_arg && !(argv[i][j+1] || i+1<argc))
				EPRINTFR("Missing argument to `-%c'\n", option)
			/* supports -oPARAM or -o PARAM (argv ternary) */
			r = popt->func(
				popt->has_arg ?
					argv[i][j+1] ? &argv[i][j+1] : argv[++i] :
					NULL,
				popt->arg
			);
			if (r)
				return r;
			if (popt->has_arg)
				break;
		}
		if (r)
			return r;
	}
	if (normal_args)
		normal_args[n_normal_args] = 0;
	return 0;
}


/* Returns the last option: behavior for normal parameter */
const static struct soption *sgetoptnormal(const struct soption optable[])
{
	while (optable->sname || optable->lname)
		optable++;
	return optable;
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


/* Compares two strings until reaching <delim> or end */
const static int strchrcmp(const char *s1, const char *s2, char delim)
{
	int r;
	while ((*s1 && *s1 != delim) || (*s2 && *s2 != delim))
		if ((r = *(s1++) - *(s2++)))
			return r;
	return 0;
}


const static struct soption *sgetoptfroms(const char *s, const struct soption optable[])
{
	while (optable->sname || optable->lname) {
		if (optable->lname && strchrcmp(optable->lname,s,'=') == 0)
			return optable;
		optable++;
	}
	return s ? NULL: optable;
}


int capture_int(const char *carg, void *pvar)
{
	int *pi = pvar;
	char *end;
	*pi = strtol(carg, &end, 10);
	/* Fails if it cannot find an int */
	return end!=carg ? 0: 1;
}


int capture_int_pedantic(const char *carg, void *pvar)
{
	int *pi = pvar;
	char *end;
	*pi = strtol(carg, &end, 10);
	/* Success only if anything but the int is on carg */
	return end!=carg && !*end ? 0: 1;
}


int capture_float(const char *carg, void *pvar)
{
	float *pf = pvar;
	return sscanf(carg, "%f",pf) == 1 ? 0 : 1;
}


int capture_double(const char *carg, void *pvar)
{
	double *pf = pvar;
	return sscanf(carg, "%lf",pf) == 1 ? 0 : 1;
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


int capture_presence_as_0(const char *carg, void *pvar)
{
	int *pi = pvar;
	*pi = 0;
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


int ppcharlen(char * const ppchar[])
{
	int i = -1;
	while (ppchar[++i]);
	return i;
}

