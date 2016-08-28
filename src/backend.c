#include "backend.h"

int C99_begin(FILE *output);
int C99_emit(FILE *output, token t);
int C99_end(FILE *output);

#define C99_PTR_NAME		"p"
#define C99_STORAGE_NAME	"storage"

backend create_c99_backend() 
{
	backend b = {
		&C99_begin,
		&C99_emit,
		&C99_end
	};
	return b;
}

int C99_begin(FILE *output)
{
	fputs("#include <stdio.h>\nint main(int argc, char *argv[]) {\n", output);
	fprintf(output, "\tchar %s[%d] = {0};\n", C99_STORAGE_NAME, BFCC_STACKSIZE);
	fprintf(output, "\tchar *%s = %s;\n", C99_PTR_NAME, C99_STORAGE_NAME);
	return ferror(output);
}

int C99_emit(FILE *output, token t)
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

int C99_end(FILE *output)
{
	fputs("\treturn 0;\n}\n", output);
	return ferror(output);
}
