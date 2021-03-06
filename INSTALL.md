Installation Instructions
=========================

Copyright (C) 2013  Rudy Matela

Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.  This file is offered as-is,
without warranty of any kind.


Basic Installation
------------------

Crash course: `make && make install`

Check install target on the Makefile for details.

Anyway, it is better to install using your operating system's package
manager:rpm, dpkg, pacman etc. If there is no package you can make one:

* [dpkg - debian](http://wiki.debian.org/HowToPackageForDebian)
* [dpkg - ubuntu](https://wiki.ubuntu.com/PackagingGuide/Complete)
* [rpm](http://fedoraproject.org/wiki/How_to_create_an_RPM_package)
* [pacman](https://wiki.archlinux.org/index.php/Creating_packages)

Submit them to your Operating System community for publication in their
official repositories.


Installing on home directory
----------------------------

To install on your home directory, run:

	make DESTDIR=~ PREFIX= MANPREFIX= install

This installs binaries on `~/bin` and manual pages on `~/man`.  To add these as
defaults for your user you need to add those to your executables and manual paths:

	echo 'export PATH=~/bin:${PATH}' >> .bashrc
	echo 'MANDATORY_MANPATH ~/man'   >> .manpath

You need to logout and login again for lines above to take effect.



Installation Options
--------------------

Crash course for packagers: `make && make install DESTDIR=/alt/dir`

Options
* `DESTDIR`: Destination dir for the package
* `CFLAGS`: Flags for the C Compiler
* `PREFIX`: Prefix to be appended to `DESTDIR`
* `MANPREFIX`: Prefix appended to `DESTDIR` for manual pages, defl: `$(PREFIX)/share`

Check the Makefile for default options.


Where is ./configure?
---------------------

[Read this](http://freecode.com/articles/stop-the-autoconf-insanity-why-we-need-a-new-build-system).

