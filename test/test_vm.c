#include "vm.h"
#include "unity.h"
#include <stdio.h>

#define VM_SETUP        vm *m = vm_setup(1024);
#define VM_TEARDOWN     vm_free(m);

void test_vm_setup_should_pass(void) 
{
    /*
        This should succeed on any PC made after 1995 with at least a shred
        of RAM available.
    */
    VM_SETUP;
    TEST_ASSERT_NOT_NULL(m);
    VM_TEARDOWN;
}

void test_vm_setup_should_fail(void)
{
    /*
        Allocating all 2^64-1 bytes seems unlikely to succeed until the year 2150.
    */
    vm *m = vm_setup((size_t)-1);
    TEST_ASSERT_NULL(m);
    VM_TEARDOWN;
}

static uint8_t out;
static int in;

static int out_char(uint8_t c)
{
    out = c;
    return 0;
}

static int in_char()
{
    return in;
}

void test_vm_set_io(void)
{
    VM_SETUP;
    TEST_ASSERT_EQUAL_INT(0, vm_set_io(m, in_char, out_char));

    in = 11;
    out = 100;

    program p;
    p.count = 2;
    /*
        Move input into output.
    */
    instruction code[] = 
    {
        {op_input, 0},
        {op_output, 0}
    };
    p.instructions = code;
    vm_set_program(m, &p);
    for (size_t i = 0; i < p.count; i++)
        TEST_ASSERT_EQUAL(0, vm_step(m));
    
    TEST_ASSERT_EQUAL_INT_MESSAGE(in, out, "Input and output should be equal.");

    VM_TEARDOWN; 
}

void test_vm_set_program_should_fail_on_empty(void)
{
    VM_SETUP;
    program p;
    p.count = 0;
    p.instructions = 0;
    TEST_ASSERT_EQUAL_INT(BFCC_ERROR_INVALID_PROGRAM, vm_set_program(m, &p));
    p.count = 1;
    TEST_ASSERT_EQUAL_INT(BFCC_ERROR_INVALID_PROGRAM, vm_set_program(m, &p));
    VM_TEARDOWN;
}

void test_vm_set_program_should_succeed(void)
{
    VM_SETUP;
    program p;
    p.count = 5;
    instruction code[] = 
    {
        {op_incr_ptr, 1},
        {op_incr_ptr, 1},
        {op_incr_ptr, 1},
        {op_incr_ptr, 1},
        {op_incr, 1}
    };
    p.instructions = code;
    TEST_ASSERT_EQUAL_INT(
        0,
        vm_set_program(m, &p)
    );
    VM_TEARDOWN;
}

void test_vm_step_fails_on_no_program(void)
{
    VM_SETUP;
    TEST_ASSERT_EQUAL_INT(BFCC_ERROR_INVALID_PROGRAM, vm_step(m));
    VM_TEARDOWN;
}

void test_vm_data_memory_pointer_null_on_null_vm(void)
{
    size_t size = 1234;
    TEST_ASSERT_NULL(vm_data_memory_pointer(0, &size));
    /*
        Check that size wasn't modified.
    */
    TEST_ASSERT_EQUAL_UINT(1234, size);
}

void test_vm_data_memory_pointer_returns_memory(void)
{
    VM_SETUP;
    size_t memory_size;
    uint8_t const *memory = vm_data_memory_pointer(m, &memory_size);
    TEST_ASSERT_NOT_NULL(memory);
    TEST_ASSERT_EQUAL_UINT(1024, memory_size);
    VM_TEARDOWN;
}

void test_vm_step_execution_halts_after_end(void)
{
    VM_SETUP;
    program p;
    p.count = 3;
    instruction code[] = 
    {
        {op_incr_ptr, 1},
        {op_incr_ptr, 1},
        {op_incr, 1}
    };
    p.instructions = code;
    vm_set_program(m, &p);
    TEST_ASSERT_EQUAL_INT(0, vm_step(m));
    TEST_ASSERT_EQUAL_INT(0, vm_step(m));
    TEST_ASSERT_EQUAL_INT(0, vm_step(m));
    TEST_ASSERT_EQUAL_INT(BFCC_ERROR_PROGRAM_TERMINATED, vm_step(m));
    VM_TEARDOWN;    
}

void test_vm_data_memory_zeroed_on_setup(void)
{
    VM_SETUP;

    size_t memory_size = 0;
    uint8_t const *memory = vm_data_memory_pointer(m, &memory_size);

    for (size_t i = 0; i < memory_size; i++)
    {
        TEST_ASSERT_EQUAL_UINT(0, memory[i]);
    }

    VM_TEARDOWN;
}

void test_vm_step_op_incr(void)
{
    VM_SETUP;
    program p;
    p.count = 4;
    /*
        Jump to position 3 in memory, increment to 4 then go to position 4 and 
        overflow to 1.
    */
    instruction code[] = 
    {
        {op_incr_ptr, 2},
        {op_incr, 4},
        {op_incr_ptr, 1},
        {op_incr, 257}
    };
    p.instructions = code;
    vm_set_program(m, &p);
    TEST_ASSERT_EQUAL_INT(0, vm_step(m));
    TEST_ASSERT_EQUAL_INT(0, vm_step(m));
    TEST_ASSERT_EQUAL_INT(0, vm_step(m));
    TEST_ASSERT_EQUAL_INT(0, vm_step(m));
    
    size_t memory_size = 0;
    uint8_t const *memory = vm_data_memory_pointer(m, &memory_size);

    TEST_ASSERT_EQUAL_UINT(0, memory[0]);
    TEST_ASSERT_EQUAL_UINT(0, memory[1]);
    TEST_ASSERT_EQUAL_UINT(4, memory[2]);
    TEST_ASSERT_EQUAL_UINT(1, memory[3]);

    VM_TEARDOWN;    
}

