#include "../inc/check_main.h"

int main(void)
{
    Suite *set_palindrome = palindrome_suite();
    SRunner *runner_palindrome = srunner_create(set_palindrome);
    srunner_run_all(runner_palindrome, CK_VERBOSE);
    int errors = srunner_ntests_failed(runner_palindrome);
    srunner_free(runner_palindrome);

    return (errors == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
