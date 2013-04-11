/*
 * hilite - runs a command, highlighting everything it sends to stderr
 * version 1.5
 *
 * Copyright (C) 2000, 2001  Mike Schiraldi <mgs21@columbia.edu>
 *
 * See www.sf.net/forum/forum.php?forum_id=104071 for news and info
 *
 * Or just www.sf.net/projects/hilite if the above link is no good
 *
 */

/*
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
 * along with this program, but Mike's a lazy bastard. To get one,
 * write to the Free Software Foundation, Inc., 59 Temple Place - Suite
 * 330, Boston, MA 02111-1307, USA.
 */

/*
 * Fixes:
 * hilite.c:104:7: error: implicit declaration of function ‘fileno’ [-Werror=implicit-function-declaration]
 *
 * Source: http://stackoverflow.com/questions/9427145/how-do-i-remove-the-following-implicit-declaration-of-function-warnings
 */
#define _POSIX_C_SOURCE 1

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

#define HEADER "\033[91m"
#define FOOTER "\033[0m"

#define FAIL(msg) { \
	fprintf (stderr, "%s: " msg "() failed: %s\n", argv[0], strerror (errno)); \
	return 1; \
}

int main(int argc, char **argv)
{
	int p[2];
	int f;

	if (argc < 2) {
		fprintf(stderr, "%s: specify a command to execute\n", argv[0]);
		return 1;
	}

	if (pipe(p) != 0)
		FAIL("pipe");

	f = fork();

	if (f == -1)
		FAIL("fork");

	if (f) {
		int status;

		close(p[1]);

		again:
		errno = 0;

		while (1) {
			int r;
			char buf[BUFSIZ];

			r = read(p[0], buf, BUFSIZ - 1);

			if (r <= 0)
				break;

			buf[r] = 0;
			fprintf(stderr, "%s%s%s", HEADER, buf, FOOTER);
		}

		if (errno == EINTR) {
			fprintf(stderr, "%s: read interrupted, trying again\n",
					argv[0]);
			goto again;
		}

		if (errno != 0)
			FAIL("read");

		if (wait(&status) != f)
			FAIL("wait");

		return WEXITSTATUS(status);
	} else {
		int fd;

		close(p[0]);
		close(fileno(stderr));

		fd = dup(p[1]);			/* dup() uses the lowest available fd, which should be stderr's 
								 * since we just closed it */

		/* Can't use stderr for these problems, since we just closed it */
		if (fd < 0) {
			printf("%s: dup() failed: %s\n", argv[0], strerror(errno));
			return 1;
		}

		if (fd != fileno(stderr)) {
			printf("%s: dup returned %d instead of %d\n", argv[0], fd,
				   fileno(stderr));
			return 1;
		}

		execvp(argv[1], &argv[1]);

		FAIL("exec");
	}
}
