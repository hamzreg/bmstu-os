#include "../inc/check_sequence.h"

void fill_sequence(sequence_t *sequence, int *storage)
{
    sequence->count = 3;

    node_t *node;

    for (int i = 0; i < sequence->count; i++)
    {
        node = create_node(&storage[i]);
        add_node(&sequence->head, node);
    }
}


START_TEST(test_palindrome)
{
    sequence_t sequence;
    init_sequence(&sequence);
    int storage[3] = {3, -2, 3};
    fill_sequence(&sequence, storage);

    int true_code = PALINDROME, code = is_palindrome(&sequence);

    free_sequence(sequence.head);

    ck_assert_int_eq(true_code, code);
}
END_TEST


START_TEST(test_not_palindrome)
{
    sequence_t sequence;
    init_sequence(&sequence);
    int storage[3] = {3, 3, 4};
    fill_sequence(&sequence, storage);

    int true_code = NOT_PALINDROME, code = is_palindrome(&sequence);

    free_sequence(sequence.head);

    ck_assert_int_eq(true_code, code);
}
END_TEST


Suite *palindrome_suite(void)
{
    Suite *s;
    TCase *tc_pos;

    s = suite_create("palindrome");

    tc_pos = tcase_create("positives");
    tcase_add_test(tc_pos, test_palindrome);
    tcase_add_test(tc_pos, test_not_palindrome);
    suite_add_tcase(s, tc_pos);

    return s;
}
