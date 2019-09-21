INTRODUCTION

The M209 is a cipher machine developed by Boris Hagelin and put into
production by the US Army during World War II. The m209 simulator is a 
command-line program, which is compatible with the original hardware.
It's descendant of Mark Blair's simulator, which is part of his Hagelin project.

SYSTEM REQUIREMENTS

The hagelin project was developed primarily on a Mac OS X system,
using the Gnu toolchain as installed by MacPorts. This version was also
developed on Mac OS X but it uses the meson build system with the ninja
backend.  It should build with little or no pain on any platform with a
standard compliant C++ compiler with the STL and a few of the Boost libraries.

Any functions which require random number generation (such as key
generation and the automatic enciphering mode) will use the
std::random_device in the <random>.  If this doesn't work on your
edit the config.h.in file in the root directory to use a different generator.

GETTING THE CODE

This version of the hagelin project is hosted at:

    https://github.com/JoeDunnStable/hagelin

Mark Blair's hagelin project is hosted at GitLab:

    https://gitlab.com/NF6X_Crypto/hagelin

COMPILING

The following steps will build the system, starting in the source directory

    meson build
    cd build
    ninja
    
The package includes a boost test executable, which can be invoked as 
follows:

    ninja check
    
The program was developed an OS X using Apple's clang++ compiler and it's
now been ported to the following:

* Window using the msvc compiler, 
* g++ and clang++ using mingw64, and
* g++ and clang++ on Ubuntu linux.

DOCUMENTATION

The included file "m209.1" in the doc directory is a Unix manual page. It is normally
installed by the "ninja install" command. To read it without
installing it, try this:

    cd doc
    nroff -man m209.1 | less

The m209 program also has built-in help which may be accessed by
running it with the -h flag, i.e.:

    ./m209 -h

For more information about the M-209 simulator:

    http://www.nf6x.net/crypto/crypto.html

    http://en.wikipedia.org/wiki/M-209

AUTHORS

Joseph Dunn <JoeDunn144@gmail.com>

Mark J. Blair, NF6X <nf6x@nf6x.net>

COPYRIGHT

Copyright (C) 2019 Joseph Dunn

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

