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

#ifndef ERRORS_H
#define ERRORS_H

#define BFCC_ERROR_MEMORY_ALLOC		-1
#define BFCC_ERROR_FILE_NOT_FOUND	-2
#define BFCC_ERROR_TOKENISATION		-3
#define BFCC_ERROR_CODE_GEN			-4

#define BFCC_ERROR_ARGUMENTS_WRONG       -100
#define BFCC_ERROR_ARGUMENTS_INFO_OUTPUT -101

#define BFCC_ERROR_INVALID_PROGRAM		-200
#define BFCC_ERROR_PROGRAM_TERMINATED	-201
#define BFCC_ERROR_PROGRAM_SEGFAULT		-202

#endif