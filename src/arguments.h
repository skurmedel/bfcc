/*
	Copyright (c) 2013, Simon Otter
	All rights reserved.
	
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:
	    * Redistributions of source code must retain the above copyright
	      notice, this list of conditions and the following disclaimer.
	    * Redistributions in binary form must reproduce the above copyright
	      notice, this list of conditions and the following disclaimer in the
	      documentation and/or other materials provided with the distribution.
	
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL SIMON OTTER BE LIABLE FOR ANY
	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include "errors.h"

typedef struct bfcc_options_t
{
    /*
        A brainfuck source file. If none were given, this value is 0.
    */
    char *input_file;
} bfcc_options;

/*
    Tries to parse the command line options, and return the output.

    If some options are wrong or incompatible, a message is printed
    to stderr and this function returns BFCC_ERROR_ARGUMENTS_WRONG.

    If the help or version text was requested, this returns 
    BFCC_ERROR_ARGUMENTS_INFO_OUTPUT.

    In both the above cases, result will be untouched.

    If the arguments were well defined this function returns 0.
*/
int parse_arguments(int argc, char *argv[], bfcc_options *result);

#endif

