evenmoreutils - a collection of command line tools
==================================================

This is evenmoreutils, a collection of command line tools inspired by
moreutils.  Both complement the standard \*NIX toolset (ls, grep, less etc).
The tools are:

* anywait: like wait, but works on any pid from the system
* easy-getopt: an easier verision of the getopt command line tool
* fit:     truncates a stream by fitting it into a certain width
* hilite:  runs a command, highligh stderr
* hl:      highlight some expressions on a stream
* mime:    prints the mimetype of a file (file -bi)
* nup:     n-up an output from a program (making 
* p:       runs parallel processes tagging or highlighting their output
* pad:     pads a stream with whitespaces to fit a certain width
* randpar: output a random parameter
* untl:    runs a program until it is sucessful (whle)
* sgetopt: simple getopt version (not a command, a lib)

They are currently *experimental* tools.  There are some known bugs (check the
manpages of each command for details).  Code is quick-and-dirty on lots of
them.


Installation
------------

Check INSTALL for instructions.


Links
-----

Joey's moreutils:  http://kitenet.net/~joey/code/moreutils/
GNU coreutils:     https://www.gnu.org/software/coreutils/
evenmoreutils:     http://matela.com.br/evenmoreutils/

