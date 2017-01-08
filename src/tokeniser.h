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

#ifndef TOKENISER_H
#define TOKENISER_H

#include <stdio.h>

#ifndef NULL
	#define NULL 0
#endif

typedef struct tokeniser_t tokeniser;

typedef enum token_t
{
	/* > */
	token_incr_ptr,
	/* < */
	token_decr_ptr,
	/* + */
	token_incr,
	/* - */
	token_decr,
	/* . */
	token_output,
	/* , */
	token_input,
	/* [ */
	token_jump_fwd,
	/* ] */
	token_jump_back,
	token_eof
} token;

/*
	Represents a reader function used by the tokeniser.

	Puts the read character into c. On EOF, c should be set to EOF.

	It should return a ferror compatible error code. End of input is marked with
	an EOF.
*/
typedef int (*tokeniser_reader)(int *c, void *reader_state);

/*
	Creates a new tokeniser that reads from the given stream.

	This method returns NULL on memory allocation errors.
*/
tokeniser *tokeniser_setup_with_stream(FILE *);

/*
	Creates a new tokeniser that gets character input from the given function.

	Will be called repeatedly with the given reader_state until EOF is returned.

	This method returns NULL on memory allocation errors.
*/
tokeniser *tokeniser_setup(tokeniser_reader, void *);

/*
	Frees the tokeniser allocated with tokeniser_setup.
*/
void tokeniser_free(tokeniser *);

/*
	Reads the next token.

	If EOF, token_eof is returned, otherwise it eats any 
	unknown characters until a legal token is found, which 
	is returned.

	Returns:

		On stream errors, returns whatever ferror() returns,
		in any other case, 0.
*/
int tokeniser_next(tokeniser *, token *);

/*
	Returns the name of a token. If the token is outside the range of the enum
	it returns INVALID_TOKEN_ID.

	Example:

		puts(token_name(token_incr_ptr)) // outputs "incr_ptr"

*/
char const *token_name(token);

#endif
