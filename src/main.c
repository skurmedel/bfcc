#include <stdio.h>
#include <stdlib.h>

#include "tokeniser.h"

int main(int argc, char *argv[])
{
	FILE *f = stdin;
	if (argc > 1)
	{
		f = fopen(argv[1], "r");
		if (!f)
		{
			printf("Unknown file.\n");
			return 1;
		}
	}

	tokeniser *t = tokeniser_setup(f);

	int c = 0;
	while (1)
	{
		token tok;
		int error = tokeniser_next(t, &tok);

		if (error)
		{
			printf("Error detected: %d.\n", error);
			return 1;
		}

		printf("%s\n", token_name(tok));

		if (tok == token_eof)
			return 0;
	}	

	return 0;
}
