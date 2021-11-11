#include <stdio.h>
#include <stdlib.h>
#include "../inc/node.h"
#include "../inc/sequence.h"
#include "../inc/list.h"

int main(int argc, char **argv)
{
    if (argc != COUNT_ARGC)
        return ERROR_ARGC;

    sequence_t sequence, add_sequence;
    int *storage = NULL;

    int code = read_sequence(argv[1], &sequence, &storage);

    if (code)
        return code;

    init_sequence(&add_sequence);
    add_sequence.count = sequence.count;
    create_sequence(&add_sequence, storage);

    print_sequence(&sequence);

    if (is_palindrome(&add_sequence))
        printf("is palindrome\n");
    else
        printf("is not palindrome\n");

    free(storage);

    return OK;
}
