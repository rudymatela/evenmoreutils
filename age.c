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
#include <libgen.h>
#include "muni.h"


static declare_fixed_capture(capture_a, char, 'a');
static declare_fixed_capture(capture_m, char, 'm');
static declare_fixed_capture(capture_c, char, 'c');


int capture_duration(const char *carg, void *pvar)
{
	double *pf = pvar;
	char mult = 's'; /* multiplier: either s, m or h */
	int r = sscanf(carg, "%lf%c",pf,&mult);
	if (r != 1 && r != 2)
		return 1;
	switch (mult) {
	case 'h':
		*pf *= 60.;
	case 'm':
		*pf *= 60.;
	case 's':
		return 0;
	default: /* unknown multiplier */
		return 1;
	}
}


int main(int argc, char **argv)
{
	/* Program options */
	static int help;
	static int version;
	static double lower_bound = 0./0.; /* NaN */
	static double upper_bound = 0./0.;
	static char stat_type = 'm';

	double file_age;
	int check_upper,
	    check_lower;
	int i;

	struct soption opttable[] = {
		{ 'h', "help",    0, capture_presence,    &help },
		{ 'v', "version", 0, capture_presence,    &version },
		{ 'o', "older",   1, capture_duration,    &lower_bound },
		{ 'n', "newer",   1, capture_duration,    &upper_bound },
		{ 'a', "access",  0, capture_a,           &stat_type },
		{ 'm', "modify",  0, capture_m,           &stat_type },
		{ 'c', "change",  0, capture_c,           &stat_type },
		{ 0,   0,         0, capture_nonoption,   0 }
	};

	/* After the call to getopt will point to an array of all nonoptions */
	char **nargv = argv + 1;

	sgetopt_setlastarg(opttable, nargv);
	if (sgetopt(argc, argv, opttable, NULL, 0)) {
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

	check_upper = upper_bound == upper_bound; /* != NaN */
	check_lower = lower_bound == lower_bound; /* != NaN */

	for (i=1; i<=argc; i++) {
		file_age = stat_age(argv[i],stat_type);
		if (file_age != file_age /* NaN */) {
			fprintf(stderr,"%s: error, unable to retrieve `%s' attributes\n", basename(argv[0]), argv[i]);
			return 1;
		}

		if (check_upper || check_lower) {
			if ((check_upper && file_age > upper_bound) ||
				(check_lower && file_age < lower_bound))
				return 1;
		} else {
			printf("%lf\n", file_age);
		}
	}

	return 0;
}


