bfcc
====

bfcc is a brainfuck compiler. It compiles "ISO" brainfuck to a number of other targets. Currently only C99.

Future ideas
------------
It would be nice to be able to create libraries through C, so we can invoke 
compiled brainfuck from Haskell. 

Usage
-----

    bfcc "filename.bf" > myprog.c
    gcc myprog.c -o myprog

You can also call bfcc without a filename, which turns it into a REPL translator,
for example:

    $ bin/bfcc
    #include <stdio.h>
    int main(int argc, char *argv[]) {
            char storage[32768] = {0};
            char *p = storage;
    ++
            // token: incr
            (*p)+=1;
            // token: incr
            (*p)+=1;
    -
            // token: decr
            (*p)-=1;

To finish the output in REPL you must send the end of input signal 
(usually CTRL+D on *nix.)

Problems
--------
- The array is fixed to 32768 bytes.
- There is no bounds checking, please don't use bfcc for your modern jetfighter
  avionics.

How to build
------------
Run `make`, note that MSys or a Linux environment is needed for this Makefile.

It can be compiled manually in any case, just compile and link the files together.
