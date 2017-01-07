#ifndef VM_H
#define VM_H

/*
    Defines the interface for the bfcc VM.

    It is a very simple register VM with just 8 instructions and 1 register.

    The registers are:

        DP              the data pointer register, valid range of the DP is 
                        determined by the data_memory_size passed to the VM 
                        during construction. 
        PC              program counter, holds the index of the next instruction
                        to be processed. Can only be modified through jumps.
    
    The instruction table follows.

    name        arg.    descr
    ------------------------------------------------------------------------
    incr_ptr    steps   increment the DP by <steps> 
                        segfault if DP+<steps> is outside [0, data_memory_size]
    decr_ptr    steps   decrement the DP by <steps>
                        segfault if DP-<steps> is outside [0, data_memory_size]
    op_incr     value   increment the value at DP by <value>. unsigned.
    op_decr     value   decrement the value at DP by <value>. unsigned.
    op_output   n/a     output the byte at DP
    op_input    n/a     read a byte into DP
    op_jmpbifz  len     if value at DP is 0, jump back <len> instructions, 
                        segfault if this is past instruction 0.
    op_jmpfifz  len     if value at DP is 0, jump forward <len> instructions, 
                        segfault if this is past the last instruction.
*/

#include "errors.h"
#include <stdint.h>
#include "ir.h"

typedef struct vm_t vm;
/*
    Input and output functions for the VM. They should behave like putc and getc.
*/
typedef int (*vm_write_uint8_t)(uint8_t c);
typedef int (*vm_read_uint8_t)(void);


/*
    Creates a new VM.

    Returns:

        NULL
            memory error, data_memory_size < 1,
            data_memory_size > SIZE_MAX/sizeof(uint8_t)
        a pointer to a VM object
*/
vm *vm_setup(size_t data_memory_size);

/*
    Sets the input/output functions.

    Returns:

        Always 0, no attempt to validate the functions is made.
    
*/
int vm_set_io(vm *m, vm_read_uint8_t, vm_write_uint8_t);

/*
    Sets the active program for the VM.

    Preconditions:

        The vm pointer points to data allocted through vm_setup.

    Returns:

        0
            Success
        BFCC_ERROR_MEMORY_ALLOC
        BFCC_ERROR_INVALID_PROGRAM
            The program is of zero length or is somehow malformed.
        
*/
int vm_set_program(vm *, program const *);

/*
    Executes the next available instruction if there is one, or does nothing if 
    the program reached its end.

    Preconditions:

        The vm pointer points to data allocted through vm_setup.
        A successful call to vm_set_program has been made.
    
    Returns:

        0
            Success
        BFCC_ERROR_PROGRAM_TERMINATED
            The program has halted.
        BFCC_ERROR_PROGRAM_SEGFAULT
            The program accessed invalid memory.
        BFCC_ERROR_INVALID_PROGRAM
            vm_set_program hasn't been called with a valid program.
        
*/
int vm_step(vm *);

/*
    Gets a const pointer to the data memory of the machine. The size_t argument 
    is set to the size of the memory.

    Preconditions:

        The vm pointer points to data allocted through vm_setup.
    
    Returns:

        null if the vm pointer is null, otherwise a pointer to the data memory.
    
*/
uint8_t const *vm_data_memory_pointer(vm *, size_t *);

/*
    Frees the VM instance.

    Preconditions:

        The vm pointer points to data allocted through vm_setup.
*/
void vm_free(vm *);

#endif