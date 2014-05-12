/*
 * muni.c - some wrappers to POSIX standard functions
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
 */
#include "muni.h"
#include "version.h"


double difftimespec(struct timespec *buftime1, struct timespec *buftime0)
{
	return          buftime1->tv_sec  - buftime0->tv_sec +
	       (double)(buftime1->tv_nsec - buftime0->tv_nsec) / 1000000000.;
}


double stat_age(const char *path, char type)
{
	struct timespec file_time;
	struct timespec curr_time;
	if (stat_time(path, &file_time, type) || clock_gettime(CLOCK_REALTIME, &curr_time))
		return 0./0.; /* Returning NaN on error */
	return difftimespec(&curr_time, &file_time);
}


int stat_time(const char *path, struct timespec *buf, char type)
{
	struct stat statbuf;
	int r;

	r = stat(path,&statbuf);
	if (r)
		return r;

	switch (type) {
	case 'a':
		(*buf) = statbuf.st_atim;
		break;

	case 'm':
		(*buf) = statbuf.st_mtim;
		break;

	case 'c':
		(*buf) = statbuf.st_ctim;
		break;
	}

	return r;
}


