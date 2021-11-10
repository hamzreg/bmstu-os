#include <stdio.h>
#include <stdlib.h>
#include "../inc/node.h"
#include "../inc/sequence.h"
#include "../inc/list.h"

int main(int argc, char **argv)
{
    if (argc != COUNT_ARGC)
        return ERROR_ARGC;

    sequence_t sequence, result;
    int *storage = NULL;

    int code = read_sequence(argv[1], &sequence, &storage);

    if (code)
        return code;

    init_sequence(&result);
    result.head = process_sequence(argv[1], &sequence, storage,
    compare_int);
    result.count = sequence.count;

    write_sequence(argv[2], &result);

    free(storage);

    return OK;
}
