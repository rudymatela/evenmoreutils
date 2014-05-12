Installaation Instructions
**************************

Copyright (C) 2013  Rudy Matela

   Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.  This file is offered as-is,
without warranty of any kind.


Basic Installation
==================

Crash course: `make && make install'

Check install target on the Makefile for details.

Anyway, it is better to install using your operating system's package
manager:rpm, dpkg, pacman etc. If there is no package you can make one:

* dpkg:   http://wiki.debian.org/HowToPackageForDebian
          https://wiki.ubuntu.com/PackagingGuide/Complete
* rpm:    http://fedoraproject.org/wiki/How_to_create_an_RPM_package
* pacman: https://wiki.archlinux.org/index.php/Creating_packages

Submit them to your Operating System community for publication in their
official repositories.


Installing on home directory
============================

Crash course:
make DESTDIR=~ PREFIX= MANPREFIX= install
echo 'export PATH=~/bin:${PATH}' >> .bashrc
echo 'MANDATORY_MANPATH ~/man'   >> .manpath
logout && login

You can easily install this software on your home directory by issuing
`make DESTDIR=~ PREFIX= MANPREFIX= install'.  This will install binaries on
`~/bin' and manuals on `~/man'. You can add `~/bin' to your $PATH variable and
configure `~/man' as a mandatory path on `~/.manpath'.  NOTE: A shorthand for
`make DESTDIR=~ PREFIX= MANPREFIX= install' is `make homeinstall'


Installation Options
====================

Crash course for packagers: `make && make install DESTDIR=/alt/dir'

Options
* DESTDIR: Destination dir for the package
* CFLAGS: Flags for the C Compiler
* PREFIX: Prefix to be appended to DESTDIR
* MANPREFIX: Prefix appended to DESTDIR for manual pages, defl: $(PREFIX)/share

Check the Makefile for default options.


Where is ./configure?
=====================

Stop the autoconf insanity!
http://freecode.com/articles/stop-the-autoconf-insanity-why-we-need-a-new-build-system

Pure make rules. Autoconf sucks. If you don't agree, you're wrong!

