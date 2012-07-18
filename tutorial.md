--
title: namgen documentation: tutorial
--

## First steps ##

Assuming you have build namgen let's take it for a spin:

    $ cd examples/trivial/ && find -type f
    ./src/build.rules
    ./src/test1.c
    ./src/test2.cpp
    ./src/test2b.cpp
    $ cat src/build.rules
    program "test1" {
       src = *.c
    }

    program "test2" {
      src = test2.cpp test2b.cpp
    }

I assume this needs no explaining; so i'll write about something else:

- build.rules should be in the same dir as the code; aka the path
  exprs generally *should* be simple filenames. this isn't a hard
  requirement, but strongly advisable.

- all files (of one target) *must* have the same extension;
  e.g. "src = test2.cpp test2b.cxx" is not supported.

- the top-level directory (the CWD where you run namgen) should contain
  neither source code nor a build.rules descriptor; if you try running
  in the 'src' dir you will not have a working makefile setup.
  this is another limitation; currently namgen does not even detect and
  warn about this situation, which it really should :-(

Anyway -- onwards to adventure:

    $ ../../namgen
    Resolving module dependencies
    Loaded: 2 program rules and 0 library rules in 1 modules
    $ find -type f
    ./install               # copy files during "make install"
    ./makefile              # the 'master-makefile'
    ./makefile.dirs         # lists all sub-dir makefiles for inclusion
    ./src/build.rules
    ./src/makefile          # ~jumps~ to the master-makefile
    ./src/makefile.ng       # the actual rules generated from build.rules
    ./src/test1.c
    ./src/test2.cpp
    ./src/test2b.cpp

The number of generated files might seem like overkill in this scenario;
we even need one more, as there *must* always be a toplevel config - for
now an empty file will do, in real life you will probably generate this
from a configure script:

    $ touch make.conf
    $ make
    [mkdep]  src/test2b.cpp
    [mkdep]  src/test2.cpp
    [mkdep]  src/test1.c
    [cc]     src/test1.o
    [link]   src/test1
    [c++]    src/test2.o
    [c++]    src/test2b.o
    [link]   src/test2
    $ file src/test1
    src/test1: ELF 64-bit LSB executable, x86-64, [...snip...], not stripped
    $ make -n install
    echo "[install]   /bin/test1" ; ./install src/test1 /bin
    echo "[install]   /bin/test2" ; ./install src/test2 /bin

As you can seen running "make install" without any configuration is likely
not what you want; let's fix that:

    $ echo "DESTDIR := out" >make.conf
    $ mkdir out
    $ make install
    [install]   out/bin/test1
    [install]   out/bin/test2
    $ make clean
    rm -f src/test1 src/test2
    (cd src && rm -rf .libs *.o *.d *.la *.lo)

The default target is to build everything, you can change that in
the top-level makefile. This file is only created when it does not
exist, so you can safely edit it; however writing your own rules to
yet another file and including it is safer with regards to possible
updates of the master-makefile template.

Realistically speaking you will need to read at least the main makefile,
so you might as well start toying with this example.
As a closing teaser the following seems to suffice for a minimal cross-
compilation setup:

    $ cat make.conf
    WIN32   := 1
    CC      := i686-w64-mingw32-gcc
    CXX     := i686-w64-mingw32-g++
    $ make
    [...snip...]
    $ file src/test1.exe 
    src/test1.exe: PE32 executable (console) Intel 80386, for MS Windows


> See you next time, code cowboy
