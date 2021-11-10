#include "../inc/check_main.h"

int main(void)
{
    Suite *set_sort = mysort_suite();
    SRunner *runner_sort = srunner_create(set_sort);
    srunner_run_all(runner_sort, CK_VERBOSE);
    int errors = srunner_ntests_failed(runner_sort);
    srunner_free(runner_sort);

    return (errors == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
