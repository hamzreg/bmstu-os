#include "../inc/check_array_objects.h"

void free_array(object_t *array)
{
    for (int i = 0; i < SIZE; i++)
        free(array[i].name);
    
    free(array);
}

int compare_array(object_t *first_array, object_t *second_array)
{
    for (int i = 0; i < SIZE; i++)
    {
        if (first_array[i].mass != second_array[i].mass)
            return NOT_EQUAL;

        if (first_array[i].volume != second_array[i].volume)
            return NOT_EQUAL;

        if (first_array[i].len_name != second_array[i].len_name)
            return NOT_EQUAL;

        if (strcmp(first_array[i].name, second_array[i].name) != 0)
            return NOT_EQUAL;
    }

    return EQUAL;
}


START_TEST(test_mysort_bad_start_array)
{
    int index = 10;

    sort_array_objects(NULL, index);
}
END_TEST


START_TEST(test_mysort_bad_index)
{
    int index = -2;

    object_t *array = malloc(SIZE * sizeof(object_t));

    array[0].len_name = 5;
    array[0].mass = 25;
    array[0].name = my_strdup("ball");
    array[0].volume = 5;

    array[1].len_name = 5;
    array[1].mass = 16;
    array[1].name = my_strdup("cube");
    array[1].volume = 4;

    array[2].len_name = 5;
    array[2].mass = 9;
    array[2].name = my_strdup("doll");
    array[2].volume = 3;

    sort_array_objects(array, index);
    free_array(array);
}
END_TEST


START_TEST(test_mysort_usual_array)
{
    object_t *array = malloc(SIZE * sizeof(object_t));

    array[0].len_name = 5;
    array[0].mass = 25;
    array[0].name = my_strdup("ball");
    array[0].volume = 5;

    array[1].len_name = 5;
    array[1].mass = 16;
    array[1].name = my_strdup("cube");
    array[1].volume = 4;

    array[2].len_name = 5;
    array[2].mass = 9;
    array[2].name = my_strdup("doll");
    array[2].volume = 3;

    object_t *final_array = malloc(SIZE * sizeof(object_t));

    final_array[0].len_name = 5;
    final_array[0].mass = 9;
    final_array[0].name = my_strdup("doll");
    final_array[0].volume = 3;

    final_array[1].len_name = 5;
    final_array[1].mass = 16;
    final_array[1].name = my_strdup("cube");
    final_array[1].volume = 4;

    final_array[2].len_name = 5;
    final_array[2].mass = 25;
    final_array[2].name = my_strdup("ball");
    final_array[2].volume = 5;

    sort_array_objects(array, SIZE - 1);

    int rc = compare_array(array, final_array);

    free_array(array);
    free_array(final_array);

    ck_assert_int_eq(rc, EQUAL);
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
    tcase_add_test(tc_neg, test_mysort_bad_index);
    suite_add_tcase(s, tc_neg);

    tc_pos = tcase_create("positives");
    tcase_add_test(tc_pos, test_mysort_usual_array);
    suite_add_tcase(s, tc_pos);

    return s;
}
