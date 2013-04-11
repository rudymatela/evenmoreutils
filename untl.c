/*
 * untl.c - runs a program until it is sucessful
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
 * Runs a command until it returns true (default interval 1 second).
 */
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <libgen.h>


/* wtf... */
typedef __pid_t pid_t;


int execvpfw(const char *path, char *const argv[])
{
	/* TODO: Refactor this, output errors on stdout, etc... */
	int r;
	pid_t pid = fork();
	if (pid) {
		waitpid(pid, &r, 0);
		return WEXITSTATUS(r);
	} else {
		execvp(path, argv);
		fprintf(stderr, "Errno: %i\n", errno);
		exit(errno?errno:-1);
	}
}


int main(int argc, char **argv)
{
	/* whle waits for 0 status, everything else for everything else */
	int status = strcmp(basename(argv[0]), "whle") != 0;
	if (argc <= 1) {
		fprintf(stderr, "%s: error: no command provided\n", argv[0]);
		return 1;
	}
	while (!!execvpfw(argv[1], argv+1) == status) {
		sleep(1);
	}
	return 0;
}

