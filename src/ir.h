#ifndef IR_H
#define IR_H

#include "tokeniser.h"

typedef struct instruction_t 
{
    enum op_t
    {
        op_incr_ptr,
        op_decr_ptr,
        op_incr,
        op_decr,
        op_output,
        op_input,
        op_jmpbifz,
        op_jmpfifz
    } op;
    size_t arg;
} instruction;

typedef struct source_t
{
    token *tokens;
    size_t count;
} source;

typedef struct program_t 
{
    instruction *instructions;
    size_t count;
} program;

/*

*/
int ir_source_read_all(source *, tokeniser *);

/*
    
*/
void ir_free_source(source *);

/*
    Assembles the source (tokens) into intermediate assembly (program).

    Preconditions:

        the source is a non-zero array of tokens.
    
    Returns:

        0
            on success
        BFCC_ERROR_MEMORY_ALLOC
            Could not allocate the output program (likely too big.)
        BFCC_ERROR_INVALID_PROGRAM
            Some kind of source problem, like an unmatched "[" or "]".
    
    The program needs to be freed with ir_free_program when obsolete.
*/
int ir_assemble_source(source const *, program *);

/*
    Frees the space used by the program. Should be called on programs initialized
    with ir_assemble_source.
*/
void ir_free_program(program *);

#endif