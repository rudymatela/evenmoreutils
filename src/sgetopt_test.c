/*
 * sgetopt_test.c - Test for the simple getopt
 *
 * Copyright (C) 2013-2015  Rudy Matela
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
	static int show_nonoption_arguments;
	static int some_integer;
	static char *some_string;

	struct soption opttable[] = {
		{ 'h', "help",    0, capture_presence,    &help },
		{ 'o', NULL,      0, capture_presence,    &show_nonoption_arguments },
		{ 'i', "integer", 1, capture_int,         &some_integer },
		{ 's', "string",  1, capture_charpointer, &some_string },
		{ 'c', "custom",  1, custom_callback,     0 },
		{ 0,   0,         0, 0,                   0 }
	};

	int i;

	/* After this call, argv+1 will point to all non-option arguments */
	if (sgetopt(argc, argv, opttable, argv+1, 0)) {
		printf("Error parsing one of the command line options\n");
		return 1;
	}

	if (help) {
		printf("Usage: check source for details\n");
		return 0;
	}

	if (show_nonoption_arguments)
		for (i=1; argv[i]; i++)
			printf("%s\n", argv[i]);

	printf("integer = %i, string = %s\n", some_integer, some_string);
	return 0;
}
