evenmoreutils - a collection of command line tools
==================================================

This is evenmoreutils, a collection of command line tools inspired by
[moreutils].  Both complement the standard \*NIX toolset (ls, grep, less
etc).  The name is a reference to the GNU [coreutils].  The tools are:

* **[age]**:     prints file age in seconds / checks if older/newer than seconds
* **[anywait]**: like [wait], but works on any process in the system
* **[ched]**:    cached run of a command
* **[fit]**:     truncates a stream by fitting it into a certain width
* **[hilite]**:  runs a command, highlight stderr
* **[hl]**:      highlight some expressions on a stream
* **[mime]**:    prints the mimetype of a file (alias for ``file -bi``)
* **[nup]**:     n-up an output from a program
* **[p]**:       runs parallel processes tagging or highlighting their output
* **[pad]**:     pads a stream with whitespaces to fit a certain width
* **[randpar]**: output a random parameter
* **[repeat]**:  repeats a command, indefinitely or *n* times
* **[untl]**:    runs a program until it is sucessful
* **[whle]**:    runs a program until it fails
* **[sgetopt]**:   an easier-to-use version of the getopt library
* **easy-getopt**: an easier-to-use version of the getopt command line tool

They are currently *experimental* tools.  There are some known bugs (check the
manpages of each command for details).  Code is quick-and-dirty on several of
them.  Patches are welcome :-)


Installation
------------

Check [INSTALL] for detailed instructions and different installation options.

Crash course for installing from source:

	wget https://github.com/rudymatela/evenmoreutils/releases/download/vx.y.z/evenmoreutils-x.y.z.tar.gz
	tar -xzvf /evenmoreutils-x.y.z.tar.gz
	cd evenmoreutils-x.y.z
	make && make install


License
-------

This is Free Software, licensed under the GPLv2 unless stated otherwise in
specific files.


[moreutils]: http://kitenet.net/~joey/code/moreutils/
[coreutils]: https://www.gnu.org/software/coreutils/

[INSTALL]: INSTALL.md
[age]:     man/age.1.adoc
[anywait]: man/anywait.1.adoc
[ched]:    man/ched.1.adoc
[fit]:     man/fit.1.adoc
[hilite]:  man/hilite.1.adoc
[hl]:      man/hl.1.adoc
[mime]:    man/mime.1.adoc
[nup]:     man/nup.1.adoc
[p]:       man/p.1.adoc
[pad]:     man/pad.1.adoc
[randpar]: man/randpar.1.adoc
[repeat]:  man/untl.1.adoc
[untl]:    man/untl.1.adoc
[whle]:    man/untl.1.adoc
[sgetopt]: man/sgetopt.1.adoc

[wait]:    http://man.cx/wait

