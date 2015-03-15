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
#include <stdlib.h>

#include "tokeniser.h"
#include "backend.h"

int main(int argc, char *argv[])
{
	FILE *f = stdin;
	if (argc > 1)
	{
		f = fopen(argv[1], "r");
		if (!f)
		{
			fprintf(stderr, "Unknown file.\n");
			return 1;
		}
	}

	backend back = create_c99_backend();
	back.begin(stdout);

	tokeniser *t = tokeniser_setup(f);

	int c = 0;
	while (1)
	{
		token tok;
		int error = tokeniser_next(t, &tok);

		if (error)
		{
			fprintf(stderr,  "Error detected: %d.\n", error);
			return 1;
		}
		if (tok == token_eof)
			break;
		
		if (!back.emit(stdout, (token) tok))
		{
			fprintf(stderr, "Failure encountered when translating token: %s\n", token_name((token) tok));
		}
		//printf("%s\n", token_name(tok));
	}

	back.end(stdout);
	return 0;
}
