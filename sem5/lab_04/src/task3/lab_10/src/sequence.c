#include "../inc/sequence.h"

void init_sequence(sequence_t *sequence)
{
    sequence->count = 0;
    sequence->head = NULL;
}


int get_count_elements(FILE *in_file, int *count)
{
    int element;

    while (fscanf(in_file, "%d", &element) == 1)
        (*count)++;

    if (!*count)
        return EMPTY_FILE;

    return OK;
}


int read_sequence(const char *filename, sequence_t *sequence,
int **storage)
{
    FILE *in_file = fopen(filename, "r");

    if (!in_file)
    {
        LOG_ERROR("%s", "File open error");
        return ERROR_FILE_OPEN;
    }

    init_sequence(sequence);

    if (get_count_elements(in_file, &sequence->count))
        return EMPTY_FILE;

    rewind(in_file);

    *storage = malloc(sequence->count * sizeof(int));

    if (!*storage)
        return ALLOCATE_ERROR;

    int element;
    int i = 0;

    while (fscanf(in_file, "%d", &element) == 1)
    {
        (*storage)[i] = element;
        i++;
    }

    create_sequence(sequence, *storage);

    if (fclose(in_file) != OK)
    {
        free_sequence(sequence->head);
        free(*storage);

        LOG_ERROR("%s", "File close error");
        return ERROR_FILE_CLOSE;
    }

    return OK;
}


void create_sequence(sequence_t *sequence, int *storage)
{
    for (int i = 0; i < sequence->count; i++)
    {
        node_t *node = create_node(&storage[i]);
        add_node(&(sequence->head), node);
    }
}


void free_sequence(node_t *head)
{
    node_t *node;

    for (; head; head = node)
    {
        node = head->next;
        free(head);
    }
}


int is_palindrome(sequence_t *sequence)
{
    int repeats = sequence->count / 2;
    char *start, *end;

    while (repeats > 0)
    {
        start = pop_front(&sequence->head);
        end = pop_back(&sequence->head);

        if (compare_int(start, end) != EQUAL)
            return NOT_PALINDROME;

        repeats--;
    }

    return PALINDROME;
}


node_t* process_sequence(const char *filename, sequence_t *sequence,
int *storage, int (*comparator)(const void *, const void *))
{
    int code = is_palindrome(sequence);

    sequence_t add_sequence;
    init_sequence(&add_sequence);
    add_sequence.count = sequence->count;

    free_sequence(sequence->head);

    create_sequence(&add_sequence, storage);
    node_t *result;

    if (code == PALINDROME)
        result = sort(add_sequence.head, comparator);
    else
        result = reverse(add_sequence.head);

    return result;
}


int write_sequence(const char *filename, sequence_t *sequence)
{
    FILE *out_file = fopen(filename, "w");

    if (!out_file)
    {
        LOG_INFO("%s", "File open error");
        return ERROR_FILE_OPEN;
    }

    int size = sequence->count;

    while (size > 0)
    {
        int *number = (int *)pop_front(&sequence->head);
        size--;
        fprintf(out_file, "%d\n", *number);
    }

    if (fclose(out_file) != OK)
    {
        free_sequence(sequence->head);
        return ERROR_FILE_CLOSE;
    }

    return OK;
}
