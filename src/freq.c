/*
 * frequency.c - determines frequency of file bytes
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
#include <string.h>
#include <ctype.h>
#include <libgen.h>


/* frequencies must be at least of size 256
 * returns NULL on error */
static int* get_frequencies(const char filename[], int frequencies[]);

static int* get_file_frequencies(FILE *file, int frequencies[]);


static int* get_frequencies(const char filename[], int frequencies[])
{
	FILE *f = fopen(filename, "r");
	if (!f)
		return NULL;
	get_file_frequencies(f, frequencies);
	fclose(f);
	return frequencies;
}


static int* get_file_frequencies(FILE *file, int frequencies[])
{
	int c;
	for (c=0; c<256; c++)
		frequencies[c]=0;
	while (c = getc(file), c != EOF)
		frequencies[c]++;
	return frequencies;
}


struct frequency {
	int n;
	int c;
};


static int freqcmp(struct frequency *a, struct frequency *b)
{
	return b->n - a->n
	     ? b->n - a->n
		 : a->c - b->c;
}

typedef int (*compar_fn_t) (const void *, const void *);

void frequencies_from_array(struct frequency sf[], const int af[])
{
	int c;
	for (c=0; c<256; c++) {
		sf[c].c = c;
		sf[c].n = af[c];
	}
	qsort(sf, 256, sizeof(*sf), (compar_fn_t) freqcmp);
}


static void print_frequencies(FILE *out, const int frequencies[])
{
	int i;
	struct frequency freq[256];
	frequencies_from_array(freq, frequencies);
	for (i=0; i<256; i++)  if (freq[i].n > 0)
		printf( isprint(freq[i].c) ?
		          " `%c': %d\n":
		          "0x%02x: %d\n",
		        freq[i].c,
		        freq[i].n);
}


static int print_file_frequencies(FILE *out, const char filename[])
{
	int frequencies[0x100];
	if (!get_frequencies(filename, frequencies))
		return 1; /* error reading file? */
	print_frequencies(out, frequencies);
	return 0; /* no error */
}


int main(int argc, char **argv)
{
	/* Program options */
	static int help;
	static int version;

	struct soption opttable[] = {
		{ 'h', "help",    0, capture_presence,    &help },
		{ 'v', "version", 0, capture_presence,    &version },
		{ 0,   0,         0, 0,                   0 }
	};

	if (sgetopt(argc, argv, opttable, argv+1, 0)) {
		fprintf(stderr,"%s: error parsing one of the command line options\n", basename(argv[0]));
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

	if (argc != 2) {
		fprintf(stderr,"%s: error, you have to pass just one file\n", basename(argv[0]));
		return 0;
	}

	print_file_frequencies(stdout, argv[1]);

	return 0;
}