void test_vm_step_op_decr(void)
{
    VM_SETUP;
    program p;
    p.count = 2;
    /*
        Jump to position 3 in memory, decrement with 2 so it underflows to 
        256-2.
    */
    instruction code[] = 
    {
        {op_incr_ptr, 2},
        {op_decr, 2}
    };
    p.instructions = code;
    vm_set_program(m, &p);
    TEST_ASSERT_EQUAL_INT(0, vm_step(m));
    TEST_ASSERT_EQUAL_INT(0, vm_step(m));
    
    size_t memory_size = 0;
    uint8_t const *memory = vm_data_memory_pointer(m, &memory_size);

    TEST_ASSERT_EQUAL_UINT(0, memory[0]);
    TEST_ASSERT_EQUAL_UINT(0, memory[1]);
    TEST_ASSERT_EQUAL_UINT(((uint8_t) -2), memory[2]);

    VM_TEARDOWN;    
}

void test_vm_step_op_jmpbifz_jumps_on_zero(void)
{
    VM_SETUP;
    program p;
    p.count = 3;
    /*
        Set position 0 to 255.
    */
    instruction code[] = 
    {
        {op_incr, 1},
        {op_decr, 1},
        {op_jmpbifz, 1},
    };
    p.instructions = code;
    vm_set_program(m, &p);
    while (vm_step(m) == 0);

    size_t memory_size = 0;
    uint8_t const *memory = vm_data_memory_pointer(m, &memory_size);

    TEST_ASSERT_EQUAL_UINT(255, memory[0]);

    VM_TEARDOWN; 
}

void test_vm_step_op_jmpfifz_jumps_on_zero(void)
{
    VM_SETUP;
    program p;
    p.count = 3;
    /*
        Set position 0 to 255.
    */
    instruction code[] = 
    {
        {op_jmpfifz, 2},
        {op_incr, 1},
        {op_decr, 1},
    };
    p.instructions = code;
    vm_set_program(m, &p);
    while (vm_step(m) == 0);

    size_t memory_size = 0;
    uint8_t const *memory = vm_data_memory_pointer(m, &memory_size);

    TEST_ASSERT_EQUAL_UINT(255, memory[0]);

    VM_TEARDOWN; 
}

void test_vm_step_op_jmpbifz_segfaults_on_too_large_arg(void)
{
    VM_SETUP;
    program p;
    p.count = 1;
    /*
        Jump outside available instruction space.
    */
    instruction code[] = 
    {
        {op_jmpbifz, 1}
    };
    p.instructions = code;
    vm_set_program(m, &p);
    TEST_ASSERT_EQUAL_INT(BFCC_ERROR_PROGRAM_SEGFAULT, vm_step(m));

    VM_TEARDOWN;    
}

void test_vm_step_op_jmpfifz_segfaults_on_too_large_arg(void)
{
    VM_SETUP;
    program p;
    p.count = 1;
    /*
        Jump outside available instruction space.
    */
    instruction code[] = 
    {
        {op_jmpfifz, 1}
    };
    p.instructions = code;
    vm_set_program(m, &p);
    TEST_ASSERT_EQUAL_INT(BFCC_ERROR_PROGRAM_SEGFAULT, vm_step(m));

    VM_TEARDOWN;    
}

void test_vm_sample_program_1(void)
{
    VM_SETUP;
    program p;
    p.count = 6;
    /*
        Write 1 to positions 0, 1, 2 with a loop.
    */
    instruction code[] = 
    {
        {op_incr_ptr, 3},
        {op_incr, 5},
        {op_decr_ptr, 3}, /* We mark 3 as end position. */
        {op_incr, 1},
        {op_incr_ptr, 1},
        {op_jmpbifz, 2},
    };
    p.instructions = code;
    vm_set_program(m, &p);
    while (vm_step(m) == 0);

    size_t memory_size = 0;
    uint8_t const *memory = vm_data_memory_pointer(m, &memory_size);

    TEST_ASSERT_EQUAL_UINT(1, memory[0]);
    TEST_ASSERT_EQUAL_UINT(1, memory[1]);
    TEST_ASSERT_EQUAL_UINT(1, memory[2]);
    TEST_ASSERT_EQUAL_UINT(5, memory[3]);
    TEST_ASSERT_EQUAL_UINT(0, memory[4]);

    VM_TEARDOWN; 
}

int main(int argc, char *argv[])
{
    UNITY_BEGIN();
    RUN_TEST(test_vm_setup_should_pass);
    RUN_TEST(test_vm_setup_should_fail);
    RUN_TEST(test_vm_set_io);
    RUN_TEST(test_vm_set_program_should_fail_on_empty);
    RUN_TEST(test_vm_set_program_should_succeed);
    RUN_TEST(test_vm_step_fails_on_no_program);
    RUN_TEST(test_vm_data_memory_pointer_null_on_null_vm);
    RUN_TEST(test_vm_data_memory_pointer_returns_memory);
    RUN_TEST(test_vm_step_execution_halts_after_end);
    RUN_TEST(test_vm_data_memory_zeroed_on_setup);
    RUN_TEST(test_vm_step_op_incr);
    RUN_TEST(test_vm_step_op_decr);
    RUN_TEST(test_vm_step_op_jmpbifz_jumps_on_zero);
    RUN_TEST(test_vm_step_op_jmpfifz_jumps_on_zero);
    RUN_TEST(test_vm_step_op_jmpbifz_segfaults_on_too_large_arg);
    RUN_TEST(test_vm_step_op_jmpfifz_segfaults_on_too_large_arg);
    RUN_TEST(test_vm_sample_program_1);
    return UNITY_END();
}