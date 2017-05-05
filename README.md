# Yices2

This distribution includes the source of Yices, documentation, tests,
and examples.

Yices 2 is developed by Bruno Dutertre, Dejan Jovanovic, and Ian Mason
at the Computer Science Laboratory, SRI International. To contact us,
report a bug, or to get more information about Yices, please visit our
[website](http://yices.csl.sri.com).


## Prerequisites

To build Yices from the source, you need:

- GCC version 4.0.x or newer (or clang 3.0 or newer)
- gperf version 3.0 or newer
- the GMP library version 4.1 or newer

+ other standard tools: make (gnumake is required), sed, etc.


To build the manual, you also need:

- a latex installation
- the latexmk tool

To build the on-line documentation, you need to install the Sphinx
python pacakge. The simplest method is:

```
sudo easy_install -U Sphinx
```

Sphinx 1.3.x or better is needed.


## Quick Instalation

Do this:

```
autoconf
./configure
make
sudo make install
```

This will install binaries and libraries in `/usr/local/`. You can
change the installation location by giving option `--prefix=...` to
the `./configure` script.

For more explanations, please check `doc/COMPILING`.

## Support for Non-Linear Arithmetic

Yices supports non-linear real and integer arithmetic, but this is not
enabled by default. If you want non-linear arithmetic, follow these
instructions:

1. Install SRI's library for polynomial manipulation. It's available
   on github (https://github.com/SRI-CSL/libpoly).

2. After you've installed libpoly, add option `--enable-mcsat` to
   the cofigure command. In details, type this in the toplevel
   Yices directory:
```
autoconf
./configure --enable-mcsat
make
sudo make install
```
3 You may need to provide `LDFLAGS/CPPFLAGS` if `./configure` fails to
  find the libpoly library. Other options may be useful too.  Try
  `./configure --help` to see what's there.


## Windows Builds

We recommend compiling using Cygwin. If you want a version that works
natively on Windows (i.e., does not depend on the Cygwin DLLs), you
can compile from Cygwin using the MinGW cross-compilers. This is
explained in doc/COMPILING.


## Documentation

To build the manual from the source, type
```
make doc
```
This will build `./doc/manual/manual.pdf`.

Other documentation is in the `./doc` directory:

- `doc/COMPILING` explains the compilation process and options in detail.
- `doc/NOTES` gives an overview of the source code.
- `doc/YICES-LANGUAGE` explains the syntax of the Yices language, and
  describes commands, functions, and heuristic parameters.

To build the Sphinx documentation:
```
cd doc/sphinx
make html
```

This will build the documentation in build/html (within directory
doc/sphinx). You can also do:
```
make epub
```
and you'll have the doc in `build/epub/Yices.epub`.
