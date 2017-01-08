#include "tokeniser.h"
#include "unity.h"
#include <stdio.h>
#include <string.h>

char const *input = "+-[],.<>";

struct tokeniser_reader_impl_state
{
    size_t i;
};

static int tokeniser_reader_impl(int *c, void *reader_state)
{
    struct tokeniser_reader_impl_state *impl_state = 
        (struct tokeniser_reader_impl_state *) reader_state;
    if (impl_state->i < strlen(input))
    {
        *c = input[impl_state->i++];
        return 0;
    }
    *c = EOF;
    return EOF;
}

#define TOKENISER_SETUP(tname, sname)\
    struct tokeniser_reader_impl_state sname = {0};\
    tokeniser * tname = tokeniser_setup(tokeniser_reader_impl, & sname);

#define TOKENISER_TEARDOWN(tname)\
    tokeniser_free(tname);

void test_tokeniser_setup_with_stream(void)
{
    tokeniser *t = tokeniser_setup_with_stream(stdin);
    TEST_ASSERT_NOT_NULL(t);
    tokeniser_free(t);
}

void test_tokeniser_setup(void)
{
    TOKENISER_SETUP(t, rstate);
    TEST_ASSERT_NOT_NULL(t);
    TOKENISER_TEARDOWN(t);
}

void test_tokeniser_next_empty_str(void)
{
    input = "";
    TOKENISER_SETUP(t, rstate);
    token tok;
    TEST_ASSERT_EQUAL_INT(0, tokeniser_next(t, &tok));
    TOKENISER_TEARDOWN(t);
}

void test_tokeniser_next(void)
{
    input = "><+-.,[]";
    token tokens[] = 
    {
        token_incr_ptr,
        token_decr_ptr,
        token_incr,
        token_decr,
        token_output,
        token_input,
        token_jump_fwd,
        token_jump_back,
        token_eof
    };
    TOKENISER_SETUP(t, rstate);
    token tok;

    for (size_t i = 0; i < strlen(input); i++)
    {
        TEST_ASSERT_EQUAL_INT_MESSAGE(0, tokeniser_next(t, &tok), "Call should count as success.");
        TEST_ASSERT_EQUAL_INT_MESSAGE(tokens[i], tok, "Wrong token returned.");
    }

    TEST_ASSERT_EQUAL_INT(0, tokeniser_next(t, &tok));
    TEST_ASSERT_EQUAL_INT(token_eof, tok);

    /*
        Check that it keeps returning EOF.
    */
    TEST_ASSERT_EQUAL_INT(0, tokeniser_next(t, &tok));
    TEST_ASSERT_EQUAL_INT(token_eof, tok);
    
    TOKENISER_TEARDOWN(t);
}

int main(int argc, char *argv[])
{
    UNITY_BEGIN();
    RUN_TEST(test_tokeniser_setup_with_stream);
    RUN_TEST(test_tokeniser_setup);
    RUN_TEST(test_tokeniser_next_empty_str);
    RUN_TEST(test_tokeniser_next);
    return UNITY_END();
}