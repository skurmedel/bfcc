#include "arguments.h"
#include "parg.h"
#include <stdio.h>

int parse_arguments(int argc, char *argv[], bfcc_options *result)
{
    struct parg_state ps;
    int c;
    char const *optstring = "h";

    parg_init(&ps);
    int optend = parg_reorder(argc, argv, optstring, NULL);

    while ((c = parg_getopt(&ps, argc, argv, optstring)) != -1)
    {
        switch (c)
        {
            case 1:
                // We deal with these later.
                break;
            case 'h':
                puts("Usage bfcc [-h] [input file]");
                return ERROR_ARGUMENTS_INFO_OUTPUT;
            default:
                fprintf(stderr, "Unhandled option -%c.\n", c);
                return ERROR_ARGUMENTS_WRONG;
        }
    }
    
    if (optend < argc)
    {
        result->input_file = argv[optend];
    }
    else
    {
        result->input_file = 0;
    }

    return 0;
}
