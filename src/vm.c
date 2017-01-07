#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm.h"

static int default_get()
{
    return fgetc(stdin);
}

static int defalt_put(uint8_t c)
{
    return fputc(c, stdout);
}

struct vm_t 
{
    /*
        The program counter.
    */
    size_t pc;
    /*
        The current program.
    */
    program const *p;
    /*
        The "RAM".
    */
    size_t   data_memory_size;
    uint8_t *data_memory;
    /*
        The brainfuck data pointer register.
    */
    size_t   dp;
    /*
        I/O functions.
    */
    vm_read_uint8_t  get;
    vm_write_uint8_t put;
};

vm *vm_setup(size_t data_memory_size)
{
    if (data_memory_size > SIZE_MAX/sizeof(uint8_t))
        return 0;
    if (data_memory_size < 1)
        return 0;
    vm *inst = malloc(sizeof(vm));
    if (inst == 0)
        return 0;
    inst->pc = 0;
    inst->p = 0;
    inst->data_memory_size = data_memory_size;
    inst->data_memory = malloc(sizeof(uint8_t) * data_memory_size);
    inst->dp = 0;
    if (inst->data_memory == 0)
    {
        free(inst);
        return 0;
    }
    /*
        In brainfuck it is assumed every memory cell is zero at the start.
    */
    memset(inst->data_memory, '\0', inst->data_memory_size);
    inst->get = default_get;
    inst->put = defalt_put;
    return inst;
}

int vm_set_io(vm *m, vm_read_uint8_t reader, vm_write_uint8_t writer)
{
    m->get = reader;
    m->put = writer;
    return 0;
}

int vm_set_program(vm *m, program const *p)
{
    if (p->count < 1 || p->instructions == 0)
        return BFCC_ERROR_INVALID_PROGRAM;
    /*
        TODO: Do more checking here.
    */
    m->p = p;
    /*
        If the program is changed mid execution, we need to 
        reset the program counter.
    */
    m->pc = 0;
    return 0;
}

int vm_step(vm *m)
{
    if (m->p == 0)
        return BFCC_ERROR_INVALID_PROGRAM;
    if (m->pc >= m->p->count)
        return BFCC_ERROR_PROGRAM_TERMINATED;

    instruction i = m->p->instructions[m->pc];
    
    int jumped = 0;

    switch (i.op)
    {
    case op_incr_ptr:
        if ((m->data_memory_size - m->dp - 1) < i.arg)
            return BFCC_ERROR_PROGRAM_SEGFAULT;
        m->dp += i.arg;
        break;
    case op_decr_ptr:
        if (i.arg > m->dp)
            return BFCC_ERROR_PROGRAM_SEGFAULT;
        m->dp -= i.arg;
        break;
    case op_incr:
        m->data_memory[m->dp] += i.arg;
        break;
    case op_decr:
        m->data_memory[m->dp] -= i.arg;
        break;
    case op_output:
        /* Todo: error checking */
        m->put(m->data_memory[m->dp]);
        break;
    case op_input:
        /* Todo: error checking */
        m->data_memory[m->dp]= m->get();
        break;
    case op_jmpfifz:
        if ((m->p->count - m->pc - 1) < i.arg)
            return BFCC_ERROR_PROGRAM_SEGFAULT;
        if (m->data_memory[m->dp] == 0)
        {
            m->pc += i.arg;
            jumped = 1;
        }
        break;
    case op_jmpbifz:
        if (i.arg > m->pc)
            return BFCC_ERROR_PROGRAM_SEGFAULT;
        if (m->data_memory[m->dp] == 0)
        {
            m->pc -= i.arg;
            jumped = 1;
        }
        break;
    default:
        return BFCC_ERROR_INVALID_PROGRAM;  
    }

    /*
        Only increment if we didn't jump, in which case we already are at the
        next instruction.
    */
    if (!jumped)
        m->pc++;
    return 0;
}

uint8_t const *vm_data_memory_pointer(vm *m, size_t *sz)
{
    if (m == 0)
        return 0;
    
    *sz = m->data_memory_size;
    return m->data_memory;
}

void vm_free(vm *m)
{
    if (m != 0)
    {
        free(m->data_memory);
        m->data_memory = 0;
    }
    free(m);
}