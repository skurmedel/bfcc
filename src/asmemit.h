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

#ifndef ASMEMIT_H
#define ASMEMIT_H

#include <stdio.h>
#include "tokeniser.h"

typedef enum 
{
	asm_ecode_success,
	/*
		A preliminary null check failed.
	*/
	asm_ecode_null_parameter,
	/*
		The particular platform and/or ASM-syntax is 
		unsupported.
	*/
	asm_ecode_unsupported_platform,
	/*
		The file stream used for output breaks down	somehow.
	*/
	asm_ecode_stream_error
} asm_ecode;

typedef struct asm_error_t
{
	/*
		Non-zero indicates error.
	*/
	asm_ecode code;
	char const *msg;
} asm_error;

typedef enum 
{ 
	asm_linux64,
	asm_windows64
} asm_platform;

typedef enum 
{
	asm_syntax_gas
} asm_syntax;

typedef token symbol;

/*
	Fetches the next IL opcode.
*/
typedef symbol (*symbol_fetcher)(void *);

typedef struct asm_config_t
{
	asm_platform platform;
	asm_syntax syntax;
	FILE *f;

	symbol_fetcher fetch_symbol;
	void *userdata;
} asm_config;

/*
	Emits assembler to the file descriptor specified
	in the config.

	Returns

		A structure of asm_error with asm_error.code set
		to asm_ecode_success on succesful ASM-emission.

		Otherwise ams_error.msg and code will describe the 
		error.
*/
asm_error asm_emit(asm_config *);

#endif
