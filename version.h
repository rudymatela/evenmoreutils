/*
 * version.h - Version file for evenmoreutils
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
#ifndef _VERSION_H
#define _VERSION_H

#define VERSION ":development version:"

#define VERSION_DISCLAIMER \
"%s (evenmoreutils) " VERSION "\n" \
"Copyright (C) 2012-2013  Rudy Matela\n" \
"This is free software; see the source for copying conditions.  There is NO\n" \
"warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"

#define print_version(PNAME) \
	printf(VERSION_DISCLAIMER, PNAME)

#endif /* _VERSION_H */
