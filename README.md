bfcc
====

bfcc is a brainfuck compiler. It compiles "ISO" brainfuck to a number of other targets. Currently only C.

Usage
-----

    bfcc "filename.bf" > myprog.c
    gcc myprog.c -o myprog

How to build
------------
Run `make`, note that MSys or a Linux environment is needed for this Makefile.

It can be compiled manually in any case, just compile and link the files together.
