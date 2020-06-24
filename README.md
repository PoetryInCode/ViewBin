# ViewBin

This really is just a slightly prettier version of hexdump. There isn't much to say about it.

![demo](./demo.gif)

To view the flag/help screen run with `-h` or `--help`

## Building

### Linux/MacOS

You can clone the repository and just run `make` inside the cloned directory. No dependencies, only a compiler and Gnu Make. It uses the default compiler
on the system, but you can specify a compiler by changing the value of `cc` in the [Makefile](./Makefile) on the first line

Run `make install` to install it on the system this will require root

Run `make uninstall` to remove it from the system this will also require root.

### Windows

The Makefile may, or may not work on windows, I'm not sure.
If it doesn't there are no dependencies. You can just run it through a compiler.
