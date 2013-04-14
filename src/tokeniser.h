/*
	copyright (c) 2013, simon otter
	all rights reserved.
	
	redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:
	    * redistributions of source code must retain the above copyright
	      notice, this list of conditions and the following disclaimer.
	    * redistributions in binary form must reproduce the above copyright
	      notice, this list of conditions and the following disclaimer in the
	      documentation and/or other materials provided with the distribution.
	
	this software is provided by the copyright holders and contributors "as is" and
	any express or implied warranties, including, but not limited to, the implied
	warranties of merchantability and fitness for a particular purpose are
	disclaimed. in no event shall simon otter be liable for any
	direct, indirect, incidental, special, exemplary, or consequential damages
	(including, but not limited to, procurement of substitute goods or services;
	loss of use, data, or profits; or business interruption) however caused and
	on any theory of liability, whether in contract, strict liability, or tort
	(including negligence or otherwise) arising in any way out of the use of this
	software, even if advised of the possibility of such damage.
*/

#ifndef TOKENISER_H
#define TOKENISER_H

#include <stdio.h>

#ifndef NULL
	#define NULL 0
#endif

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

/*
	Creates a new tokeniser.

	This method returns NULL on memory allocation errors.
*/
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
