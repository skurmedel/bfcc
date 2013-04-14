#include <stdio.h>
#include <stdlib.h>

#include "tokeniser.h"

int main(int argc, char *argv[])
{
	tokeniser *t = tokeniser_setup(stdin);

	int c = 0;
	while (c++ < 5)
	{
		token tok;
		int error = tokeniser_next(t, &tok);

		if (error)
		{
			printf("Error detected: %d.\n", error);
			return 1;
		}

		printf("%s\n", token_name(tok));
	}	

	return 0;
}
