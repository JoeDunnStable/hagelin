INTRODUCTION

    This is the Hagelin project, a suite of simulators for cipher
    machines based on Boris Hagelin's designs. Well, it's actually
    just one simulator for the M-209 Converter, but I plan to add more
    simulators someday.

    The m209 simulator is a command-line program which is compatible
    with real M-209 simulators.


SYSTEM REQUIREMENTS

    The hagelin project is developed primarily on a Mac OS X system,
    using the Gnu toolchain as installed by MacPorts. It should build
    with little or no pain in any fairly recent UNIX-like environment,
    such as Linux.

    It can probably be built on Windows systems using the Cygwin
    environment, which is available from: http://www.cygwin.com

    It requires a C++ compiler such as g++, and a make utility such as
    Gnu make.

    It also requires the boost C++ libraries.

    Basically, if the provided configure script can't figure out how
    to build it, you're on your own! :-)

    Any functions which require random number generation (such as key
    generation and the automatic enciphering mode) will use the
    arc4random() function if it is available, or else will use the
    random() function. If random() is used, the generator will be
    seeded with srandomdev() if that function is available, or else
    with srandom(time()). In the last case, note that multiple runs
    less than a second apart may produce identical results! Thus, on
    systems which lack both arc4random() and srandomdev() (such as the
    cygwin environment under Windows), do not try to generate multiple
    key files without letting at least a second pass between each
    invocation of the m209 program.

GETTING THE CODE

    The hagelin project is hosted at GitLab:

    https://gitlab.com/NF6X_Crypto/hagelin

COMPILING

    ./configure
    make
    make check
    make install

DOCUMENTATION

    The included file "m209.1" is a Unix manual page. It is normally
    installed by the "make install" command. To read it without
    installing it, try this:

        nroff -man m209.1 | less

    The m209 program also has built-in help which may be accessed by
    running it with the -h flag, i.e.:

        ./m209 -h

    For more information about the M-209 simulator:

    	http://www.nf6x.net/crypto/crypto.html

        http://en.wikipedia.org/wiki/M-209

AUTHOR

    Mark J. Blair, NF6X <nf6x@nf6x.net>

COPYRIGHT

   Copyright (C) 2009-2013 Mark J. Blair, NF6X

   Hagelin is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
 
   Hagelin is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with Hagelin.  If not, see <http://www.gnu.org/licenses/>.

