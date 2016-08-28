#ifndef BACKEND_H
#define BACKEND_H

#include <stdio.h>

#include "tokeniser.h"

#define BFCC_STACKSIZE	(32 * 1024)
#define BFCC_OUTPUT		stdout

// Todo: This doesn't support state.
typedef struct backend_t
{
	/*
		Initializes the output stream with whatever data
		is needed.

		Does not need to be thread safe.

		Should return 0 on success.
	*/
	int (*begin)(FILE *output);
	/*
		Emits the necessary opcodes for the token t.

		Does not need to be thread safe.

		Should return 0 on success.
	*/
	int (*emit)(FILE *output, token t);
	/*
		Called after all the tokens have been emitted.

		Does not need to be thread safe.

		Should return 0 on success.
	*/
	int (*end)(FILE *output);
} backend;

/*
	Creates a backend that generates C99 code and writes it to the 
	output "executable". The C code can	then be compiled by, for 
	example GCC and executed.
*/
extern backend create_c99_backend();

#endif