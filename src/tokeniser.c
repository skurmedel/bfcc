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

#include "tokeniser.h"
#include "stdlib.h"

typedef struct tokeniser_t
{
	FILE *f;
} tokeniser;

tokeniser *tokeniser_setup(FILE *f)
{
	tokeniser *t = malloc(sizeof(tokeniser));
	
	if (!f)
		return NULL;

	t->f = f;
	return t;
}

void tokeniser_free(tokeniser *t)
{
	free(t);
}

int tokeniser_next(tokeniser *t, token *tok)
{
	FILE *f = t->f;
	int err;
	token res;

	while (1)
	{
		int c = fgetc(f);
		err = ferror(f);

		if (err != 0)
			goto error;

		if (feof(f))
		{
			res = token_eof;
			goto success;
		}

		switch ((char) c)
		{
			case '+':
				res = token_incr;
				goto success;
			case '-':
				res = token_decr;
				goto success;
			case '>':
				res = token_incr_ptr;
				goto success;
			case '<':
				res = token_decr_ptr;
				goto success;
			case '.':
				res = token_output;
				goto success;
			case ',':
				res = token_input;
				goto success;
			case '[':
				res = token_jump_fwd;
				goto success;
			case ']':
				res = token_jump_back;
				goto success;
			default:
				continue;
		}
	}

success:
	*tok = res;
	return 0;

error:
	return err;
}

static struct token_name_mapping
{
	token t;
	char const *name;
} 
name_map[9] =
{
	{ token_incr_ptr, "incr_ptr" },
	{ token_decr_ptr, "decr_ptr" },
	{ token_incr, "incr" },
	{ token_decr, "decr" },
	{ token_output, "output" },
	{ token_input, "input" },
	{ token_jump_fwd, "jump_fwd" },
	{ token_jump_back, "jump_back" },
	{ token_eof, "eof" }
};

char const *token_name(token t)
{
	/* 
		Can crash here if someone gives a crap value. 

		Let's save them if so.
	*/
	if (((unsigned int) t) > token_eof)
	{
		return "INVALID_TOKEN_ID";
	}

	return name_map[t].name;
}