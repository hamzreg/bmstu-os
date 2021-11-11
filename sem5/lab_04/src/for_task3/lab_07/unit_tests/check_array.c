#include "../inc/check_array.h"


START_TEST(test_mysort_bad_start_array)
{
    int array_len = 10;

    mysort(NULL, array_len, sizeof(int), compare_int); 
}
END_TEST


START_TEST(test_mysort_bad_len_array)
{
    int array[] = {1, 3, 2};
    
    mysort(array, 0, sizeof(int), compare_int);
}
END_TEST


START_TEST(test_mysort_usual_array)
{
    int array[] = {3, 9, 6, 2, 0, 6, 3, -2, 5, -1};
    int final_array[] = {-2, -1, 0, 2, 3, 3, 5, 6, 6, 9};

    mysort(array, sizeof(array) / sizeof(array[0]), sizeof(int), compare_int);
    ck_assert_mem_eq(final_array, array, sizeof(array) / sizeof(array[0]));
}
END_TEST


Suite *mysort_suite(void)
{
    Suite *s;
    TCase *tc_neg;
    TCase *tc_pos;

    s = suite_create("mysort");

    tc_neg =  tcase_create("negatives");
    tcase_add_test(tc_neg, test_mysort_bad_start_array);
    tcase_add_test(tc_neg, test_mysort_bad_len_array);
    suite_add_tcase(s, tc_neg);

    tc_pos = tcase_create("positives");
    tcase_add_test(tc_pos, test_mysort_usual_array);
    suite_add_tcase(s, tc_pos);

    return s;
}


START_TEST(test_key_bad_start_array)
{
    int array[] = {1, 2, 3}; 
    int *new_array;
    int *end;
    int array_len = 10;

    int rc = key(NULL, array + array_len, &new_array, &end);
    ck_assert_int_eq(rc, ERROR_POINTER);
}
END_TEST


START_TEST(test_key_bad_end_array)
{
    int array[] = {1, 2, 3};
    int *new_array;
    int *end;

    int rc = key(array, NULL, &new_array, &end);
    ck_assert_int_eq(rc, ERROR_POINTER);
}
END_TEST


START_TEST(test_key_equal_start_end)
{
    int array[] = {1, 2, 3};
    int *new_array;
    int *end;

    int rc = key(array, array, &new_array, &end);
    ck_assert_int_eq(rc, EMPTY_ARRAY);
}
END_TEST


START_TEST(test_key_usual)
{
    int array[] = {3, 9, 6, 2, 0, 6, 3, -2, 5, -1};
    int array_len = 10;
    int *new_array;
    int *end;
    int final_array[] = {6, 3, 5};

    key(array, array + array_len, &new_array, &end);
    ck_assert_mem_eq(final_array, new_array, sizeof(final_array) / sizeof(final_array[0]));
    free(new_array);
}
END_TEST


Suite *key_suite(void)
{
    Suite *s;
    TCase *tc_neg;
    TCase *tc_pos;

    s = suite_create("key");

    tc_neg =  tcase_create("negatives");
    tcase_add_test(tc_neg, test_key_bad_start_array);
    tcase_add_test(tc_neg, test_key_bad_end_array);
    tcase_add_test(tc_neg, test_key_equal_start_end);
    suite_add_tcase(s, tc_neg);

    tc_pos = tcase_create("positives");
    tcase_add_test(tc_pos, test_key_usual);
    suite_add_tcase(s, tc_pos);

    return s;
}

