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
#include <string.h>

#include "tokeniser.h"
#include "backend.h"
#include "arguments.h"

#define ERROR_MEMORY_ALLOC		-1
#define ERROR_FILE_NOT_FOUND	-2
#define ERROR_TOKENISATION		-3
#define ERROR_CODE_GEN			-4

#define IS_ERROR(code) ((code) != 0)
#define FATAL_IF_ERROR(code, info) if (IS_ERROR((code))) { \
		printf("Fatal error (%s).\n", (info));\
		return ERROR_CODE_GEN;\
	}
#define CHECK_ALLOCATION(ptr, info) if (ptr == NULL) { \
		printf("Memory allocation failed (%s).\n", (info));\
		return ERROR_MEMORY_ALLOC;\
	}

int main(int argc, char *argv[])
{
	int ecode;

	bfcc_options bfopts = {0};
	if (parse_arguments(argc, argv, &bfopts) != 0)
	{
		return -2;
	}

	FILE *f = stdin;
	if (bfopts.input_file != 0)
	{
		f = fopen(bfopts.input_file, "r");
		if (!f)
		{
			fprintf(stderr, "Unknown file.\n");
			return ERROR_FILE_NOT_FOUND;
		}
	}

	c99_options opts;
	c99_options_default(&opts);
	
	backend back = create_c99_backend(&opts);

	ecode = back.begin(&back, stdout);
	FATAL_IF_ERROR(ecode, "Backend preamble generation");

	tokeniser *t = tokeniser_setup(f);
	CHECK_ALLOCATION(t, "Tokeniser setup");

	while (1)
	{
		token tok;
		int error = tokeniser_next(t, &tok);

		if (IS_ERROR(error))
		{
			fprintf(stderr,  "Tokenisation error detected: %d.\n", error);
			return ERROR_TOKENISATION;
		}
		if (tok == token_eof)
			break;
		
		if (IS_ERROR(back.emit(&back, stdout, (token) tok)))
		{
			fprintf(stderr, "Failure encountered when translating token: %s\n", token_name((token) tok));
		}
	}

	ecode = back.end(&back, stdout);
	FATAL_IF_ERROR(ecode, "Backend could not finish")
	return 0;
}
