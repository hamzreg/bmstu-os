#include <stdio.h>
#include "../inc/return_codes.h"
#include "../inc/array_objects_t.h"
#include "../inc/base_functions.h"

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 3)
        return ERROR_ARGC;

    array_objects_t objects;

    int code = get_objects(argv[1], &objects);

    if (code != OK)
        return code;

    if (argc == 2)
        get_sorted_objects(&objects);

    if (argc == 3)
        get_some_objects(argv[2], &objects);

    free_array_objects(&objects);

    return OK;
}
