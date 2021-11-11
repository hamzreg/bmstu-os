#include "../inc/check_main.h"

int main(void)
{
    Suite *set_sum = sum_numbers_suite();
    SRunner *runner_sum = srunner_create(set_sum);
    srunner_run_all(runner_sum, CK_VERBOSE);
    int errors = srunner_ntests_failed(runner_sum);
    srunner_free(runner_sum);


    Suite *set_sort = mysort_suite();
    SRunner *runner_sort = srunner_create(set_sort);
    srunner_run_all(runner_sort, CK_VERBOSE);
    errors += srunner_ntests_failed(runner_sort);
    srunner_free(runner_sort);


    Suite *set_key = key_suite();
    SRunner *runner_key = srunner_create(set_key);
    srunner_run_all(runner_key, CK_VERBOSE);
    errors += srunner_ntests_failed(runner_key);
    srunner_free(runner_key);

    return (errors == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

