# TiTi - Tiny Terminfo

This is an absolutely minimal termcap / terminfo library using a single
hardcoded database to support only one terminal type.

It was written to support an emscripten build of [zsh](https://www.zsh.org/)
targetting only [xterm.js](https://xtermjs.org/) as the terminal. However, it
could also be useful for other minimal situations or cases where you want to
avoid building the whole of ncurses.

## Building

A basic `Makefile` is included and running `make` should be sufficient on most
platforms. This builds a static library. To build the library with emscripten,
use:

    emmake make CFLAGS="-O1"

When building software against libtiti, you may need to override some configure
variables, e.g.

    ./configure ac_cv_func_setupterm="no" ac_cv_func_initscr=no --with-term-lib=titi

## Content

The library does not extend to the curses interfaces so will not be usable for
any programs that use these. The empty curses.h include file may help if
configure scripts look for it.

Included is a zsh script named `mkdb` which can be used to generate a C include
file containing terminal definitions for `$TERM`. The output is sorted by the
termcap key so the termcap interfaces can use a binary search while the
terminfo interfaces search sequentially. Swapping this or creating a sorted
index would be a trivial option if needed.

The included file is based on definitions for xterm-256color but changed to
reflect actual behaviour of xterm.js.

For emscripten, [xterm-pty](https://xterm-pty.netlify.app/) is needed to monkey
patch further terminal interfaces such as `ioctl()` calls.

## Tests

Some tests of `tparm()` can be run with `make test`. To run the same tests
against the system libraries, use `make check`.
