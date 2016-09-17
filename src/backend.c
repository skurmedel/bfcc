#include "backend.h"
#include <assert.h>

int C99_begin(backend *me, FILE *output);
int C99_emit(backend *me, FILE *output, token t);
int C99_end(backend *me, FILE *output);

#define C99_PTR_NAME		"p"
#define C99_STORAGE_NAME	"storage"

extern void c99_options_default(c99_options *opts)
{
	opts->stack_size = BFCC_DEFAULT_STACKSIZE;
}


backend create_c99_backend(c99_options *opts) 
{
	assert(opts != 0);

	backend b = {
		opts,
		&C99_begin,
		&C99_emit,
		&C99_end
	};
	return b;
}

int C99_begin(backend *me, FILE *output)
{
	c99_options *opts = (c99_options *) me->state; 
	
	fputs("#include <stdio.h>\n", output);
	fputs("#include <stdlib.h>\n", output);
	fputs("#include <string.h>\n", output);
	fputs("int main(int argc, char *argv[]) {\n", output);
	fprintf(output, "\tchar *%s = malloc(%d);\n", C99_STORAGE_NAME, opts->stack_size);
	fprintf(output, "\tif (%s == 0) { puts(\"E: Could not allocate storage.\"); return -1; }\n", C99_STORAGE_NAME);
	fprintf(output, "\tmemset(%s, '\\0', %d);\n", C99_STORAGE_NAME, opts->stack_size);
	fprintf(output, "\tchar *%s = %s;\n", C99_PTR_NAME, C99_STORAGE_NAME);
	return ferror(output);
}

int C99_emit(backend *me, FILE *output, token t)
{
#ifdef NDEBUG
	fprintf(output, "\t// token: %s\n", token_name(t));
#endif
	switch (t)
	{
	case token_incr_ptr:
		fprintf(output, "\t%s++;\n", C99_PTR_NAME);
		break;
	case token_decr_ptr:
		fprintf(output, "\t%s--;\n", C99_PTR_NAME);
		break;
	case token_incr:
		fprintf(output, "\t(*%s)+=1;\n", C99_PTR_NAME);
		break;
	case token_decr:
		fprintf(output, "\t(*%s)-=1;\n", C99_PTR_NAME);
		break;
	case token_output:
		fprintf(output, "\tputchar(*%s);\n", C99_PTR_NAME);
		break;
	case token_input:
		fprintf(output, "\t*%s = getchar();\n", C99_PTR_NAME);
		break;
	case token_jump_fwd:
		fprintf(output, "\twhile (*%s) {\n", C99_PTR_NAME);
		break;
	case token_jump_back:
		fputs("\t}\n", output);
		break;
	case token_eof:
		fputs("\t// End of Program.", output);
		break;
	}
	return ferror(output);
}

int C99_end(backend *me, FILE *output)
{
	fputs("\treturn 0;\n}\n", output);
	return ferror(output);
}
