#include "../inc/check_numbers.h"


START_TEST(test_sum_numbers_bad_start_array)
{
    int array[] = {1, 2, 3};

    int rc = sum_numbers(NULL, array + 3);
    ck_assert_int_eq(rc, ERROR_POINTER);
}
END_TEST


START_TEST(test_sum_numbers_bad_end_array)
{
    int array[] = {1, 2, 3};

    int rc = sum_numbers(array, NULL);
    ck_assert_int_eq(rc, ERROR_POINTER);
}
END_TEST


START_TEST(test_sum_numbers_empty_array)
{
    int array[] = {1, 2, 3};

    int rc = sum_numbers(array, array);
    ck_assert_int_eq(rc, EMPTY_ARRAY);
}
END_TEST


START_TEST(test_sum_numbers_usual_array)
{
    int array[] = {3, 5, -1, 0, 6, 8, -2};
    int sum = 19;

    int rc = sum_numbers(array, array + 7);
    ck_assert_int_eq(rc, sum);
}
END_TEST


START_TEST(test_sum_numbers_one_element)
{
    int array[] = {4};
    int sum = 4;

    int rc = sum_numbers(array, array + 1);
    ck_assert_int_eq(rc, sum);
}
END_TEST


Suite *sum_numbers_suite(void)
{
    Suite *s;
    TCase *tc_neg;
    TCase *tc_pos;

    s = suite_create("numebrs");
    tc_neg =  tcase_create("negatives");
    tcase_add_test(tc_neg, test_sum_numbers_bad_start_array);
    tcase_add_test(tc_neg, test_sum_numbers_bad_end_array);
    tcase_add_test(tc_neg, test_sum_numbers_empty_array);
    suite_add_tcase(s, tc_neg);

    tc_pos = tcase_create("positives");
    tcase_add_test(tc_pos, test_sum_numbers_usual_array);
    tcase_add_test(tc_pos, test_sum_numbers_one_element);
    suite_add_tcase(s, tc_pos);

    return s;
}

