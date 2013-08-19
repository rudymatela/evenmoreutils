/*
 * sgetopt.h - Simple getopt version
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
#ifndef _SGETOPT_H
#define _SGETOPT_H

struct soption {
	char sname;
	char *lname;
	int has_arg;
	int (*func)(const char*,void*); /* must return 0 on success */
	void *arg;
};


/* Caution: they only work on static opttable */
#define lengthof(X) (sizeof(X) / sizeof (X)[0])
#define sgetopt_getlastarg(T) (T[lengthof(T)-1].arg)
#define sgetopt_setlastarg(T,A) (sgetopt_getlastarg(T) = (A))
#define sgetopt_nnonopts(T) capture_nonoption(0, sgetopt_getlastarg(T))

/* Use with caution, works ok with: int, char, float, double, long */
#define declare_fixed_capture(fname, type, value) \
	int fname(const char *carg, void *pvar) \
	{ \
		type *p = pvar; \
		if (!p) \
			return 1; \
		*p = value; \
		return 0; \
	}


/* Main function */
int sgetopt(int argc, char * const argv[], const struct soption optable[], char *normal_args[], int stop_at_nonoption);


/* Capture functions */
int capture_int(const char *carg, void *pvar);
int capture_int_pedantic(const char *carg, void *pvar);
int capture_charpointer(const char *carg, void *pvar);
int capture_presence(const char *carg, void *pvar);
int capture_presence_as_0(const char *carg, void *pvar);
int capture_nonoption(const char *carg, void *pvar);
int capture_float(const char *carg, void *pvar);
int capture_double(const char *carg, void *pvar);


/* Auxiliar functions */
int ppcharlen(char * const ppchar[]);

#endif
