/*
 * sgetopt_test.c - Test for the simple getopt
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


int custom_callback(const char *arg, void *pvar)
{
	printf("Custom callback with %s as parameter\n", arg);
	return 0;
}


int main(int argc, char **argv)
{
	static int help;
	static int version;
	static int show_numnonoptions;
	static int show_nonoptions;
	static int only_long;
	static int some_integer;
	static float some_float;
	static double some_double;
	static char *some_string;

	struct soption opttable[] = {
		{ 'h', "help",    0, capture_presence,    &help },
		{ 'v', "version", 0, capture_presence,    &version },
		{ 'n', 0,         0, capture_presence,    &show_numnonoptions },
		{ 'o', 0,         0, capture_presence,    &show_nonoptions },
		{ 0,   "long",    0, capture_presence,    &only_long },
		{ 'i', "integer", 1, capture_int,         &some_integer },
		{ 'f', "float",   1, capture_float,       &some_float },
		{ 'd', "double",  1, capture_double,      &some_double },
		{ 's', "string",  1, capture_charpointer, &some_string },
		{ 'c', "custom",  1, custom_callback,     0 },
		{ 0,   0,         0, 0,                   0 }
	};

	int i;

	/* After this call, argv+1 will point to all non-options */
	if (sgetopt(argc, argv, opttable, argv+1, 0)) {
		printf("Error parsing one of the command line options\n");
		return 1;
	}

	if (help) {
		printf("Usage: check source for details\n");
		return 0;
	}
	if (version) {
		printf("2013 version\n");
		return 0;
	}

	if (show_numnonoptions)
		printf("%i\n", ppcharlen(argv+1));

	if (show_nonoptions)
		for (i=1; argv[i]; i++)
			printf("%s\n", argv[i]);

	printf("integer = %i, string = %s, float = %.1f, "
	       "double = %.1lf, bool(long) = %i\n",
	       some_integer,
	       some_string,
	       some_float,
	       some_double,
	       only_long);
	return 0;
}

