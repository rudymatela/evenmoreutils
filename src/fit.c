/*
 * fit.c - truncates files or standard input to a certain width
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
 *
 *
 *
 * This command could be easily implemented with a shell one liner:
 * 
 * columns=$1
 * [ -z "$columns" ] && columns=$COLUMNS
 * [ -z "$columns" ] && columns=`tput cols`
 * [ -z "$columns" ] && columns=`stty size | sed -e 's/.* //'`
 * [ -z "$columns" ] && columns=80
 *
 * sed -ue "s/\t/        /" | sed -ue "s,\\(.\{$columns\}\\).*,\1,"
 *
 * The last line can also be:
 *
 * sed -ue "s/\t/        /" | cut -c{1-$columns}
 *
 *
 * It was, in fact, that way. It was changed to allow more flexible parameters.
 * And there are also some quirky details that is better to treat in a more
 * stable language than bash. It would take maybe half as long as this program
 * to treat everything in a more bug prone way. IE: Tab handling
 *
 * TODO: Get tab width from termcap??
 *
 * NOTE: This does not work with streams containing wide chars (that is, occupy
 * two bytes in memory but one slot in the screen). Example, unicode: á é í ó ú
 *
 * TODO: fit -- add support for "..." on suppressed end of lines
 */
#include "sgetopt.h"
#include "version.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define DEFAULT_TERM_WIDTH 80
#define DEFAULT_TAB_SIZE 8


int ctabs;
int width;
int tabsize = DEFAULT_TAB_SIZE;
int pad;
int wrap;


void putchars(int c, int q)
{
	while (q--)
		putchar(c);
}


int gettermwidth(void)
{
	int width = 0;
	char *columns = getenv("COLUMNS");
	if (columns)
		width = atoi(columns);
	if (!width) {
		struct winsize w;
		w.ws_col = 0;
		if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != -1)
			width = w.ws_col;
	}
	return width ? width : DEFAULT_TERM_WIDTH;
}


void fitfile(FILE *pf)
{
	/* TODO: Refactor this, ugly code */
	int ch, count = 0;
	int q;
	while ((ch = getc(pf)) != EOF) {
		switch (ch) {
		case '\n':
			if (pad && count < width)
				putchars(' ', width - count);
			putchar('\n');
			count = 0;
			break;
		case '\t':
			q = tabsize - count % tabsize;
			if (count < width) {
				if (ctabs)
					putchars(' ', width-count<q?width-count:q);
				else
					putchar(ch);
			} else if (wrap) {
				putchar('\n');
				count = 0;
				break;
			}
			count += q;
			break;
		default:
			if (count < width) {
				putchar(ch);
			} else if (wrap) {
				putchar('\n');
				ungetc(ch, pf);
				count = 0;
				break;
			}
			count ++;
			break;
		}
	}
}


int fitfiles(char *filenames[])
{
	int i;
	FILE *input;
	for (i=0; filenames[i]; i++) {
		if (strcmp(filenames[i], "-")==0) {
			fitfile(stdin);
			continue;
		}
		input = fopen(filenames[i],"r");
		if (!input) {
			fprintf(stderr, "error: could not open ``%s''\n", filenames[i]);
			continue;
		}
		fitfile(input);
		fclose(input);
	}
	return 0;
}


int main(int argc, char **argv)
{
	/* Program options */
	static int help;
	static int version;

	struct soption opttable[] = {
		{ 'w', "width",       1, capture_int,           &width },
		{ 't', "tabsize",     1, capture_int,           &tabsize },
		{ 'c', "converttabs", 0, capture_presence,      &ctabs },
		{ 'p', "pad",         0, capture_presence,      &pad },
		{ 'h', "help",        0, capture_presence,      &help },
		{ 'v', "version",     0, capture_presence,      &version },
		{ 'W', "wrap",        0, capture_presence,      &wrap },
		{ 'M', "no-wrap",     0, capture_presence_as_0, &wrap },
	/*	{ 'e', "ellipsis",    0, capture_presence,      &elipsis }, TODO */
		{ 0,   0,             0, capture_nonoption,   0 }
	};

	/* Configures the default behavior for when the program is pad */
	if (strcmp(basename(argv[0]), "pad") == 0) {
		wrap = 1;
		pad = 1;
	}

	width = gettermwidth();

	if (sgetopt(argc, argv, opttable, argv+1, 0)) {
		char *progname = basename(argv[0]);
		fprintf(stderr,"%s: error parsing one of the command line options\n", progname);
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

	if (argv[1])
		fitfiles(argv+1);
	else
		fitfile(stdin);
	return 0;
}

