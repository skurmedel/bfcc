#ifndef BACKEND_H
#define BACKEND_H

#include <stdio.h>

#include "tokeniser.h"

#define BFCC_DEFAULT_BUFFERSIZE		(32 * 1024)
#define BFCC_C99_OUTPUT				stdout

// Todo: This doesn't support state, we really want that in the future.
typedef struct backend_t
{
	/*
		An implementation specific piece of data that can hold settings,
		state, and what not.
	*/
	void *state;

	/*
		Initializes the output stream with whatever data
		is needed.

		Does not need to be thread safe.

		Should return 0 on success.
	*/
	int (*begin)(struct backend_t *me, FILE *output);
	/*
		Emits the necessary opcodes for the token t.

		Does not need to be thread safe.

		Should return 0 on success.
	*/
	int (*emit)(struct backend_t *me, FILE *output, token t);
	/*
		Called after all the tokens have been emitted.

		Does not need to be thread safe.

		Should return 0 on success.
	*/
	int (*end)(struct backend_t *me, FILE *output);
} backend;

/*
	A structure that holds various options for the C99 backend.
*/
typedef struct c99_options_t 
{
	size_t 		buffer_size;
} c99_options;

/*
	Sets the default options for a C99 backend.
*/
extern void c99_options_default(c99_options *);

/*
	Creates a backend that generates C99 code and writes it to the 
	output "executable". The C code can	then be compiled by, for 
	example GCC and executed.
*/
extern backend create_c99_backend(c99_options *);

#endif