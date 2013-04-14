#ifndef TOKENISER_H
#define TOKENISER_H

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

#include <stdio.h>

typedef struct tokeniser_t
{
	FILE *f;
} tokeniser;

typedef enum token_t
{
	token_incr_ptr,
	token_decr_ptr,
	token_incr,
	token_decr,
	token_output,
	token_input,
	token_jump_fwd,
	token_jump_back,
	token_eof
} token;

tokeniser *tokeniser_setup(FILE *);
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

char const *token_name(token);

#endif
